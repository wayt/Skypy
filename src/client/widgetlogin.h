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

    QString const& getEmailText() const { return _emailInput->text(); }
    QString const& getPasswordText() const { return _passwordInput->text(); }
private slots:
    void on__connectButton_clicked();

private:
};

#endif // WIDGETLOGIN_H
