/*
 ============================================================================
 Name		: HelloLDD.h
 Author	  : leftup
 Copyright   : Your copyright notice
 Description : HelloLDD.h - CHelloLDD class header
 ============================================================================
 */

// This file defines the API for HelloLDD.dll

#ifndef __HELLOLDD_H__
#define __HELLOLDD_H__
#include "ekern.h"
#include "hellouserside.h"
#include "LDDCommon.h"
class DHelloLDDDriverFactory: public DLogicalDevice
{
public:

	DHelloLDDDriverFactory();
	virtual TInt Install();
	virtual void GetCaps(TDes8& aDes) const;
	virtual TInt Create(DLogicalChannelBase*&aChannel);
};

class DHelloLDDChannel: public DLogicalChannelBase
	{
	//IMPORT_C virtual ~DLogicalChannelBase();
public:
	TInt DoCreate(TInt aUnit,const TDesC8* aInfo,const TVersion& aVer);	
	TInt Request(TInt aReqNo, TAny* a1, TAny* a2);	
	};
#endif  // __HELLOLDD_H__

