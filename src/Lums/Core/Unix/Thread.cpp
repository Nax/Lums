#include <cstdlib>
#include <Lums/Core/Thread.h>
#include <Lums/Core/Util.h>

using namespace lm;

namespace lm
{

namespace impl
{

#if defined(LUMS_OS_MACOS)
void* threadTrampoline(void* arg);
#else
void* threadTrampoline(void* arg)
{
    void** args = (void**)arg;
    Callback cb = (Callback)args[0];
    void* arg2 = (void*)args[1];
    free(arg);
    cb(arg2);
    return nullptr;
}
#endif

}

}


void Thread::start(Callback cb, void* arg)
{
    destroy();

    _handle = (Handle)malloc(sizeof(pthread_t));

    void** args = (void**)malloc(sizeof(void*) * 2);
    args[0] = (void*)cb;
    args[1] = arg;

    pthread_create(_handle, nullptr, &::lm::impl::threadTrampoline, args);
}

void Thread::join()
{
    if (_handle)
    {
        pthread_join(*_handle, nullptr);
        free(_handle);
        _handle = nullptr;
    }
}

void Thread::detach()
{
    if (_handle)
    {
        pthread_detach(*_handle);
        free(_handle);
        _handle = nullptr;
    }
}

void Thread::destroy()
{
    if (_handle)
    {
        Logger::warn("Thread must either be detached or joined.");
        detach();
    }
}
