#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QCloseEvent>
#include <QLineEdit>

#include "audiosocket.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);

private:
    QGridLayout *_layG;
    QPushButton *_pbOpen;
    QPushButton *_pbStart;
    QSlider *_sliderSound;
    AudioSocket *_socket;
    QLineEdit *_leIp;

private slots:
    void _pbOpen_clicked();
    void _pbStart_clicked();
    void _sliderSound_valueChanged(int value);
};

#endif // MAINWINDOW_H
