#pragma once
#include <string>

#include "../external/enet.h"
#include "../packet/packet.hpp"

class ENetClient {
public:
	ENetClient();
	~ENetClient();

	void SendPacketRaw(const NetMessageType type, const uint8_t* data, const size_t data_size) const;
	void SendGenericText(const std::string& packet) const;
	void SendGameMessage(const std::string& packet) const;
	void SendGamePacket(const GameUpdatePacket& packet) const;

	// proxy shit is messy rn
	void SetProxy(const std::string& proxy_ip, const uint16_t proxy_port);
	void SetProxyAuth(const std::string& proxy_username, const std::string& proxy_password);
	void ResetProxy();

protected:
	bool Connect(const std::string& ip, const uint16_t port, const bool type2);
	void Disconnect();

	ENetHost* host;
	ENetPeer* peer;

private:
	ENetSocks5Info proxy_info;
};
