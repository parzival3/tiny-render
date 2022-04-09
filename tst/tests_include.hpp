#pragma once
#if defined(_MSC_VER)
#pragma warning(push, 1)
#pragma warning(disable : 4388)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 5027)
#pragma warning(disable : 4820)
#pragma warning(disable : 5026)
#pragma warning(disable : 4820)
#include "gtest/gtest.h"
#pragma warning(pop)
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop
#else
#include "gtest/gtest.h"
#endif
