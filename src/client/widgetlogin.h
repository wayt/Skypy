#ifndef WIDGETLOGIN_H
#define WIDGETLOGIN_H

#include <QWidget>
#include "ui_widgetlogin.h"

class WidgetLogin : public QWidget, private Ui::WidgetLogin
{
    Q_OBJECT

public:
    explicit WidgetLogin(QWidget *parent = 0);

    void initialize();
    void unload();

    QString const& getEmailText() const { return _email; }
    QString const& getPasswordText() const { return _password; }
private slots:
    void on__connectButton_clicked();

private:
    QString _email;
    QString _password;
};

#endif // WIDGETLOGIN_H
