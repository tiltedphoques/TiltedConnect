#include "catch.hpp"

#include "Server.h"
#include "Client.h"
#include <iostream>

struct TestServer final : Server
{
    void OnUpdate() override
    {
        const std::string data("Hello from server!");
        SendToAll(data.c_str(), data.size() + 1);
    }

    void OnConsume(const void* apData, const uint32_t aSize, void* aCookie) override
    {
        REQUIRE((void*)42 == aCookie);

        std::cout << static_cast<const char*>(apData) << std::endl;
    }

    void* OnConnection(uint32_t aHandle) override
    {
        return reinterpret_cast<void*>(42);
    }

    void OnDisconnection(void* aCookie) override
    {
        REQUIRE((void*)42 == aCookie);
    }
};

struct TestClient final : Client
{
    void OnConsume(const void* apData, const uint32_t aSize) override
    {
        //std::cout << static_cast<const char*>(apData) << std::endl;

        const std::string data("Hello from client!");
        Send(data.c_str(), data.size() + 1);
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