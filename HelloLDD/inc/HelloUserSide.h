#ifndef HELLOUSER_H_
#define HELLOUSER_H_
#include <e32cmn.h>
#include <e32ver.h>
enum TVer
	{
	EMajorVersionNumber=1,
	EMinorVersionNumber=0,
	EBuildVersionNumber=0
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


class RHelloLDDChannel : public RBusLogicalChannel
	{
public:
	enum TControl
		{
		EControlRead,
		EControlWrite
		};
public:
#ifndef __KERNEL_MODE__
	inline TInt Open();
	inline TVersion VersionRequired() const;
	inline void Read(TInt& aInt);
	inline void Write(TInt& aInt);
#endif
	};

#include "hellouserside.inl"

#endif /*HELLOUSER_H_*/
