#pragma once
#include <cstdint>
#include <limits>

namespace NodelNet
{
	//This file contains definition for constant types that can be altered to improve performance or other aspects.

	/* Contains all constant values that can be changed to accomodate application*/
	struct Constants
	{
		static constexpr uint64_t max_packet_size = 10000000000;
		static constexpr uint64_t default_ip_endpoint_port = 10;
		static constexpr uint64_t max_connection_timeout = 10;
		static constexpr uint64_t max_sessions_connections = 10;
		static constexpr bool process_packet_async = true;
	};
	using session_id_t = uint64_t;
	//General definition to alter 
#define MAX_PACKET_SIZE USHRT_MAX;
#define DEFAULT_IP_ENDPOINT_PORT 8080;

//Theses definitions should be tweaked in order to accommodate the type of game server.
#define MAX_CONNECTION_TIMEOUT 30000;
#define MAX_SESION_CONNECTIONS 1000;
#define MAX_SERVER_CONNECTIONS 10000;

//TO DO:
#define MAX_ALOCATED_MEM_MB 1000;
#define PROCESS_PACKET_ASYNC_TRUE 1; //bool
#define MAX_PACKET_STACK 1000;

}