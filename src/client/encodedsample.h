#ifndef ENCODEDSAMPLE_H
#define ENCODEDSAMPLE_H

#include <QByteArray>

class EncodedSample
{
public:
    EncodedSample();
    EncodedSample(const QByteArray &data);
    virtual ~EncodedSample();

    inline int size() const { return _encodedSample.size(); }

    inline bool isValid() const { return !_encodedSample.isNull(); }

    void setEncodedSample(const char *data, int nbBytes);
    inline void setEncodedSample(const unsigned char *data, int nbBytes) { setEncodedSample((const char*)data, nbBytes); }
    inline void setEncodedSample(const QByteArray &encodedSample) { _encodedSample = encodedSample; }
    inline const QByteArray& encodedSample() const { return _encodedSample; }

    inline const char* data() const { return _encodedSample.constData(); }
    inline const unsigned char* udata() const { return (const unsigned char*)_encodedSample.data(); }

    inline char operator[](int idx) const { return _encodedSample[idx]; }

private:
    QByteArray _encodedSample;
};

#endif // ENCODEDSAMPLE_H
