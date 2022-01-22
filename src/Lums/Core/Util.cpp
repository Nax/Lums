#include <cstdio>
#include <cstdarg>
#include <sys/time.h>
#include <Lums/Core/Util.h>

using namespace lm;

namespace
{

const char* const kLogLevelName[] = {
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
};

const char* getTimeStr()
{
    static char buf[64];
    timeval tv;
    tm tm;
    size_t tmp;

    gettimeofday(&tv, nullptr);
    localtime_r(&tv.tv_sec, &tm);
    tmp = strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);
    snprintf(buf + tmp, sizeof(buf) - tmp, ".%6d", tv.tv_usec);
    return buf;
}

void log(int lvl, const char* fmt, va_list ap)
{
    FILE* out;

    out = (lvl >= 2) ? stderr : stdout;
    fprintf(out, "%s %s - ", getTimeStr(), kLogLevelName[(int)lvl]);
    vfprintf(out, fmt, ap);
    fputs("\n", out);
}

}

void lm::Logger::debug(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(0, fmt, ap);
    va_end(ap);
}

void lm::Logger::info(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(1, fmt, ap);
    va_end(ap);
}

void lm::Logger::warn(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(2, fmt, ap);
    va_end(ap);
}

void lm::Logger::error(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(3, fmt, ap);
    va_end(ap);
}
