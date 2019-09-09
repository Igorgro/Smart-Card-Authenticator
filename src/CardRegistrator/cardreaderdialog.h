#ifndef CARDREADERDIALOG_H
#define CARDREADERDIALOG_H

#include <QDialog>
#include "ICardHandler.h"
#include "CardReader.h"

namespace Ui {
class CardReaderDialog;
}

class CardReaderDialog : public QDialog, public ICardHandler
{
    Q_OBJECT

public:
    explicit CardReaderDialog(QWidget *parent = nullptr);

    HRESULT HandleCard(Card* pCard);
    Card* GetCard();

    ~CardReaderDialog();

public slots:
    void myClose();
    void onSetLabel(QString label);

private:
    Ui::CardReaderDialog *ui;
    Card* _pCard;

signals:
    void tryClose();
    void setLabel(QString label);
};

#endif // CARDREADERDIALOG_H
