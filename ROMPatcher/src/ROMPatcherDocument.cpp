/*
========================================================================
 Name        : ROMPatcherDocument.cpp
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "ROMPatcherDocument.h"
#include "ROMPatcherAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CROMPatcherDocument::CROMPatcherDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CROMPatcherDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CROMPatcherDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CROMPatcherDocument
 */
CROMPatcherDocument* CROMPatcherDocument::NewL( CEikApplication& aApp )
	{
	CROMPatcherDocument* self = new ( ELeave ) CROMPatcherDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CROMPatcherDocument::CreateAppUiL()
	{
	return new ( ELeave ) CROMPatcherAppUi;
	}
				
