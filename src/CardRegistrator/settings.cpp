#include "settings.h"
#include <Windows.h>
#include<QDir>

Settings* Settings::GetInstance()
{
    static Settings* _instance = new Settings();
    return _instance;
}

Settings::Settings()
{
    if (!QDir("C:/ProgramData/Smart Card Authenticator").exists())
    {
        QDir().mkdir("C:/ProgramData/Smart Card Authenticator");
    }

    LPWSTR sPort = new WCHAR[32]();
    GetPrivateProfileStringW(L"General", L"Port", L"COM1", sPort, 32, L"C:/ProgramData/Smart Card Authenticator/Settings.ini");

    _port = QString::fromWCharArray(sPort);

    delete sPort;
}

QString Settings::getPort()
{
    return _port;
}

void Settings::setPort(QString port)
{
    _port = port;
}

void Settings::saveToDisk()
{
    LPWSTR sPort = new WCHAR[32]();
    getPort().toWCharArray(sPort);

    WritePrivateProfileStringW(L"General", L"Port", sPort, L"C:/ProgramData/Smart Card Authenticator/Settings.ini");

    delete sPort;
}
