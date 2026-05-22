#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <generate.hpp>

//region Debug output
#include <fmt/format.h>
#include <fmt/ranges.h>
// #include <fmt/color.h>
using fmt::print, fmt::format;

#ifdef DEBUG
    #define ICECREAM_FMT
    #include <icecream.hpp>
#endif
//endregion
//region IceCream output cheat sheet
// - IC() just outputs args
// - IC_F() accepts `fmt` as 1st arg
// - IC_A() accepts `callable` as 1st arg; returns the value from callable
// - IC_FA() mixin: fmt, callable, ... ; returns the value from callable
// - IC_() overrides `fmt` for higher-order IC:
//   IC_F("#X", a, IC_("d", b));
//
// fmt     ::= [[fill]align][sign]["#"][width]["." precision][type]
// type    ::= s(tring) | c(har) | x | X(hex) | a | A(hex float) | f | F(loat) |
//
// fmt     ::= [range][":"fmt]
// range   ::= "[" slicing | index "]"
// slicing ::= [lower] ":" [upper] [ ":" [step] ]
//
// https://github.com/renatoGarcia/icecream-cpp#iostreams-format-string
//endregion
//region Format cheat sheet
// format ::= "{" [arg_id] [":" fmt | chrono] "}"
// fmt    ::= [[fill]align][sign]["#"]["0"][width]["." precision]["L"][type]
// type   ::= s(tring) | c(har) | x | X(hex) | a | A(hex float) | f | F(loat)
//
// fmt    ::= ["n"][range_type][range_underlying_spec]  "n"(o brackets)
//
// range   ::= "[" slicing | index "]"
// slicing ::= [lower] ":" [upper] [ ":" [step] ]
//
// https://fmt.dev/12.0/syntax/
//endregion
//region Read input
// #include <read.h>
// using std::istream;
// istream &operator>>(istream &ins, Tester &t) {
//     t.x = read(ins);
//     t.f = read(ins);
//     return ins;
// }
//endregion
//region Read input cheat sheet
//Read a T from cin, reprompting if they type in something wrong
//Examples:
//int x = read("Enter integer");
//const int x = read<int>("Please enter an int");
//string s = read<string>("What is your name?");
//
// const string name = readline("Please enter your name: ");
//
// you can choose to be notified when a read error occurs
// auto temp = read_opt<unsigned int>("Please enter your age:\n");
// if (!temp or temp > 130) {}
//
//endregion

int main() {
    //region IceCream configuration
    #ifdef DEBUG
    IC_CONFIG
     // .disable()
     // .include_context(true)
     // .context_delimiter(" ")
     .prefix("")
     .output(std::cout)
     .line_wrap_width(-1)
     .show_c_string(true)
     .decay_char_array(true);
    #endif
    //endregion
    using namespace std;

    IC(generate_bool<vector<short>>());
    IC(generate_bool<vector<short>>(10, 1123));
    IC(generate<vector<short>>());
    IC(generate<vector<int>>());
    IC(generate<vector<int>>(10, 1, 12, 1123));
    IC(generate<vector<size_t>>());
    IC(generate<vector<float>>());

    IC(generate<deque<short>>());
    IC(generate<deque<int>>());
    IC(generate<deque<size_t>>());
    IC(generate<deque<float>>());

    IC(generate<queue<short>>());
    IC(generate<queue<int>>());
    IC(generate<queue<size_t>>());
    IC(generate<queue<float>>());

    IC(generate<list<short>>());
    IC(generate<list<int>>());
    IC(generate<list<size_t>>());
    IC(generate<list<float>>());

    return 0;
}
