#ifndef AUDIOENCODER_H
#define AUDIOENCODER_H

#include <QString>
#include <opus/opus.h>

#include "singleton.h"
#include "audiosample.h"
#include "encodedsample.h"

#define MAX_DATA_BYTES  4000

class AudioEncoder : public Singleton<AudioEncoder>
{
public:
    AudioEncoder();
    virtual ~AudioEncoder();

    inline bool isInitialized() const { return _init; }

    inline int gain() const { return _gain; }
    bool setGain(int gain);

    inline const QString& errorText() const { return _errText; }

    inline AudioSample::eFrequency frequency() const { return _frequency; }
    inline AudioSample::eChannel channel() const { return _channel; }

    bool init(AudioSample::eFrequency frequency = AudioSample::FREQ_48000, AudioSample::eChannel channel = AudioSample::MONO);

    bool encode(const AudioSample &sample, EncodedSample &encodedSample);
    bool decode(AudioSample &sample, const EncodedSample &encodedSample);

private:
    bool _encoder_ctl(int request, int data);
    bool _encoder_ctl(int request, void *data);
    bool _decoder_ctl(int request, int data);
    bool _decoder_ctl(int request, void *data);

private:
    QString _errText;
    OpusEncoder *_encoder;
    OpusDecoder *_decoder;
    AudioSample::eFrequency _frequency;
    AudioSample::eChannel _channel;
    int _gain;
    bool _init;

private:
    static const int _enable = 1;
    static const int _disable = 0;
};

#define sAudioEncoder AudioEncoder::instance()

#endif // AUDIOENCODER_H
