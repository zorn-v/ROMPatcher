/*
========================================================================
 Name        : ROMPatcherListBox.cpp
 Author      : -=ZoRn=-
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <ROMPatcher.mbg>
#include <ROMPatcher.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "ROMPatcherListBox.h"
#include "ROMPatcherListBoxView.h"
#include "ROMPatcher.hrh"
#include "ROMPatcherListBox.hrh"
// ]]] end generated region [Generated User Includes]
//#include <bautils.h>
//#include <s32file.h>
//#include <eikedwin.h>
// [[[ begin generated region: do not modify [Generated Constants]
_LIT( KROMPatcherFile, "\\resource\\apps\\ROMPatcher.mif" );
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CROMPatcherListBox::CROMPatcherListBox()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iListBox = NULL;
	// ]]] end generated region [Generated Contents]
	}
/** 
 * Destroy child controls.
 */
CROMPatcherListBox::~CROMPatcherListBox()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iListBox;
	iListBox = NULL;
	iListBoxEventDispatch.Close();		
	// ]]] end generated region [Generated Contents]
	}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CROMPatcherListBox
 */
CROMPatcherListBox* CROMPatcherListBox::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CROMPatcherListBox* self = CROMPatcherListBox::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CROMPatcherListBox
 */
CROMPatcherListBox* CROMPatcherListBox::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CROMPatcherListBox* self = new ( ELeave ) CROMPatcherListBox();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver );
	return self;
	}
			
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CROMPatcherListBox::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    {
	    SetContainerWindowL( *aParent );
	    }
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	InitializeControlsL();

	SetRect( aRect );
	ActivateL();
	// [[[ begin generated region: do not modify [Post-ActivateL initializations]
	// ]]] end generated region [Post-ActivateL initializations]
	
	}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CROMPatcherListBox::CountComponentControls() const
	{
	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CROMPatcherListBox::ComponentControl( TInt aIndex ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
	case EListBox:
		return iListBox;
		}
	// ]]] end generated region [Generated Contents]
	
	// handle any user controls here...
	return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CROMPatcherListBox::SizeChanged()
	{
	CCoeControl::SizeChanged();
	LayoutControls();

	// [[[ begin generated region: do not modify [Generated Contents]
			
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CROMPatcherListBox::LayoutControls()
	{
	iListBox->SetExtent( TPoint( 0, 0 ), iListBox->MinimumSize() );
	}
// ]]] end generated function

/**
 *	Handle key events.
 */				
TKeyResponse CROMPatcherListBox::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	if ( aKeyEvent.iCode == EKeyLeftArrow 
		|| aKeyEvent.iCode == EKeyRightArrow )
		{
		// Listbox takes all events even if it doesn't use them
		return EKeyWasNotConsumed;
		}
	
	// ]]] end generated region [Generated Contents]
	
	if ( iFocusControl != NULL
		&& iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
		{
		return EKeyWasConsumed;
		}
	return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
	}
				
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CROMPatcherListBox::InitializeControlsL()
	{
	iListBox = new ( ELeave ) CAknSingleGraphicStyleListBox;
	iListBox->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_ROMPATCHER_LIST_BOX_LIST_BOX );
		iListBox->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
	// the listbox owns the items in the list and will free them
	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	// setup the icon array so graphics-style boxes work
	SetupListBoxIconsL();
	
	iListBox->SetListBoxObserver( this );
	AddListBoxEventHandlerL( 
			iListBox, 
			EEventEnterKeyPressed, 
			&CROMPatcherListBox::HandleListBoxEnterKeyPressedL );
	
	// add list items
	
	iListBox->SetFocus( ETrue );
	iFocusControl = iListBox;
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CROMPatcherListBox::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( EROMPatcherListBoxViewId ) )->ClientRect() );

	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 *	Draw container contents.
 */				
void CROMPatcherListBox::Draw( const TRect& aRect ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 *	Add a list box item to a list.
 */
void CROMPatcherListBox::AddListBoxItemL(
		CEikTextListBox* aListBox,
		const TDesC& aString)
	{
	CTextListBoxModel* model = aListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	itemArray->AppendL( aString );
	aListBox->HandleItemAdditionL();
	}
				
// ]]] end generated function

void CROMPatcherListBox::CurrentItemTextL(
		CEikTextListBox* aListBox,
		TDes& aString)
	{
	CTextListBoxModel* model = aListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	aString = (*itemArray)[aListBox->CurrentItemIndex()];
	}

TInt CROMPatcherListBox::CurrentItemIconL(CEikTextListBox* aListBox)
	{
	TBuf<256> text;
	CurrentItemTextL(aListBox,text);
	return text[0]-0x30;
	}

void CROMPatcherListBox::ChangeItemIconL(
		CEikTextListBox* aListBox, TUint aIconIndex)
	{
	CTextListBoxModel* model = aListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	TInt ItemIndex = aListBox->CurrentItemIndex();
	if(ItemIndex>=0&&aIconIndex>=0&&aIconIndex<=9)
		{
		TBuf<256> item = (*itemArray)[ItemIndex];
		item[0] = aIconIndex+0x30;
	
		itemArray->Delete(ItemIndex);
		itemArray->InsertL(ItemIndex,item);
		}
	}

// [[[ begin generated function: do not modify
/**
 *	Get the listbox.
 */
CAknSingleGraphicStyleListBox* CROMPatcherListBox::ListBox()
	{
	return iListBox;
	}
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Create a list box item with the given column values.
 */
void CROMPatcherListBox::CreateListBoxItemL( TDes& aBuffer, 
		TInt aIconIndex,
		const TDesC& aMainText )
	{
	_LIT ( KStringHeader, "%d\t%S" );
	
	aBuffer.Format( KStringHeader(), aIconIndex, &aMainText );
	} 
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Add an item to the list by reading the text items from the array resource
 *	and setting a single image property (if available) from an index
 *	in the list box's icon array.
 *	@param aResourceId id of an ARRAY resource containing the textual
 *	items in the columns
 *	@param aIconIndex the index in the icon array, or -1
 */
void CROMPatcherListBox::AddListBoxResourceArrayItemL( TInt aResourceId, TInt aIconIndex )
	{
	CDesCArray* array = iCoeEnv->ReadDesCArrayResourceL( aResourceId );
	CleanupStack::PushL( array );
	// This is intended to be large enough, but if you get 
	// a USER 11 panic, consider reducing string sizes.
	TBuf<512> listString; 
	CreateListBoxItemL( listString, aIconIndex, ( *array ) [ 0 ] );
	AddListBoxItemL( iListBox, listString );
	CleanupStack::PopAndDestroy( array );
	} 
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Set up the list's icon array.
 */
void CROMPatcherListBox::SetupListBoxIconsL()
	{
	CArrayPtr< CGulIcon >* icons = NULL;		
	icons = new (ELeave) CAknIconArray( 8 );
	CleanupStack::PushL( icons );
	// for EListBoxRompatcherNeutral_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherNeutral_3, EMbmRompatcherNeutral_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherApplied_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherApplied_3, EMbmRompatcherApplied_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherNotice_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherNotice_3, EMbmRompatcherNotice_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherError_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherError_3, EMbmRompatcherError_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherNeutral_auto_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherNeutral_auto_3, EMbmRompatcherNeutral_auto_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherApplied_auto_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherApplied_auto_3, EMbmRompatcherApplied_auto_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherNotice_auto_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherNotice_auto_3, EMbmRompatcherNotice_auto_3_mask,
			NULL, EAspectRatioPreserved ) );
	// for EListBoxRompatcherError_auto_3Index
	icons->AppendL( LoadAndScaleIconL(
			KROMPatcherFile, EMbmRompatcherError_auto_3, EMbmRompatcherError_auto_3_mask,
			NULL, EAspectRatioPreserved ) );
	CleanupStack::Pop( icons );
	
	if ( icons != NULL )
		{
		iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
		}
	}
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/** 
 *	Handle commands relating to markable lists.
 */
TBool CROMPatcherListBox::HandleMarkableListCommandL( TInt aCommand )
	{
	 
	return EFalse;
	}
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/** 
 * Override of the HandleListBoxEventL virtual function
 */
void CROMPatcherListBox::HandleListBoxEventL(
		CEikListBox* aListBox,
		TListBoxEvent anEventType)
	{
	for (int i = 0; i < iListBoxEventDispatch.Count(); i++)
		{
		const TListBoxEventDispatch& currEntry = iListBoxEventDispatch[i];
		if ( currEntry.src == aListBox && currEntry.event == anEventType )
			{
			( this->*currEntry.handler )( aListBox, anEventType );
			break;
			}
		}
	}
				
// ]]] end generated function

/** 
 * Helper function to register MEikListBoxObserver event handlers
 */
void CROMPatcherListBox::AddListBoxEventHandlerL(
		CEikListBox* aListBox,
		TListBoxEvent anEvent,
		ListBoxEventHandler aHandler)
	{
	TListBoxEventDispatch entry;
	entry.src = aListBox;
	entry.event = anEvent;
	entry.handler = aHandler;
	TInt err = iListBoxEventDispatch.Append( entry );
	User::LeaveIfError( err );
	}
				
/** 
 * Handle the EEventEnterKeyPressed event.
 */
void CROMPatcherListBox::HandleListBoxEnterKeyPressedL( 
		CEikListBox* /* aListBox */, 
		TListBoxEvent /* anEventType */ )
	{
	// TODO: implement enterKeyPressed event handler
	}
				
// [[[ begin generated function: do not modify
/**
 *	This routine loads and scales a bitmap or icon.
 *
 *	@param aFileName the MBM or MIF filename
 *	@param aBitmapId the bitmap id
 *	@param aMaskId the mask id or -1 for none
 *	@param aSize the TSize for the icon, or NULL to use real size
 *	@param aScaleMode one of the EAspectRatio* enums when scaling
 *
 */
CGulIcon* CROMPatcherListBox::LoadAndScaleIconL( 
		const TDesC& aFileName,
		TInt aBitmapId, 
		TInt aMaskId, 
		TSize* aSize, 
		TScaleMode aScaleMode )
	{
	CFbsBitmap* bitmap;
	CFbsBitmap* mask;
	AknIconUtils::CreateIconL( bitmap, mask, aFileName, aBitmapId, aMaskId );
	
	TSize size;
	if ( aSize == NULL )
		{
		// Use size from the image header.  In case of SVG,
		// we preserve the image data for a while longer, since ordinarily
		// it is disposed at the first GetContentDimensions() or SetSize() call.
		AknIconUtils::PreserveIconData( bitmap );
		AknIconUtils::GetContentDimensions( bitmap, size );
		}
	else
		{
		size = *aSize;
		}
	
	AknIconUtils::SetSize( bitmap, size, aScaleMode );
	AknIconUtils::SetSize( mask, size, aScaleMode );
	
	if ( aSize == NULL )
		{
		AknIconUtils::DestroyIconData( bitmap );
		}
	
	return CGulIcon::NewL( bitmap, mask );
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Get the array of selected item indices, with respect to the list model.
 * The array is sorted in ascending order.
 * The array should be destroyed with two calls to CleanupStack::PopAndDestroy(),
 * the first with no argument (referring to the internal resource) and the
 * second with the array pointer.
 * @return newly allocated array, which is left on the cleanup stack;
 *	or NULL for empty list. 
 */
RArray< TInt >* CROMPatcherListBox::GetSelectedListBoxItemsLC( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return NULL;
		
	// get currently selected indices
	const CListBoxView::CSelectionIndexArray* selectionIndexes =
		aListBox->SelectionIndexes();
	TInt selectedIndexesCount = selectionIndexes->Count();
	if ( selectedIndexesCount == 0 )
		return NULL;
		
	// copy the indices and sort numerically
	RArray<TInt>* orderedSelectedIndices = 
		new (ELeave) RArray< TInt >( selectedIndexesCount );
	
	// push the allocated array
	CleanupStack::PushL( orderedSelectedIndices );
	
	// dispose the array resource
	CleanupClosePushL( *orderedSelectedIndices );
	
	// see if the search field is enabled
	CAknListBoxFilterItems* filter = model->Filter();
	if ( filter != NULL )
		{
		// when filtering enabled, translate indices back to underlying model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			{
			TInt filteredItem = ( *selectionIndexes ) [ idx ];
			TInt actualItem = filter->FilteredItemIndex ( filteredItem );
			orderedSelectedIndices->InsertInOrder( actualItem );
			}
		}
	else
		{
		// the selection indices refer directly to the model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			orderedSelectedIndices->InsertInOrder( ( *selectionIndexes ) [ idx ] );
		}	
		
	return orderedSelectedIndices;
	}
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Delete the selected item or items from the list box.
 */
void CROMPatcherListBox::DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return;
	
	RArray< TInt >* orderedSelectedIndices = GetSelectedListBoxItemsLC( aListBox );		
	if ( !orderedSelectedIndices )
		return;
		
	// Delete selected items from bottom up so indices don't change on us
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	TInt currentItem = 0;
	
	for ( TInt idx = orderedSelectedIndices->Count(); idx-- > 0; ) 
		{
		currentItem = ( *orderedSelectedIndices )[ idx ];
		itemArray->Delete ( currentItem );
		}
	
	// dispose the array resources
	CleanupStack::PopAndDestroy();
	
	// dispose the array pointer
	CleanupStack::PopAndDestroy( orderedSelectedIndices );
	
	// refresh listbox's cursor now that items are deleted
	AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
		aListBox, currentItem, ETrue );
	}
				
// ]]] end generated function
