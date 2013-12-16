#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = 0);
    virtual ~MainWindow();

private:
    QFormLayout *_layF;
    QLineEdit *_leMdp;
    QLineEdit *_leMail;
    QPushButton *_pbConnection;

private slots:
    void _pbConnection_clicked();
};

#endif // WIDGET_H
