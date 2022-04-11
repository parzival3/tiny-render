#pragma once
#if defined(_MSC_VER)
#pragma warning(push, 1)
#pragma warning(disable : 4388)
#pragma warning(disable : 4625)
#pragma warning(disable : 4623)
#pragma warning(disable : 4626)
#pragma warning(disable : 4643)
#pragma warning(disable : 5031)
#pragma warning(disable : 4820)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)
#pragma warning(disable : 5041)
#pragma warning(disable : 5219)
#pragma warning(disable : 4582)
#pragma warning(disable : 4710)
#include <range/v3/all.hpp>
#pragma warning(pop)
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <range/v3/all.hpp>
#pragma GCC diagnostic pop
#else
#include <range/v3/all.hpp>
#endif
