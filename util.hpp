#pragma once

#include <iostream>
#include <type_traits>
#include <tuple>

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