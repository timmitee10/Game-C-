#pragma once
#include "Connection.h"
#include "Packet.h"

#include "flatbuffers/flatbuffers.h"
#include "FBS/Position_generated.h"
class GameObjectManager;
namespace NodelNet
{

	class Server
	{
	public:
		bool Initialize(IPEndPoint ip);
		void Frame();
	protected:
		virtual void OnConnect(Connection& newConnection);
		virtual void OnDisconnect(Connection& lostConnection, std::string reason);
		void CloseConnection(int connectionIndex, std::string reason);
		virtual bool ProcessPacket(std::shared_ptr<Packet> packet);
		virtual bool ProcessMove(moveObject move);
		virtual bool ProcessCreateObject(createObject value);
		virtual bool ProcessDeleteObject(removeObject value);
		
		unsigned int clientCount = 0;
		Socket listeningSocket;
		std::vector<Connection> connections;
		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd;
		flatbuffers::Verifier verifier;
		flatbuffers::FlatBufferBuilder fbb;
		GameObjectManager* gameobjects;
	};
}