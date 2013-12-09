#ifndef ENCODEDSAMPLE_H
#define ENCODEDSAMPLE_H

#include <QByteArray>
#include <QString>
#include <opus/opus.h>

#include "audiosample.h"

#define MAX_DATA_BYTES  4000

class EncodedSample
{
public:
    EncodedSample();
    virtual ~EncodedSample();

    inline int gain() const { return _gain; }
    void setGain(int gain);

    inline int size() const { return _encodedSample.size(); }
    inline void setEncodedSample(const QByteArray &encodedSample) { _encodedSample = encodedSample; }
    inline const QByteArray& encodedSample() const { return _encodedSample; }

    bool init(AudioSample::eFrequency frequency = AudioSample::FREQ_48000, AudioSample::eChannel channel = AudioSample::MONO);

    bool encode(const AudioSample &sample);
    bool decode(AudioSample &sample);

    const QString& errText() const { return _errText; }

private:
    bool _encoder_ctl(int request, int data);
    bool _encoder_ctl(int request, void *data);
    bool _decoder_ctl(int request, int data);
    bool _decoder_ctl(int request, void *data);

private:
    int _gain;
    QByteArray _encodedSample;
    OpusEncoder *_encoder;
    OpusDecoder *_decoder;
    QString _errText;
    int _channelCount;

private:
    static const int _enable = 1;
    static const int _disable = 0;
};

#endif // ENCODEDSAMPLE_H
