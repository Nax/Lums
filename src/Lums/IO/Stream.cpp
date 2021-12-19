#include <Lums/IO/Stream.h>

using namespace lm;

Stream::Stream(int mode)
: _mode{mode}
{

}

Stream::~Stream()
{

}

long Stream::read(char* dst, std::size_t len)
{
    return -1;
}

long Stream::write(const char* src, std::size_t len)
{
    return -1;
}

long Stream::_read(char* dst, std::size_t len)
{
    return -1;
}

long Stream::_write(const char* src, std::size_t len)
{
    return -1;
}

