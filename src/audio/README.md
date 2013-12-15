Audio
=====

Resources needed
----------------

You need the portaudio library and the opus library.

#### Windows
A resources needed are available in AudioPackage folder

#### Linux
* Fedora: You can install them by the following command: `yum install portaudio opus`
* Other: You can download the source at this page: [opus](http://www.opus-codec.org/downloads/) and [portaudio](http://www.portaudio.com/download.html)

Audio Manager
-------------
This class serves as an interface with the audio.

* You can init the audio with this 2 functions

```cpp
sAudioManager->setInputDevice(DEFAULT_DEVICE, AudioSample::MONO, AudioSample::FREQ_48000);
sAudioManager->setOutputDevice(DEFAULT_DEVICE, AudioSample::MONO, AudioSample::FREQ_48000);
```
DEFAULT_DEVICE is a define for choose the default input/output device but you can specify another one.
To do this you have 2 useful **static** functions in AudioStream class
```cpp
int AudioStream::deviceCount();
const QString& AudioStream::deviceName(int device);
```

* After you can start the audio threads

```cpp
sAudioManager->start();
```

* And of course you can stop them

```cpp
sAudioManager->quit();
sAudioManager->terminate();
```

There are one thread for recording the input, one thread for playing the output and another one for encode and decode the stream.

#### Input
To access to the encoded data you have one method
```cpp
sAudioManager->inputQueue();
```
This method return a QSynchronizedQueue. This queue is particular because the function dequeue is blocking if another thread push data.

#### Output
To push encoded data for playing song you have one method
```cpp
sAudioManager->push(encodedData);
```

AudioEncoder
------------
This class serves for control the encoder and decoder

* You can init the encoder/decoder with this function

```cpp
sAudioEncoder->init(AudioSample::FREQ_48000, AudioSample::MONO);
```
This function is automatically call when you start the audio manager

* You can set/get the value of the decoder gain with the following functions

```cpp
bool setGain(int gain);
int gain() const;
```
The gain must be between -32768 and 32767

Delete resources
----------------
**Don't forget to delete audio resources when your program stop**

```cpp
delete sAudioManager
delete sAudioEncoder
```
