#ifndef ADDCARDDIALOG_H
#define ADDCARDDIALOG_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class AddCardDialog;
}

class AddCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCardDialog(QWidget *parent = nullptr);

    ~AddCardDialog();

private:
    bool RegisterCard(QString cardID, QString username, QString password);

private slots:
    void on_readCardButton_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddCardDialog *ui;
};

#endif // ADDCARDDIALOG_H
