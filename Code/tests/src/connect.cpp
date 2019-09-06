#include "catch.hpp"

#include "Server.hpp"
#include "Client.hpp"
#include "Packet.hpp"
#include <iostream>

using namespace TiltedPhoques;

struct TestServer final : Server
{
    void OnUpdate() override
    {
        const std::string data("Hello from server!");

		Packet packet(data.size() + 1);
		std::copy_n(data.begin(), data.size() + 1, packet.GetData());

		SendToAll(&packet);
    }

    void OnConsume(const void* apData, const uint32_t aSize, ConnectionId_t aId) override
    {
        std::cout << static_cast<const char*>(apData) << std::endl;
    }

    void OnConnection(ConnectionId_t aId) override
    {
    }

    void OnDisconnection(ConnectionId_t aId) override
    {
    }
};

struct TestClient final : Client
{
    void OnConsume(const void* apData, const uint32_t aSize) override
    {
        //std::cout << static_cast<const char*>(apData) << std::endl;

        const std::string data("Hello from client!");

		Packet packet(data.size() + 1);
		std::copy_n(data.begin(), data.size() + 1, packet.GetData());

        Send(&packet);
    }

    void OnConnected() override
    {
        std::cout << "Connected" << std::endl;
    }

    void OnDisconnected(EDisconnectReason aReason) override
    {
        std::cout << "Disconnected " << aReason << std::endl;
    }
};

TEST_CASE("Connect tests", "[connect]")
{
#ifdef WIN32
    TestServer server;
    REQUIRE(server.Host(12547, 20) == true);
    while (true)
        server.Update();
#else
    TestClient client;
    REQUIRE(client.Connect("127.0.0.1:12547") == true);
    while (true)
        client.Update();
#endif

}