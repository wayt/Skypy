#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>

#include "audiostream.h"
#include "playaudio.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    QGridLayout *_layG;
    QPushButton *_pbOpen;
    QPushButton *_pbStart;
    QSlider *_sliderSound;
    AudioStream *_input;
    AudioStream *_output;
    PlayAudio *_playAudio;

private slots:
    void _pbOpen_clicked();
    void _pbStart_clicked();
};

#endif // MAINWINDOW_H
