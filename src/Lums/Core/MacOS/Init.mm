#import <Cocoa/Cocoa.h>

namespace
{

NSAutoreleasePool* sReleasePool;

}

namespace lm
{

void platformInit()
{
    [NSApplication sharedApplication];
    sReleasePool = [[NSAutoreleasePool alloc] init];
}

void platformQuit()
{
    [sReleasePool drain];
}

}
