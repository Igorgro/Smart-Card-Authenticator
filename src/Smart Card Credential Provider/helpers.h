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
#include <credentialprovider.h>

#pragma warning(push)
#pragma warning(disable : 4995)
#include <shlwapi.h>
#pragma warning(pop)

//makes a copy of a field descriptor using CoTaskMemAlloc
HRESULT FieldDescriptorCoAllocCopy(
    __in const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
    );

//makes a copy of a field descriptor on the normal heap
HRESULT FieldDescriptorCopy(
    __in const CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR& rcpfd,
    __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* pcpfd
    );


HRESULT GetCredentialsByCard(LPWSTR sCardID, LPWSTR* sUsername, LPWSTR* sPassword);

HRESULT FileLog(
	__in LPCSTR csLog);

HRESULT SaveBytes(BYTE* pBytes, DWORD cBytes);

HRESULT MyCredPackAuthenticationBufferW(LPWSTR pszUsername, LPWSTR pszPassword, PBYTE pPackedCredentials, PDWORD pcbPackedCredentials);
