#include <Lums/Core/Thread.h>

using namespace lm;

Thread::Thread()
: _handle{}
{

}

Thread::Thread(Callback cb, void* arg)
{
    start(cb, arg);
}

Thread::Thread(Thread&& other)
: _handle{other._handle}
{
    other._handle = nullptr;
}

Thread::~Thread()
{
    destroy();
}

Thread& Thread::operator=(Thread&& other)
{
    destroy();
    _handle = other._handle;
    other._handle = nullptr;
    return *this;
}

Thread::Handle Thread::handle()
{
    return _handle;
}

bool Thread::valid()
{
    return !!_handle;
}
