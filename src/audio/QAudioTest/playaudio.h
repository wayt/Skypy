#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H

#include <QThread>

#include "audiostream.h"
#include "encodedsample.h"

class PlayAudio : public QThread
{
    Q_OBJECT

public:
    PlayAudio(QObject *parent = 0);
    virtual ~PlayAudio();

    inline void setInput(AudioStream *input) { _input = input; }
    inline const AudioStream* input() const { return _input; }

    inline void setOutput(AudioStream *output) { _output = output; }
    inline const AudioStream* output() const { return _output; }

public slots:
    void setGain(int gain);

protected:
    virtual void run();

private:
    AudioStream *_input;
    AudioStream *_output;
    EncodedSample *_encodedSample;
};

#endif // PLAYAUDIO_H
