#include "widgetconfigwindow.h"
#include "clientmgr.h"
#include "audiostream.h"
#include "audiomanager.h"
#include <iostream>

WidgetConfigWindow::WidgetConfigWindow(QWidget *parent) :
    QDialog(parent),
    Ui::WidgetConfigWindow()
{
    setupUi(this);
}

void WidgetConfigWindow::show()
{
    _localCallCheck->setChecked(sClientMgr->settings().value("forceLocalCall", false).toBool());
    int deviceCount = AudioStream::deviceCount();
    _inputDevicesList->clear();
    _outputDevicesList->clear();

    int inputDev = sAudioManager->getCurrentInputDevice();
    int outputDev = sAudioManager->getCurrentOutputDevice();

    for (int i = 0; i < deviceCount; ++i)
    {
        _inputDevicesList->addItem(AudioStream::deviceName(i));
        if (inputDev == i)
            _inputDevicesList->setCurrentRow(i);
        _outputDevicesList->addItem(AudioStream::deviceName(i));
        if (outputDev == i)
            _outputDevicesList->setCurrentRow(i);
    }

    QString addr = sClientMgr->settings().value("serverHost", "wayt.me").toString();
    QString port = sClientMgr->settings().value("serverPort", "5000").toString();
    _serverAddress->setText(addr + ":" + port);
    
    QDialog::show();
}

void WidgetConfigWindow::on_buttonBox_accepted()
{
    sClientMgr->settings().setValue("forceLocalCall", _localCallCheck->isChecked());

    QList<QListWidgetItem*> items = _inputDevicesList->selectedItems();
    if (items.size() > 0)
    {
        int index = AudioStream::deviceIndex(items.first()->text());
        if (index != -1 && index != sAudioManager->getCurrentInputDevice())
        {
            sAudioManager->input()->closeDevice();;
            if (!sAudioManager->setInputDevice(index, AudioSample::MONO, AudioSample::FREQ_48000))
                std::cout << "FAIL INIT AUDIO INPUT: " << sAudioManager->errorText().toStdString() << std::endl;
            else
                sClientMgr->settings().setValue("inputDevice", items.first()->text());
        }
    }

    items = _outputDevicesList->selectedItems();
    if (items.size() > 0)
    {
        int index = AudioStream::deviceIndex(items.first()->text());
        if (index != -1 && index != sAudioManager->getCurrentOutputDevice())
        {
            sAudioManager->output()->closeDevice();;
            if (!sAudioManager->setOutputDevice(index, AudioSample::MONO, AudioSample::FREQ_48000))
                std::cout << "FAIL INIT AUDIO OUPUT: " << sAudioManager->errorText().toStdString() << std::endl;
            else
                sClientMgr->settings().setValue("outputDevice", items.first()->text());
        }
    }

    QString addr = _serverAddress->text();
    QStringList strList = addr.split(':');
    if (strList.size() == 2)
    {
        bool valide;
        QString host = strList.first();
        quint16 port = strList.back().toUInt(&valide);
        if (valide && host.size() > 0)
        {
            sClientMgr->settings().setValue("serverHost", host);
            sClientMgr->settings().setValue("serverPort", port);
        }
    }

    sClientMgr->settings().sync();
}
