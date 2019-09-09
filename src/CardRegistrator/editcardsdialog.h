#ifndef EDITCARDDIALOG_H
#define EDITCARDDIALOG_H

#include <QDialog>

namespace Ui {
class EditCardDialog;
}

class EditCardsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCardsDialog(QWidget *parent = nullptr);
    ~EditCardsDialog();

private slots:
    void on_idsList_itemSelectionChanged();

    void on_deleteButton_clicked();

    void on_saveButton_clicked();

private:
    void LoadIDs();

private:
    Ui::EditCardDialog *ui;
};

#endif // EDITCARDDIALOG_H
