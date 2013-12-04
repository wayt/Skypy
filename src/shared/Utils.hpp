#ifndef UTILS_H_
# define UTILS_H_

namespace Utils
{
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

}
#endif /* !UTILS_H_ */
