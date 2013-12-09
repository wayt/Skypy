#include "encodedsample.h"

EncodedSample::EncodedSample() :
    _encodedSample()
{
}

EncodedSample::EncodedSample(const QByteArray &data) :
    _encodedSample(data)
{
}

EncodedSample::~EncodedSample()
{
}

void EncodedSample::setEncodedSample(const char *data, int nbBytes)
{
    _encodedSample.clear();
    _encodedSample.resize(nbBytes);

    for (int i = 0; i < nbBytes; ++i)
        _encodedSample[i] = data[i];
}
