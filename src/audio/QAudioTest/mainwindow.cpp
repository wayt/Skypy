#include <QMessageBox>
#include <QDebug>
#include <portaudio.h>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _layG(new QGridLayout),
    _pbOpen(new QPushButton("Open device", this)),
    _pbStart(new QPushButton("Start recording and encoding", this)),
    _sliderSound(new QSlider(Qt::Horizontal, this)),
    _input(new AudioStream()),
    _output(new AudioStream()),
    _playAudio(new PlayAudio(this))
{
    Pa_Initialize();

    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(_layG);

    _layG->addWidget(_pbOpen, 0, 0);
    _layG->addWidget(_pbStart, 1, 0);
    _layG->addWidget(_sliderSound, 2, 0);

    _pbStart->setEnabled(false);

    _playAudio->setInput(_input);
    _playAudio->setOutput(_output);

    QObject::connect(_pbOpen, SIGNAL(clicked()), this, SLOT(_pbOpen_clicked()));
    QObject::connect(_pbStart, SIGNAL(clicked()), this, SLOT(_pbStart_clicked()));
    QObject::connect(_sliderSound, SIGNAL(valueChanged(int)), _playAudio, SLOT(setGain(int)));

    _sliderSound->setTracking(true);
    _sliderSound->setRange(-32768, 32767);
}

MainWindow::~MainWindow()
{
    _playAudio->quit();

    delete _input;
    delete _output;

    Pa_Terminate();
}

void MainWindow::_pbOpen_clicked()
{
    _input->stop();
    _input->closeDevice();
    _input->clearInputDevice();
    _output->stop();
    _output->closeDevice();
    _output->clearOutputDevice();

    if (!_input->setInputDevice(-1, AudioSample::MONO, AudioStream::LOW_LATENCY))
    {
        QMessageBox::critical(this, "Error", "Unable to set input device\n" + _input->getErrorText());
        return ;
    }
    if (!_output->setOutputDevice(-1, AudioSample::MONO, AudioStream::LOW_LATENCY))
    {
        QMessageBox::critical(this, "Error", "Unable to set output device\n" + _output->getErrorText());
        return ;
    }

    if (!_input->openDevice(AudioSample::FREQ_48000))
    {
        QMessageBox::critical(this, "Error", "Unable to open input device\n" + _input->getErrorText());
        return ;
    }
    if (!_output->openDevice(AudioSample::FREQ_48000))
    {
        QMessageBox::critical(this, "Error", "Unable to open output device\n" + _output->getErrorText());
        return ;
    }

    _pbStart->setEnabled(true);
}

void MainWindow::_pbStart_clicked()
{
    if (_input->isStarted() || _output->isStarted())
    {
        _input->stop();
        _output->stop();
        _playAudio->quit();
        _pbStart->setText("Start recording and encoding");
        return ;
    }

    _playAudio->start();

    if (!_input->start())
    {
        QMessageBox::critical(this, "Error", "Unable to start input device\n" + _input->getErrorText());
        return ;
    }
    if (!_output->start())
    {
        QMessageBox::critical(this, "Error", "Unable to start output device\n" + _output->getErrorText());
        return ;
    }

    _pbStart->setText("Stop recording and encoding");
}
