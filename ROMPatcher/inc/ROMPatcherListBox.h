/*
========================================================================
 Name        : ROMPatcherListBox.h
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef ROMPATCHERLISTBOX_H
#define ROMPATCHERLISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
#include <akniconutils.h>
#include <gulicon.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
#include <eiklbo.h>
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknSingleGraphicStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for ROMPatcherListBox
 * 
 * @class	CROMPatcherListBox ROMPatcherListBox.h
 */
class CROMPatcherListBox : public CCoeControl
	,MEikListBoxObserver	{
public:
	// constructors and destructor
	CROMPatcherListBox();
	static CROMPatcherListBox* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CROMPatcherListBox* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CROMPatcherListBox();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	void InitializeControlsL();
	void LayoutControls();
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	static void AddListBoxItemL(
			CEikTextListBox* aListBox,
			const TDesC& aString);
	static RArray< TInt >* GetSelectedListBoxItemsLC( CEikTextListBox* aListBox );
	static void DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox );
	CAknSingleGraphicStyleListBox* ListBox();
	static void CreateListBoxItemL( TDes& aBuffer, 
			TInt aIconIndex,
			const TDesC& aMainText );
	void AddListBoxResourceArrayItemL( TInt aResourceId, TInt aIconIndex );
	void SetupListBoxIconsL();
	static CGulIcon* LoadAndScaleIconL(
			const TDesC& aFileName,
			TInt aBitmapId,
			TInt aMaskId,
			TSize* aSize,
			TScaleMode aScaleMode );
	TBool HandleMarkableListCommandL( TInt aCommand );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	static void CurrentItemTextL(CEikTextListBox* aListBox,TDes& aString);
	static TInt CurrentItemIconL(CEikTextListBox* aListBox);
	static void ChangeItemIconL(CEikTextListBox* aListBox, TUint aIconIndex);
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknSingleGraphicStyleListBox* iListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin [Overridden Methods]
protected: 
	void HandleListBoxEventL(
			CEikListBox* aListBox,
			TListBoxEvent anEventType);
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	void HandleListBoxEnterKeyPressedL( 
			CEikListBox* aListBox, 
			TListBoxEvent anEventType );
	// ]]] end [User Handlers]
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
		EListBox,
		
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		ELastControl
		};
	enum TListBoxImages
		{
		// [[[ begin generated region: do not modify [Generated Enums]
		EListBoxRompatcherNeutral_3Index = 0,
		EListBoxRompatcherApplied_3Index = 1,
		EListBoxRompatcherNotice_3Index = 2,
		EListBoxRompatcherError_3Index = 3,
		EListBoxRompatcherNeutral_auto_3Index = 4,
		EListBoxRompatcherApplied_auto_3Index = 5,
		EListBoxRompatcherNotice_auto_3Index = 6,
		EListBoxRompatcherError_auto_3Index = 7,
		EListBoxFirstUserImageIndex
		
		// ]]] end generated region [Generated Enums]
		
		};
	
	// [[[ begin [MEikListBoxObserver support]
private: 
	typedef void ( CROMPatcherListBox::*ListBoxEventHandler )( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent );
	
	void AddListBoxEventHandlerL( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent, 
			ListBoxEventHandler aHandler );
	
	struct TListBoxEventDispatch 
		{ 
		CEikListBox* src; 
		TListBoxEvent event; 
		ListBoxEventHandler handler;
		};
		
	RArray< TListBoxEventDispatch > iListBoxEventDispatch;
	// ]]] end [MEikListBoxObserver support]
	
	};
				
#endif // ROMPATCHERLISTBOX_H
