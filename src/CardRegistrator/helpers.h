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

#pragma once
#include <Windows.h>
#include <QString>

//makes a copy of a field descriptor using CoTaskMemAlloc
HRESULT FileLog(
	__in LPCSTR csLog);

QString ConstUnsignedCharToQString (const unsigned char* chArr);

HRESULT FileLog(LPCSTR csLog);
