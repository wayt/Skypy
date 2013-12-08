#include <QMessageBox>
#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _layG(new QGridLayout),
    _pbOpen(new QPushButton("Open audio device", this)),
    _pbStart(new QPushButton("Start", this)),
    _sliderSound(new QSlider(Qt::Horizontal, this)),
    _input(new AudioStream()),
    _output(new AudioStream()),
    _socket(new AudioSocket(this)),
    _leIp(new QLineEdit(this))
{
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(_layG);

    _layG->addWidget(_pbOpen, 0, 0);
    _layG->addWidget(_leIp, 1, 0);
    _layG->addWidget(_pbStart, 2, 0);
    _layG->addWidget(_sliderSound, 3, 0);

    _pbStart->setEnabled(false);

    _sliderSound->setTracking(true);
    _sliderSound->setRange(-32768, 32767);

    _leIp->setInputMask("009.009.009.009;_");
    _leIp->setText("127.000.000.001");

    _socket->setInput(_input);
    _socket->setOutput(_output);

    QObject::connect(_pbOpen, SIGNAL(clicked()), this, SLOT(_pbOpen_clicked()));
    QObject::connect(_pbStart, SIGNAL(clicked()), this, SLOT(_pbStart_clicked()));
    QObject::connect(_sliderSound, SIGNAL(valueChanged(int)), _socket, SLOT(setGain(int)));
}

MainWindow::~MainWindow()
{
    delete _input;
    delete _output;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    _socket->quit();

    _input->stop();
    _output->stop();

    e->accept();
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
        _socket->quit();

        _input->stop();
        _output->stop();

        _pbStart->setText("Start");
        _pbOpen->setEnabled(true);

        return ;
    }

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

    _socket->setHostAddr(QHostAddress(_leIp->text()));
    _socket->setGain(_sliderSound->value());
    _socket->start();

    _pbOpen->setEnabled(false);
    _pbStart->setText("Stop");
}
