#pragma once

#include <random>
#include <iostream>
#include <type_traits>
#include <tuple>

namespace util
{

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

template<typename T, class=void> struct is_container : std::false_type {};

template<typename T>
struct is_container<T, std::void_t<typename T::iterator>> : std::true_type {};

template<typename T, typename = std::enable_if_t<is_container<T>::value>>
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