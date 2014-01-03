#ifndef AUDIOSAMPLE_H
#define AUDIOSAMPLE_H

#define NB_FRAMES_PER_BUFFER    960 // 20 ms at 48000 Hz
#define NB_MAX_CHANNEL          2 // Stereo

#define NB_MAX_FRAMES           (NB_FRAMES_PER_BUFFER * NB_MAX_CHANNEL)

#ifdef AUDIO_USE_FLOAT
# define SAMPLE         float
# define SAMPLE_FORMAT  paFloat32
# define ZERO           0.f
# define MIN_VALUE      -1.f
# define MAX_VALUE      1.f
#else
# define SAMPLE         short
# define SAMPLE_FORMAT  paInt16
# define ZERO           0
# define MIN_VALUE      -128
# define MAX_VALUE      127
#endif

class AudioSample
{
public:
    enum eFrequency
    {
        FREQ_8000       = 0,
        FREQ_12000      = 1,
        FREQ_16000      = 2,
        FREQ_24000      = 3,
        FREQ_48000      = 4
    };

    enum eChannel
    {
        MONO    = 0,
        STEREO  = 1
    };

public:
    AudioSample();
    AudioSample(const SAMPLE *buffer, int nbFrame);
    AudioSample(const AudioSample &other);
    virtual ~AudioSample();

    AudioSample& operator=(const AudioSample &other);
    AudioSample& operator+=(const AudioSample &other);

    inline const SAMPLE* buffer() const { return _buffer; }
    inline SAMPLE* buffer() { return _buffer; }
    void setBuffer(const SAMPLE *buffer, int nbFrame);

    inline SAMPLE const& operator[](int idx) const { return _buffer[idx]; }
    inline SAMPLE& operator[](int idx) { return _buffer[idx]; }

    void clearBuffer();

    inline int nbFrame() const { return _nbFrame; }
    inline void setNbFrame(int nbFrame) { _nbFrame = nbFrame; }

    void dumpBuffer() const;

public:
    static double frequencyToDouble(eFrequency frequency);
    static int channelToInt(eChannel channel);

private:
    int _nbFrame;
    SAMPLE _buffer[NB_MAX_FRAMES];
};

#endif // AUDIOSAMPLE_H
