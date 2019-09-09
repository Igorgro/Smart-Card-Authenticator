#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    static Settings* GetInstance();

    void saveToDisk();

    void setPort(QString _port);
    QString getPort();

private:
    Settings();

    QString _port;
};

#endif // SETTINGS_H
