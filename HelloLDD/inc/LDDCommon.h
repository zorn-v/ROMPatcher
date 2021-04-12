#ifndef LDDCOMMON_H_
#define LDDCOMMON_H_

#define DECLARE_STANDARD_LDD() \
TInt KernelModuleEntry(TInt) \
{ return KErrNone; } \
EXPORT_C DLogicalDevice* CreateLogicalDevice()

#endif /*LDDCOMMON_H_*/
