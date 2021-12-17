#ifndef LM_INCLUDED_CORE_NON_COPYABLE_H
#define LM_INCLUDED_CORE_NON_COPYABLE_H

namespace lm
{

/**
 * Standard utility class to prevent copy.
 */
class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

private:
};

}

#endif /* LM_INCLUDED_CORE_NON_COPYABLE_H */
