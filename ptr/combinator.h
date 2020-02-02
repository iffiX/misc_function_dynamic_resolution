#ifndef COMBINATOR_H
#define COMBINATOR_H

#include <array>
template <typename Int, Int begin, Int end, Int step>
class RangeEnumerator {
public:
    typedef Int out_type;

    bool finished() {
        return _current + step > end;
    }

    void reset() {
        _current = begin;
    }

    void next() {
        if (_current + step <= end)
            _current += step;
    }

    Int get() {
        return _current;
    }

private:
    Int _current = begin;
};

template <typename... T>
constexpr auto make_array(T&&... values) ->
    std::array<typename std::decay<typename std::common_type<T...>::type>::type,
        sizeof...(T)> {
    return std::array<
            typename std::decay<typename std::common_type<T...>::type>::type,
            sizeof...(T)>{std::forward<T>(values)...};
}

template <typename Value, size_t N>
class ArrayEnumerator {
public:
    typedef Value out_type;
    ArrayEnumerator() = delete;

    template <typename... V>
    ArrayEnumerator(V && ... values) : _data(make_array(std::forward<V>(values)...)) {};

    bool finished() {
        return _current == _data.size() - 1;
    }

    void reset() {
        _current = 0;
    }

    void next() {
        if (!finished())
            _current += 1;
    }

    Value get() {
        return _data[_current];
    }

private:
    size_t _current = 0;
    std::array<typename std::decay<Value>::type, N> _data;
};

template <typename E1, typename E2, typename E3, typename E4, typename E5,
          typename E6, typename E7, typename E8, typename E9, typename E10>
class Combinator {
public:
    E1 e1; E2 e2; E3 e3; E4 e4; E5 e5;
    E6 e6; E7 e7; E8 e8; E9 e9; E10 e10;

    typedef std::tuple<
            typename E1::out_type, typename E2::out_type, typename E3::out_type,
            typename E4::out_type, typename E5::out_type, typename E6::out_type,
            typename E7::out_type, typename E8::out_type, typename E9::out_type,
            typename E10::out_type> out_type;

    Combinator() = delete;
    Combinator(E1 &e1, E2 &e2, E3 &e3, E4 &e4, E5 &e5,
               E6 &e6, E7 &e7, E8 &e8, E9 &e9, E10 &e10) :
               e1(e1), e2(e2), e3(e3), e4(e4), e5(e5), e6(e6), e7(e7), e8(e8), e9(e9), e10(e10) {}

    bool finished() {
        return e1.finished() && e2.finished() && e3.finished() && e4.finished() && e5.finished()
               && e6.finished() && e7.finished() && e8.finished() && e9.finished() && e10.finished();
    }

    void reset() {
        e1.reset(); e2.reset(); e3.reset(); e4.reset(); e5.reset();
        e6.reset(); e7.reset(); e8.reset(); e9.reset(); e10.reset();
    }

    void next() {
        out_type result;
//        if (finished())
//            return;
        if (e10.finished()) {
            e10.reset();
            if (e9.finished()) {
                e9.reset();
                if (e8.finished()) {
                    e8.reset();
                    if (e7.finished()) {
                        e7.reset();
                        if (e6.finished()) {
                            e6.reset();
                            if (e5.finished()) {
                                e5.reset();
                                if (e4.finished()) {
                                    e4.next();
                                    if (e3.finished()) {
                                        e3.reset();
                                        if (e2.finished()) {
                                            e2.reset();
                                            if (e1.finished()) {
                                                e1.reset();
                                            }
                                            else
                                                e1.next();
                                        }
                                        else
                                            e2.next();
                                    }
                                    else
                                        e3.next();
                                }
                                else
                                    e4.next();
                            }
                            else
                                e5.next();
                        }
                        else
                            e6.next();
                    }
                    else
                        e7.next();
                }
                else
                    e8.next();
            }
            else
                e9.next();
        }
        else
            e10.next();
    }

    out_type get() {
        return std::make_tuple(e1.get(), e2.get(), e3.get(), e4.get(), e5.get(),
                               e6.get(), e7.get(), e8.get(), e9.get(), e10.get());
    }
};

#endif //COMBINATOR_H
