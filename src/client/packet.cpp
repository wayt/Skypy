#include "packet.hpp"

template<>
char Packet::read<char>(quint32 pos) const
{
    char val = *((char const*)&_storage[pos]);
    return val;
}
