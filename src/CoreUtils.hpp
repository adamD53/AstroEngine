#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>

#include <exception>
#include <filesystem>
#include <source_location>
#include <string_view>

namespace Utils
{

inline void core_assert(
    const bool condition,
    const std::string_view message,
    const std::source_location location = std::source_location::current()
)
{
    if (!condition)
    {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_APPLICATION,
            "ASSERTION FAILED: %.*s | SDL Error: %s\n  File: %s:%u (%s)",
            static_cast<int>(message.size()),
            message.data(),
            SDL_GetError(),
            location.file_name(),
            location.line(),
            location.function_name()
        );
        std::terminate();
    }
}

inline auto ResolveAssetPath(const std::filesystem::path& relativePath) -> std::filesystem::path
{
    if (std::filesystem::exists(relativePath))
    {
        return relativePath;
    }

    auto* basePathRaw = SDL_GetBasePath();
    if (basePathRaw != nullptr)
    {
        const auto basePath = std::filesystem::path(basePathRaw);
        SDL_free(const_cast<char*>(basePathRaw));

        const auto candidateA = basePath / relativePath;
        if (std::filesystem::exists(candidateA))
        {
            return candidateA;
        }

        const auto candidateB = basePath.parent_path() / relativePath;
        if (std::filesystem::exists(candidateB))
        {
            return candidateB;
        }
    }

    const auto candidateParent = std::filesystem::path("..") / relativePath;
    if (std::filesystem::exists(candidateParent))
    {
        return candidateParent;
    }

    return relativePath;
}

} // namespace Utils
