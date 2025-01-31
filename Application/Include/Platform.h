#pragma once

//----------------------------------------------------------------------
// Declare C Standard Library functions with C linkage.
// These functions are used for memory operations.
//----------------------------------------------------------------------
extern "C"
{
    // Copies 'size' bytes from source to destination.
    void* __cdecl memcpy(void* dest, const void* source, size_t size);

    // Sets 'size' bytes of the memory block pointed to by ptr to the specified value.
    void* __cdecl memset(void* ptr, int value, size_t size);

#ifdef _WIN32
// On Windows platforms, use intrinsic versions for performance.
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#endif
}

//----------------------------------------------------------------------
// Define integer types and machine-specific types based on the compiler/platform.
//----------------------------------------------------------------------

#if defined(_MSC_VER)

// Microsoft Visual C++ specific type definitions.
typedef signed char int8;
typedef short int16;
typedef int int32;
typedef __int64 int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;

// For 64-bit architectures (x64 or ARM64) on MSVC.
#if defined(_M_X64) || defined(_M_ARM64)
typedef __int64 machine;
typedef unsigned __int64 umachine;
typedef unsigned __int64 machine_address;
#else
typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;
#endif

// Define the restrict keyword for MSVC.
#define restrict __restrict

#elif defined(__ORBIS__) || defined(__PROSPERO__)

// For platforms like Orbis or Prospero.
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

// For GCC and compatible compilers.
typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// Define machine-specific types based on architecture.
#if defined(__LP64__) || defined(_WIN64)
typedef long long machine;
typedef unsigned long long umachine;
typedef unsigned long long machine_address;
#else
typedef int machine;
typedef unsigned int umachine;
typedef unsigned int machine_address;
#endif

// If the restrict keyword is not defined, define it.
#if !defined(restrict)
#define restrict __restrict__
#endif

#endif

//----------------------------------------------------------------------
// Undefine potential conflicting macros.
//----------------------------------------------------------------------
#undef CopyMemory
#undef FillMemory
#undef ClearMemory

//----------------------------------------------------------------------
// Inline memory helper functions.
//----------------------------------------------------------------------

/**
 * @brief Copies memory from source to destination.
 *
 * @param source Pointer to the source memory block.
 * @param dest Pointer to the destination memory block.
 * @param size Number of bytes to copy.
 */
inline void CopyMemory(const void* source, void* dest, uint32 size)
{
    memcpy(dest, source, size);
}

/**
 * @brief Fills memory with a specified value.
 *
 * @param ptr Pointer to the memory block.
 * @param size Number of bytes to fill.
 * @param value The value to set each byte to.
 */
inline void FillMemory(void* ptr, uint32 size, uint8 value)
{
    memset(ptr, value, size);
}

/**
 * @brief Clears memory by setting all bytes to 0.
 *
 * @param ptr Pointer to the memory block.
 * @param size Number of bytes to clear.
 */
inline void ClearMemory(void* ptr, uint32 size)
{
    memset(ptr, 0, size);
}
