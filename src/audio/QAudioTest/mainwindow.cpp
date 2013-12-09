#include <QMessageBox>

#include "audiomanager.h"
#include "audioencoder.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _layG(new QGridLayout),
    _pbOpen(new QPushButton("Open audio device", this)),
    _pbStart(new QPushButton("Start", this)),
    _sliderSound(new QSlider(Qt::Horizontal, this)),
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

    QObject::connect(_pbOpen, SIGNAL(clicked()), this, SLOT(_pbOpen_clicked()));
    QObject::connect(_pbStart, SIGNAL(clicked()), this, SLOT(_pbStart_clicked()));
    QObject::connect(_sliderSound, SIGNAL(valueChanged(int)), this, SLOT(_sliderSound_valueChanged(int)));
}

MainWindow::~MainWindow()
{
    delete sAudioEncoder;
    delete sAudioManager;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    _socket->quit();

    e->accept();
}

void MainWindow::_pbOpen_clicked()
{
    if (!sAudioManager->setInputDevice(DEFAULT_DEVICE, AudioSample::MONO, AudioSample::FREQ_48000))
    {
        QMessageBox::critical(this, "Error", "Fail to init input device: " + sAudioManager->errorText());
        return ;
    }
    if (!sAudioManager->setOutputDevice(DEFAULT_DEVICE, AudioSample::MONO, AudioSample::FREQ_48000))
    {
        QMessageBox::critical(this, "Error", "Fail to init output device: " + sAudioManager->errorText());
        return ;
    }

    _pbStart->setEnabled(true);
}

void MainWindow::_pbStart_clicked()
{
    if (sAudioManager->input()->isStarted() || sAudioManager->output()->isStarted())
    {
        _socket->quit();
        sAudioManager->quit();

        _pbStart->setText("Start");
        _pbOpen->setEnabled(true);

        return ;
    }

    if (!sAudioManager->start())
    {
        QMessageBox::critical(this, "Error", "Unable to start device\n" + sAudioManager->errorText());
        return ;
    }

    sAudioEncoder->setGain(_sliderSound->value());
    _socket->setHostAddr(QHostAddress(_leIp->text()));
    _socket->start();

    _pbOpen->setEnabled(false);
    _pbStart->setText("Stop");
}

void MainWindow::_sliderSound_valueChanged(int value)
{
    sAudioEncoder->setGain(value);
}
