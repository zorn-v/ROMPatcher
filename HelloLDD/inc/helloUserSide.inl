#ifndef __KERNEL_MODE__
_LIT(KDeviceName,"HelloLDD");
inline TInt RHelloLDDChannel::Open()
{
return(DoCreate(KDeviceName,VersionRequired(),KNullUnit,NULL,NULL));
}

inline TVersion RHelloLDDChannel::VersionRequired() const
{
return(TVersion(EMajorVersionNumber,
EMinorVersionNumber,
EBuildVersionNumber));
}

inline void RHelloLDDChannel::Read(TInt& aInt)
{
DoControl(EControlRead,&aInt);
}

inline void RHelloLDDChannel::Write(TInt& aInt)
{
DoControl(EControlWrite,&aInt);
}

#endif