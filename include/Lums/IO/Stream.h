#ifndef LUMS_INCLUDED_IO_STREAM_H
#define LUMS_INCLUDED_IO_STREAM_H

#include <cstdint>
#include <cstddef>
#include <Lums/IO/ABI.h>
#include <Lums/Util/NonCopyable.h>

namespace lm
{

class LUMS_API_IO Stream : private NonCopyable
{
public:
    enum Mode : std::uint8_t
    {
        Read = 0x01,
        Write = 0x02,
        Binary = 0x04,
    };

    Stream(int mode);
    virtual ~Stream();

    long read(char* dst, std::size_t len);
    long write(const char* src, std::size_t len);

protected:
    virtual long _read(char* dst, std::size_t len) = 0;
    virtual long _write(const char* src, std::size_t len) = 0;

    int _mode;
};

}

#endif /* LUMS_INCLUDED_IO_STREAM_H */
