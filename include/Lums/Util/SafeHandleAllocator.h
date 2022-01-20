#ifndef LUMS_INCLUDED_UTIL_SAFE_HANDLE_ALLOCATOR_H
#define LUMS_INCLUDED_UTIL_SAFE_HANDLE_ALLOCATOR_H

#include <cstddef>
#include <vector>

namespace lm
{

template <typename T>
class SafeHandleAllocator
{
public:
    SafeHandleAllocator() : _size{1} {}

    std::size_t size() const { return _size; }

    T alloc()
    {
        T val;

        if (_freeList.empty())
        {
            val = T((typename T::Type)_size++);
        }
        else
        {
            val = _freeList.back();
            _freeList.pop_back();
        }

        return val;
    }

    void dealloc(T val)
    {
        if (val)
        {
            _freeList.push_back(val);
        }
    }

private:
    std::size_t     _size;
    std::vector<T>  _freeList;
};

}

#endif /* LUMS_INCLUDED_UTIL_SAFE_HANDLE_ALLOCATOR_H */
