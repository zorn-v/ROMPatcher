/*
========================================================================
 Name        : ROMPatcherListBoxView.h
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef ROMPATCHERLISTBOXVIEW_H
#define ROMPATCHERLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]

// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CROMPatcherListBox;
// ]]] end generated region [Generated Forward Declarations]
_LIT(KListDataFile,"D:\\RPData.dat");
_LIT(KAutoStartFile,"C:\\System\\Data\\RPAuto.dat");
// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

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
/*
			TBuf8<256> buff;
            TUint16 readed = 0;
            TInt err = KErrNone;
            TUint Length = aCheckData.Length();
            TMetroTrkMemoryInfo info(aAddress,Min(Length,256),&buff);
            while(err==KErrNone&&readed<Length)
            	{
            	err = DoControl(6,NULL,(TAny*)&info);
            	readed += info.iLength;
            	Length = info.iLength-readed;
            	if(err!=KErrNone||buff.Compare(aCheckData.Mid(readed-info.iLength,readed))!=0)
            		return EFalse;
            	
            	info.iAddress += readed;
            	info.iLength = Min(Length,256);            	
            	}
*/
            CleanupStack::PopAndDestroy();
            return ETrue;
        	}
        /*
        inline TInt ShadowPage(TUint32 aAddress)
        	{
        	TUint32 RemappedAddress;
        	return DoControl(0,(TAny*)aAddress,(TAny*)&RemappedAddress);
        	}
        */
        inline TInt Patch(TUint32 aAddress, TDesC8 &aOldVal, TDesC8 &aNewVal, TBool aPatched)
        {
        //0-ReadMemory, 1-WriteMemory  

        TInt err = -1;
        TRPMemoryInfo info(aAddress,NULL,NULL);
        if(!aPatched&&CheckPatched(aAddress,aOldVal))
        	{
        	info.iLength = aNewVal.Length();
        	info.iData = &aNewVal;
        	}
        if(aPatched&&CheckPatched(aAddress,aNewVal))
        	{
        	info.iLength = aOldVal.Length();
        	info.iData = &aOldVal;
        	}
        if(info.iData != NULL)
        	{
        	//ShadowPage(aAddress & 0xFFFFF000);
        	err = DoControl(1,(TAny*)&info,NULL);
        	}
        return err;
        }
};

/**
 * Avkon view class for ROMPatcherListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CROMPatcherListBoxView ROMPatcherListBoxView.h
 */
class CROMPatcherListBoxView : public CAknView
	{
public:
	// constructors and destructor
	CROMPatcherListBoxView();
	static CROMPatcherListBoxView* NewL();
	static CROMPatcherListBoxView* NewLC();        
	void ConstructL();
	virtual ~CROMPatcherListBoxView();

public:
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );

protected:
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	TInt ApplyPatch(TPtr8& aData, TBool aPatched);
	void PatchFileName(TDes& aFileName);
	void SaveList();
	void SaveAuto();
//	TUint FindFileAddr(TPtr8& aFileName);
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	
	// [[[ begin [Overridden Methods]
protected: 
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	TBool HandleTestMenuItemSelectedL( TInt aCommand );
	TBool HandleEnable_capsMenuItemSelectedL( TInt aCommand );
	TBool HandleApplyMenuItemSelectedL( TInt aCommand );
	TBool HandleAbout_MenuItemSelectedL( TInt aCommand );
	TBool HandlePatch_infoMenuItemSelectedL( TInt aCommand );
	TBool HandleAdd_to_autostartMenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CROMPatcherListBox* iROMPatcherListBox;
	// ]]] end generated region [Generated Instance Variables]

	RPDriver iDriver;
	TBool iDrvLoaded;
	TBool iCapsPatched;
	TUint32 iCapsAddress;
	TBuf8<4>  iCapsOldVal;
	TBuf8<4>  iCapsNewVal;
	CArrayFix<TBool>* iPatched;
	CArrayFix<TBool>* iAutoStart;
	};

#endif // ROMPATCHERLISTBOXVIEW_H			
