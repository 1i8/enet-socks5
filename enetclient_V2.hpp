#pragma once

#include <sstream>
#include <string>
#include <utility>

namespace enet {
	#include "enet.h"
}

namespace internal {
	template<class T>
	class vec2 {
	public:
		constexpr inline vec2(const T& x = 0, const T& y = 0) noexcept : x(x), y(y) {}

		constexpr inline bool operator==(const vec2<T>& other) const noexcept { return x == other.x && y == other.y; }
		constexpr inline bool operator!=(const vec2<T>& other) const noexcept { return x != other.x || y != other.y; }

		constexpr inline vec2<T> operator+(const vec2<T>& other) noexcept { return vec2<T>(x + other.x, y + other.y); }
		constexpr inline vec2<T> operator-(const vec2<T>& other) noexcept { return vec2<T>(x - other.x, y - other.y); }
		constexpr inline vec2<T> operator/(const vec2<T>& other) noexcept { return vec2<T>(x / other.x, y / other.y); }
		constexpr inline vec2<T> operator*(const vec2<T>& other) noexcept { return vec2<T>(x * other.x, y * other.y); }

		inline operator std::string() const noexcept {
			if constexpr (std::is_convertible_v<T, std::string>) {
				return "x -> " + static_cast<std::string>(x) +
					", y -> " + static_cast<std::string>(y);
			}
			else {
				return "x -> " + std::to_string(x) +
					", y -> " + std::to_string(y);
			}
		}

		T x, y;
	};

	template<class T>
	class vec3 {
	public:
		constexpr inline vec3(const T& x = 0, const T& y = 0, const T& z = 0)
			noexcept : x(x), y(y), z(z) {}

		constexpr inline bool operator==(const vec3<T>& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
		constexpr inline bool operator!=(const vec3<T>& other) const noexcept { return x != other.x || y != other.y || other.z; }

		constexpr inline vec3<T> operator+(const vec3<T>& other) noexcept { return vec3<T>(x + other.x, y + other.y, z + other.z); }
		constexpr inline vec3<T> operator-(const vec3<T>& other) noexcept { return vec3<T>(x - other.x, y - other.y, z - other.z); }
		constexpr inline vec3<T> operator/(const vec3<T>& other) noexcept { return vec3<T>(x / other.x, y / other.y, z / other.z); }
		constexpr inline vec3<T> operator*(const vec3<T>& other) noexcept { return vec3<T>(x * other.x, y * other.y, z * other.z); }

		inline operator std::string() const noexcept {
			if constexpr (std::is_convertible_v<T, std::string>) {
				return "x -> " + static_cast<std::string>(x) +
					", y -> " + static_cast<std::string>(y) +
					", z -> " + static_cast<std::string>(z);
			}
			else {
				return "x -> " + std::to_string(x) +
					", y -> " + std::to_string(y) +
					", z -> " + std::to_string(z);
			}
		}

		T x, y, z;
	};
}

using vec2i = internal::vec2<int>;
using vec3i = internal::vec3<int>;
using vec2f = internal::vec2<float>;
using vec3f = internal::vec3<float>;

enum class PacketType : int8_t {
	State,
	CallFunction,
	UpdateStatus,
	TileChangeRequest,
	SendMapData,
	SendTileUpdateData,
	SendTileUpdateDataMultiple,
	TileActivateRequest,
	TileApplyDamage,
	SendInventoryState,
	ItemActivateRequest,
	ItemActivateObjectRequest,
	SendTileTreeState,
	ModifyItemInventory,
	ItemChangeObject,
	SendLock,
	SendItemDatabaseData,
	SendParticleEffect,
	SetIconState,
	ItemEffect,
	SetCharacterState,
	PingReply,
	PingRequest,
	GotPunched,
	AppCheckResponse,
	AppIntegrityFail,
	Disconnect,
	BattleJoin,
	BattleEvent,
	UseDoor,
	SendParental,
	GoneFishin,
	Steam,
	PetBattle,
	NPC,
	Special,
	SendParticleEffectV2,
	ActiveArrowToItem,
	SelectTileIndex,
	SendPlayerTributeData,
	PvEUnk1,
	PvEUnk2,
	PvEUnk3,
	PvEUnk4,
	PvEUnk5,
	SetExtraMods,
	OnStepOnTileMod
};

enum class NetMessage : int32_t {
	Unknown,
	ServerHello,
	GenericText,
	GameMessage,
	GamePacket,
	Error,
	Track,
	ClientLogRequest,
	ClientLogResponse,
};

//non standard :(
#ifdef _MSC_VER
#pragma pack(push, 1)
#define PACKED
#elif defined(__GNUC__) || defined(__clang__)
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

struct GamePacket {
	PacketType type; //0
	int8_t objtype; //1
	int8_t count1; //2
	int8_t count2; //3
	int32_t netid; //4
	int32_t item; //8
	int32_t flags; //12
	float float1; //16
	int32_t int_data; //20
	vec2f pos; //24
	vec2f pos2; //32
	float float2; //40
	vec2i tile; //44
	uint32_t data_size; //52
	uint32_t data; //56

	std::string Print() const noexcept {
		std::stringstream result;

		result << "<--Game Packet-->\ntype: " << static_cast<size_t>(type) << '\n';

		auto PrintMember = [&](auto member, const std::string& name) {
			if (member != decltype(member){}) {
				if constexpr (std::is_same_v<decltype(member), int8_t>) {
					result << name + ": " << +member << '\n';
				}
				else { result << name + ": " << member << '\n'; }
			}
		};

		PrintMember(objtype, "objtype");
		PrintMember(count1, "count1");
		PrintMember(count2, "count2");
		PrintMember(netid, "netid");
		PrintMember(item, "item");
		PrintMember(float1, "float1");
		PrintMember(int_data, "int_data");
		PrintMember(pos.x, "pos.x");
		PrintMember(pos.y, "pos.y");
		PrintMember(pos2.x, "pos2.x");
		PrintMember(pos2.y, "pos2.y");
		PrintMember(float2, "float2");
		PrintMember(tile.x, "tile.x");
		PrintMember(tile.y, "tile.y");
		PrintMember(data_size, "data_size");
		PrintMember(data, "data");

		result << "<--------------->";

		return result.str();
	}
private:
	friend std::ostream& operator<<(std::ostream& os, const GamePacket& game_packet) noexcept {
		return os << game_packet.Print();
	}
} PACKED;


#undef PACKED
#ifdef _MSC_VER
#pragma pack(pop)
#endif

class ENetClient {
public:
	bool ENetConnect(const std::string& server_ip, uint16_t server_port, bool using_proxy, bool type2) noexcept {
		if (host != nullptr) {
			ENetDisconnect();
		}

		host = enet::enet_host_create(0, 1, 2, 0, 0);
		if (host == nullptr) {
			return false;
		}

		host->usingNewPacket = type2;

		if (using_proxy) {
			host->usingProxy = true;
			host->proxyInfo = {
				proxy_info.ip.data(),
				proxy_info.port, {
					proxy_info.username.data(),
					proxy_info.password.data()
				}
			};
		}

		host->checksum = enet::enet_crc32;
		if (enet::enet_host_compress_with_range_coder(host) < 0)
			return false;

		enet::ENetAddress address;
		enet::enet_address_set_host(&address, server_ip.c_str());
		address.port = server_port;

		peer = enet::enet_host_connect(host, &address, 2, 0);
		if (peer == nullptr)
			return false;

		return true;
	}
	void ENetDisconnect() noexcept {
		if (peer != nullptr) {
			enet::enet_peer_disconnect(peer, 0);
			peer = nullptr;
		}
		if (host != nullptr) {
			enet::enet_host_destroy(host);
			host = nullptr;
		}
	}

	void SendPacketRaw(NetMessage type, const std::byte* data, size_t size) const {
		if (host != nullptr && peer != nullptr) {
			enet::ENetPacket* packet = enet::enet_packet_create(0, size + 5, enet::ENET_PACKET_FLAG_RELIABLE);
			std::memcpy(packet->data, &type, 4);
			std::memcpy(packet->data + 4, data, size);
			enet_peer_send(peer, 0, packet);
		}
	}
	void SendGenericText(const std::string& packet) const {
#ifndef NDEBUG
		std::cout
			<< ">--SendGenericText--<\n"
			<< packet
			<< "\n>-------------------<\n\n";
#endif

		SendPacketRaw(
			NetMessage::GenericText,
			reinterpret_cast<const std::byte*>(packet.data()),
			packet.length()
		);
	}
	void SendGameMessage(const std::string& packet) const {
#ifndef NDEBUG
		std::cout
			<< ">--SendGameMessage--<\n"
			<< packet
			<< "\n>-------------------<\n\n";
#endif

		SendPacketRaw(
			NetMessage::GameMessage,
			reinterpret_cast<const std::byte*>(packet.data()),
			packet.length()
		);
	}
	void SendGamePacket(const GamePacket& packet) const {
#ifndef NDEBUG
		std::cout
			<< ">--SendGamePacket--<\n"
			<< packet
			<< "\n>-----------------<\n\n";
#endif

		SendPacketRaw(
			NetMessage::GameMessage,
			reinterpret_cast<const std::byte*>(&packet),
			sizeof(GamePacket)
		);
	}
protected:
	struct ServerInfo {
		std::string ip;
		uint16_t port;
		int_fast32_t type;
		int_fast32_t type2;
	} server_info = {};

	struct ProxyInfo {
		std::string ip;
		uint16_t port;
		std::string username;
		std::string password;
	} proxy_info = {};

	enet::ENetHost* host = nullptr;
	enet::ENetPeer* peer = nullptr;
};
