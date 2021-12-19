#include <Lums/IO/StreamBuffer.h>

using namespace lm;

StreamBuffer::StreamBuffer(char* buffer, std::size_t len)
: Stream{Stream::Read | Stream::Write | Stream::Binary}
, _cursor{}
, _len{len}
, _buffer{buffer}
{

}

StreamBuffer::StreamBuffer(const char* buffer, std::size_t len)
: Stream{Stream::Read | Stream::Binary}
, _cursor{}
, _len{len}
, _buffer{const_cast<char*>(buffer)}
{

}

StreamBuffer::~StreamBuffer()
{

}
