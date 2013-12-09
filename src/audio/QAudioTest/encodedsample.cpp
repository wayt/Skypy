#include <QtGlobal>

#include "encodedsample.h"

EncodedSample::EncodedSample() :
    _gain(0),
    _encodedSample(),
    _encoder(NULL),
    _decoder(NULL),
    _errText(),
    _channelCount(0)
{
}

EncodedSample::~EncodedSample()
{
    if (_encoder)
        opus_encoder_destroy(_encoder);

    if (_decoder)
        opus_decoder_destroy(_decoder);
}

void EncodedSample::setGain(int gain)
{
    if (!_decoder_ctl(OPUS_SET_GAIN(gain)))
        return ;

    _gain = gain;
}

bool EncodedSample::init(AudioSample::eFrequency frequency, AudioSample::eChannel channel)
{
    double sample = AudioSample::frequencyToDouble(frequency);
    int channelCount = AudioSample::channelToInt(channel);
    int errorCode;

    if (qFuzzyCompare(sample, 0.))
    {
        _errText = "Invalid frequency choose";
        return false;
    }
    if (channelCount == 0)
    {
        _errText = "Incorrect number of channel";
        return false;
    }

    _encoder = opus_encoder_create(int(sample), channelCount, OPUS_APPLICATION_VOIP, &errorCode);
    if (errorCode != OPUS_OK)
    {
        _encoder = NULL;
        _errText = opus_strerror(errorCode);
        return false;
    }

    _encoder_ctl(OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
    _encoder_ctl(OPUS_SET_VBR(_disable));

    _decoder = opus_decoder_create(int(sample), channelCount, &errorCode);
    if (errorCode != OPUS_OK)
    {
        _decoder = NULL;
        _errText = opus_strerror(errorCode);
        return false;
    }

    _channelCount = channelCount;

    return true;
}

bool EncodedSample::encode(const AudioSample &sample)
{
    int nbBytes;
    unsigned char data[MAX_DATA_BYTES];

    if (!_encoder)
    {
        _errText = "You must init the encoder first";
        return false;
    }

#ifdef AUDIO_USE_FLOAT
    nbBytes = opus_encode_float(_encoder, sample.buffer(), NB_FRAMES_PER_BUFFER, data, MAX_DATA_BYTES);
#else
    nbBytes = opus_encode(_encoder, sample.buffer(), NB_FRAMES_PER_BUFFER, data, MAX_DATA_BYTES);
#endif
    if (nbBytes < 0)
    {
        _errText = opus_strerror(nbBytes);
        return false;
    }

    _encodedSample = QByteArray((const char*)(data), nbBytes);

    return true;
}

bool EncodedSample::decode(AudioSample &sample)
{
    int nbBytes;
    const unsigned char *data = (const unsigned char*)(_encodedSample.constData());

    if (!_decoder)
    {
        _errText = "You must init the decoder first";
        return false;
    }

#ifdef AUDIO_USE_FLOAT
    nbBytes = opus_decode_float(_decoder, data, _encodedSample.size(), sample.buffer(), NB_FRAMES_PER_BUFFER, 0);
#else
    nbBytes = opus_decode(_decoder, data, _encodedSample.size(), sample.buffer(), NB_FRAMES_PER_BUFFER, 0);
#endif
    if (nbBytes < 0)
    {
        _errText = opus_strerror(nbBytes);
        return false;
    }

    sample.setNbFrame(nbBytes * _channelCount);

    return true;
}

bool EncodedSample::_encoder_ctl(int request, int data)
{
    int errorCode;

    if (!_encoder)
    {
        _errText = "You must init the encoder first";
        return false;
    }

    errorCode = opus_encoder_ctl(_encoder, request, data);
    if (errorCode != OPUS_OK)
    {
        _errText = opus_strerror(errorCode);
        return false;
    }

    return true;
}

bool EncodedSample::_encoder_ctl(int request, void* data)
{
    int errorCode;

    if (!_encoder)
    {
        _errText = "You must init the encoder first";
        return false;
    }

    errorCode = opus_encoder_ctl(_encoder, request, data);
    if (errorCode != OPUS_OK)
    {
        _errText = opus_strerror(errorCode);
        return false;
    }

    return true;
}

bool EncodedSample::_decoder_ctl(int request, int data)
{
    int errorCode;

    if (!_decoder)
    {
        _errText = "You must init the decoder first";
        return false;
    }

    errorCode = opus_decoder_ctl(_decoder, request, data);
    if (errorCode != OPUS_OK)
    {
        _errText = opus_strerror(errorCode);
        return false;
    }

    return true;
}

bool EncodedSample::_decoder_ctl(int request, void *data)
{
    int errorCode;

    if (!_decoder)
    {
        _errText = "You must init the decoder first";
        return false;
    }

    errorCode = opus_decoder_ctl(_decoder, request, data);
    if (errorCode != OPUS_OK)
    {
        _errText = opus_strerror(errorCode);
        return false;
    }

    return true;
}
