#ifndef DOMAIN_H
#define DOMAIN_H

#include <array>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

/**
 * In the real planner, Value is a variant class, created dynamically(number, string...)
 * or statically (constant symbols) by solvers.
 */

typedef int Value;

class Domain {
public:
    struct Variable {
        /// In the real planner, value is a shared_ptr pointed to a Value class
        Value value;
    };

    class Function {
    public:
        typedef std::unordered_map<std::string, Variable*> ArgMap;
        typedef std::vector<Variable*> ArgPack;
        /**
         * ArgMap provides a bridge between derived Function class argument names
         * and the base Function class, so you may selectively change one argument
         * value. But a typical call to any function will assign arguments one by
         * one to ArgPack.
         *
         * Both ArgMap and ArgPack stores pointers to actual argument objects in
         * derived classes. (eg: "Variable SomeFunction.some_arg")
         */
        ArgMap arg_map;
        ArgPack arg_pack;
        Domain& domain;
        std::function<bool()> func;

        Function() = delete;

        explicit Function(Domain &d) : domain(d){};

        Function(Function &f) : domain(f.domain), func(f.func) {}

        virtual bool operator()() = 0;
    };

public:
    std::unordered_map<std::string, std::shared_ptr<Function>> functions;

    template <typename Str>
    void registerFunction(Str && func_name, Function *instance) {
        if (functions.count(func_name) != 0)
            throw std::runtime_error("Duplicate function name");
        functions[func_name].reset(instance);
    }

    template <typename Str, typename ...Var>
    bool call(Str && func_name, Var && ... args) {
        auto func = functions.find(func_name);
        if (func == functions.end())
            throw std::runtime_error("Function doesn't exist");
        collectArgs((*func->second), (*func->second).arg_pack, 0, args...);
        return (*func->second)();
    }

    template <typename Var, typename ...VarPack>
    void collectArgs(Function &func, std::vector<Variable*> &args, size_t idx, Var && next_arg, VarPack && ... remain_args) {
        if (idx >= args.size())
            throw std::runtime_error("Too many arguments to unpack");
        *args[idx] = next_arg;
        collectArgs(func, args, idx + 1, remain_args...);
    }

    void collectArgs(Function &func, std::vector<Variable*> &args, size_t idx) {}
};


#endif //DOMAIN_H
