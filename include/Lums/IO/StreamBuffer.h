#ifndef LUMS_INCLUDED_IO_STREAM_BUFFER_H
#define LUMS_INCLUDED_IO_STREAM_BUFFER_H

#include <Lums/IO/Stream.h>

namespace lm
{

class LUMS_API_IO StreamBuffer : public Stream
{
public:
    StreamBuffer(char* buffer, std::size_t len);
    StreamBuffer(const char* buffer, std::size_t len);
    ~StreamBuffer();

private:
    std::size_t     _cursor;
    std::size_t     _len;
    char*           _buffer;
};

}

#endif /* LUMS_INCLUDED_IO_STREAM_BUFFER_H */
