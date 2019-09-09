//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// CSmartCardProvider implements ICredentialProvider, which is the main
// interface that logonUI uses to decide which tiles to display.
// In this sample, we have decided to show two tiles, one for
// Administrator and one for Guest.  You will need to decide what
// tiles make sense for your situation.  Can you enumerate the
// users who will use your method to log on?  Or is it better
// to provide a tile where they can type in their username?
// Does the user need to interact with something other than the
// keyboard before you can recognize which user it is (such as insert 
// a smartcard)?  We call these "event driven" credential providers.  
// We suggest that such credential providers first provide one basic tile which
// tells the user what to do ("insert your smartcard").  Once the
// user performs the action, then you can callback into LogonUI to
// tell it that you have new tiles, and include a tile that is specific
// to the user that the user can then interact with if necessary.

#include <credentialprovider.h>
#include "CSmartCardProvider.h"

// CSmartCardProvider ////////////////////////////////////////////////////////

CSmartCardProvider::CSmartCardProvider():
    _cRef(1),
	_pcpc(NULL),
	_pcpe(nullptr),
	_upAdviseContext(NULL),
	_sPort(new WCHAR[32]())
{
	FileLog("CSmartCardProvider constructor\n");

	//loading port settings
	GetPrivateProfileStringW(L"General", L"Port", L"COM1", _sPort, 32, L"C:/ProgramData/Smart Card Authenticator/Settings.ini");

    DllAddRef();
}

CSmartCardProvider::~CSmartCardProvider()
{
	FileLog("CSmartCardProvider destuctor\n");
    if(_pcpc != NULL)
	{
		_pcpc->Release();
	}

	//Delete card reader. This also cause stopping of reading data from com port
	delete _pCardReader;

    DllRelease();
	FileLog("CSmartCardProvider destuctor end\n");
}





// SetUsageScenario is the provider's cue that it's going to be asked for tiles
// in a subsequent call.  
HRESULT CSmartCardProvider::SetUsageScenario(
    __in CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,
    __in DWORD dwFlags
    )
{
	FileLog("CSmartCardProvider SetUsageScenario\n");
    UNREFERENCED_PARAMETER(dwFlags);
    HRESULT hr;

    static bool s_bCredCreated = false;

    // Decide which scenarios to support here. Returning E_NOTIMPL simply tells the caller
    // that we're not designed for that scenario.
    switch (cpus)
    {
    case CPUS_LOGON:
    case CPUS_UNLOCK_WORKSTATION: 
	case CPUS_CREDUI:
        // A more advanced credprov might only enumerate tiles for the user whose owns the locked
        // session, since those are the only creds that wil work
        if (!s_bCredCreated)
        {
            _cpus = cpus;

			_pcpc = new CSmartCardCredential();
			hr =_pcpc->Initialize(_cpus, s_rgCredProvFieldDescriptors, s_rgFieldStatePairs, L"Insert smart card");

            s_bCredCreated = true;

			//Starting reading cards
			_pCardReader = new CardReader(_sPort, (ICardHandler*)this);
        }
        else
        {
            hr = S_OK;
        }
        break;

	//This credentials provider cannot be used to change passwords
    case CPUS_CHANGE_PASSWORD:
        hr = E_NOTIMPL;
        break;

    default:
        hr = E_INVALIDARG;
        break;
    }

    return hr;
}

// SetSerialization takes the kind of buffer that you would normally return to LogonUI for
// an authentication attempt.  It's the opposite of ICredentialProviderCredential::GetSerialization.
// GetSerialization is implement by a credential and serializes that credential.  Instead,
// SetSerialization takes the serialization and uses it to create a credential.
//
// SetSerialization is called for two main scenarios.  The first scenario is in the credui case
// where it is prepopulating a tile with credentials that the user chose to store in the OS.
// The second situation is in a remote logon case where the remote client may wish to 
// prepopulate a tile with a username, or in some cases, completely populate the tile and
// use it to logon without showing any UI.
HRESULT CSmartCardProvider::SetSerialization(
    __in const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs
    )
{
	UNREFERENCED_PARAMETER(pcpcs);
	HRESULT hr = E_NOTIMPL;

    return hr;
}

// Called by LogonUI to give you a callback.  Providers often use the callback if they
// some event would cause them to need to change the set of tiles that they enumerated
HRESULT CSmartCardProvider::Advise(
    __in ICredentialProviderEvents* pcpe,
    __in UINT_PTR upAdviseContext
    )
{
	_pcpe = pcpe;
	_pcpe->AddRef();
	_upAdviseContext = upAdviseContext;

	return S_OK;
}

// Called by LogonUI when the ICredentialProviderEvents callback is no longer valid.
HRESULT CSmartCardProvider::UnAdvise()
{
	_pcpe->Release();
	_pcpe = nullptr;
	_upAdviseContext = NULL;

    return S_OK;
}

// Called by LogonUI to determine the number of fields in your tiles.  This
// does mean that all your tiles must have the same number of fields.
// This number must include both visible and invisible fields. If you want a tile
// to have different fields from the other tiles you enumerate for a given usage
// scenario you must include them all in this count and then hide/show them as desired 
// using the field descriptors.
HRESULT CSmartCardProvider::GetFieldDescriptorCount(
    __out DWORD* pdwCount
    )
{
	FileLog("CSmartCardProvider GetFieldDescriptorCount\n");
    *pdwCount = SFI_NUM_FIELDS;

    return S_OK;
}

// Gets the field descriptor for a particular field
HRESULT CSmartCardProvider::GetFieldDescriptorAt(
    __in DWORD dwIndex, 
    __deref_out CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR** ppcpfd
    )
{    
	FileLog("CSmartCardProvider GetFieldDescriptorAt\n");
    HRESULT hr;

    // Verify dwIndex is a valid field.
    if ((dwIndex < SFI_NUM_FIELDS) && ppcpfd)
    {
        hr = FieldDescriptorCoAllocCopy(s_rgCredProvFieldDescriptors[dwIndex], ppcpfd);
    }
    else
    { 
        hr = E_INVALIDARG;
    }

    return hr;
}

// Sets pdwCount to the number of tiles that we wish to show at this time.
// Sets pdwDefault to the index of the tile which should be used as the default.
//
// The default tile is the tile which will be shown in the zoomed view by default. If 
// more than one provider specifies a default tile the behavior is the last used cred
// prov gets to specify the default tile to be displayed
//
// If *pbAutoLogonWithDefault is TRUE, LogonUI will immediately call GetSerialization
// on the credential you've specified as the default and will submit that credential
// for authentication without showing any further UI.
HRESULT CSmartCardProvider::GetCredentialCount(
    __out DWORD* pdwCount,
    __out_range(<,*pdwCount) DWORD* pdwDefault,
    __out BOOL* pbAutoLogonWithDefault
    )
{
	FileLog("CSmartCardProvider GetCredentialCount\n");
    HRESULT hr = S_OK;

	*pdwCount = 1;
	*pdwDefault = 0;
	*pbAutoLogonWithDefault = FALSE;

    return hr;
}

// Returns the credential at the index specified by dwIndex. This function is called by logonUI to enumerate
// the tiles.
HRESULT CSmartCardProvider::GetCredentialAt(
    __in DWORD dwIndex, 
    __deref_out ICredentialProviderCredential** ppcpc
    )
{
	FileLog("CSmartCardProvider GetCredentialAt\n");
    HRESULT hr;

    // Validate parameters.
    if((dwIndex < 1) && ppcpc)
    {
        hr = _pcpc->QueryInterface(IID_ICredentialProviderCredential, reinterpret_cast<void**>(ppcpc));
    }
    else
    {
        hr = E_INVALIDARG;
    }
        
    return hr;
}

HRESULT CSmartCardProvider::HandleCard(Card* card)
{
	FileLog("CSmartCardProvider HandleCard\n");

	LPWSTR sCardID;
	if (SUCCEEDED(card->GetCardID(&sCardID)))
	{
		LPWSTR sUsername;
		LPWSTR sPassword;

		//if smartcard was found in the users database we should change the credential
		if (SUCCEEDED(GetCredentialsByCard(sCardID, &sUsername, &sPassword)))
		{
			_pcpc->SetUsername(sUsername);
			_pcpc->SetPassword(sPassword);

			//If card was read, logonui should immediately
			//authorize user using this credentials
			_pcpc->EnableAutoLogin();

			//reload credentials, because login/password has changed
			_pcpe->CredentialsChanged(_upAdviseContext);
		}
		delete card;
	}

	return S_OK;
}



