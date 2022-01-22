#ifndef LUMS_INCLUDED_CORE_THREAD_H
#define LUMS_INCLUDED_CORE_THREAD_H

#include <Lums/Util/NonCopyable.h>
#include <Lums/Util/Common.h>

#if defined(LUMS_OS_UNIX)
# include <pthread.h>
#endif

namespace lm
{

class Thread : private NonCopyable
{
public:
#if defined(LUMS_OS_UNIX)
    using Handle = pthread_t*;
#else

#endif
    using Callback = void(*)(void*);

    Thread();
    Thread(Callback cb, void* arg = nullptr);
    Thread(Thread&& other);
    ~Thread();
    Thread& operator=(Thread&& other);

    Handle handle();

    bool valid();
    void start(Callback cb, void* arg = nullptr);
    void join();
    void detach();


private:
    void destroy();

    Handle  _handle;
};

}

#endif
