#ifndef LUMS_INNCLUDED_CORE_UTIL_H
#define LUMS_INNCLUDED_CORE_UTIL_H

namespace lm
{

namespace Logger
{
    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void warn(const char* fmt, ...);
    void error(const char* fmt, ...);
}

}

#endif
