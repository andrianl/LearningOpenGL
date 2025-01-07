#include <iostream>
#include <new>

// Structure to store memory information
struct MemoryInfo
{
    std::size_t totalAllocated = 0;
    std::size_t totalFreed = 0;
    std::size_t allocationCount = 0;    // Number of memory allocations
    std::size_t deallocationCount = 0;  // Number of memory deallocations
    std::size_t getUsedMemory() const { return totalAllocated - totalFreed; }
};

// Global variable to store memory information
MemoryInfo memoryInfo;

// Overloaded new operator
void* operator new(std::size_t size)
{
    //std::cout << "Memory Allocation: " << size << " bytes" << std::endl;
    memoryInfo.totalAllocated += size;  // Update total allocated memory
    memoryInfo.allocationCount++;       // Update number of memory allocations
    void* p = std::malloc(size);
    if (!p)
    {
        throw std::bad_alloc();
    }
    return p;
}

// Overloaded delete operator
void operator delete(void* p, std::size_t size) noexcept
{
    //std::cout << "Free Memory: " << size << " bytes" << std::endl;
    memoryInfo.totalFreed += size;   // Update total freed memory
    memoryInfo.deallocationCount++;  // Update number of memory deallocations
    std::free(p);
}

// Function to get memory information
MemoryInfo& getMemoryInfo()
{
    return memoryInfo;
}
