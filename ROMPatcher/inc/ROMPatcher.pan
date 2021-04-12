
#ifndef ROMPATCHER_PAN_H
#define ROMPATCHER_PAN_H

/** ROMPatcher application panic codes */
enum TROMPatcherPanics
	{
	EROMPatcherUi = 1
	// add further panics here
	};

inline void Panic(TROMPatcherPanics aReason)
	{
	_LIT(applicationName,"ROMPatcher");
	User::Panic(applicationName, aReason);
	}

#endif // ROMPATCHER_PAN_H
