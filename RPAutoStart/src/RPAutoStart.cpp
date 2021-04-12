/*
============================================================================
 Name		: RPAutoStart.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Exe source file
============================================================================
*/

//  Include Files  

#include "RPAutoStart.h"
#include <e32base.h>
#include <e32std.h>

#include <f32file.h>
#include <bautils.h>


//  Constants


//  Global Variables

//LOCAL_D CConsoleBase* console;  // write all messages to this
LOCAL_D RPDriver rDriver;

//  Local Functions

LOCAL_C TBool ConvertTextData(TPtrC8 aData, TPtr8 aConverted)
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

LOCAL_C TInt PosPatchData(TPtr8& aData, TChar aChar, TBool aTrimOnly)
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

LOCAL_C TUint FindFileAddr(TPtrC8 aFileName)
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

LOCAL_C TInt ApplyPatch(TPtr8& aData)
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
				    //HBufC8* searchData = aPatched ? newData : oldData;
				    pos = pFileData.Find(*oldData);
				    while(pos>=0)
				    	{
				    	if(rDriver.Patch(off+pos,*oldData,*newData)!=KErrNone)
				    		{
				    		res = EWarning;
				    		break;
				    		}
				    	else
				    		{
				    		pCount++;
				    		pFileData.Delete(pos,oldData->Length());
				    		pos = pFileData.Find(*oldData);
				    		}
				    	}
				    CleanupStack::PopAndDestroy(); //fileData
					}
				else
					{
					pCount++;
					err = rDriver.Patch(addr,*oldData,*newData);
					}
				}
			else res = EWarning;
				
			if(err!=KErrNone)
				{
				errCount++;
				res = EWarning;
				}
			if(res==ENormal)
				res = EApply;
			CleanupStack::PopAndDestroy(2); //oldData, newData
			}
		PosPatchData(aData,'\n',ETrue);
		}
	if(errCount==pCount)
		res = EBad;
	return res;
	}

LOCAL_C void PatchFileName(TDesC8& aFileName,TDes& aPatchFileName)
	{
	TBuf<256> fName;
	fName.Copy(aFileName);
	aPatchFileName.Format(_L("E:\\Patches\\%S.rmp"),&fName);
	}

LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//
//	console->Write(_L("Hello, world!\n"));
	_LIT(KAutoStartFile,"C:\\System\\Data\\RPAuto.dat");
	_LIT(KListDataFile,"D:\\RPData.dat");
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	if(BaflUtils::FileExists(fs,KAutoStartFile))
    	{
    	RFile file;
		User::LeaveIfError(file.Open(fs, KAutoStartFile, EFileStream|EFileRead|EFileShareReadersOnly));
		CleanupClosePushL(file);
		TInt size;
		TInt read = 0;
		TBool drvLoaded = EFalse;
		User::LeaveIfError(file.Size(size));
		
		RFile listFile;
		fs.SetAtt(KListDataFile,KEntryAttNormal,KEntryAttSystem|KEntryAttHidden|KEntryAttReadOnly);
		fs.Delete(KListDataFile);
		User::LeaveIfError(listFile.Create(fs, KListDataFile, EFileStream|EFileWrite|EFileShareExclusive));
		CleanupClosePushL(listFile);
		
		while( read < size)
			{
			TBuf8<256> buff;
			User::LeaveIfError(file.Read(buff,1));
			read += buff[0]+1;
			User::LeaveIfError(file.Read(buff,buff[0]));

			TBuf<256> fName;
			PatchFileName(buff,fName);
			if(BaflUtils::FileExists(fs,fName))
				{
				RFile patchFile;
				if(drvLoaded==EFalse)
					{
#ifndef __WINS__
					User::FreeLogicalDevice(_L("RomPEngine"));
					TInt err = User::LoadLogicalDevice(_L("RPEngine.ldd"));
				  	if ((err!=KErrNone) && (err!=KErrAlreadyExists))
					{
						User::Leave(err);
					}
				  	
				  	err = rDriver.Open();
				  	if (err!=KErrNone)
				  	{
				  		User::FreeLogicalDevice(_L("RomPEngine"));
				  		User::Leave(err);
				  	}
#endif
				  	drvLoaded = ETrue;
					}
		    	
				User::LeaveIfError(patchFile.Open(fs, fName, EFileStreamText|EFileRead|EFileShareReadersOnly));
				CleanupClosePushL(patchFile);

				TInt size;
				User::LeaveIfError(patchFile.Size(size));
				
				HBufC8* fileData = HBufC8::NewLC(size);
				TPtr8 pFileData = fileData->Des();
				User::LeaveIfError(patchFile.Read(pFileData,size));
				patchFile.Close();

			    TUint index = ApplyPatch(pFileData);

			    CleanupStack::PopAndDestroy(2); // filedata, patchFile
			    

				TBuf8<257> item8;
				item8.Append(index!=EBad);
				item8.Append(index+ELastIcon+0x30);
				item8.Append('\t');
				item8.Append(buff);
				User::LeaveIfError(listFile.Write(item8));
				}
			}
		listFile.Close();
		fs.SetAtt(KListDataFile,KEntryAttSystem|KEntryAttHidden|KEntryAttReadOnly,KEntryAttNormal);
		CleanupStack::PopAndDestroy(); // listFile
		
		file.Close();
		CleanupStack::PopAndDestroy(); // file
    	}
	CleanupStack::PopAndDestroy(); // fs
	}


LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
/*	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
*/
	MainL();

	// Delete active scheduler
//	CleanupStack::PopAndDestroy(scheduler);
	}


//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());
	if (mainError)
		return mainError;
	
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}



