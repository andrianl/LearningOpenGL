// SSEDetector.cpp
#include "SSE.h"

SSELevel GetSupportedSSELevel()
{
    SSELevel level = SSELevel::None;

#if defined(_MSC_VER)
// MSVC specific checks
#if defined(__AVX512F__)
    level = SSELevel::AVX512;
#elif defined(__AVX2__)
    level = SSELevel::AVX2;
#elif defined(__AVX__)
    level = SSELevel::AVX;
#elif defined(_M_IX86_FP)
#if _M_IX86_FP == 2
    level = SSELevel::SSE2;
#elif _M_IX86_FP == 1
    level = SSELevel::SSE;
#else
    level = SSELevel::None;
#endif
#else
    // 64-bit Windows (MSVC enables SSE2 by default)
    level = SSELevel::SSE2;
#endif
#else
// GCC/Clang checks
#if defined(__AVX512F__)
    level = SSELevel::AVX512;
#elif defined(__AVX2__)
    level = SSELevel::AVX2;
#elif defined(__AVX__)
    level = SSELevel::AVX;
#elif defined(__SSE4_2__)
    level = SSELevel::SSE4_2;
#elif defined(__SSE4_1__)
    level = SSELevel::SSE4_1;
#elif defined(__SSSE3__)
    level = SSELevel::SSSE3;
#elif defined(__SSE3__)
    level = SSELevel::SSE3;
#elif defined(__SSE2__)
    level = SSELevel::SSE2;
#elif defined(__SSE__)
    level = SSELevel::SSE;
#else
    level = SSELevel::None;
#endif
#endif

    return level;
}

std::string SSELevelToString(SSELevel level)
{
    switch (level)
    {
        case SSELevel::AVX512: return "AVX-512";
        case SSELevel::AVX2: return "AVX2";
        case SSELevel::AVX: return "AVX";
        case SSELevel::SSE4_2: return "SSE4.2";
        case SSELevel::SSE4_1: return "SSE4.1";
        case SSELevel::SSSE3: return "SSSE3";
        case SSELevel::SSE3: return "SSE3";
        case SSELevel::SSE2: return "SSE2";
        case SSELevel::SSE: return "SSE";
        default: return "None";
    }
}
