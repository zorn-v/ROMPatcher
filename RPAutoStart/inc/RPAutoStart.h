/*
============================================================================
 Name		: RPAutoStart.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Exe header file
============================================================================
*/

#ifndef __RPAUTOSTART_H__
#define __RPAUTOSTART_H__


//  Include Files

#include <e32base.h>


//  Function Prototypes

GLDEF_C TInt E32Main();

enum TIcons
{
ENormal = 0,
EApply,
EWarning,
EBad,
ELastIcon
};

enum TMethods
{
EAbsolute = 1,
ERelative,
ESearchNReplace,
ELastMethod
};

class TRPMemoryInfo
{
public:
	inline TRPMemoryInfo(const TUint32 aAddress, const TInt32 aLength, TDesC8 *aData)
				: iAddress(aAddress),
				  iLength(aLength),
				  iData(aData) {};
	
public:

	TUint32 iAddress;
	TInt32	iLength;
	TDesC8*	iData;
};
	
class RPDriver : public RBusLogicalChannel
{
public:
        inline TInt Open()  {
        	return DoCreate(_L("RomPEngine"),TVersion(1,0,0),KNullUnit,NULL,NULL);
        }
        inline TBool CheckPatched(TUint32 aAddress, TDesC8 &aCheckData)
        	{
			HBufC8* buff = HBufC8::NewLC(aCheckData.Length());
			TInt err = KErrNone;
			TPtr8 chBuff = buff->Des();
			chBuff.SetLength(aCheckData.Length());
			TRPMemoryInfo info(aAddress,aCheckData.Length(),&chBuff);
			err = DoControl(0,(TAny*)&info,NULL);
			if(chBuff!=aCheckData)
				{
				CleanupStack::PopAndDestroy();
	            return EFalse;
				}

            CleanupStack::PopAndDestroy();
            return ETrue;
        	}
        inline TInt Patch(TUint32 aAddress, TDesC8 &aOldVal, TDesC8 &aNewVal)
        {
        //0-ReadMemory, 1-WriteMemory  

        TInt err = -1;
        TRPMemoryInfo info(aAddress,NULL,NULL);
        if(CheckPatched(aAddress,aOldVal))
        	{
        	info.iLength = aNewVal.Length();
        	info.iData = &aNewVal;
        	}
        if(info.iData != NULL)
        	{
        	//ShadowPage(aAddress & 0xFFFFF000);
        	err = DoControl(1,(TAny*)&info,NULL);
        	}
        return err;
        }
};

#endif  // __RPAUTOSTART_H__


