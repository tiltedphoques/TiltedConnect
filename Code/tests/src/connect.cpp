#include "catch.hpp"

#include "Server.h"
#include "Client.h"

struct TestServer : Server
{
    void OnUpdate() override
    {
    }

    void OnConsume(const void* apData, const uint32_t aSize, void* aCookie) override
    {
        REQUIRE((void*)42 == aCookie);
    }

    void* OnConnection(uint32_t aHandle) override
    {
        return (void*)42;
    }

    void OnDisconnection(void* aCookie) override
    {
        REQUIRE((void*)42 == aCookie);
    }
};

struct TestClient : Client
{
    void OnConsume(const void* apData, const uint32_t aSize) override
    {
        
    }
    void OnConnected() override
    {
        
    }
    void OnDisconnected(EDisconnectReason aReason) override
    {
        
    }
};

TEST_CASE("Connect tests", "[connect]")
{
    TestServer server;
    REQUIRE(server.Host(12547, 20) == true);

    TestClient client;
    REQUIRE(client.Connect("127.0.0.1:12547") == true);

    while(true)
    {
        server.Update();
        client.Update();
    }
}