#include <QFile>
#include "addcarddialog.h"
#include "ui_addcarddialog.h"
#include "cardreaderdialog.h"
#include "CardReader.h"
#include "settings.h"
#include "sqlite3.h"
#include "messagedialog.h"
#include "helpers.h"
#include "credentialsstorage.h"

AddCardDialog::AddCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCardDialog)
{
    CredentialsStorage::GetInstance()->LoadCredentials();
    ui->setupUi(this);
}

AddCardDialog::~AddCardDialog()
{
    delete ui;
}

void AddCardDialog::on_readCardButton_clicked()
{
    CardReaderDialog* pcrd = new CardReaderDialog(this);

    LPWSTR sPort = new WCHAR[32]();
    Settings::GetInstance()->getPort().toWCharArray(sPort);

    CardReader* pcr = new CardReader(sPort, pcrd);

    pcrd->exec();

    if (pcrd->GetCard() != nullptr)
    {
        LPWSTR cardID;
        pcrd->GetCard()->GetCardID(&cardID);
        ui->cardIDEdit->setText(QString::fromWCharArray(cardID));
        delete pcrd->GetCard();
    }
    delete pcrd;
    delete pcr;
}


void AddCardDialog::on_okButton_clicked()
{
    if (!CredentialsStorage::GetInstance()->HasCredential(ui->cardIDEdit->text()))
    {
        CredentialsStorage::GetInstance()->SetCredential(ui->cardIDEdit->text(), Credential(ui->usernameEdit->text(), ui->passwordEdit->text()));
        MessageDialog* pid = new MessageDialog(this, "Card successfully registered", T_INFO);
        pid->exec();

        delete pid;
        close();
    }
    else
    {
        MessageDialog* pid = new MessageDialog(this, "Card is already registered", T_ERROR);
        pid->exec();

        delete pid;
    }

}

void AddCardDialog::on_cancelButton_clicked()
{
    close();
}
















