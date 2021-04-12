/*
========================================================================
 Name        : ROMPatcherDocument.h
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef ROMPATCHERDOCUMENT_H
#define ROMPATCHERDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CROMPatcherDocument ROMPatcherDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CROMPatcherDocument : public CAknDocument
	{
public: 
	// constructor
	static CROMPatcherDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CROMPatcherDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // ROMPATCHERDOCUMENT_H
