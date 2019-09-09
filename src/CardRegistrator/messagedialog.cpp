#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent, QString message, DialogType type) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    ui->messageLabel->setText(message);
    SetType(type);
}

void MessageDialog::SetType(DialogType type)
{
    switch (type)
    {
        case T_INFO:
            setWindowTitle(QString("Information"));
            ui->icon->setPixmap(QPixmap(":/images/info.png"));
            break;
        case T_WARNING:
            setWindowTitle(QString("Warning"));
            ui->icon->setPixmap(QPixmap(":/images/warning.png"));
            break;
        case T_ERROR:
            setWindowTitle(QString("Error"));
            ui->icon->setPixmap(QPixmap(":/images/error.png"));
            break;
    }
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::on_okButton_clicked()
{
    close();
}
