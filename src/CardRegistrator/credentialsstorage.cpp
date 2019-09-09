#include "credentialsstorage.h"
#include "helpers.h"
#include <sqlite3.h>
#include <QDebug>

CredentialsStorage* CredentialsStorage::GetInstance()
{
    static CredentialsStorage* _instance = new CredentialsStorage();
    return _instance;
}

CredentialsStorage::CredentialsStorage()
{
    LoadCredentials();
}

QList<QString> CredentialsStorage::GetIDs()
{
    return _storage.keys();
}
Credential CredentialsStorage::GetCredential (QString id)
{
    return _storage.value(id);
}
bool CredentialsStorage::HasCredential (QString id)
{
    return _storage.contains(id);
}
void CredentialsStorage::SetCredential (QString id, Credential cred)
{
    if (_storage.contains(id)) RemoveCredential(id);
    _storage.insert(id, cred);
    SaveCredential(id);
}
void CredentialsStorage::RemoveCredential (QString id)
{
    _storage.remove(id);

    sqlite3* db;
    sqlite3_open("C:/ProgramData/Smart Card Authenticator/Users.db", &db);

    QString statement = QString("DELETE FROM 'Users' WHERE CardID='").append(id).append("'");
    sqlite3_exec(db, statement.toLocal8Bit().data(), nullptr, nullptr, nullptr);

    sqlite3_close(db);
}


void CredentialsStorage::LoadCredentials()
{
    _storage.clear();

    sqlite3* db;
    sqlite3_open("C:/ProgramData/Smart Card Authenticator/Users.db", &db);

    QString statement = QString("SELECT * FROM Users");
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, statement.toLocal8Bit().data(), -1, &pstmt, nullptr);
    while (sqlite3_step(pstmt) == SQLITE_ROW)
    {
        QString cardID = ConstUnsignedCharToQString(sqlite3_column_text(pstmt, 0));
        QString username = ConstUnsignedCharToQString(sqlite3_column_text(pstmt, 1));
        QString password = ConstUnsignedCharToQString(sqlite3_column_text(pstmt, 2));

        _storage.insert(cardID, Credential(username, password));
    }

    sqlite3_close(db);
}


void CredentialsStorage::SaveCredential(QString id)
{
    sqlite3* db;
    sqlite3_open("C:/ProgramData/Smart Card Authenticator/Users.db", &db);

    QString statement = QString("SELECT * FROM 'Users' WHERE CardID='").append(id).append("'");
    sqlite3_stmt *pstmt;
    LPWSTR sStatement = new WCHAR[128]();
    statement.toWCharArray(sStatement);

    sqlite3_prepare16(db, sStatement, -1, &pstmt, nullptr);

    //If card isn't already in database we need to add it
    if(sqlite3_step(pstmt)==SQLITE_DONE)
    {
        statement = QString("INSERT INTO Users (CardID, Username, Password) VALUES('").append(id);
        statement.append("', '").append(_storage.value(id).GetUsername());
        statement.append("', '").append(_storage.value(id).GetPassword()).append("')");

        sqlite3_exec(db, statement.toLocal8Bit().data(), nullptr, nullptr, nullptr);
    }
    else
    {
        statement = QString("UPDATE Users SET Username='").append(_storage.value(id).GetUsername());
        statement.append("', Password='").append(_storage.value(id).GetPassword());
        statement.append("' WHERE ").append("CardID='").append(id).append("'");

        sqlite3_exec(db, statement.toLocal8Bit().data(), nullptr, nullptr, nullptr);
    }


    sqlite3_close(db);
}























