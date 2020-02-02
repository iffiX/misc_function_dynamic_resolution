#ifndef FUNC_CREATOR_H
#define FUNC_CREATOR_H
#include <boost/preprocessor.hpp>
#include "domain.h"

#define EXPAND(...)  __VA_ARGS__

#define COMMA() ,

#define NONE()

#define EMPTY_LIST_INIT(...)   {}

/**
 * Boost cannot detect an empty tuple (), below is the fix from
 * https://stackoverflow.com/questions/18269381/boost-pp-tuple-size-and-empty-tuple
 */
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _TRIGGER_PARENTHESIS_(...) ,

#define ISEMPTY(...) \
    _ISEMPTY( \
        /* test if there is just one argument, eventually an empty one */ \
        HAS_COMMA(__VA_ARGS__), \
        /* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */ \
        HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
        /* test if the argument together with a parenthesis adds a comma */ \
        HAS_COMMA(__VA_ARGS__ (/*empty*/)), \
        /* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
        HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/)) \
    )

#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,

#define TUPLE_TO_ARGS(...)  __VA_ARGS__
#define PRINT_TUPLE( TUPLE_NAME ) TUPLE_TO_ARGS TUPLE_NAME

#define TUPLE_IS_EMPTY(tuple) ISEMPTY(PRINT_TUPLE(tuple))

///**
// * STR_FOR_EACH accepts any number of tokens and convert them to string
// * @note will not print trailing comma
// */
//// first if creates parenthesis for COMMA, second if creates body, so COMMA() will not be evaluated here
//#define STR_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
//                                                 BOOST_PP_STRINGIZE(token) \
//                                                 BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )
//
//#define STR_FOR_EACH_INNER1(seq) BOOST_PP_SEQ_FOR_EACH_I(STR_FOR_EACH_INNER2, \
//                                                         BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(seq), 1), \
//                                                         seq)
//
//#define STR_FOR_EACH(...) EXPAND(STR_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ((__VA_ARGS__))))


/**
 * ARG_DECL_CREATE accept a tuple of arg name tokens (eg: (a, b, c)), and create argument members for the functor
 */
#define ARG_DECL_CREATE_FOR_EACH_INNER2(r, type, idx, token) type token;

#define ARG_DECL_CREATE_FOR_EACH_INNER1(type, args_seq) BOOST_PP_SEQ_FOR_EACH_I(ARG_DECL_CREATE_FOR_EACH_INNER2, type, args_seq)

#define ARG_DECL_CREATE(type, args) EXPAND(ARG_DECL_CREATE_FOR_EACH_INNER1(type, BOOST_PP_TUPLE_TO_SEQ(args)))

/**
 * ARG_CREATE accept a tuple of arg name tokens (eg: (a, b, c)), and create argument constructor list
 * eg: (after :)  a(), b(), c(), d()
 */
#define ARG_CST_CREATE_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
                                                            token()\
                                                            BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )

#define ARG_CST_CREATE_FOR_EACH_INNER1(args_seq) BOOST_PP_SEQ_FOR_EACH_I(ARG_CST_CREATE_FOR_EACH_INNER2, \
                                                                         BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(args_seq), 1), \
                                                                         args_seq)

#define ARG_CST_CREATE(args) EXPAND(ARG_CST_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(args)))


/**
 * ARG_MAP_CREATE accept a tuple of arg name tokens (eg: (a, b, c)), and create argument member map for the functor
 */

#define ARG_MAP_CREATE_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
                                                            {BOOST_PP_STRINGIZE(token), &token}\
                                                            BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )

#define ARG_MAP_CREATE_FOR_EACH_INNER1(args_seq) BOOST_PP_SEQ_FOR_EACH_I(ARG_MAP_CREATE_FOR_EACH_INNER2, \
                                                                         BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(args_seq), 1), args_seq)

#define ARG_MAP_CREATE_CHECKED(args) {EXPAND(ARG_MAP_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(args)))}

#define ARG_MAP_CREATE(args)  BOOST_PP_IF(TUPLE_IS_EMPTY(args),\
                                          EMPTY_LIST_INIT,\
                                          ARG_MAP_CREATE_CHECKED)(args)

/**
 * ARG_PACK_CREATE accept a tuple of arg name tokens (eg: (a, b, c)), and create argument member packet for the functor
 */

#define ARG_PACK_CREATE_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
                                                             &token\
                                                             BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )

#define ARG_PACK_CREATE_FOR_EACH_INNER1(args_seq) BOOST_PP_SEQ_FOR_EACH_I(ARG_PACK_CREATE_FOR_EACH_INNER2, \
                                                                          BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(args_seq), 1), args_seq)

#define ARG_PACK_CREATE_CHECKED(args) {EXPAND(ARG_PACK_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(args)))}

#define ARG_PACK_CREATE(args)  BOOST_PP_IF(TUPLE_IS_EMPTY(args),\
                                           EMPTY_LIST_INIT,\
                                           ARG_PACK_CREATE_CHECKED)(args)

/**
 * FUNC_DEP_MAP_CREATE accept a tuple of function name tokens (eg: (afunc, bfunc, cfunc)),
 * and create function dependence map for the functor
 */

#define FUNC_DEP_MAP_CREATE_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
                                                                 {BOOST_PP_STRINGIZE(token), func_dep_idx::token}\
                                                                 BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )

#define FUNC_DEP_MAP_CREATE_FOR_EACH_INNER1(deps_seq) BOOST_PP_SEQ_FOR_EACH_I(FUNC_DEP_MAP_CREATE_FOR_EACH_INNER2, \
                                                                              BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(deps_seq), 1), deps_seq)

#define FUNC_DEP_MAP_CREATE_CHECKED(deps)  {EXPAND(FUNC_DEP_MAP_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(deps)))}

#define FUNC_DEP_MAP_CREATE(deps) BOOST_PP_IF(TUPLE_IS_EMPTY(deps),\
                                              EMPTY_LIST_INIT,\
                                              FUNC_DEP_MAP_CREATE_CHECKED)(deps)


#define CALL(name, ...) func_dep[func_dep_idx::name]->call(__VA_ARGS__)
#define BODY(...) (__VA_ARGS__)
#define ARG(...) (__VA_ARGS__)
#define DEP(...) (__VA_ARGS__)

#define FUNC(name, dom, depends, args, body) \
    class __internal_func_##name : public Domain::Function {                        \
    public:                                                                         \
        enum func_dep_idx {                                                         \
            EXPAND depends                                                          \
            BOOST_PP_IF(TUPLE_IS_EMPTY(depends), NONE, COMMA)()                     \
            __func_dep_idx_end                                                      \
        };                                                                          \
        ARG_DECL_CREATE(Domain::Variable, args)        /* create argument members*/ \
                                                                                    \
        explicit __internal_func_##name(Domain &d) :                                \
            ARG_CST_CREATE(args),                                                   \
            Domain::Function(d) {                                                   \
            arg_map = ARG_MAP_CREATE(args);                                         \
            arg_pack = ARG_PACK_CREATE(args);                                       \
            func_dep_map = FUNC_DEP_MAP_CREATE(depends);                            \
            func_dep.resize(__func_dep_idx_end);                                    \
        }                                                                           \
        bool operator()() override {                                                \
            EXPAND body                                                             \
        }                                                                           \
    };                                                                              \
    dom.registerFunction(BOOST_PP_STRINGIZE(name), new __internal_func_##name(dom)) \

#endif //FUNC_CREATOR_H
