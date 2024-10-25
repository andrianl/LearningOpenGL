#pragma once

extern "C"
{
    void *__cdecl memcpy(void *, const void *, size_t);
    void *__cdecl memset(void *, int, size_t);

#ifdef _WIN32
    #pragma intrinsic(memcpy)
    #pragma intrinsic(memset)
#endif
}

#if defined(_MSC_VER)

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef __int64 int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;

#if defined(_M_X64) || defined(_M_ARM64)

typedef __int64 machine;
typedef unsigned __int64 umachine;
typedef unsigned __int64 machine_address;

#else

typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;

#endif

#define restrict __restrict

#elif defined(__ORBIS__) || defined(__PROSPERO__)

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef long machine;
typedef unsigned long umachine;
typedef unsigned long machine_address;

#elif defined(__GNUC__)

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#if defined(__LP64__) || defined(_WIN64)

typedef long long machine;
typedef unsigned long long umachine;
typedef unsigned long long machine_address;

#else

typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;

#endif

#if !defined(restrict)

#define restrict __restrict__

#endif

#endif

inline machine_address GetPointerAddress(const volatile void *ptr)
{
    return (reinterpret_cast<machine_address>(ptr));
}

#undef CopyMemory
#undef FillMemory
#undef ClearMemory

inline void CopyMemory(const void *source, void *dest, uint32 size)
{
    memcpy(dest, source, size);
}

inline void FillMemory(void *ptr, uint32 size, uint8 value)
{
    memset(ptr, value, size);
}

inline void ClearMemory(void *ptr, uint32 size)
{
    memset(ptr, 0, size);
}