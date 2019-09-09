#include "cardreaderdialog.h"
#include "ui_cardreaderdialog.h"

CardReaderDialog::CardReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardReaderDialog),
    _pCard(nullptr)
{
    connect(this, &CardReaderDialog::tryClose, this, &CardReaderDialog::myClose);
    connect(this, &CardReaderDialog::setLabel, this, &CardReaderDialog::onSetLabel);
    ui->setupUi(this);
}

HRESULT CardReaderDialog::HandleCard(Card* pCard)
{
    _pCard = pCard;

    LPWSTR sCardID;
    _pCard->GetCardID(&sCardID);
    emit setLabel(QString("<html><head/><body><p align=\"center\">ID: ").append(QString::fromWCharArray(sCardID)).append(QString("</p></body></html>")));

    Sleep(800);

    emit tryClose();
    return S_OK;
}

Card* CardReaderDialog::GetCard()
{
    return _pCard;

}


CardReaderDialog::~CardReaderDialog()
{
    delete ui;
}

void CardReaderDialog::myClose()
{
    close();
}

void CardReaderDialog::onSetLabel(QString label)
{
    ui->cardIDLabel->setText(label);
}
