//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Standard dll required functions and class factory implementation.

#include <windows.h>
#include <unknwn.h>
#include "Dll.h"
#include "helpers.h"
#include "CSmartCardProvider.h"

static LONG g_cRef = 0;   // global dll reference count
HINSTANCE g_hinst = NULL; // global dll hinstance

//extern HRESULT CSample_CreateInstance(__in REFIID riid, __deref_out void** ppv);
//EXTERN_C GUID CLSID_CSample;

class CClassFactory : public IClassFactory
{
public:
    CClassFactory() : _cRef(1) 
    {
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(__in REFIID riid, __deref_out void **ppv)
    {
		FileLog("CClassFactory QueryInterface\n");
        static const QITAB qit[] = 
        {
            QITABENT(CClassFactory, IClassFactory),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
		/*UNREFERENCED_PARAMETER(riid);
		*ppv = (IClassFactory*)this;*/

    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
		FileLog("CClassFactory AddRef\n");
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
		FileLog("CClassFactory Release\n");
        LONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
            delete this;
        return cRef;
    }

    // IClassFactory
    IFACEMETHODIMP CreateInstance(__in IUnknown* pUnkOuter, __in REFIID riid, __deref_out void **ppv)
    {
		FileLog("CClassFactory CreateInstance\n");
        HRESULT hr;
        if (!pUnkOuter)
        {
			CSmartCardProvider* pProvider = new CSmartCardProvider();

			if (pProvider)
			{
				hr = pProvider->QueryInterface(riid, ppv);
				pProvider->Release();
			}
			else
			{
				hr = E_OUTOFMEMORY;
			}
        }
        else
        {
            *ppv = NULL;
            hr = CLASS_E_NOAGGREGATION;
        }
        return hr;
    }

    IFACEMETHODIMP LockServer(__in BOOL bLock)
    {
        if (bLock)
        {
            DllAddRef();
        }
        else
        {
            DllRelease();
        }
        return S_OK;
    }

private:
    ~CClassFactory()
    {
    }
    long _cRef;
};


void DllAddRef()
{
	FileLog("DllAddRef\n");
    InterlockedIncrement(&g_cRef);
}

void DllRelease()
{
	FileLog("DllRelease\n");
    InterlockedDecrement(&g_cRef);
}

STDAPI DllCanUnloadNow()
{
	FileLog("DllCanUnloadNow\n");
	if (g_cRef > 0) return S_FALSE;
	else return S_OK;
}

STDAPI DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, __deref_out void** ppv)
{
	FileLog("DllGetClassObject\n");

	*ppv = NULL;

	HRESULT hr;

	if (CLSID_CSample == rclsid)
	{
		CClassFactory* pcf = new CClassFactory();
		if (pcf)
		{
			hr = pcf->QueryInterface(riid, ppv);
			pcf->Release();
		}
		else
		{
			hr = E_OUTOFMEMORY;
		}
	}
	else
	{
		hr = CLASS_E_CLASSNOTAVAILABLE;
	}
	return hr;
}

STDAPI_(BOOL) DllMain(__in HINSTANCE hinstDll, __in DWORD dwReason, __in void *)
{
	FileLog("DllMain\n");
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDll);
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    
    g_hinst = hinstDll;
    return TRUE;
}

