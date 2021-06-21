#include "Server.h"
#include <iostream>

#include "Player.h"
#include "GameObjectManager.h"
#include "Bullet.h"
namespace NodelNet
{
	bool Server::Initialize(IPEndPoint ip)
	{
		master_fd.clear();
		connections.clear();

		listeningSocket = Socket(ip.GetVersion());
		if (listeningSocket.Create(Protocol::TCP) == NETResult::Success)
		{
			std::cout << "Socket successfully created." << std::endl;
			listeningSocket.Bind(ip);
			if (listeningSocket.Listen(5) == NETResult::Success)
			{
				WSAPOLLFD listeningSocketFD = {};
				listeningSocketFD.fd = listeningSocket.GetHandle();
				listeningSocketFD.events = POLLRDNORM;
				listeningSocketFD.revents = 0;

				master_fd.push_back(listeningSocketFD);

				std::cout << "Socket successfully listening." << std::endl;
				return true;
			}
			else
			{
				std::cerr << "Failed to listen." << std::endl;
			}
			listeningSocket.Close();
		}
		else
		{
			std::cerr << "Socket failed to create." << std::endl;
		}
		return false;
	}

	void Server::Frame()
	{
		for (int i = 0; i < connections.size(); i++)
		{
			if (connections[i].pm_outgoing.HasPendingPackets())
			{
				master_fd[i + 1].events = POLLRDNORM | POLLWRNORM;
			}
		}

		use_fd = master_fd;

		if (WSAPoll(use_fd.data(), use_fd.size(), 1) > 0)
		{
#pragma region listener
			WSAPOLLFD& listeningSocketFD = use_fd[0];
			if (listeningSocketFD.revents & POLLRDNORM)
			{
				Socket newConnectionSocket;
				IPEndPoint newConnectionEndpoint;
				if (listeningSocket.Accept(newConnectionSocket, newConnectionEndpoint) == NETResult::Success)
				{
					connections.emplace_back(Connection(newConnectionSocket, newConnectionEndpoint));
					Connection& acceptedConnection = connections[connections.size() - 1];
					WSAPOLLFD newConnectionFD = {};
					newConnectionFD.fd = newConnectionSocket.GetHandle();
					newConnectionFD.events = POLLRDNORM;
					newConnectionFD.revents = 0;
					master_fd.push_back(newConnectionFD);
					OnConnect(acceptedConnection);
				}
				else
				{
					std::cerr << "Failed to accept new connection." << std::endl;
				}
			}
#pragma endregion Code specific to the listening socket

			for (int i = use_fd.size() - 1; i >= 1; i--)
			{
				int connectionIndex = i - 1;
				Connection& connection = connections[connectionIndex];

				if (use_fd[i].revents & POLLERR)
				{
					CloseConnection(connectionIndex, "POLLERR");
					continue;
				}

				if (use_fd[i].revents & POLLHUP)
				{
					CloseConnection(connectionIndex, "POLLHUP");
					continue;
				}

				if (use_fd[i].revents & POLLNVAL)
				{
					CloseConnection(connectionIndex, "POLLNVAL");
					continue;
				}

				if (use_fd[i].revents & POLLRDNORM)
				{
					int bytesReceived = 0;

					if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
					{
						bytesReceived = recv(use_fd[i].fd, (char*)&connection.pm_incoming.currentPacketSize + connection.pm_incoming.currentPacketExtractionOffset, sizeof(uint16_t) - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}
					else //Process Packet Contents
					{
						bytesReceived = recv(use_fd[i].fd, (char*)&connection.buffer + connection.pm_incoming.currentPacketExtractionOffset, connection.pm_incoming.currentPacketSize - connection.pm_incoming.currentPacketExtractionOffset, 0);
					}


					if (bytesReceived == 0)
					{
						CloseConnection(connectionIndex, "Recv==0");
						continue;
					}

					if (bytesReceived == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							CloseConnection(connectionIndex, "Recv<0");
							continue;
						}
					}

					if (bytesReceived > 0)
					{
						connection.pm_incoming.currentPacketExtractionOffset += bytesReceived;
						if (connection.pm_incoming.currentTask == PacketManagerTask::ProcessPacketSize)
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == sizeof(uint16_t))
							{
								connection.pm_incoming.currentPacketSize = ntohs(connection.pm_incoming.currentPacketSize);
								if (connection.pm_incoming.currentPacketSize > Constants::max_packet_size)
								{
									CloseConnection(connectionIndex, "Packet size too large.");
									continue;
								}
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketContents;
							}
						}
						else
						{
							if (connection.pm_incoming.currentPacketExtractionOffset == connection.pm_incoming.currentPacketSize)
							{
								std::shared_ptr<Packet> packet = std::make_shared<Packet>();
								packet->buffer.resize(connection.pm_incoming.currentPacketSize);
								memcpy(&packet->buffer[0], connection.buffer, connection.pm_incoming.currentPacketSize);

								connection.pm_incoming.Append(packet);

								connection.pm_incoming.currentPacketSize = 0;
								connection.pm_incoming.currentPacketExtractionOffset = 0;
								connection.pm_incoming.currentTask = PacketManagerTask::ProcessPacketSize;
							}
						}
					}
				}

				if (use_fd[i].revents & POLLWRNORM)
				{
					PacketManager& pm = connection.pm_outgoing;
					while (pm.HasPendingPackets())
					{
						if (pm.currentTask == PacketManagerTask::ProcessPacketSize)
						{
							pm.currentPacketSize = pm.Retrieve()->buffer.size();
							uint16_t bigEndianPacketSize = htons(pm.currentPacketSize);
							int bytesSent = send(use_fd[i].fd, (char*)(&bigEndianPacketSize) + pm.currentPacketExtractionOffset, sizeof(uint16_t) - pm.currentPacketExtractionOffset, 0);
							if (bytesSent > 0)
							{
								pm.currentPacketExtractionOffset += bytesSent;
							}

							if (pm.currentPacketExtractionOffset == sizeof(uint16_t))
							{
								pm.currentPacketExtractionOffset = 0;
								pm.currentTask = PacketManagerTask::ProcessPacketContents;
							}
							else
							{
								break;
							}
						}
						else
						{
							char* bufferPtr = &pm.Retrieve()->buffer[0];
							int bytesSent = send(use_fd[i].fd, (char*)(bufferPtr)+pm.currentPacketExtractionOffset, pm.currentPacketSize - pm.currentPacketExtractionOffset, 0);
							if (bytesSent > 0)
							{
								pm.currentPacketExtractionOffset += bytesSent;
							}

							if (pm.currentPacketExtractionOffset == pm.currentPacketSize)
							{
								pm.currentPacketExtractionOffset = 0;
								pm.currentTask = PacketManagerTask::ProcessPacketSize;
								pm.Pop();
							}
							else
							{
								break;
							}
						}
					}
					if (!pm.HasPendingPackets())
					{
						master_fd[i].events = POLLRDNORM;
					}
				}

			}
		}

		for (int i = connections.size() - 1; i >= 0; i--)
		{
			while (connections[i].pm_incoming.HasPendingPackets())
			{
				std::shared_ptr<Packet> frontPacket = connections[i].pm_incoming.Retrieve();
				if (!ProcessPacket(frontPacket))
				{
					CloseConnection(i, "Failed to process incoming packet.");
					break;
				}
				connections[i].pm_incoming.Pop();
			}
		}

	}

	void Server::OnConnect(Connection& newConnection)
	{
		std::cout << newConnection.GetIPEndPoint() << " - New connection accepted (" << clientCount << ")." << std::endl;
		clientCount++;
	}

	void Server::OnDisconnect(Connection& lostConnection, std::string reason)
	{
		std::cout << "[" << reason << "] Connection lost: " << lostConnection.GetIPEndPoint() << "." << std::endl;
	}

	void Server::CloseConnection(int connectionIndex, std::string reason)
	{
		Connection& connection = connections[connectionIndex];
		OnDisconnect(connection, reason);
		master_fd.erase(master_fd.begin() + (connectionIndex + 1));
		use_fd.erase(use_fd.begin() + (connectionIndex + 1));
		connection.Close();
		connections.erase(connections.begin() + connectionIndex);
	}

	bool Server::ProcessPacket(std::shared_ptr<Packet> packet)
	{
		std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
		auto packetType = packet->GetPacketType();
		std::cout << "Packet type" << (int)packetType << std::endl;

		switch (packetType)
		{
		case PacketType::PT_CreatePlayer:
			std::cout << player->GetPos().x << player->GetPos().y << std::endl;
			auto tempCreateObject = GetCreateObject(packet->buffer.data());
			break;
		case PacketType::PT_CreateBullet:
			auto tempCreateBullet = GetCreateBullet(packet->buffer.data());
			
			break;
		case PacketType::PT_Move:
			auto tempMoveObject = GetMoveObject(packet->buffer.data());

			break;
		}
		return true;
	}
	bool Server::ProcessMove(moveObject move)
	{
		move.Verify(verifier);
		auto obj = gameobjects->GetObjectById(move.uid());
		obj->SetRotation(move.rotation());
		if (obj->GetPos().x == move.position()->x())
		{
			if (obj->GetPos().y == move.position()->y())
			{
				obj->SetPosition(sf::Vector2f(move.position()->x(), move.position()->y()));
			}
		}
		return false;
	}

	bool Server::ProcessCreateObject(createObject value)
	{
		value.Verify(verifier);
		switch (value.type())
		{
		case objectTypes::objectTypes_Bullet:
			std::make_shared<Bullet>(gameobjects,texture,,,);
			value.position();
			value.rotation();
			value.uid();
			break;
		case objectTypes::objectTypes_Player:
			gameobjects->Append(
				std::make_shared<Player>(Player(100, 2, gameobjects, ))
			);
			break;
		}
		return false;
	}

	bool Server::ProcessDeleteObject(removeObject value)
	{
		if (!value.Verify(verifier))
			return false;
		gameobjects->Remove(gameobjects->GetObjectById(value.uid()).get());
		return true;
	}

}