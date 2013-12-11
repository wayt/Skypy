#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    virtual ~Widget();

private:
    QFormLayout *_layF;
    QLineEdit *_leMdp;
    QLineEdit *_leMail;
    QPushButton *_pbConnection;

private slots:
    void _pbConnection_clicked();
};

#endif // WIDGET_H
