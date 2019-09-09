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
#include "include/sqlite3.h"
#include <stdio.h>
#include <intsafe.h>
#include <strsafe.h>
#include <wincred.h>

// 
// Copies the field descriptor pointed to by rcpfd into a buffer allocated 
// using CoTaskMemAlloc. Returns that buffer in ppcpfd.
// 
HRESULT FieldDescriptorCoAllocCopy(
    __in const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
    )
{
    HRESULT hr;
    DWORD cbStruct = sizeof(**ppcpfd);

    CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* pcpfd = (CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR*)CoTaskMemAlloc(cbStruct);
    if (pcpfd)
    {
        pcpfd->dwFieldID = rcpfd.dwFieldID;
        pcpfd->cpft = rcpfd.cpft;

        if (rcpfd.pszLabel)
        {
            hr = SHStrDupW(rcpfd.pszLabel, &pcpfd->pszLabel);
        }
        else
        {
            pcpfd->pszLabel = NULL;
            hr = S_OK;
        }
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        *ppcpfd = pcpfd;
    }
    else
    {
        CoTaskMemFree(pcpfd);  
        *ppcpfd = NULL;
    }

    return hr;
}

//
// Coppies rcpfd into the buffer pointed to by pcpfd. The caller is responsible for
// allocating pcpfd. This function uses CoTaskMemAlloc to allocate memory for 
// pcpfd->pszLabel.
//
HRESULT FieldDescriptorCopy(
    __in const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* pcpfd
    )
{
    HRESULT hr;
    CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR cpfd;

    cpfd.dwFieldID = rcpfd.dwFieldID;
    cpfd.cpft = rcpfd.cpft;

    if (rcpfd.pszLabel)
    {
        hr = SHStrDupW(rcpfd.pszLabel, &cpfd.pszLabel);
    }
    else
    {
        cpfd.pszLabel = NULL;
        hr = S_OK;
    }

    if (SUCCEEDED(hr))
    {
        *pcpfd = cpfd;
    }

    return hr;
}


HRESULT GetCredentialsByCard(
	__in LPWSTR sCardID,
	__out LPWSTR* sUsername,
	__out LPWSTR* sPassword)
{
	HRESULT hr;

	sqlite3 *db;
	sqlite3_open("C:/ProgramData/Smart Card Authenticator/Users.db", &db);
	FileLog("SQLite opened\n");

	LPWSTR sState = new WCHAR[128]();
	StringCchCatW(sState, 128, L"SELECT * FROM 'Users' WHERE CardID='");
	StringCchCatW(sState, 128, sCardID);
	StringCchCatW(sState, 128, L"'");

	sqlite3_stmt *pstmt;
	sqlite3_prepare16(db, sState, -1, &pstmt, nullptr);

	
	if (sqlite3_step(pstmt) == SQLITE_ROW)
	{
		*sUsername = new WCHAR[100]();
		*sPassword = new WCHAR[100]();
		
		LPCSTR csLogin = (LPCSTR)sqlite3_column_text(pstmt, 1);
		mbstowcs_s(NULL, *sUsername, 100, csLogin, strlen(csLogin));

		LPCSTR csPassword = (LPCSTR)sqlite3_column_text(pstmt, 2);
		mbstowcs_s(NULL, *sPassword, 100, csPassword, strlen(csPassword));

		hr = S_OK;
	}
	else
	{
		*sUsername = nullptr;
		*sPassword = nullptr;
		hr = E_INVALIDARG;
	}

	delete sState;
	sqlite3_close(db);

	FileLog("SQLite closed\n");

	return hr;
}

HRESULT FileLog(
	__in LPCSTR csLog)
{
#if defined(_DEBUG)
	FILE* f = nullptr;
	fopen_s(&f, "C:/ProgramData/Smart Card Authenticator/CredentialProvider.log", "a");
	if (f)
	{
		fprintf(f, "%s", csLog);
		fclose(f);
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
#else
	UNREFERENCED_PARAMETER(csLog);
	return E_NOTIMPL;
#endif // defined(_DEBUG)
}

HRESULT SaveBytes(BYTE* pBytes, DWORD cBytes)
{
	FILE* f;
	fopen_s(&f, "C:/ProgramData/Smart Card Authenticator/CredentialProvider.bin", "wb");

	if (f)
	{
		for (DWORD i = 0; i < cBytes; i++)
		{
			fprintf(f, "%c", pBytes[i]);
		}
		fclose(f);
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT MyCredPackAuthenticationBufferW(LPWSTR pszUsername, LPWSTR pszPassword, PBYTE pPackedCredentials, PDWORD pcbPackedCredentials)
{
	//If buffer is not set we just return the amount of byte, needed to pack credentials
	*pcbPackedCredentials = 63 + (lstrlenW(pszUsername) + 1) * 2 + (lstrlenW(pszPassword) + 1) * 2;
	if (!pPackedCredentials)
	{
		return S_OK;
	}
	else
	{
		pPackedCredentials[0] = 2;
		pPackedCredentials[24] = (BYTE)(lstrlenW(pszUsername) * 2);
		pPackedCredentials[26] = pPackedCredentials[24] + 2;
		pPackedCredentials[32] = 64;
		pPackedCredentials[40] = (BYTE)(lstrlenW(pszPassword) * 2);
		pPackedCredentials[42] = pPackedCredentials[40] + 2;
		pPackedCredentials[48] = 64 + pPackedCredentials[26];

		INT pos;
		INT pos1;
		for (pos = 64, pos1 = 0; pos1 <= lstrlenW(pszUsername); pos += 2, pos1++)
		{
			pPackedCredentials[pos] = (BYTE)pszUsername[pos1];
		}
		for (pos1 = 0; pos1 <= lstrlenW(pszPassword); pos += 2, pos1++)
		{
			pPackedCredentials[pos] = (BYTE)pszPassword[pos1];
		}

		return S_OK;
	}
}
