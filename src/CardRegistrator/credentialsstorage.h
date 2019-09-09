#ifndef CREDENTIALSSTORAGE_H
#define CREDENTIALSSTORAGE_H

#include <QMap>
#include <QString>

#include "credential.h"


class CredentialsStorage
{
public:
    static CredentialsStorage* GetInstance();

    QList<QString> GetIDs();
    Credential GetCredential (QString id);
    bool HasCredential (QString id);
    void SetCredential (QString id, Credential cred);
    void RemoveCredential (QString id);

    void LoadCredentials();
    void SaveCredential(QString id);

    CredentialsStorage();

private:
    QMap<QString, Credential> _storage;
};

#endif // CREDENTIALSSTORAGE_H
