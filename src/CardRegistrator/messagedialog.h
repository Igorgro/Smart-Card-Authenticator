#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class MessageDialog;
}
typedef enum _DialogType
{
    T_INFO = 1,
    T_WARNING = 2,
    T_ERROR = 3
} DialogType;
class MessageDialog : public QDialog
{
    Q_OBJECT

public:


    explicit MessageDialog(QWidget *parent = nullptr, QString message = "[TEXT]", DialogType type = T_INFO);
    ~MessageDialog();

private:
    void SetType(DialogType type);

private slots:
    void on_okButton_clicked();

private:
    Ui::MessageDialog *ui;
};

#endif // INFODIALOG_H
