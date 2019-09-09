#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <QString>

class Credential
{
public:
    Credential();
    Credential(QString username, QString password);

    QString GetUsername() const;
    QString GetPassword() const;

    void SetUsername(QString username);
    void SetPassword(QString password);

private:
    QString _username;
    QString _password;

};

#endif // CREDENTIAL_H
