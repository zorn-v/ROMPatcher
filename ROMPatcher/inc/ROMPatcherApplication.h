/*
========================================================================
 Name        : ROMPatcherApplication.h
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef ROMPATCHERAPPLICATION_H
#define ROMPATCHERAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidROMPatcherApplication = { 0xE4DD2DCB };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CROMPatcherApplication ROMPatcherApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CROMPatcherApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // ROMPATCHERAPPLICATION_H		
