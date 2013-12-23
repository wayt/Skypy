#ifndef UTILS_H_
# define UTILS_H_

#include "SharedDefines.h"

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#ifdef UNIX
# include <sys/time.h>
#else
# define NOMINMAX
# include <time.h>
# include <Winsock2.h>
# include <sys/types.h>
# include <sys/timeb.h>
#endif
#include <cmath>
#include <limits>

#include <sstream>
#include <vector>

namespace Utils
{

    inline uint32 getMSTime()
    {
        timeval time;
#ifdef UNIX
        gettimeofday(&time, NULL);
#else
        struct _timeb timebuffer;
        _ftime_s (&timebuffer);
        time.tv_sec = (long)timebuffer.time;
        time.tv_usec = timebuffer.millitm * 1000;
#endif
        return (time.tv_sec * 1000) + (time.tv_usec / 1000);
    }

    inline uint32 getTime()
    {
        return time(NULL);
    }

    inline uint32 getMSTimeDiff(uint32 prev, uint32 now)
    {
        return now - prev;
    }

    inline uint32 getMSTimeDiffToNow(uint32 time)
    {
        return getMSTimeDiff(time, getMSTime());
    }

    template<class T>
    inline T to(char const* buff)
    {
        T val;
        std::stringstream ss;
        ss << buff;
        ss >> val;
        return val;
    }

    template<class T>
    inline T to(std::string const& str)
    {
        return to<T>(str.c_str());
    }

    inline void intToString(int in, std::string& val)
    {
        std::stringstream ss;
        ss << in;
        ss >> val;
    }

    static  std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    static std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string &trim(std::string &s) { return ltrim(rtrim(s)); }
    inline void trim_quote(std::string& s)
    {
        if (s.size() < 2)
            return;
        if (s[0] == '"' && s[s.size() - 1] == '"')
            s = s.substr(1, s.size() - 2);
    }

    inline void split(std::string const& s, char c, std::vector<std::string> &elems)
    {
        std::istringstream ss(s);
        std::string item;
        while (std::getline(ss, item, c))
            elems.push_back(trim(item));
    }

class endian
{
private:
    static bool __is_big()
    {
        int i = 1;
        char* ptr = (char*)&i;
        return *ptr == 1 ? false : true;
    }
    public:

    template<class T>
    static void big_to_native(T& u)
    {
        if (__is_big())
            return;

        union
        {
            T u;
            char u8[sizeof(T)];
        } src, dst;

        src.u = u;

        for (size_t i = 0; i < sizeof(T); ++i)
            dst.u8[i] = src.u8[sizeof(T) - i - 1];

        u = dst.u;
    }

    template<class T>
    static void native_to_big(T& u)
    {
        if (__is_big())
            return;

        union
        {
            T u;
            char u8[sizeof(T)];
        } src, dst;

        src.u = u;

        for (size_t i = 0; i < sizeof(T); ++i)
            dst.u8[i] = src.u8[sizeof(T) - i - 1];

        u = dst.u;
    }
};

class Timer
{
public:
    Timer(uint32 interval) : _internalTimer(interval * IN_MILLISECONDS), _baseInterval(interval * IN_MILLISECONDS) {}
    void update(uint32 const diff)
    {
        if (_internalTimer <= diff)
            _internalTimer = 0;
        else
            _internalTimer -= diff;
    }

    void reset() { _internalTimer = _baseInterval; }
    bool passed() const { return (_internalTimer == 0); }
private:
    uint32 _internalTimer;
    uint32 _baseInterval;
};

}
#endif /* !UTILS_H_ */
