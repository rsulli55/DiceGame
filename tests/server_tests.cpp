
#include <catch2/catch.hpp>
#include "../src/server/netserver.h"
#include "../src/client/netclient.h"

/* NOTE: This test assumes that the server is already runnning */
/* For that reason, it is not automatically included with the `tests` target */

TEST_CASE("Trial runs of game", "[gameserver]") {
    spdlog::set_level(spdlog::level::debug);

    int num_clients = 4;
    std::vector<qdg::NetClient> clients;

    for (int i = 0; i < num_clients; ++i) {
        clients.emplace_back();
    }
}


