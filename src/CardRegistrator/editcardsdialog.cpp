#include "editcardsdialog.h"
#include "ui_editcarddialog.h"
#include "credentialsstorage.h"
#include "messagedialog.h"

EditCardsDialog::EditCardsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCardDialog)
{
    CredentialsStorage::GetInstance()->LoadCredentials();
    ui->setupUi(this);
    LoadIDs();
}

EditCardsDialog::~EditCardsDialog()
{
    delete ui;
}

void EditCardsDialog::LoadIDs()
{
    ui->idsList->clear();
    ui->cardIDEdit->setText(QString(""));
    ui->usernameEdit->setText(QString(""));
    ui->passwordEdit->setText(QString(""));

    QList<QString> ids = CredentialsStorage::GetInstance()->GetIDs();
    for (int i = 0; i < ids.length(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(ids.at(i), ui->idsList);
    }
}

void EditCardsDialog::on_idsList_itemSelectionChanged()
{
    ui->cardIDEdit->setText(ui->idsList->currentItem()->text());

    Credential currentCred = CredentialsStorage::GetInstance()->GetCredential(ui->idsList->currentItem()->text());

    ui->usernameEdit->setText(currentCred.GetUsername());
    ui->passwordEdit->setText(currentCred.GetPassword());
}

void EditCardsDialog::on_deleteButton_clicked()
{
    CredentialsStorage::GetInstance()->RemoveCredential(ui->cardIDEdit->text());
    LoadIDs();
}

void EditCardsDialog::on_saveButton_clicked()
{
    Credential cred = Credential(ui->usernameEdit->text(), ui->passwordEdit->text());
    CredentialsStorage::GetInstance()->SetCredential(ui->cardIDEdit->text(), cred);

    MessageDialog *pid = new MessageDialog(this, QString("Card successfully saved"));
    pid->exec();
    delete pid;
}
