//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <credentialprovider.h>
#include <windows.h>
#include <strsafe.h>

#include "CSmartCardCredential.h"
#include "helpers.h"
#include "guid.h"
#include "include/CardReader.h"
#include "include/ICardHandler.h"

#define MAX_DWORD   0xffffffff        // maximum DWORD

class CSmartCardProvider : public ICredentialProvider, public ICardHandler
{
  public:
    // IUnknown
    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return ++_cRef;
    }
    
    IFACEMETHODIMP_(ULONG) Release()
    {
        LONG cRef = --_cRef;
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

    IFACEMETHODIMP QueryInterface(__in REFIID riid, __deref_out void** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CSmartCardProvider, ICredentialProvider), // IID_ICredentialProvider
            {0},
        };
        return QISearch(this, qit, riid, ppv);		
    }

  public:
	
	//ICredentialProvider
	IFACEMETHODIMP SetUsageScenario(__in CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus, __in DWORD dwFlags);
    IFACEMETHODIMP SetSerialization(__in const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs);

    IFACEMETHODIMP Advise(__in ICredentialProviderEvents* pcpe, __in UINT_PTR upAdviseContext);
    IFACEMETHODIMP UnAdvise();

    IFACEMETHODIMP GetFieldDescriptorCount(__out DWORD* pdwCount);
    IFACEMETHODIMP GetFieldDescriptorAt(__in DWORD dwIndex,  __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd);

    IFACEMETHODIMP GetCredentialCount(__out DWORD* pdwCount,
                                      __out_range(<,*pdwCount) DWORD* pdwDefault,
                                      __out BOOL* pbAutoLogonWithDefault);
    IFACEMETHODIMP GetCredentialAt(__in DWORD dwIndex, 
                                   __deref_out ICredentialProviderCredential** ppcpc);

	CSmartCardProvider();

	//ICardHandler
	HRESULT HandleCard(Card* card);
  protected:
    
    __override ~CSmartCardProvider();
    
  private:
    



private:
    LONG              _cRef;
    
	// We have only one credential, because we have no any information
	// about user who would like to login. This information will be
	// available only after smart card will be read
	CSmartCardCredential *_pcpc;
    CREDENTIAL_PROVIDER_USAGE_SCENARIO      _cpus;
	ICredentialProviderEvents* _pcpe;
	UINT_PTR _upAdviseContext;
	LPWSTR _sPort;
	CardReader* _pCardReader;
};
