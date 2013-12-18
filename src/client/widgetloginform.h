#ifndef WIDGETLOGINFORM_H
#define WIDGETLOGINFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>

class WidgetLoginForm : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetLoginForm(QWidget *parent = 0);

    void initialize();
    void unload();

    QString getEmailText() const { return _leMail->text(); }
    QString getPasswordText() const { return _leMdp->text(); }
private:
    QFormLayout *_layF;
    QLineEdit *_leMdp;
    QLineEdit *_leMail;
    QPushButton *_pbConnection;

private slots:
    void _pbConnection_clicked();

};

#endif // WIDGETLOGINFORM_H
