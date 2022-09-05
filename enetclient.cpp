#include "enetclient.hpp"

#ifdef _DEBUG
#include <iostream>
#include <format>
#endif

ENetClient::ENetClient() :
	host(nullptr),
	peer(nullptr),
	proxy_info(ENetSocks5Info{}) {
	static bool init = false;
	if (!init) {
		if (enet_initialize() == 0)
			atexit(enet_deinitialize);

		init = true;
	}
}

ENetClient::~ENetClient() { ResetProxy(); }

void ENetClient::SendPacketRaw(const NetMessageType type, const uint8_t* data, const size_t data_size) const {
	if (host != nullptr && peer != nullptr) {
		ENetPacket* packet = enet_packet_create(0, data_size + 5, ENET_PACKET_FLAG_RELIABLE);

		memcpy(packet->data, &type, 4);
		memcpy(packet->data + 4, data, data_size);

		enet_peer_send(peer, 0, packet);
		enet_host_flush(host); // flush host so packet gets sent right away
	}
}

void ENetClient::SendGenericText(const std::string& packet) const {
#ifdef _DEBUG
	std::cout << std::format(
		"[SendGenericText]\n"
		"{}\n"
		"[===============]\n\n",
		packet.c_str()
	);
#endif

	SendPacketRaw(
		NetMessageType::GenericText,
		reinterpret_cast<const uint8_t*>(packet.data()),
		packet.length()
	);
}

void ENetClient::SendGameMessage(const std::string& packet) const {
#ifdef _DEBUG
	std::cout << std::format(
		"[SendGameMessage]\n"
		"{}\n"
		"[===============]\n\n",
		packet.c_str()
	);
#endif

	SendPacketRaw(
		NetMessageType::GameMessage,
		reinterpret_cast<const uint8_t*>(packet.data()),
		packet.length()
	);
}

void ENetClient::SendGamePacket(const GameUpdatePacket& packet) const {
#ifdef _DEBUG
	std::cout << std::format(
		"[SendGamePacket]\n"
		"{}"
		"[==============]\n\n",
		packet.Print().c_str()
	);
#endif

	SendPacketRaw(
		NetMessageType::GamePacket,
		reinterpret_cast<const uint8_t*>(&packet),
		sizeof(GameUpdatePacket)
	);
}

void ENetClient::SetProxy(const std::string& proxy_ip, const uint16_t proxy_port) {
	proxy_info.ip = new char[256];

	strcpy(const_cast<char*>(proxy_info.ip), proxy_ip.c_str());

	proxy_info.port = proxy_port;
}

void ENetClient::SetProxyAuth(const std::string& proxy_username, const std::string& proxy_password) {
	proxy_info.auth.username = new char[256];
	proxy_info.auth.password = new char[256];

	strcpy(const_cast<char*>(proxy_info.auth.username), proxy_username.c_str());
	strcpy(const_cast<char*>(proxy_info.auth.password), proxy_password.c_str());
}

void ENetClient::ResetProxy() {
	if (proxy_info.ip != nullptr) {
		delete[] proxy_info.ip;
		proxy_info.ip = nullptr;
	}

	proxy_info.port = 0;

	if (proxy_info.auth.username != nullptr) {
		delete[] proxy_info.auth.username;
		proxy_info.auth.username = nullptr;
	}

	if (proxy_info.auth.password != nullptr) {
		delete[] proxy_info.auth.password;
		proxy_info.auth.password = nullptr;
	}
}

bool ENetClient::Connect(const std::string& ip, const uint16_t port, const bool type2) {
	if (host != nullptr) {
		enet_host_destroy(host);
		host = nullptr;
	}

	host = enet_host_create(0, 1, 2, 0, 0);
	if (host == nullptr)
		return false;

	host->usingNewPacket = type2;

	if (proxy_info.port != 0) {
		host->usingProxy = true;
		host->proxyInfo = proxy_info;
	}

	host->checksum = enet_crc32;
	if (enet_host_compress_with_range_coder(host) < 0)
		return false;

	ENetAddress address{};
	enet_address_set_host(&address, ip.c_str());
	address.port = port;

	peer = enet_host_connect(host, &address, 2, 0);
	if (peer == nullptr)
		return false;

	enet_host_flush(host);
	return true;
}

void ENetClient::Disconnect() {
	if (peer != nullptr) {
		SendGamePacket({ GamePacketType::Disconnect });
		enet_peer_disconnect(peer, 0);
		peer = nullptr;
	}
	if (host != nullptr) {
		enet_host_destroy(host);
		host = nullptr;
	}
}
