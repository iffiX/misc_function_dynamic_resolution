#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "domain.h"
#include "combinator.h"
#include "func_creator.h"

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace std;

#define R1 100
#define R2 100
#define R3 100

void module_1_init(Domain &d) {
    FUNC(some_circuit, d,\
         ARG(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10), \
         BODY(
             return CALL(and_circuit, a1, a2)
                 && CALL(and_circuit, a3, a4)
                 && CALL(and_circuit, a5, a6)
                 && !CALL(not_circuit, a7)
                 && a8.value >= R1 && a9.value >= R2 && a10.value >= R3;
         )
    );
}

void module_2_init(Domain &d) {
    /// another module, which contains all the functions referenced by module 1
    FUNC(and_circuit, d,\
         ARG(in1, in2), \
         BODY(
            return in1.value && in2.value;
    )
    );
    FUNC(or_circuit, d,\
         ARG(in1, in2), \
         BODY(
            return in1.value || in2.value;
    )
    );
    FUNC(not_circuit, d,\
         ARG(in1), \
         BODY(
            return !in1.value;
    )
    );
    FUNC(xor_circuit, d,\
         ARG(in1, in2), \
         BODY(
            return in1.value != in2.value;
    )
    );
}

void module_foo_init(Domain &d) {
    /// represent other modules, create some foo functions
    class FooFunction : public Domain::Function {
    public:
        explicit FooFunction(Domain &d) : Domain::Function(d) {}
        bool operator()() override {};
    };

    for (int i=0; i<10000; i++) {
        d.registerFunction(fmt::format("func_foo_{}", i), new FooFunction(d));
    }
}


int main() {
    Domain d;
    module_1_init(d);
    module_2_init(d);
    module_foo_init(d);


    typedef RangeEnumerator<int, 0, 1, 1> bool_enum;
    bool_enum x1, x2, x3, x4, x5, x6, x7;
    RangeEnumerator<int, 1, R1, 1> x8;
    RangeEnumerator<int, 1, R2, 1> x9;
    RangeEnumerator<int, 1, R3, 1> x10;
    Domain::Variable v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;

    auto comb = make_combinator(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10);

    bool found;


    found = false;
    while (!comb.finished()) {
        auto c = std::move(comb.get());
        /// a single satisfaction search step (worst case, no heuristic, all combinations must be enumerated
        v1.value = std::get<0>(c);
        v2.value = std::get<1>(c);
        v3.value = std::get<2>(c);
        v4.value = std::get<3>(c);
        v5.value = std::get<4>(c);
        v6.value = std::get<5>(c);
        v7.value = std::get<6>(c);
        v8.value = std::get<7>(c);
        v9.value = std::get<8>(c);
        v10.value = std::get<9>(c);

        if (d.call("some_circuit", v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)) {
            found = true;
            break;
        }
        else
            comb.next();
    }
    if (found) {
        fmt::print("found: {}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n",
                   v1.value, v2.value, v3.value, v4.value, v5.value,
                   v6.value, v7.value, v8.value, v9.value, v10.value);
    }
}