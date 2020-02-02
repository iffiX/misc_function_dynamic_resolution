#ifndef FUNC_CREATOR_H
#define FUNC_CREATOR_H
#include <boost/preprocessor.hpp>
#include "domain.h"
#define EXPAND(...)  __VA_ARGS__

#define COMMA() ,

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

#define ARG_MAP_CREATE(args) {EXPAND(ARG_MAP_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(args)))}

/**
 * ARG_PACK_CREATE accept a tuple of arg name tokens (eg: (a, b, c)), and create argument member packet for the functor
 */

#define ARG_PACK_CREATE_FOR_EACH_INNER2(r, size, idx, token) BOOST_PP_IF(idx, (), )\
                                                             &token\
                                                             BOOST_PP_IF(BOOST_PP_LESS(idx, size), COMMA, )

#define ARG_PACK_CREATE_FOR_EACH_INNER1(args_seq) BOOST_PP_SEQ_FOR_EACH_I(ARG_PACK_CREATE_FOR_EACH_INNER2, \
                                                                          BOOST_PP_SUB(BOOST_PP_SEQ_SIZE(args_seq), 1), args_seq)

#define ARG_PACK_CREATE(args) {EXPAND(ARG_PACK_CREATE_FOR_EACH_INNER1(BOOST_PP_TUPLE_TO_SEQ(args)))}




#define CALL(name, ...) domain.call(#name, __VA_ARGS__)
#define BODY(...) (__VA_ARGS__)
#define ARG(...) (__VA_ARGS__)

#define FUNC(name, dom, args, body) \
    class __internal_func_##name : public Domain::Function {                        \
    public:                                                                         \
        ARG_DECL_CREATE(Domain::Variable, args)        /* create argument members*/ \
                                                                                    \
        explicit __internal_func_##name(Domain &d) :                                \
            ARG_CST_CREATE(args),                                                   \
            Domain::Function(d) {                                                   \
            arg_map = ARG_MAP_CREATE(args);                                         \
            arg_pack = ARG_PACK_CREATE(args);                                       \
        }                                                                           \
        bool operator()() override {                                                \
            EXPAND body                                                             \
        }                                                                           \
    };                                                                              \
    dom.registerFunction(BOOST_PP_STRINGIZE(name), new __internal_func_##name(dom)) \

#endif //FUNC_CREATOR_H
