#include <Cocoa/Cocoa.h>
#include <Lums/Core/Util.h>
#include <Lums/Core/Thread.h>

using namespace lm;

namespace lm
{

namespace impl
{

void* threadTrampoline(void* arg)
{
    void** args = (void**)arg;
    Thread::Callback cb = (Thread::Callback)args[0];
    void* arg2 = (void*)args[1];
    free(arg);

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    cb(arg2);
    [pool drain];

    return nullptr;
}

}

}
