#pragma once
#include <iostream>
#include <source_location>
#include <string_view>

namespace Utils
{

inline void core_assert(
    bool condition, std::string_view message, const std::source_location location = std::source_location::current()
)
{
    if (!condition)
    {
        std::cerr << "ASSERTION FAILED: " << message << "\n"
                  << "File: " << location.file_name() << "\n"
                  << "Line: " << location.line() << "\n"
                  << "Function: " << location.function_name() << std::endl;
        std::terminate();
    }
}
} // namespace Utils
