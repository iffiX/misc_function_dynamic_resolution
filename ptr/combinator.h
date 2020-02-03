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

namespace Helper
{
    template<size_t... Is>
    struct Seq { };

    template<size_t N, size_t... Is>
    struct GenSeq : GenSeq<N - 1, N - 1, Is...> { };

    template<size_t... Is>
    struct GenSeq<0, Is...> : Seq<Is...> { };
}

template <typename ...Enumerator>
class Combinator {
public:
    std::tuple<typename std::decay<Enumerator>::type...> enumerators;

    typedef std::tuple<typename std::decay<Enumerator>::type::out_type...> out_type;

    Combinator(typename std::decay<Enumerator>::type &... enums) :
            enumerators(std::make_tuple(std::move(enums)...)){}

    bool finished() {
        return _enumFinished<sizeof...(Enumerator)-1>();
    }

    void reset() {
        _enumReset<sizeof...(Enumerator)-1>();
    }

    void next() {
        _enumNext<sizeof...(Enumerator)-1>();
    }

    out_type get() {
        return std::move(_enumGetForEach(Helper::GenSeq<sizeof...(Enumerator)>()));
    }

private:
    template <size_t idx, typename std::enable_if<(idx > 0), int>::type = 0>
    bool _enumFinished() {
        return std::get<idx>(enumerators).finished() && _enumFinished<idx-1>();
    }

    template <size_t idx, typename std::enable_if<idx == 0, int>::type = 0>
    bool _enumFinished(){
        return std::get<idx>(enumerators).finished();
    }

    template <size_t idx, typename std::enable_if<(idx > 0), int>::type = 0>
    bool _enumReset() {
        std::get<idx>(enumerators).reset();
        _enumReset<idx-1>();
    }

    template <size_t idx, typename std::enable_if<idx == 0, int>::type = 0>
    bool _enumReset(){
        std::get<idx>(enumerators).reset();
    }

    template <size_t idx, typename std::enable_if<(idx > 0), int>::type = 0>
    bool _enumNext() {
        if (std::get<idx>(enumerators).finished()) {
            std::get<idx>(enumerators).reset();
            _enumNext<idx-1>();
        }
        else
            std::get<idx>(enumerators).next();
    }

    template <size_t idx, typename std::enable_if<idx == 0, int>::type = 0>
    bool _enumNext(){
        if (std::get<idx>(enumerators).finished()) {
            std::get<idx>(enumerators).reset();
        }
        else
            std::get<idx>(enumerators).next();
    }

    template <size_t ...indexes>
    out_type _enumGetForEach(Helper::Seq<indexes...>)
    {
        return std::move(std::make_tuple(std::get<indexes>(enumerators).get()...));
    }
};

#endif //COMBINATOR_H
