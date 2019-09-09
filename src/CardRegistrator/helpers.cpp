//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Helper functions for copying parameters and packaging the buffer
// for GetSerialization.


#include "helpers.h"
#include "sqlite3.h"
#include <stdio.h>

QString ConstUnsignedCharToQString (const unsigned char* chArr)
{
    QString str;
    for (int i = 0; chArr[i] != '\0'; i++)
    {
        str.append(QChar(chArr[i]));
    }

    return str;
}

HRESULT FileLog(
	__in LPCSTR csLog)
{
	FILE* f;
    fopen_s(&f, "D:/reg_log.log", "a");
	fprintf(f, "%s", csLog);
	fclose(f);

	return S_OK;
}
