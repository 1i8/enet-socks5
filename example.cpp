#include "enetclient.hpp"

int main() {
	ENetClient client;

	client.proxy_info = {
		.ip = "127.0.0.1", // proxy ip address
		.port = 7777,      // proxy port

		// if your proxy needs username & password auth then:
		.username = "proxy-username",
		.password = "proxy-password"
	};

	// then get growtopia server data and connect to it with ENetClient::Connect

	// then call enet_host_service with 'client.host' to get connected to proxy and to receive enet events

	return EXIT_SUCCESS;
}
