#include "credential.h"

Credential::Credential():
    _username(),
    _password()
{

}

Credential::Credential(QString username, QString password):
    _username(username),
    _password(password)
{

}

QString Credential::GetUsername() const {return _username;}
QString Credential::GetPassword() const {return _password;}

void Credential::SetUsername(QString username){_username = username;}
void Credential::SetPassword(QString password){_password = password;}
