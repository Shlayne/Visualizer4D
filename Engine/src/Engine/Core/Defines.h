#pragma once

#include <utility>

#define EXPAND(x) x

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) EXPAND(STRINGIFY_IMPL(x))

#define UNUSED(...) static_cast<void>(__VA_ARGS__)

#define BIND_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }
