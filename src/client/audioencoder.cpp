#include <QtGlobal>

#include "audioencoder.h"

AudioEncoder::AudioEncoder() :
    Singleton<AudioEncoder>(),
    _encoder(NULL),
    _decoder(NULL),
    _frequency(AudioSample::FREQ_8000),
    _channel(AudioSample::MONO),
    _init(false)
{
}

AudioEncoder::~AudioEncoder()
{
    if (_encoder)
        opus_encoder_destroy(_encoder);

    if (_decoder)
        opus_decoder_destroy(_decoder);
}

bool AudioEncoder::setGain(int gain)
{
    if (!_decoder_ctl(OPUS_SET_GAIN(gain)))
        return false;

    _gain = gain;
    return true;
}

bool AudioEncoder::init(AudioSample::eFrequency frequency, AudioSample::eChannel channel)
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

    _channel = channel;
    _frequency = frequency;
    _init = true;

    return true;
}

bool AudioEncoder::encode(const AudioSample &sample, EncodedSample &encodedSample)
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

    encodedSample.setEncodedSample((const char*)(data), nbBytes);

    return true;
}

bool AudioEncoder::decode(AudioSample &sample, const EncodedSample &encodedSample)
{
    int nbBytes;
    int channelCount = AudioSample::channelToInt(_channel);
    const unsigned char *data = (const unsigned char*)(encodedSample.encodedSample().constData());

    if (!_decoder)
    {
        _errText = "You must init the decoder first";
        return false;
    }

#ifdef AUDIO_USE_FLOAT
    nbBytes = opus_decode_float(_decoder, data, encodedSample.size(), sample.buffer(), NB_FRAMES_PER_BUFFER, 0);
#else
    nbBytes = opus_decode(_decoder, data, encodedSample.size(), sample.buffer(), NB_FRAMES_PER_BUFFER, 0);
#endif
    if (nbBytes < 0)
    {
        _errText = opus_strerror(nbBytes);
        return false;
    }

    sample.setNbFrame(nbBytes * channelCount);

    return true;
}

bool AudioEncoder::_encoder_ctl(int request, int data)
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

bool AudioEncoder::_encoder_ctl(int request, void* data)
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

bool AudioEncoder::_decoder_ctl(int request, int data)
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

bool AudioEncoder::_decoder_ctl(int request, void *data)
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
