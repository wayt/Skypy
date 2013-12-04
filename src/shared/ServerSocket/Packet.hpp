#ifndef PACKET_H_
# define PACKET_H_

#include <vector>
#include <cstring>
#include "Utils.hpp"
#include "Opcodes.hpp"
#include "SharedDefines.h"

#define PACKET_DEFAULT_SIZE 512
#define PACKET_HEADER_SIZE 4

class Packet
{
public:
    Packet(uint16 code, uint32 size = PACKET_DEFAULT_SIZE) :
        _rpos(4), _wpos(2)
    {
        _storage.reserve(size + 4);
        *this << uint16(code);
    }

    Packet(uint16 code, uint8 buff[], uint16 size) :
        _rpos(4), _wpos(0)
    {
        _storage.reserve(size + 4);
        *this << uint16(size);
        *this << uint16(code);
        append(buff, size);
    }

    Packet(uint32 size = PACKET_DEFAULT_SIZE) :
        _rpos(4), _wpos(4)
    {
        _storage.reserve(size + 4);
    }

    void setOpcode(uint16 code)
    {
        uint16 saved = _wpos;
        _wpos = 2;
        *this << uint16(code);
        _wpos = saved;
    }

    Packet& operator<<(uint8 value)
    {
        append<uint8>(value);
        return *this;
    }

    Packet& operator<<(bool value)
    {
        uint8 val = value ? 1 : 0;
        append<uint8>(val);
        return *this;
    }

    Packet& operator<<(uint16 value)
    {
        Utils::endian::native_to_big<uint16>(value);
        append<uint16>(value);
        return *this;
    }

    Packet& operator<<(uint32 value)
    {
        Utils::endian::native_to_big<uint32>(value);
        append<uint32>(value);
        return *this;
    }

    Packet& operator<<(uint64 value)
    {
        Utils::endian::native_to_big<uint64>(value);
        append<uint64>(value);
        return *this;
    }

    Packet& operator<<(int8 value)
    {
        append<int8>(value);
        return *this;
    }

    Packet& operator<<(int16 value)
    {
        Utils::endian::native_to_big<int16>(value);
        append<int16>(value);
        return *this;
    }

    Packet& operator<<(int32 value)
    {
        Utils::endian::native_to_big<int32>(value);
        append<int32>(value);
        return *this;
    }

    Packet& operator<<(int64 value)
    {
        Utils::endian::native_to_big<int64>(value);
        append<int64>(value);
        return *this;
    }

    Packet& operator<<(float value)
    {
        Utils::endian::native_to_big<float>(value);
        append<float>(value);
        return *this;
    }

    Packet& operator<<(double value)
    {
        Utils::endian::native_to_big<double>(value);
        append<double>(value);
        return *this;
    }

    Packet& operator<<(std::string const& value)
    {
        append((uint8 const*)value.c_str(), value.length());
        append((uint8)0);
        return *this;
    }

    Packet& operator>>(std::string& value)
    {
        value.clear();
        while (_rpos < size())
        {
            char c = read<char>();
            if (c == 0)
                break;
            value += c;
        }
        return *this;
    }

    Packet& operator>>(uint8& value)
    {
        value = read<uint8>();
        return *this;
    }

    Packet& operator>>(bool& value)
    {
        uint8 val = read<uint8>();
        value = (val == 0 ? false : true);
        return *this;
    }

    Packet& operator>>(uint16& value)
    {
        value = read<uint16>();
        return *this;
    }

    Packet& operator>>(uint32& value)
    {
        value = read<uint32>();
        return *this;
    }

    Packet& operator>>(uint64& value)
    {
        value = read<uint64>();
        return *this;
    }

    Packet& operator>>(int8& value)
    {
        value = read<int8>();
        return *this;
    }

    Packet& operator>>(int16& value)
    {
        value = read<int16>();
        return *this;
    }

    Packet& operator>>(int32& value)
    {
        value = read<int32>();
        return *this;
    }

    Packet& operator>>(int64& value)
    {
        value = read<int64>();
        return *this;
    }

    Packet& operator>>(float& value)
    {
        value = read<float>();
        return *this;
    }

    Packet& operator>>(double& value)
    {
        value = read<double>();
        return *this;
    }

    size_t size() const
    {
        return _storage.size();
    }

    const uint8* content() const
    {
        return &_storage[0];
    }

    uint16 getOpcode() const
    {
        return read<uint16>(2);
    }

    static const unsigned int HeaderSize = PACKET_HEADER_SIZE;
    static const unsigned int MaxBodySize = PACKET_DEFAULT_SIZE;

private:
    template<class T>
    void append(T value)
    {
        append((uint8*)&value, sizeof(value));
    }

    void append(const uint8* data, uint32 size)
    {
        if (_storage.size() < _wpos + size)
            _storage.resize(_wpos + size);
        memcpy(&_storage[_wpos], data, size);
        _wpos += size;
        updateSize();
    }

    void updateSize()
    {
        uint16 size = _wpos - 2;
        Utils::endian::native_to_big<uint16>(size);
        memcpy(&_storage[0], (const uint8*)&size, 2);
    }

    template<class T>
    T read()
    {
        T val = read<T>(_rpos);
        _rpos += sizeof(T);
        return val;
    }

    template<class T>
    T read(uint32 pos) const
    {
        T val = *((T const*)&_storage[pos]);
        Utils::endian::big_to_native<T>(val);
        return val;
    }

    std::vector<uint8> _storage;
    uint16 _rpos;
    uint16 _wpos;
};

#endif /* !PACKET_H_ */
