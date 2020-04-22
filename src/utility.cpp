#include "utility.h"

namespace qdg {
    int roll_n_sided_die(int n) {
        static std::random_device rd;
        static std::mt19937 rng {rd()};
        static std::uniform_int_distribution<int> dist {1, 6};
        
        auto param = std::uniform_int_distribution<int>::param_type {1, n};

        return dist(rng, param);
    }

    int roll_one_die() {
        return roll_n_sided_die(6);
    }
}
