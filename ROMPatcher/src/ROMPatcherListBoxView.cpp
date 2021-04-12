/*
========================================================================
 Name        : ROMPatcherListBoxView.cpp
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <ROMPatcher.rsg>
// ]]] end generated region [Generated System Includes]

#include <aknmessagequerydialog.h>
#include <bautils.h>
//#include <bacline.h>

// [[[ begin generated region: do not modify [Generated User Includes]
#include "ROMPatcher.hrh"
#include "ROMPatcherListBoxView.h"
#include "ROMPatcherListBox.hrh"
#include "ROMPatcherListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CROMPatcherListBoxView::CROMPatcherListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iROMPatcherListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	}
/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CROMPatcherListBoxView::~CROMPatcherListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iROMPatcherListBox;
	iROMPatcherListBox = NULL;
	// ]]] end generated region [Generated Contents]
	delete iPatched;
	iPatched = NULL;
	delete iAutoStart;
	iAutoStart = NULL;
	if(iDrvLoaded)
		{
		iDriver.Close();
		User::FreeLogicalDevice(_L("RomPEngine"));
		}
	}

void CROMPatcherListBoxView::DynInitMenuPaneL(
	    TInt aResourceId, CEikMenuPane* aMenuPane)
	    {
		if (aResourceId == R_ROMPATCHER_LIST_BOX_MENU_PANE1_MENU_PANE)
			{
			aMenuPane->SetItemDimmed(EROMPatcherListBoxViewTestMenuItemCommand, iCapsPatched);
			aMenuPane->SetItemDimmed(EROMPatcherListBoxViewEnable_capsMenuItemCommand, !iCapsPatched);
			}
		
		if (aResourceId == R_ROMPATCHER_LIST_BOX_MENU_PANE2)
			{
			TInt index = iROMPatcherListBox->ListBox()->CurrentItemIndex();
			if(index>=0)
				{
				aMenuPane->SetItemDimmed(EROMPatcherListBoxViewApplyMenuItemCommand, (*iPatched)[index]);
				aMenuPane->SetItemDimmed(EROMPatcherListBoxViewDisableMenuItemCommand, !(*iPatched)[index]);

				aMenuPane->SetItemDimmed(EROMPatcherListBoxViewAdd_to_autostartMenuItemCommand, (*iAutoStart)[index]);
				aMenuPane->SetItemDimmed(EROMPatcherListBoxViewRemove_from_autostartMenuItemCommand, !(*iAutoStart)[index]);
				}
			}	
	    }


/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CROMPatcherListBoxView
 */
CROMPatcherListBoxView* CROMPatcherListBoxView::NewL()
	{
	CROMPatcherListBoxView* self = CROMPatcherListBoxView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CROMPatcherListBoxView
 */
CROMPatcherListBoxView* CROMPatcherListBoxView::NewLC()
	{
	CROMPatcherListBoxView* self = new ( ELeave ) CROMPatcherListBoxView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CROMPatcherListBoxView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_ROMPATCHER_LIST_BOX_ROMPATCHER_LIST_BOX_VIEW );
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	iDrvLoaded = EFalse;
	iCapsAddress = 0xC0000148;

	iCapsOldVal.FillZ(4);
	iCapsOldVal[0] = 0x1E;
	iCapsNewVal.FillZ(4);
	iCapsNewVal[0] = 0x10;

	_LIT(KSettingsFile,"\\System\\data\\RP.dat");
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	if(BaflUtils::FileExists(fs,KSettingsFile))
    	{
    	RFile file;
		User::LeaveIfError(file.Open(fs, KSettingsFile, EFileStream|EFileRead|EFileShareReadersOnly));
		CleanupClosePushL(file);
		TInt size;
		User::LeaveIfError(file.Size(size));
		if(size>=12)
			{
			TBuf8<12> buff;
			User::LeaveIfError(file.Read(buff,12));
			iCapsAddress = buff[3]|(buff[2]<<8)|(buff[1]<<16)|(buff[0]<<24);
			iCapsOldVal = buff.Mid(4,4);
			iCapsNewVal = buff.Mid(8,4);
			}
		file.Close();
		CleanupStack::PopAndDestroy(); // file
    	}
	CleanupStack::PopAndDestroy(); // fs

#ifndef __WINS__
	User::FreeLogicalDevice(_L("RomPEngine"));
	TInt err = User::LoadLogicalDevice(_L("RPEngine.ldd"));
  	if ((err!=KErrNone) && (err!=KErrAlreadyExists))
	{
		User::Leave(err);
	}
  	
  	err = iDriver.Open();
  	if (err!=KErrNone)
  	{
  		User::FreeLogicalDevice(_L("RomPEngine"));
  		User::Leave(err);
  	}
  	iCapsPatched = iDriver.CheckPatched(iCapsAddress,iCapsNewVal);
  	iDrvLoaded = ETrue;
#endif
/*  	CCommandLineArguments* args = CCommandLineArguments::NewLC();
  	if(args->Count()>1)
  		if(args->Arg(1)==_L("auto"))
  		{
  		iDriver.Patch(iCapsAddress,iCapsOldVal,iCapsNewVal,iCapsPatched);
  		AppUi()->HandleCommandL( EEikCmdExit );
  		}
  	CleanupStack::PopAndDestroy(); //args
 */
	}
	
/**
 * @return The UID for this view
 */
TUid CROMPatcherListBoxView::Id() const
	{
	return TUid::Uid( EROMPatcherListBoxViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CROMPatcherListBoxView::HandleCommandL( TInt aCommand )
	{   
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		case EROMPatcherListBoxViewApplyMenuItemCommand:
			commandHandled = HandleApplyMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewDisableMenuItemCommand:
			commandHandled = HandleApplyMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewAdd_to_autostartMenuItemCommand:
			commandHandled = HandleAdd_to_autostartMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewRemove_from_autostartMenuItemCommand:
			commandHandled = HandleAdd_to_autostartMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewPatch_infoMenuItemCommand:
			commandHandled = HandleApplyMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewTestMenuItemCommand:
			commandHandled = HandleTestMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewEnable_capsMenuItemCommand:
			commandHandled = HandleTestMenuItemSelectedL( aCommand );
			break;
		case EROMPatcherListBoxViewAbout_MenuItemCommand:
			commandHandled = HandleAbout_MenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CROMPatcherListBoxView::DoActivateL(
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
	CEikButtonGroupContainer* cba = AppUi()->Cba();
	if ( cba != NULL ) 
		{
		cba->MakeVisible( EFalse );
		}
	
	if ( iROMPatcherListBox == NULL )
		{
		iROMPatcherListBox = CROMPatcherListBox::NewL( ClientRect(), NULL, this );
		iROMPatcherListBox->SetMopParent( this );
		AppUi()->AddToStackL( *this, iROMPatcherListBox );
		} 
	// ]]] end generated region [Generated Contents]
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	if(BaflUtils::FileExists(fs,_L("E:\\Patches")))
		{
		TBuf<256> pName;
		CDir* dirList;
		User::LeaveIfError(fs.GetDir(_L("E:\\Patches\\*.rmp"),
							KEntryAttNormal,ESortByName,dirList));

		
		HBufC* cFileData = NULL;
		if(BaflUtils::FileExists(fs,KListDataFile))
	   		{
	   		RFile file;
	   		User::LeaveIfError(file.Open(fs, KListDataFile, EFileStream|EFileRead|EFileShareReadersOnly));
	   		CleanupClosePushL(file);	   		

	   		TInt size;
			User::LeaveIfError(file.Size(size));
			
			HBufC8* fileData = HBufC8::NewLC(size);
			TPtr8 pFileData = fileData->Des();
			User::LeaveIfError(file.Read(pFileData,size));
		    file.Close();
		    
		    cFileData = HBufC::NewLC(size);
		    cFileData->Des().Copy(pFileData);
	   		}
		
		HBufC* cAutoData = NULL;
		if(BaflUtils::FileExists(fs,KAutoStartFile))
	   		{
	   		RFile autoFile;
	   		User::LeaveIfError(autoFile.Open(fs, KAutoStartFile, EFileStream|EFileRead|EFileShareReadersOnly));
	   		CleanupClosePushL(autoFile);	   		

	   		TInt size;
			User::LeaveIfError(autoFile.Size(size));
			
			HBufC8* autoData = HBufC8::NewLC(size);
			TPtr8 pAutoData = autoData->Des();
			User::LeaveIfError(autoFile.Read(pAutoData,size));
			autoFile.Close();		    
		    
		    cAutoData = HBufC::NewLC(size);
		    cAutoData->Des().Copy(pAutoData);
	   		}
		
		if(dirList->Count()>0)
			{
			iPatched = new (ELeave) CArrayFixFlat<TBool>(dirList->Count());
			iAutoStart = new (ELeave) CArrayFixFlat<TBool>(dirList->Count());
			for (TInt i=0;i<dirList->Count();i++)
				{
				pName = (*dirList)[i].iName;
				pName.Delete(pName.Length()-4,4);

				TInt  index = ENormal;
				TBool patched = EFalse;
				if(cFileData!=NULL)
					{
					TInt pos = cFileData->Find(pName)-3;
					if(pos>=0) patched = (*cFileData)[pos]==1 ? 1 : 0;
					if(pos>=0) index = (*cFileData)[pos+1]-0x30;
					}
				iPatched->AppendL(patched);
				
				TBool autoStart = EFalse;
				if(cAutoData!=NULL)
					{
					TInt pos = cAutoData->Find(pName);
					if(pos>0)
						{
						autoStart = ETrue;
						if(index<ELastIcon)
							index += ELastIcon;
						}
					}
				iAutoStart->AppendL(autoStart);
				
				CROMPatcherListBox::CreateListBoxItemL(pName, index,(*dirList)[i].iName );
				pName.Delete(pName.Length()-4,4);
				CROMPatcherListBox::AddListBoxItemL( iROMPatcherListBox->ListBox(), pName );
				}			
			}
		delete dirList;
		if(cAutoData!=NULL) CleanupStack::PopAndDestroy(3); //cAutoData, autoData, autoFile
		if(cFileData!=NULL) CleanupStack::PopAndDestroy(3); //cFileData, fileData, file

		}	
	CleanupStack::PopAndDestroy(); //fs
	}

/**
 */
void CROMPatcherListBoxView::DoDeactivate()
	{

	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	CEikButtonGroupContainer* cba = AppUi()->Cba();
	if ( cba != NULL ) 
		{
		cba->MakeVisible( ETrue );
		cba->DrawDeferred();
		}
	
	if ( iROMPatcherListBox != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iROMPatcherListBox );
		delete iROMPatcherListBox;
		iROMPatcherListBox = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

// [[[ begin generated function: do not modify
void CROMPatcherListBoxView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_ROMPATCHER_LIST_BOX_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}
// ]]] end generated function

// [[[ begin generated function: do not modify
void CROMPatcherListBoxView::CleanupStatusPane()
	{
	}
// ]]] end generated function

/** 
 * Handle status pane size change for this view (override)
 */
void CROMPatcherListBoxView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TInt result;
	TRAP( result, SetupStatusPaneL() ); 
	}
	
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CROMPatcherListBoxView::HandleTestMenuItemSelectedL( TInt /*aCommand*/ )
	{
	if(iDrvLoaded)
		{
  		if(iDriver.Patch(iCapsAddress,iCapsOldVal,iCapsNewVal,iCapsPatched)!=KErrNone)
  			return ETrue;
		}
	iCapsPatched = !iCapsPatched;
	return ETrue;
	}

/*
void AddLog(TDesC8& aLogLine)
	{
	_LIT(KLogFile,"C:\\RP.log");
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFile file;
	if(BaflUtils::FileExists(fs,KLogFile))
		User::LeaveIfError(file.Open(fs, KLogFile, EFileStreamText|EFileWrite|EFileShareExclusive));
   	else
   		User::LeaveIfError(file.Create(fs, KLogFile, EFileStreamText|EFileWrite|EFileShareExclusive));
    
	CleanupClosePushL(file);
	TInt seek = 0;
//	User::LeaveIfError(file.Size(size));
	User::LeaveIfError(file.Seek(ESeekEnd,seek));
	User::LeaveIfError(file.Write(aLogLine));
	file.Close();
	CleanupStack::PopAndDestroy(2); // file, fs
	}
*/

TBool ConvertTextData(TPtrC8 aData, TPtr8 aConverted)
	{
	TInt len = aData.Length();
	if((len%2)!=0) return EFalse;
	aConverted.FillZ(len/2);
	for(TInt i=0;i<len;i+=2)
		{
		//addr = addr | ()
		TChar t1 = aData[i];
		if(t1>=0x30&&t1<=0x39)
			t1 -= 0x30;
		else if(t1>=0x41&&t1<=0x46)
			t1 -= 0x37;
		else
			return EFalse;

		TChar t2 = aData[i+1];
		if(t2>=0x30&&t2<=0x39)
			t2 -= 0x30;
		else if(t2>=0x41&&t2<=0x46)
			t2 -= 0x37;
		else
			return EFalse;

		aConverted[i/2] = (t1<<4)|t2;
		//addr = addr | ((t1<<8)<<(len-i));
		}
	return ETrue;
	}

TInt PosPatchData(TPtr8& aData, TChar aChar, TBool aTrimOnly)
	{
	TInt pos = aData.Locate(aChar);
	if(pos<=0||aTrimOnly) 
		{
		pos = aData.Locate('\n');
		if(pos<=0)pos = aData.Length();
		aData.Delete(0,pos);
		aData.Trim();
		return -1;
		}
	return pos;
	}
/*
void WideToNormal(TDes& aWide, TPtr8& aNormal)
	{
	aNormal.SetLength(aWide.Length()*2);
	for(TInt i=0;i<aWide.Length();i++)
		{
		aNormal[i*2]   =  aWide[i] & 0xFF;
        aNormal[i*2+1] = (aWide[i] & 0xFF00)>>8;
		}
	}
*/
TUint FindFileAddr(TPtrC8 aFileName)
	{
	TUint addr = 0;
	TUint dirAddr = UserSvr::RomRootDirectoryAddress(); //F8013470
	HBufC8* hFileName = HBufC8::NewLC(aFileName.Length());
	TPtr8 fileName = hFileName->Des();
	fileName.Copy(aFileName);
	while(fileName.Length()>0)
		{
		TUint oldAddr = dirAddr;
		TInt pos = fileName.Locate('\\');
		TBool last = pos<0;
		if(last) pos = fileName.Length();

		TPtrC8 fDir(fileName.Left(pos));
		
		TUint* PInt=(TUint*)dirAddr;
        TUint size=*PInt;
        TUint dPos = dirAddr+13;
        while((dirAddr+size)>dPos)
        	{
        	TUint8* PInt8=(TUint8*)dPos;
        	TUint8 nLen = *PInt8;
        	
        	HBufC8* Data = HBufC8::NewLC(nLen);
        	TPtr8 pData = Data->Des();
        	pData.SetLength(nLen);
         	
        	for(TUint i=0;i<nLen;i++)
        		{
        		PInt8=(TUint8*)(dPos+(i*2)+1);
        		pData[i] = *PInt8;
        		}
        	pData.UpperCase();
        	if(pData.Compare(fDir)==0)
        		{
        		PInt = (TUint*)(dPos-5);
        		dirAddr = *PInt;
        		break;
        		}
        	CleanupStack::PopAndDestroy();
        	
        	dPos += (nLen*2+10);
        	if((nLen%2)==0)
        		dPos += 2;
        	}
        if(oldAddr==dirAddr)
        	break;
        if(last&&oldAddr!=dirAddr)
        	{
        	addr = dirAddr;
        	break;
        	}
        fileName.Delete(0,pos+1);
		}
	CleanupStack::PopAndDestroy();
	return addr;
	}

TInt CROMPatcherListBoxView::ApplyPatch(TPtr8& aData, TBool aPatched)
	{
	TInt res = ENormal;
	TUint pCount = 0;
	TUint errCount = 0;
	TInt pos = aData.Locate('\n')+1;
	if(pos<=1) return EBad;
	aData.Delete(0,pos);
	aData.Trim();
	aData.UpperCase();
	while(aData.Length()>0)
		{
		pos = PosPatchData(aData,':',EFalse);
		if(pos<0) continue;
		TPtrC8 method(aData.Left(pos));
		TUint8 met = 0;
		TBuf<256> zFname;
		if(method==_L8("ABS")) met = EAbsolute;
		if(method==_L8("REL")) met = ERelative;
		if(method==_L8("SNR")) met = ESearchNReplace;
		if(met>0&&met<ELastMethod)
			{
			aData.Delete(0,pos+1);
			pos = PosPatchData(aData,':',EFalse);
			if(pos<0)
				{
				res = EWarning;
				continue;
				}
			TUint off = 0;
			if(met == ERelative||met == ESearchNReplace)
				{
				TPtrC8 fName(aData.Left(pos));
				if(met == ESearchNReplace)
					{
					TBuf<256> tmp;
					tmp.Copy(fName);
					zFname.Format(_L("Z:\\%S"),&tmp);
					}
				off = FindFileAddr(fName);
				if(off==0)
					{
					res = EWarning;
					continue;
					}
				aData.Delete(0,pos+1);
				pos = PosPatchData(aData,':',EFalse);
				if(pos<0)
					{
					res = EWarning;
					continue;
					}
				}

			TUint32 addr = 0;
			if(met!=ESearchNReplace)
				{
				TPtrC8 address(aData.Left(pos));
				HBufC8* addrData = HBufC8::NewLC(address.Length()/2);
				if(!ConvertTextData(address,addrData->Des()))
					{
					PosPatchData(aData,'\n',ETrue);
					CleanupStack::PopAndDestroy(); //addrData
					res = EWarning;
					continue;
					}
				TInt len = addrData->Length()-1;
				for(TInt i = len; i>=0; i--)
					addr = addr | ((*addrData)[i]<<((len-i)*8));

				CleanupStack::PopAndDestroy(); //addrData
				addr += off;
				aData.Delete(0,pos+1);
				pos = PosPatchData(aData,':',EFalse);
				if(pos<0)
					{
					res = EWarning;
					continue;
					}				
				}
			
			TPtrC8 oldbytes(aData.Left(pos));
			HBufC8* oldData = HBufC8::NewLC(oldbytes.Length()/2);
			if(!ConvertTextData(oldbytes,oldData->Des()))
				{
				PosPatchData(aData,'\n',ETrue);

				CleanupStack::PopAndDestroy(); //oldData
				res = EWarning;
				continue;
				}
			aData.Delete(0,pos+1);
			pos = aData.Locate('\n')-1;
			if(pos<0) pos = aData.Length();
			TPtrC8 newbytes(aData.Left(pos));
			HBufC8* newData = HBufC8::NewLC(newbytes.Length()/2);
			if(!ConvertTextData(newbytes,newData->Des()))
				{
				PosPatchData(aData,'\n',ETrue);
					
				CleanupStack::PopAndDestroy(2); //oldData, newData
				res = EWarning;
				continue;
				}
			TInt err = KErrNone;
			if(oldData->Length()==newData->Length())
				{
				if(met==ESearchNReplace)
					{
					RFs fs;
					User::LeaveIfError(fs.Connect());
					CleanupClosePushL(fs);
					
					RFile file;
					User::LeaveIfError(file.Open(fs, zFname, EFileStream|EFileRead|EFileShareReadersOnly));
					CleanupClosePushL(file);

					TInt size;
					User::LeaveIfError(file.Size(size));
										
					HBufC8* fileData = HBufC8::NewLC(size);
					TPtr8 pFileData = fileData->Des();
					User::LeaveIfError(file.Read(pFileData,size));
				    file.Close();
				    HBufC8* searchData = aPatched ? newData : oldData;
				    pos = pFileData.Find(*searchData);
				    while(pos>=0)
				    	{
				    	if(iDriver.Patch(off+pos,*oldData,*newData, aPatched)!=KErrNone)
				    		{
				    		res = EWarning;
				    		break;
				    		}
				    	else
				    		{
				    		pCount++;
				    		pFileData.Delete(pos,searchData->Length());
				    		pos = pFileData.Find(*searchData);
				    		}
				    	}
				    CleanupStack::PopAndDestroy(); //fileData
					}
				else
					{
					pCount++;
					err = iDriver.Patch(addr,*oldData,*newData, aPatched);
					}
				}
			else res = EWarning;
				
			if(err!=KErrNone)
				{
				errCount++;
				res = EWarning;
				}
			if(res==ENormal&&!aPatched)
				res = EApply;
			CleanupStack::PopAndDestroy(2); //oldData, newData
			}
		PosPatchData(aData,'\n',ETrue);
		}
	if(errCount==pCount)
		res = EBad;
	return res;
	}

void CROMPatcherListBoxView::PatchFileName(TDes& aFileName)
	{
	TBuf<256> fName;
	CROMPatcherListBox::CurrentItemTextL( iROMPatcherListBox->ListBox(), fName );
	fName.Delete(0,fName.Locate('\t')+1);
	aFileName.Format(_L("E:\\Patches\\%S.rmp"),&fName);
	}

void CROMPatcherListBoxView::SaveList()
	{
	CTextListBoxModel* model = iROMPatcherListBox->ListBox()->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFile file;
		
	fs.SetAtt(KListDataFile,KEntryAttNormal,KEntryAttSystem|KEntryAttHidden|KEntryAttReadOnly);
	fs.Delete(KListDataFile);
	User::LeaveIfError(file.Create(fs, KListDataFile, EFileStream|EFileWrite|EFileShareExclusive));
		
	CleanupClosePushL(file);

	for(TInt i=0; i<itemArray->Count(); i++)
		{
		TBuf8<257> item8;
		item8.Append((*iPatched)[i]);
		item8.Append((*itemArray)[i]);
		User::LeaveIfError(file.Write(item8));
		}

	file.Close();
	fs.SetAtt(KListDataFile,KEntryAttSystem|KEntryAttHidden|KEntryAttReadOnly,KEntryAttNormal);
	CleanupStack::PopAndDestroy(2); // file, fs
	}

void CROMPatcherListBoxView::SaveAuto()
	{
	CTextListBoxModel* model = iROMPatcherListBox->ListBox()->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFile file;
		
	fs.Delete(KAutoStartFile);
	User::LeaveIfError(file.Create(fs, KAutoStartFile, EFileStream|EFileWrite|EFileShareExclusive));
		
	CleanupClosePushL(file);

	for(TInt i=0; i<itemArray->Count(); i++)
		{
		if((*iAutoStart)[i])
			{
			TBuf8<257> item8;
			item8.Append((*itemArray)[i].Length()-2);
			item8.Append((*itemArray)[i].Mid(2));
			User::LeaveIfError(file.Write(item8));
			}
		}
	file.Close();
	CleanupStack::PopAndDestroy(2); // file, fs
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CROMPatcherListBoxView::HandleApplyMenuItemSelectedL( TInt aCommand )
	{
	if(iDrvLoaded)
		{
		TBuf<256> fname;
		PatchFileName(fname);
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		if(BaflUtils::FileExists(fs,fname))
	    	{
	    	RFile file;
			User::LeaveIfError(file.Open(fs, fname, EFileStreamText|EFileRead|EFileShareReadersOnly));
			CleanupClosePushL(file);

			TInt size;
			User::LeaveIfError(file.Size(size));
			
			HBufC8* fileData = HBufC8::NewLC(size);
			TPtr8 pFileData = fileData->Des();
			User::LeaveIfError(file.Read(pFileData,size));
		    file.Close();

		    TUint index = 0;

		    if(aCommand == EROMPatcherListBoxViewApplyMenuItemCommand)
		    	index = ApplyPatch(pFileData,EFalse);
		    else if(aCommand == EROMPatcherListBoxViewDisableMenuItemCommand)
		    	index = ApplyPatch(pFileData,ETrue);
		    else if(aCommand == EROMPatcherListBoxViewPatch_infoMenuItemCommand)
		    	{
		    	TInt pos = pFileData.Locate('\n');
		    	if(pos>0)
		    		{
		    		pFileData.Trim();
		    		TBuf<256> pTitle;
		    		CROMPatcherListBox::CurrentItemTextL( iROMPatcherListBox->ListBox(), pTitle );
		    		pTitle.Delete(0,pTitle.Locate('\t')+1);
		    		
		    		TInt nPos = pFileData.Find(_L8("\\n"));
		    		while(nPos!=KErrNotFound)
		    			{
		    			pFileData[nPos] = 0x0D;
		    			pFileData[nPos+1] = 0x0A;
		    			nPos = pFileData.Find(_L8("\\n"));
		    			}
		    		TBuf<256> msg;
		    		msg.Copy(pFileData.Left(Min(pos,256)));
		    		CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
		    		dlg->PrepareLC(R_ABOUT_MESSAGE_NOTE);
		    		dlg->QueryHeading()->SetTextL(pTitle);
		    		dlg->RunLD();
		    		}
		    	}

		    CleanupStack::PopAndDestroy(2); // filedata, file
		    if(aCommand != EROMPatcherListBoxViewPatch_infoMenuItemCommand)
		    	{
			    size = iROMPatcherListBox->ListBox()->CurrentItemIndex();
			    if(index!=EBad)
					(*iPatched)[size]=!(*iPatched)[size];
				if((*iAutoStart)[size])
					index += ELastIcon;
					
			    CROMPatcherListBox::ChangeItemIconL( iROMPatcherListBox->ListBox(), index );
			    SaveList();
		    	}
	    	}
		CleanupStack::PopAndDestroy(); // fs
		}

	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CROMPatcherListBoxView::HandleAbout_MenuItemSelectedL( TInt /*aCommand*/ )
	{
	TBuf<256> msg(_L("ROM Patcher by -=ZoRn=-\nThanks to:\nFCA00000 - for perms hack\nleftup - for driver\nNightShift79 - for SVG graphics"));
	CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
	dlg->PrepareLC(R_ABOUT_MESSAGE_NOTE);
	dlg->QueryHeading()->SetTextL(_L("ROM Patcher"));
	dlg->RunLD();
	return ETrue;
	}
	
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CROMPatcherListBoxView::HandleAdd_to_autostartMenuItemSelectedL( TInt aCommand )
	{
	// TODO: implement selected event handler
	TInt curIndex = iROMPatcherListBox->ListBox()->CurrentItemIndex();
	if(curIndex>=0)
		{
		TInt curIcon = CROMPatcherListBox::CurrentItemIconL( iROMPatcherListBox->ListBox() );
		if(aCommand == EROMPatcherListBoxViewAdd_to_autostartMenuItemCommand)
			{
			(*iAutoStart)[curIndex]=ETrue;
			if(curIcon<ELastIcon)
				CROMPatcherListBox::ChangeItemIconL( iROMPatcherListBox->ListBox(), curIcon+ELastIcon );
			}
		if(aCommand == EROMPatcherListBoxViewRemove_from_autostartMenuItemCommand)
			{
			(*iAutoStart)[curIndex]=EFalse;
			if(curIcon>=ELastIcon)
				CROMPatcherListBox::ChangeItemIconL( iROMPatcherListBox->ListBox(), curIcon-ELastIcon );
			}
		SaveAuto();
		SaveList();
		}
	return ETrue;
	}
				
