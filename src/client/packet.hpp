#ifndef PACKET_HPP
#define PACKET_HPP

#include <vector>
#include <cstring>
#include <QtEndian>
#include <iostream>
#include <iomanip>
#include <QString>

#define PACKET_DEFAULT_SIZE 512
#define PACKET_HEADER_SIZE 4

class Packet
{
public:
    Packet(quint16 code, quint32 size = PACKET_DEFAULT_SIZE) :
        _rpos(4), _wpos(2)
    {
        _storage.reserve(size + 4);
        *this << quint16(code);
    }

    Packet(quint16 code, char buff[], quint16 size) :
        _rpos(4), _wpos(0)
    {
        _storage.reserve(size + 4);
        *this << quint16(size);
        *this << quint16(code);
        append(buff, size);
    }

    void setOpcode(quint16 code)
    {
        quint16 saved = _wpos;
        _wpos = 2;
        *this << quint16(code);
        _wpos = saved;
    }

    Packet& operator<<(quint8 value)
    {
        append<quint8>(value);
        return *this;
    }

    Packet& operator<<(bool value)
    {
        quint8 val = value ? 1 : 0;
        append<quint8>(val);
        return *this;
    }

    Packet& operator<<(quint16 value)
    {
        value = qToBigEndian<quint16>(value);
        append<quint16>(value);
        return *this;
    }

    Packet& operator<<(quint32 value)
    {
        value = qToBigEndian<quint32>(value);
        append<quint32>(value);
        return *this;
    }

    Packet& operator<<(quint64 value)
    {
        value = qToBigEndian<quint64>(value);
        append<quint64>(value);
        return *this;
    }

    Packet& operator<<(qint8 value)
    {
        append<qint8>(value);
        return *this;
    }

    Packet& operator<<(char value)
    {
        append<char>(value);
        return *this;
    }

    Packet& operator<<(qint16 value)
    {
        value = qToBigEndian<qint16>(value);
        append<qint16>(value);
        return *this;
    }

    Packet& operator<<(qint32 value)
    {
        value = qToBigEndian<qint32>(value);
        append<qint32>(value);
        return *this;
    }

    Packet& operator<<(qint64 value)
    {
        value = qToBigEndian<qint64>(value);
        append<qint64>(value);
        return *this;
    }

    Packet& operator<<(float value)
    {
        value = qToBigEndian<float>(value);
        append<float>(value);
        return *this;
    }

    Packet& operator<<(double value)
    {
        value = qToBigEndian<double>(value);
        append<double>(value);
        return *this;
    }

    Packet& operator<<(std::string const& value)
    {
        append((char const*)value.c_str(), value.length());
        append((char)0);
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

    Packet& operator<<(QString const& value)
    {
        append((char const*)value.toLocal8Bit(), value.length());
        append((char)0);
        return *this;
    }

    Packet& operator>>(QString& value)
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

    Packet& operator>>(quint8& value)
    {
        value = read<quint8>();
        return *this;
    }

    Packet& operator>>(bool& value)
    {
        quint8 val = read<quint8>();
        value = (val == 0 ? false : true);
        return *this;
    }

    Packet& operator>>(quint16& value)
    {
        value = read<quint16>();
        return *this;
    }

    Packet& operator>>(quint32& value)
    {
        value = read<quint32>();
        return *this;
    }

    Packet& operator>>(quint64& value)
    {
        value = read<quint64>();
        return *this;
    }

    Packet& operator>>(qint8& value)
    {
        value = read<qint8>();
        return *this;
    }

    Packet& operator>>(qint16& value)
    {
        value = read<qint16>();
        return *this;
    }

    Packet& operator>>(qint32& value)
    {
        value = read<qint32>();
        return *this;
    }

    Packet& operator>>(qint64& value)
    {
        value = read<qint64>();
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

    quint64 size() const
    {
        return _storage.size();
    }

    const char* content() const
    {
        return &_storage[0];
    }

    quint16 getOpcode() const
    {
        return read<quint16>(2);
    }

    static const unsigned int HeaderSize = PACKET_HEADER_SIZE;
    static const unsigned int MaxBodySize = PACKET_DEFAULT_SIZE;

    void dumpHex()
    {
        quint64 size = _storage.size();
        for (quint64 i = 0; i < size; ++i)
        {
            if (i % 4 == 0)
                std::cout << std::endl;
            std::cout << qint32(_storage[i]) << " - ";
        }
        std::cout << std::endl;
    }

private:
    template<class T>
    void append(T value)
    {
        append((char*)&value, sizeof(value));
    }

    void append(const char* data, quint32 size)
    {
        if (_storage.size() < _wpos + size)
            _storage.resize(_wpos + size);
        memcpy(&_storage[_wpos], data, size);
        _wpos += size;
        updateSize();
    }

    void updateSize()
    {
        quint16 size = _wpos - 4;
        size = qToBigEndian<quint16>(size);
        memcpy(&_storage[0], (const quint8*)&size, 2);
    }

    template<class T>
    T read()
    {
        T val = read<T>(_rpos);
        _rpos += sizeof(T);
        return val;
    }

    template<class T>
    T read(quint32 pos) const
    {
        T val = *((T const*)&_storage[pos]);
        if (sizeof(T) > 1)
            val = qFromBigEndian<T>(val);
        return val;
    }

    std::vector<char> _storage;
    quint16 _rpos;
    quint16 _wpos;
};


#endif // PACKET_HPP
