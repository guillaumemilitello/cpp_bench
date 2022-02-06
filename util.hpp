#pragma once

#include <random>
#include <iostream>
#include <type_traits>
#include <tuple>

namespace util {

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct tuple_hash
{
    template<typename... Args>
    std::size_t operator() (const std::tuple<Args...>& tuple) const
    {
        size_t seed = 0;
        HashValueImpl<std::tuple<Args...>>::apply(seed, tuple);
        return seed;
    }
private:
    template <class T>
    static void hash_combine(std::size_t& seed, const T& value)
    {
        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine: http://stackoverflow.com/questions/4948780
        seed ^= std::hash<T>()(value) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }

    // Recursive template code derived from Matthieu M.
    template <class T, std::size_t I = std::tuple_size<T>::value-1>
    struct HashValueImpl
    {
        static void apply(std::size_t& seed, const T& tuple)
        {
            HashValueImpl<T, I-1>::apply(seed, tuple);
            hash_combine(seed, std::get<I>(tuple));
        }
    };

    template <class T>
    struct HashValueImpl<T,0>
    {
        static void apply(std::size_t& seed, const T& tuple)
        {
            hash_combine(seed, std::get<0>(tuple));
        }
    };
};

template<typename T>
struct generate_random
{
    T _min, _max;
public:
    explicit generate_random(const T& min, const T& max) : _min(min), _max(max) {}
    T operator()() { return _min + rand() % (( _max + 1 ) - _min); }
};

template<typename T>
std::vector<T> get_random_vector(size_t size, const T& min, const T& max)
{
    std::vector<T> v(size);
    std::generate_n(v.begin(), size, generate_random(min, max));
    return v;
}

template<typename T, class=void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T, std::void_t<typename T::iterator>> : std::true_type {};

} // namespace util

template<typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T, S>& v)
{
    os << "(" << v.first << "," << v.second << ")";
    return os;
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, std::tuple<Args...> const& t)
{
  bool first = true;
  os << "(";
  apply([&](auto&&... args){ ((os << (first ? "" : ",") << args, first = false), ...); }, t);
  os << ")";
  return os;
}

template<typename T, typename = std::enable_if_t<util::is_container<T>::value>>
std::ostream& operator<<(std::ostream& os, const T& c)
{
    os << "[";
    size_t i=0;
    for (const auto& v : c)
    {
        os << v;
        if (i++ < c.size() - 1)
        {
            os << ",";
        }
    }
    os << "]";
    return os;
}
