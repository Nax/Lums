#ifndef LUMS_INCLUDED_UTIL_SAFE_HANDLE_H
#define LUMS_INCLUDED_UTIL_SAFE_HANDLE_H

namespace lm
{

template <typename T, typename Tag>
class SafeHandle
{
public:
    using Type = T;

    SafeHandle() : _value{} {};
    explicit SafeHandle(T value) : _value{value} {}

    operator bool() const { return !!_value; }
    T value() const { return _value; }

private:
    T _value;
};

template <typename T, typename Tag>
inline bool operator==(SafeHandle<T, Tag> a, SafeHandle<T, Tag> b)
{
    return a.value() == b.value();
}

template <typename T, typename Tag>
inline bool operator!=(SafeHandle<T, Tag> a, SafeHandle<T, Tag> b)
{
    return !(a == b);
}

}

#endif /* LUMS_INCLUDED_UTIL_SAFE_HANDLE_H */
