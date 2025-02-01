// SSEDetector.h
#pragma once

#include <string>

enum class SSELevel
{
    None,
    SSE,
    SSE2,
    SSE3,
    SSSE3,
    SSE4_1,
    SSE4_2,
    AVX,
    AVX2,
    AVX512
};

// Function to get the supported SSE level
SSELevel GetSupportedSSELevel();

// Function to convert SSELevel to string
std::string SSELevelToString(SSELevel level);
