#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"
#include <QDebug>
#include <Windows.h>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ListPorts();
    ui->portCombo->setCurrentText(Settings::GetInstance()->getPort());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::ListPorts()
{
    DWORD cbNeeded;
    DWORD pcReturned;
    EnumPortsW(NULL, 1, NULL, 0, &cbNeeded, &pcReturned);

    BYTE *pbPorts = new BYTE[cbNeeded];
    EnumPortsW(NULL, 1, pbPorts, cbNeeded, &cbNeeded, &pcReturned);

    PORT_INFO_1 *pPorts = reinterpret_cast<PORT_INFO_1*>(pbPorts);
    for (DWORD i = 0; i < pcReturned; i++)
    {
        QString port = QString::fromWCharArray(pPorts[i].pName);

        //only com ports
        if (port.contains(QString("COM")))
        {
           ui->portCombo->addItem(port.mid(0, port.length()-1));
        }

    }
}

void SettingsDialog::on_cancelButton_clicked()
{
    close();
}

void SettingsDialog::on_okButton_clicked()
{
    Settings::GetInstance()->setPort(ui->portCombo->currentText());
    Settings::GetInstance()->saveToDisk();

    close();
}
