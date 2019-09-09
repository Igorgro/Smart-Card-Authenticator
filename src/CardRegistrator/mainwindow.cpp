#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcarddialog.h"
#include "editcardsdialog.h"
#include "settingsdialog.h"
#include "settings.h"
#include <QDebug>
#include <QFile>
#include <sqlite3.h>
#include <Windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateText();
    CheckDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addCardButton_clicked()
{
    AddCardDialog *pacd = new AddCardDialog(this);
    pacd->exec();

    delete  pacd;
}

void MainWindow::on_editCardButton_clicked()
{
    EditCardsDialog *pecd = new EditCardsDialog(this);
    pecd->exec();

    delete  pecd;
}

void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog* psd = new SettingsDialog(this);
    psd->exec();

    updateText();

    delete psd;
}

void MainWindow::updateText()
{
    ui->helpLabel->setText(QString("You Z-2 USB reader must be connected to ").append(Settings::GetInstance()->getPort()).append(" to register new cards"));
}

void MainWindow::CheckDatabase()
{
    //Check if database has not been created yet
    if (!QFile("C:/ProgramData/Smart Card Authenticator/Users.db").exists())
    {
        sqlite3 *db;

        sqlite3_open("C:/ProgramData/Smart Card Authenticator/Users.db", &db);

        QString statement = QString("CREATE TABLE Users (CardID TEXT, Username TEXT, Password TEXT)");

        sqlite3_exec(db, statement.toLocal8Bit().data(), nullptr, nullptr, nullptr);

        sqlite3_close(db);
    }
}





















