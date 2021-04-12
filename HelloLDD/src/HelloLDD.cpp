/*
 ============================================================================
 Name		: HelloLDD.cpp
 Author	  : leftup
 Copyright   : Your copyright notice
 Description : CHelloLDD DLL source
 ============================================================================
 */

#include "HelloLDD.h"


_LIT(KLddName,"RomPEngine");

DECLARE_STANDARD_LDD()
	{
	return new DHelloLDDDriverFactory;
	}

TInt DHelloLDDDriverFactory::Install()
	{
	return SetName(&KLddName);
	}

void DHelloLDDDriverFactory::GetCaps(TDes8& aDes) const
	{
	}

TInt DHelloLDDDriverFactory::Create(DLogicalChannelBase*&aChannel)
	{
	aChannel = new DHelloLDDChannel;
	return aChannel ? KErrNone : KErrNoMemory;
	}

DHelloLDDDriverFactory::DHelloLDDDriverFactory()
	{
	iParseMask = 0;
	iVersion = TVersion(EMajorVersionNumber,EMinorVersionNumber,EBuildVersionNumber);
	}

TInt DHelloLDDChannel::DoCreate(TInt aUnit,const TDesC8* aInfo,const TVersion& aVer)
	{
	if(!Kern::QueryVersionSupported(TVersion(EMajorVersionNumber,EMinorVersionNumber,EBuildVersionNumber),aVer))
		return KErrNotSupported;
	return KErrNone;
	}

TInt DHelloLDDChannel::Request(TInt aReqNo, TAny* a1, TAny* a2)
	{
	TRPMemoryInfo *info = (TRPMemoryInfo *)a1;
	if (!info || !info->iData || info->iLength > info->iData->Length())
		return KErrArgument;
	
	switch (aReqNo)
		{
	case 0:
		{
		memcpy((TAny*)info->iData->Ptr(),(TAny*)info->iAddress,info->iLength);
		break;
		}
	case 1:
		{
		TUint romStart = Epoc::RomHeader();
		TUint romEnd   = romStart+(*(TUint*)(romStart+0x90));
		if(info->iAddress>romStart&&info->iAddress<romEnd)
			Epoc::AllocShadowPage(info->iAddress&0xFFFFF000);
		memcpy((TAny*)info->iAddress,(TAny*)info->iData->Ptr(),info->iLength);
		break;
		}
	default:
		return KErrNotSupported;
		}
	return KErrNone;
	}

