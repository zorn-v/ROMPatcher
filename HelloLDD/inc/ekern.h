#ifndef EKERN_H_
#define EKERN_H_
#define __KERNEL_MODE__

#include <e32rom.h>
#include <e32event.h>
#include <e32cmn.h>
class HBuf8;
class NFastMutex;
class Kern;
class DObject;
class DThread;
class DProcess;
class HBuf;
class NThreadBase;
class DChunk;
class TDfcQue;
class TChunkCreateInfo;
class DMutex;
class SThreadCreateInfo;
class NFastSemaphore;
class NThread;
class SNThreadCreateInfo;
class DCodeSeg;
typedef TInt TKernelHookType;
class SDblQue;
class TRawEvent;
class TProcessCreateInfo;
class DSemaphore;
class TTraceSource;
class TPriListBase;
class NTimer;
class NFastSemaphore;
class TMessageQue;
class DLogicalDevice;
class DPhysicalDevice;
class TThreadCleanup;
const TInt KNumPriorities=64;


class DBase
	{
public:
	IMPORT_C virtual ~DBase();
	IMPORT_C void Delete(DBase*);
	IMPORT_C void AsyncDelete(void);
	IMPORT_C void* operator new(unsigned int);
	IMPORT_C void* operator new(unsigned int, unsigned int);
protected:
	TAny* iForAsyncDelete;
	};

class DObject : public DBase
	{
public:
//	IMPORT_C DObject();
	IMPORT_C virtual TInt Close(TAny* aPtr);
	IMPORT_C virtual TInt RequestUserHandle(DThread* aThread,
			TOwnerType aType);
	IMPORT_C virtual TInt AddToProcess(DProcess* aProcess);
	IMPORT_C virtual void DoAppendName(TDes& aName);
	IMPORT_C TInt AppendName(TDes8 &);
	IMPORT_C TInt AsyncClose(void);
	IMPORT_C TInt CheckedOpen(void);
	IMPORT_C TInt AppendFullName(TDes8 &);
	IMPORT_C TInt TraceAppendName(TDes8 &, int);
	IMPORT_C TInt DoAppendFullName(TDes8 &);
	IMPORT_C TInt TraceAppendFullName(TDes8 &, int);
	IMPORT_C TInt Name(TDes8 &);
	IMPORT_C TInt SetName(TDesC8 const*);
	IMPORT_C TInt FullName(TDes8 &);
	IMPORT_C TInt SetOwner(DObject*);
	IMPORT_C DObject(void);
	IMPORT_C ~DObject();
	
public:
	TInt iAccessCount;//8
	DObject* iOwner;//c
	TUint8 iContainerID;//10
	TUint8 iProtection;
	TUint8 iSpare[2];
	HBuf* iName;//14
//	TInt iFP1[2];
public:
	static NFastMutex Lock;
	};

class NFastMutex
	{
public:
	IMPORT_C TInt Wait(void);
	IMPORT_C TInt Signal(void);
public:
	NThreadBase* iHoldingThread;//0
	TBool iWaiting;//4
	};
class SDblQueLink
	{
public:
	SDblQueLink* iNext;
	SDblQueLink* iPrev;
	};

class SDblQue
	{
public:
/*	IMPORT_C TBool IsEmpty() const;
	IMPORT_C void SetOffset(TInt aOffset);
	IMPORT_C void Reset();*/
protected:
	SDblQueLink iHead;
	};

class TPriListLink:public SDblQueLink
	{
private:
	TUint8 iPriority;
	TUint8 iSpare[3];
	};


class TPriListBase
	{
	IMPORT_C TInt ChangePriority(TPriListLink *, int);
	IMPORT_C TInt HighestPriority(void);
	IMPORT_C TInt Add(TPriListLink *);
	IMPORT_C TInt First(void);
	IMPORT_C TInt Remove(TPriListLink *);
	IMPORT_C  TPriListBase(int);
private:
	TInt iExist[2];
	};

template <class T,TInt num>
class TPriList:TPriListBase
	{
	TPriListLink* iHead[num];
	};
typedef TPriList<NThreadBase,64> TThreadWaitList;

class NFastSemaphore
	{
public:
	 IMPORT_C TInt Wait(void);
	 IMPORT_C TInt Reset(void);
	 IMPORT_C TInt Signal(void);
	 IMPORT_C TInt SignalN(int);
	 IMPORT_C void SetOwner(NThreadBase *);
public:
	TUint iCount;
	NThreadBase* iWaitingThread;//?
	};

class NTimer:public SDblQueLink
	{
public:
	IMPORT_C void Again(int);
	IMPORT_C TInt Cancel(void);
	IMPORT_C TInt OneShot(int);
	IMPORT_C TInt OneShot(int, int);
	TAny* iPtr;
	TAny* iFunction;
	TInt iTriggerTime ;
	TBool iCompleteInDfc;
	TUint8 iState;
	TUint8 iSpare[3];
	};

class Kern
	{
public:
	IMPORT_C static TInt AccessCode(void);
	IMPORT_C static TInt ChunkClose(DChunk *);
	IMPORT_C static TInt Containers(void);
	IMPORT_C static TInt DfcQCreate(TDfcQue *&, int, TDesC8  const*);
	IMPORT_C static TInt KUSafeRead(void  const*, void *, int);
	IMPORT_C static TInt PowerModel(void);
	IMPORT_C static TInt RandomSalt(unsigned long);
	IMPORT_C static TInt SystemTime(void);
	IMPORT_C static TInt ThreadKill(DThread *, TExitType, int, TDesC8  const&);
	IMPORT_C static TInt TickPeriod(void);
	IMPORT_C static TInt AddHalEntry(int, int (*)(void *, int, void *, void *), void *);
	IMPORT_C static TInt AddHalEntry(int, int (*)(void *, int, void *, void *), void *, int);
	IMPORT_C static TInt ChunkCommit(DChunk *, int, int);
	IMPORT_C static TInt ChunkCreate(TChunkCreateInfo  const&, DChunk *&, unsigned long &, unsigned long &);
	IMPORT_C static TInt CloseHandle(DThread *, int);
	IMPORT_C static TInt CodeSegList(void);
	IMPORT_C static TInt CodeSegLock(void);
	IMPORT_C static TInt HalFunction(int, int, void *, void *);
	IMPORT_C static TInt HalFunction(int, int, void *, void *, int);
	IMPORT_C static TInt KUSafeWrite(void *, void  const*, int);
	IMPORT_C static TInt MutexCreate(DMutex *&, TDesC8  const&, unsigned int);
	IMPORT_C static TInt MutexSignal(DMutex &);
	IMPORT_C static TInt PollingWait(int (*)(void *), void *, int, int);
	IMPORT_C static TInt SafeReAlloc(void *&, int, int);
	IMPORT_C static TInt AppendFormat(TDes8 &, char  const*, std::__va_list);
	IMPORT_C static TInt ChunkAddress(DChunk *, int, int, unsigned long &);
	IMPORT_C static TInt FindHalEntry(int);
	IMPORT_C static TInt FindHalEntry(int, int);
	IMPORT_C static TInt ThreadCreate(SThreadCreateInfo &);
	IMPORT_C static TInt ThreadFromId(unsigned int);
	IMPORT_C static TInt ThreadResume(DThread &);
	IMPORT_C static TInt ValidateName(TDesC8  const&);
	IMPORT_C static TInt CurrentThread(void);
	IMPORT_C static TInt EndAccessCode(void);
	IMPORT_C static TInt MachineConfig(void);
	IMPORT_C static TInt NotifyChanges(unsigned int);
	IMPORT_C static TInt ProcessCreate(DProcess *&, TProcessCreateInfo &, HBuf8 *&, int *);
	IMPORT_C static TInt ProcessFromId(unsigned int);
	IMPORT_C static TInt SemaphoreWait(DSemaphore &, int);
	IMPORT_C static TInt SetSystemTime(long long  const&, unsigned int);
	IMPORT_C static TInt ThreadDesRead(DThread *, void  const*, TDes8 &, int, int);
	IMPORT_C static TInt ThreadRawRead(DThread *, void  const*, void *, int);
	IMPORT_C static TInt ThreadSuspend(DThread &, int);
	IMPORT_C static TInt CurrentProcess(void);
	IMPORT_C static TInt FreeRamInBytes(void);
	IMPORT_C static TInt KUDesSetLength(TDes8 &, int);
	IMPORT_C static TInt RemoveHalEntry(int);
	IMPORT_C static TInt RemoveHalEntry(int, int);
	IMPORT_C static TInt ThreadDesWrite(DThread *, void *, TDesC8  const&, int, int, DThread *);
	IMPORT_C static TInt ThreadRawWrite(DThread *, void *, void  const*, int, DThread *);
	IMPORT_C static TInt WaitForRequest(TRequestStatus &);
	IMPORT_C static TInt OpenSharedChunk(DThread *, void  const*, int, int &);
	IMPORT_C static TInt OpenSharedChunk(DThread *, int, int);
	IMPORT_C static TInt RequestComplete(DThread *, TRequestStatus *&, int);
	IMPORT_C static TInt RoundToPageSize(unsigned long);
	IMPORT_C static TInt SemaphoreCreate(DSemaphore *&, TDesC8  const&, int);
	IMPORT_C static TInt SemaphoreSignal(DSemaphore &);
	IMPORT_C static TInt SetTraceHandler(int (*)(TDesC8  const&, TTraceSource));
	IMPORT_C static TInt NThreadToDThread(NThread *);
	IMPORT_C static TInt ObjectFromHandle(DThread *, int, int);
	IMPORT_C static TInt RoundToChunkSize(unsigned long);
	IMPORT_C static TInt ThreadGetDesInfo(DThread *, void  const*, int &, int &, unsigned char *&, int);
	IMPORT_C static TInt ValidateFullName(TDesC8  const&);
	IMPORT_C static TInt MakeHandleAndOpen(DThread *, DObject *);
	IMPORT_C static TInt NotifyThreadDeath(DThread *);
	IMPORT_C static TInt SetThreadPriority(int, DThread *);
	IMPORT_C static TInt AsyncNotifyChanges(unsigned int);
	IMPORT_C static TInt CodeSegFromAddress(unsigned long, DProcess *);
	IMPORT_C static TInt MachinePowerStatus(void);
	IMPORT_C static TInt PanicCurrentThread(TDesC8  const&, int);
	IMPORT_C static TInt ThreadGetDesLength(DThread *, void  const*);
	IMPORT_C static TInt ChunkCommitPhysical(DChunk *, int, int, unsigned long  const*);
	IMPORT_C static TInt ChunkCommitPhysical(DChunk *, int, int, unsigned long);
	IMPORT_C static TInt ChunkPhysicalAddress(DChunk *, int, int, unsigned long &, unsigned long &, unsigned long &, unsigned long *);
	IMPORT_C static TInt CodeSegGetMemoryInfo(DCodeSeg &, TProcessMemoryInfo &, DProcess *);
	IMPORT_C static TInt ChunkCommitContiguous(DChunk *, int, int, unsigned long &);
	IMPORT_C static TInt QueryVersionSupported(TVersion  const&, TVersion  const&);
	IMPORT_C static TInt ThreadGetDesMaxLength(DThread *, void  const*);
	IMPORT_C static TInt DoCurrentThreadHasCapability(TCapability);
	IMPORT_C static TInt DoCurrentThreadHasCapability(TCapability, char  const*);
	IMPORT_C static TInt Exit(int);
	IMPORT_C static TInt Free(void *);
	IMPORT_C static TInt Alloc(int);
	IMPORT_C static TInt Fault(char  const*, int);
	IMPORT_C static TInt AllocZ(int);
	IMPORT_C static TInt Printf(char  const*, ...);
	IMPORT_C static TInt Random(void);
	IMPORT_C static TInt DfcQue0(void);
	IMPORT_C static TInt DfcQue1(void);
	IMPORT_C static TInt Message(void);
	IMPORT_C static TInt ReAlloc(void *, int, int);
	IMPORT_C static TInt Restart(int);
	IMPORT_C static TInt SetHook(TKernelHookType, int (*)(void), int);
	IMPORT_C static TInt AddEvent(TRawEvent  const&);
	IMPORT_C static TInt DfcQInit(TDfcQue *, int, TDesC8  const*);
	IMPORT_C static TInt InfoCopy(TDes8 &, unsigned char  const*, int);
	IMPORT_C static TInt InfoCopy(TDes8 &, TDesC8  const&);
	IMPORT_C static TInt KUDesGet(TDes8 &, TDesC8  const&);
	IMPORT_C static TInt KUDesPut(TDes8 &, TDesC8  const&);
	IMPORT_C static TInt NanoWait(unsigned long);
	IMPORT_C static TInt SafeRead(void  const*, void *, int);
	IMPORT_C static TInt SvMsgQue(void);
	IMPORT_C static TInt AsyncFree(void *);
	IMPORT_C static TInt ColdStart(void);
	IMPORT_C static TInt KUDesInfo(TDesC8  const&, int &, int &);
	IMPORT_C static TInt KUSafeDec(int &);
	IMPORT_C static TInt KUSafeInc(int &);
	IMPORT_C static TInt MutexWait(DMutex &);
	IMPORT_C static TInt PowerGood(void);
	IMPORT_C static TInt SafeClose(DObject *&, void *);
	IMPORT_C static TInt SafeWrite(void *, void  const*, int);
	IMPORT_C static TInt SuperPage(void);
	IMPORT_C static TInt TickCount(void);
	IMPORT_C static TInt TimerDfcQ(void);
	
	};

class  HBuf8:public TBufBase8
	{
public:
	IMPORT_C static HBuf8* New(TDesC8  const&);
	IMPORT_C static HBuf8* New(int);
	IMPORT_C static HBuf8* ReAlloc(int);
	};

class NKern
	{
public:
	IMPORT_C static TInt FSSetOwner(NFastSemaphore *, NThreadBase *);
	IMPORT_C static TInt LockSystem(void);
	IMPORT_C static TInt ThreadKill(NThread *);
	IMPORT_C static TInt ThreadKill(NThread *, NFastMutex *);
	IMPORT_C static TInt TickPeriod(void);
	IMPORT_C static TInt TimerTicks(int);
	IMPORT_C static TInt FastCounter(void);
	IMPORT_C static TInt FlashSystem(void);
	IMPORT_C static TInt ThreadCreate(NThread *, SNThreadCreateInfo &);
	IMPORT_C static TInt ThreadResume(NThread *);
	IMPORT_C static TInt ThreadResume(NThread *, NFastMutex *);
	IMPORT_C static TInt UnlockSystem(void);
	IMPORT_C static TInt CurrentThread(void);
	IMPORT_C static TInt ThreadEnterCS(void);
	IMPORT_C static TInt ThreadLeaveCS(void);
	IMPORT_C static TInt ThreadRelease(NThread *, int);
	IMPORT_C static TInt ThreadRelease(NThread *, int, NFastMutex *);
	IMPORT_C static TInt ThreadSuspend(NThread *, int);
	IMPORT_C static TInt CurrentContext(void);
	IMPORT_C static TInt LockedSetClear(unsigned long &, unsigned long, unsigned long);
	IMPORT_C static TInt SchedulerHooks(unsigned long &, unsigned long &);
	IMPORT_C static TInt LockedSetClear8(unsigned char &, unsigned char, unsigned char);
	IMPORT_C static TInt PreemptionPoint(void);
	IMPORT_C static TInt RotateReadyList(int);
	IMPORT_C static TInt RecordIntLatency(int, int);
	IMPORT_C static TInt DisableInterrupts(int);
	IMPORT_C static TInt RestoreInterrupts(int);
	IMPORT_C static TInt ThreadForceResume(NThread *);
	IMPORT_C static TInt ThreadForceResume(NThread *, NFastMutex *);
	IMPORT_C static TInt ThreadSetPriority(NThread *, int);
	IMPORT_C static TInt ThreadSetPriority(NThread *, int, NFastMutex *);
	IMPORT_C static TInt WaitForAnyRequest(void);
	IMPORT_C static TInt RecordThreadLatency(int);
	IMPORT_C static TInt ThreadRequestSignal(NThread *);
	IMPORT_C static TInt ThreadRequestSignal(NThread *, NFastMutex *);
	IMPORT_C static TInt ThreadRequestSignal(NThread *, int);
	IMPORT_C static TInt DisableAllInterrupts(void);
	IMPORT_C static TInt FastCounterFrequency(void);
	IMPORT_C static TInt InsertSchedulerHooks(void);
	IMPORT_C static TInt RemoveSchedulerHooks(void);
	IMPORT_C static TInt ThreadGetUserContext(NThread *, void *, unsigned long &);
	IMPORT_C static TInt ThreadSetUserContext(NThread *, void *);
	IMPORT_C static TInt SetRescheduleCallback(void (*)(NThread *));
	IMPORT_C static TInt Exit(void);
	IMPORT_C static TInt Lock(void);
	IMPORT_C static TInt Block(unsigned long, unsigned int);
	IMPORT_C static TInt Block(unsigned long, unsigned int, NFastMutex *);
	IMPORT_C static TInt Sleep(unsigned long);
	IMPORT_C static TInt FMWait(NFastMutex *);
	IMPORT_C static TInt FSWait(NFastSemaphore *);
	IMPORT_C static TInt Unlock(void);
	IMPORT_C static TInt SafeDec(int &);
	IMPORT_C static TInt SafeInc(int &);
	IMPORT_C static TInt FMSignal(NFastMutex *);
	IMPORT_C static TInt FSSignal(NFastSemaphore *);
	IMPORT_C static TInt FSSignal(NFastSemaphore *, NFastMutex *);
	IMPORT_C static TInt SafeSwap(void *, void *&);
	IMPORT_C static TInt FSSignalN(NFastSemaphore *, int);
	IMPORT_C static TInt FSSignalN(NFastSemaphore *, int, NFastMutex *);
	IMPORT_C static TInt LockedAdd(int &, int);
	IMPORT_C static TInt LockedDec(int &);
	IMPORT_C static TInt LockedInc(int &);
	IMPORT_C static TInt NanoBlock(unsigned long, unsigned int, void *);
	IMPORT_C static TInt SafeSwap8(unsigned char, unsigned char &);
	IMPORT_C static TInt TickCount(void);
	};

class DSemaphore : public DObject
	{
public:
	~DSemaphore();
	static  void Delete(DSemaphore*){};
public:
	TInt iCount;//18
	TUint8 iResetting;//1c
	TUint8 iPad1;
	TUint8 iPad2;
	TUint8 iPad3;
	SDblQue iSuspendedQ;//20
	TThreadWaitList iWaitQ;//28
public:
	friend class Monitor;
	};

class DObjectCon : public DBase
	{

public:
	~DObjectCon();
	static DObjectCon* New(TInt aUniqueID);
	IMPORT_C void Remove(DObject* aObj);
	IMPORT_C TInt Add(DObject* aObj);
	IMPORT_C DObject* operator[](TInt aIndex);
	IMPORT_C DObject* At(TInt aFindHandle);
	IMPORT_C TInt CheckUniqueFullName(DObject* aOwner, const TDesC8& aName);
	IMPORT_C TInt CheckUniqueFullName(DObject* aObject);
	IMPORT_C TInt FindByName(TInt& aFindHandle, const TDesC8& aMatch,TKName& aName);
	IMPORT_C TInt FindByFullName(TInt& aFindHandle, const TDesC8& aMatch,TFullName& aFullName);
	IMPORT_C TInt OpenByFullName(DObject*& aObject, const TDesC8& aMatch);
protected:
	DObjectCon(TInt aUniqueID);
	TBool NamesMatch(DObject* aObject, DObject* aCurrentObject);
	TBool NamesMatch(DObject* aObject, const TDesC8& aObjectName,
			DObject* aCurrentObject);
public:
	TInt iUniqueID;
private:
	TInt iAllocated;
	TInt iCount;
	DObject** iObjects;
	DMutex* iMutex;
	};
typedef TAny* NThreadExitHandler;
typedef TAny* NThreadStateHandler;
typedef TAny* NThreadExceptionHandler;
typedef TAny* NThreadTimeoutHandler;
struct SNThreadHandlers
{
NThreadExitHandler iExitHandler;//0
NThreadStateHandler iStateHandler;//4
NThreadExceptionHandler iExceptionHandler;//8
NThreadTimeoutHandler iTimeoutHandler;//c
};
class NThreadBase : public TPriListLink
{
public:
	//NThreadBase();
	//TInt Create(SNThreadCreateInfo& anInfo,TBool aInitial);
	IMPORT_C void CheckSuspendThenReady();
	IMPORT_C void Ready();
	//void DoCsFunction();
	IMPORT_C TBool Suspend(TInt aCount);
	IMPORT_C TBool Resume();
	IMPORT_C TBool ForceResume();
	IMPORT_C void Release(TInt aReturnCode);
	IMPORT_C void RequestSignal();
	IMPORT_C void SetPriority(TInt aPriority);
	//void SetEntry(NThreadFunction aFunction);
	IMPORT_C void Kill();
	//void Exit();
	//void ForceExit();
public:
	NFastMutex* iHeldFastMutex;// fast mutex held 0x0c
	NFastMutex* iWaitFastMutex;// fast mutex on which blocked 0x10
	TAny* iAddressSpace;
	TInt iTime; // time remaining//18
	TInt iTimeslice;//1C
	NFastSemaphore iRequestSemaphore;
	TAny* iWaitObj;// object on which this thread is waiting
	TInt iSuspendCount; // how many times we have been suspended  //0x2C
	TInt iCsCount; // critical section count //30
	TInt iCsFunction; // what to do on leaving CS://34
	// +n=suspend n times, 0=nothing, -1=exit
	NTimer iTimer;
	TInt iReturnValue;//50
	TLinAddr iStackBase;//0x54
	TInt iStackSize;//0x58
	SNThreadHandlers *iHandlers; // + thread event handlers 0x5C
	const TAny* iFastExecTable;//const SFastExecTable* iFastExecTable;//60
	const TAny* iSlowExecTable;//const SSlowExecEntry* iSlowExecTable; //first entry iEntries[0]  //64
	TLinAddr iSavedSP;//0x68
	TAny* iExtraContext; // coprocessor context
	TInt iExtraContextSize; // +ve=dynamically allocated
	//uncompleted
};

typedef void (*TDfcFn)(TAny*);
class TDfc
	{
public:
	IMPORT_C TDfc(TDfcFn aFn, TAny* aPtr);
	IMPORT_C TDfc(TDfcFn aFn, TAny* aPtr, TInt aPri);
	IMPORT_C TDfc(TDfcFn aFn, TAny* aPtr, TDfcQue* aQ, TInt aPri);
	IMPORT_C void Add();
	IMPORT_C void Cancel();
	IMPORT_C void Enque();
	IMPORT_C void Enque(NFastMutex* aMutex);
	IMPORT_C void DoEnque();
	//inline TBool Queued();
	//inline TBool IsIDFC();
	//inline void SetDfcQ(TDfcQue* aQ);
	//inline void SetFunction(TDfcFn aFn);
	TDfc(){};
	};

class TMessageBase : public SDblQueLink
	{
public:
	enum TState {EFree,EDelivered,EAccepted};
public:
	//TMessageBase() : iState(EFree), iQueue(NULL) {}
	IMPORT_C void Send(TMessageQue* aQ);
	IMPORT_C TInt SendReceive(TMessageQue* aQ);
	IMPORT_C void Forward(TMessageQue* aQ, TBool aReceiveNext);
	IMPORT_C void Complete(TInt aResult, TBool aReceiveNext);
	IMPORT_C void Cancel();
	IMPORT_C void PanicClient(const TDesC8& aCategory, TInt aReason);
public:
	IMPORT_C DThread* Client();
public:
	TUint8 iState;
	TMessageQue* iQueue;
	NFastSemaphore iSem;
	TInt iValue;
	};

class TMessageQue : private TDfc
	{
public:
	TMessageQue(){};
	IMPORT_C TMessageQue(TDfcFn aFunction, TAny* aPtr,
			TDfcQue* aDfcQ, TInt aPriority);
	IMPORT_C void Receive();
	IMPORT_C TMessageBase* Poll();
	IMPORT_C TMessageBase* Last();
	IMPORT_C void CompleteAll(TInt aResult);
	//using TDfc::SetDfcQ;
public:
	//inline static void Lock() {NKern::FMWait(&MsgLock);}
	//inline static void Unlock() {NKern::FMSignal(&MsgLock);}
	//inline void UnlockAndKick() {Enque(&MsgLock);}
public:
	SDblQue iQ;
	TBool iReady;
	TMessageBase* iMessage;
	static NFastMutex MsgLock;
	friend class TMessageBase;
	};

class TSecondLink:public SDblQueLink
	{
	IMPORT_C TInt At(long long  const&, void (*)(void *), void *);
	IMPORT_C void Cancel(void);
	IMPORT_C  TSecondLink(void);
	
private:
	TInt64 iTime;//8
	TAny* iPtr;//10
	TAny*iCallBack;//14
	};

class TPropertySubsRequest : public SDblQueLink
	{
public:
	/*TPropertySubsRequest(TPropertyCompleteFn aCompleteFn, TAny* aPtr)
		{
		iNext = NULL;
		iCompleteFn = aCompleteFn;
		iPtr = aPtr;
		}*/
	//TPropertyCompleteFn iCompleteFn;
	TAny* iCompleteFn;
	TAny* iPtr;
private:
	friend class TProperty;
	//DProcess* iProcess;
	TAny* iProcess;
};

class TProperty
{
public:
	//static TInt Init();
	//static TInt Attach(TUid aCategory, TUint aKey, TProperty** aProp);
	//static TInt Open(TUid aCategory, TUint aKey, TProperty** aProp);
	//void Close();
	//TInt Define(const TPropertyInfo*, DProcess*);
	//TInt Delete(DProcess*);
	//TInt Subscribe(TPropertySubsRequest* aSubs, DProcess*);
	//void Cancel(TPropertySubsRequest* aSubs);
	//TInt GetI(TInt* aValue, DProcess*);
	//TInt GetB(TUint8* aBuf, TInt* aSize, DProcess*, TBool aUser);
	//TInt SetI(TInt aValue, DProcess*);
	//TInt SetB(const TUint8*, TInt aSize, DProcess*, TBool aUser);
	const TUid iCategory;
	const TUint iKey;
private:
	enum
		{KCompletionDfcPriority = 2};
	static TDfc CompletionDfc;
	static SDblQue CompletionQue;
	static DMutex* FeatureLock;
	static TProperty* Table[1024];
	TUint8 iType;
	TUint8 iAttr;
	TUint iReadPolicy;//TCompiledSecurityPolicy iReadPolicy;
	TUint iWritePolicy;//TCompiledSecurityPolicy iWritePolicy;
	TUint32 iOwner;
	TUint iRefCount;
	// The property value
	// Meaningful for defined properties only
	// (ie. iType != RProperty::ETypeLimit)
	union // the value is protected by the system lock

		{
		//TBuf* iBuf;
		TAny* iBuf;
		TInt iValue;
		};
	};


class RPropertyRef
	{
public:
	RPropertyRef() {iProp = NULL;}
	IMPORT_C TInt Attach(TUid aCategory, TInt aKey);
	IMPORT_C TInt Open(TUid aCategory, TInt aKey);
	IMPORT_C void Close();
	IMPORT_C TInt Define(TInt aAttr, const TSecurityPolicy& aReadPolicy,
			const TSecurityPolicy& aWritePolicy,
			TInt aPreallocate=0, DProcess* aProcess = NULL);
	IMPORT_C TInt Delete(DProcess* aProcess = NULL);
	IMPORT_C TInt Subscribe(TPropertySubsRequest& aRequest,
			DProcess* aProcess = NULL);
	IMPORT_C void Cancel(TPropertySubsRequest& aRequest);
	IMPORT_C TInt Get(TInt& aValue, DProcess* aProcess = NULL);
	IMPORT_C TInt Set(TInt aValue, DProcess* aProcess = NULL);
	IMPORT_C TInt Get(TDes8& aDes, DProcess* aProcess = NULL);
	IMPORT_C TInt Set(const TDesC8& aDes, DProcess* aProcess = NULL);
	//IMPORT_C TBool GetStatus(TPropertyStatus& aStatus);
	TBool GetStatus(TAny* aStatus);
private:
	TProperty* iProp;
};

class Epoc
	{
public:
	IMPORT_C static TInt ExceptionInfo();
	IMPORT_C static TInt FreeShadowPage(TUint32 aRomAddr);
	IMPORT_C static TInt RomProcessInfo(TProcessCreateInfo &aProcessCreateInfo, TRomImageHeader  const& aRomImageHeader);
	IMPORT_C static TInt AllocShadowPage(TUint32 aRomAddr);
	IMPORT_C static TInt FreePhysicalRam(TUint32, int);
	IMPORT_C static TInt AllocPhysicalRam(int, TUint32 &, int);
	IMPORT_C static TInt ClaimPhysicalRam(TUint32, int);
	IMPORT_C static TInt FreezeShadowPage(TUint32);
	IMPORT_C static TInt LinearToPhysical(TUint32);
	IMPORT_C static TInt SetMonitorEntryPoint(void (*)(void *));
	IMPORT_C static TInt SetMonitorExceptionHandler(TUint32);
	IMPORT_C static TUint RomHeader();
	};

class TChannelCreateInfo
{
public:
	TVersion iVersion;
	TInt iUnit;
	const TDesC8* iPhysicalDevice;
	const TDesC8* iInfo;
};

class DLogicalChannelBase : public DObject
	{
public:
	IMPORT_C virtual ~DLogicalChannelBase();
public:
	virtual TInt Request(TInt aReqNo, TAny* a1, TAny* a2)=0;
	IMPORT_C virtual TInt DoCreate(TInt aUnit,
			const TDesC8* aInfo,
			const TVersion& aVer);
public:
	DLogicalDevice* iDevice;
	DPhysicalDevice* iPhysicalDevice;
	DBase* iPdd;
	};



class DLogicalDevice : public DObject
	{
public:
	IMPORT_C virtual ~DLogicalDevice();
	IMPORT_C virtual TBool QueryVersionSupported(const TVersion& aVer) const;
	IMPORT_C virtual TBool IsAvailable(TInt aUnit, const TDesC8* aDriver,
			const TDesC8* aInfo) const;
	TInt ChannelCreate(DLogicalChannelBase*& pC, TChannelCreateInfo& aInfo);
	TInt FindPhysicalDevice(DLogicalChannelBase*
	aChannel, TChannelCreateInfo& aInfo);
	virtual TInt Install()=0;
	virtual void GetCaps(TDes8& aDes) const =0;
	virtual TInt Create(DLogicalChannelBase*&aChannel)=0;
public:
	TVersion iVersion;
	TUint iParseMask;
	TUint iUnitsMask;
	DCodeSeg* iCodeSeg;
	TInt iOpenChannels;
	};

class DPhysicalDevice : public DObject
	{
public:
	enum TInfoFunction
		{
		EPriority=0,
		};
public:
	IMPORT_C virtual ~DPhysicalDevice();
	IMPORT_C virtual TBool QueryVersionSupported(const TVersion& aVer)
	const;
	IMPORT_C virtual TBool IsAvailable(TInt aUnit, const TDesC8* aInfo)
	const;
	virtual TInt Install() =0;
	virtual void GetCaps(TDes8& aDes) const =0;
	virtual TInt Create(DBase*& aChannel, TInt aUnit, const TDesC8* aInfo,
			const TVersion& aVer) =0;
	virtual TInt Validate(TInt aUnit, const TDesC8* aInfo, const TVersion&
	aVer) =0;
	IMPORT_C virtual TInt Info(TInt aFunction, TAny* a1);
public:
	TVersion iVersion;
	TUint iUnitsMask;
	DCodeSeg* iCodeSeg;
	};

class DLogicalChannel : public DLogicalChannelBase
	{
public:
	enum
		{EMinRequestId=0xc0000000, ECloseMsg=0x80000000};
public:
	IMPORT_C DLogicalChannel();
	IMPORT_C virtual ~DLogicalChannel();
	IMPORT_C virtual TInt Close(TAny*);
	IMPORT_C virtual TInt Request(TInt aReqNo, TAny* a1, TAny* a2);
	IMPORT_C virtual void HandleMsg(TMessageBase* aMsg)=0;
	IMPORT_C void SetDfcQ(TDfcQue* aDfcQ);
public:
	static void MsgQFunc(TAny* aPtr);
public:
	TDfcQue* iDfcQ;
	TMessageQue iMsgQ;
	};

class TThreadCleanup : public TPriListLink
	{
public:
	IMPORT_C TThreadCleanup();
	void ChangePriority(TInt aNewPriority);
	IMPORT_C void Remove();
	virtual void Cleanup()=0;
public:
	DThread* iThread;
public:
	friend class Monitor;
	};

class RMessageKBase : public SDblQueLink
	{
public:
	TBool IsFree() const
		{
		return !iNext;
		}
	TBool IsDelivered() const
		{
		return iNext!=0 && (TLinAddr(iNext) & 3)==0;
		}
	TBool IsAccepted() const
		{
		return ((TLinAddr)iNext & 3)==3;
		}
public:
	TInt iFunction;
	};
class RMessageK : public RMessageKBase
	{
public:
	enum TMsgType
		{EDisc=0, ESync=1, ESession=2, EGlobal=3};
	inline TInt ArgType(TInt aParam) const;
	inline TInt Arg(TInt aParam) const;
	void Free();
	//static RMessageK* NewMsgBlock(TInt aCount, TInt aType);
	IMPORT_C DThread* Thread() const;
	//static RMessageK* MessageK(TInt aHandle, DThread* aThread);
	IMPORT_C static RMessageK* MessageK(TInt aHandle);
public:
	TInt iArgs[4];
	TUint16 iArgFlags; // describes which arguments are descriptors/handles
	TUint8 iPool; // 0=disconnect msg, 1=thread sync message,
	// 2=from session pool, 3=from global pool
	TUint8 iPad;
	//DSession* iSession; // pointer to session
	TAny* iSession;
	// 
	SDblQueLink iSessionLink; // attaches message to session
	DThread* iClient; // pointer to client thread (not reference counted)
	TRequestStatus* iStatus; // pointer to user side TRequestStatus
	};

class NThread : public NThreadBase
	{
public:
	TInt Create(SNThreadCreateInfo& aInfo, TBool aInitial);
	inline void Stillborn()
		{
		}
	// Value indicating what event caused thread to
	// enter privileged mode.
	enum TUserContextType
		{
		EContextNone=0, /* Thread has no user context */
		EContextException=1, /* HW exception while in user mode */
		EContextUndefined,
		EContextUserInterrupt, /* Preempted by int in user mode */
		// Killed while preempted by interrupt taken in user mode
		EContextUserInterruptDied,
		// Preempted by interrupt taken in executive call handler
		EContextSvsrInterrupt1,
		// Killed while preempted by interrupt taken in
		// executive call handler
		EContextSvsrInterrupt1Died,
		// Preempted by interrupt taken in executive call handler
		EContextSvsrInterrupt2,
		// Killed while preempted by interrupt taken in
		// executive call handler */
		EContextSvsrInterrupt2Died,
		EContextWFAR, // Blocked on User::WaitForAnyRequest()
		// Killed while blocked on User::WaitForAnyRequest()
		EContextWFARDied,
		EContextExec, // Slow executive call
		EContextKernel, // Kernel-side context (for kernel threads)
		};
	IMPORT_C static const  TAny* //TArmContextElement*
	const* UserContextTables();
	IMPORT_C TUserContextType UserContextType();
/*	inline TInt SetUserContextType()
		{
		return iSpare3=UserContextType();
		}
	inline void ResetUserContextType()
		{
		if (iSpare3>EContextUndefined)
			iSpare3=EContextUndefined;
		}
	void GetUserContext(TArmRegSet& aContext, TUint32& aAvailRegistersMask);
	void SetUserContext(const TArmRegSet& aContext);
	void ModifyUsp(TLinAddr aUsp);*/
#ifdef __CPU_ARM_USE_DOMAINS
	TUint32 Dacr();
	void SetDacr(TUint32 aDacr);
	TUint32 ModifyDacr(TUint32 aClearMask, TUint32 aSetMask);
#endif
#ifdef __CPU_HAS_COPROCESSOR_ACCESS_REG
	void SetCar(TUint32 aDacr);
#endif
	IMPORT_C TUint32 Car();
	IMPORT_C TUint32 ModifyCar(TUint32 aClearMask, TUint32 aSetMask);
#ifdef __CPU_HAS_VFP
	void SetFpExc(TUint32 aDacr);
#endif
	IMPORT_C TUint32 FpExc();
	IMPORT_C TUint32 ModifyFpExc(TUint32 aClearMask, TUint32 aSetMask);
	};

class TTrapHandler
	{
public:
	IMPORT_C TTrapHandler();
	IMPORT_C virtual void Trap()=0;
	IMPORT_C virtual void UnTrap()=0;
	IMPORT_C virtual void Leave(TInt aValue)=0;
	};


class DThread : public DObject
	{
public:
	enum
		{EDefaultUserTimeSliceMs = 20};
	enum TThreadState
		{
		ECreated,
		EDead,
		EReady,
		EWaitSemaphore,
		EWaitSemaphoreSuspended,
		EWaitMutex,
		EWaitMutexSuspended,
		EHoldMutexPending,
		EWaitCondVar,
		EWaitCondVarSuspended,
		};
	enum TOperation
		{
		ESuspend=0,
		EResume=1,
		EForceResume=2,
		EReleaseWait=3,
		EChangePriority=4,
		};
public:
	DThread();
	void Destruct();
	TInt Create(SThreadCreateInfo& aInfo);
	TInt SetPriority(TThreadPriority aPriority);
	IMPORT_C void RequestComplete(TRequestStatus*& aStatus,
			TInt aReason);
	IMPORT_C TInt DesRead(const TAny* aPtr, TUint8* aDes,
			TInt aMax, TInt aOffset, TInt aMode);
	IMPORT_C TInt DesWrite(const TAny* aPtr, const TUint8* aDes,
			TInt aLength, TInt aOffset, TInt aMode,
			DThread* aOriginatingThread);
	// not memory model dependent
	TInt DoCreate(SThreadCreateInfo& aInfo);
	IMPORT_C void SetThreadPriority(TInt aThreadPriority);
	/*void SetDefaultPriority(TInt aDefaultPriority);
	void AbortTimer(TBool aAbortAbsolute);
	void Suspend(TInt aCount);
	void Resume();
	void ForceResume();
	void Exit();
	void Die(TExitType aType, TInt aReason, const TDesC& aCategory);
	TInt Logon(TRequestStatus* aStatus, TBool aRendezvous);
	void Rendezvous(TInt aReason);
	// memory model dependent
	TInt AllocateSupervisorStack();
	void FreeSupervisorStack();
	void FreeUserStack();
	TInt AllocateUserStack(TInt aSize);
	TInt RawRead(const TAny* aSrc, TAny* aDest, TInt aLength, TInt aFlags);
	TInt RawWrite(const TAny* aDest, const TAny* aSrc, TInt aLength,
			TInt aFlags, DThread* aOriginatingThread);
	DChunk* OpenSharedChunk(const TAny* aAddress, TBool aWrite,
			TInt& aOffset);
	static void DefaultUnknownStateHandler(DThread* aThread,
			TInt& aOperation, TInt aParameter);
	static void EpocThreadFunction(TAny* aPtr);
	static TDfc* EpocThreadExitHandler(NThread* aThread);
	static void EpocThreadTimeoutHandler(NThread* aThread,
			TInt aOp);*/
public:
	TUint32 iIpcCount;
	TLinAddr iUserStackRunAddress;
	TInt iUserStackSize;
	TUint32 iFlags;
	DProcess* iOwningProcess;
	SDblQueLink iProcessLink;
	TInt iThreadPriority;
	//DObjectIx* iHandles;
	TUint iId;
	RAllocator* iAllocator;
	RAllocator* iCreatedAllocator;
	//TTrap* iFrame;
	TTrapHandler* iTrapHandler;
	//RArray<STls> iTls;
	//CActiveScheduler* iScheduler;
	TExceptionHandler iExceptionHandler;
	TUint iExceptionMask;
	//TExcTrap* iExcTrap;
	TInt iDebugMask;
	//TThreadMessage iKernMsg;
	DObject* iTempObj;
	DObject* iExtTempObj;
	TAny* iTempAlloc;
	SDblQue iOwnedLogons;
	SDblQue iTargetLogons;
	RMessageK iSyncMsg;
	TDfc iKillDfc;
	SDblQue iClosingLibs;
	TPriListLink iWaitLink;
	TInt iDefaultPriority; // default scheduling priority
	TAny* iWaitObj; // object on which this thread is waiting
	// handler for extra thread states - used by RTOS
	// personality layers
	//TUnknownStateHandler iUnknownStateHandler;
	TAny* iUnknownStateHandler;;
	// pointer to extra data used by RTOS personality layers
	TAny* iExtras;
	TAny* iSupervisorStack;// thread¡¯s supervisor mode stack
	TInt iSupervisorStackSize;
	TUint8 iSupervisorStackAllocated;
	TUint8 iThreadType;
	TUint8 iExitType;
	TUint8 iPad1;
	TInt iExitReason;
	TBufC<KMaxExitCategoryName> iExitCategory;
	// list of held mutexes, used only for acquisition
	// order checking
	SDblQue iMutexList;
	// things to clean up when we die
	TPriList<TThreadCleanup,KNumPriorities> iCleanupQ;
	//TTimer iTimer;
	TInt iTimer;
	NThread iNThread;
	};

class TDblQueLinkBase
	{
public:
	inline TDblQueLinkBase() : iNext(NULL){};
	IMPORT_C void Enque(TDblQueLinkBase* aLink);
	IMPORT_C void AddBefore(TDblQueLinkBase* aLink);
public:

	TDblQueLinkBase* iNext;

	TDblQueLinkBase* iPrev;
	};

class TDeltaQueLink : public TDblQueLinkBase
	{
public:
	TInt iDelta;
	};

typedef void (*TTickCallBack)(TAny *aPtr);
class TTickQ;
class TTickLink : public TDeltaQueLink
	{
public:
	IMPORT_C TTickLink();
	IMPORT_C void Periodic(TInt aPeriod,TTickCallBack aCallBack,TAny *aPtr);
	IMPORT_C void OneShot(TInt aTime,TTickCallBack aCallBack,TAny *aPtr);
	IMPORT_C void Lock(TInt aTicks,TTickCallBack aCallBack, TAny *aPtr);
	IMPORT_C void Cancel();
	//IMPORT_C TInt GetNextLock(TTimerLockSpec aMark, TInt &aTickCount) const;

private:
	enum {EOneShotTimer = 0, ELockTimer = -1}; // iPeriod for non-periodics
	TInt iPeriod;
	TAny *iPtr;
	TTickCallBack iCallBack;
	TUint iLastLock;
//	friend class TTickQ;
	};

class Cache
	{
	IMPORT_C static void AtomicSyncMemory(void);
	IMPORT_C static void SyncMemoryAfterDmaRead(unsigned long, unsigned int);
	IMPORT_C static void SyncMemoryBeforeDmaRead(unsigned long, unsigned int);
	IMPORT_C static void SyncMemoryBeforeDmaRead(unsigned long, unsigned int, unsigned long);
	IMPORT_C static void SyncMemoryBeforeDmaWrite(unsigned long, unsigned int);
	IMPORT_C static void SyncMemoryBeforeDmaWrite(unsigned long, unsigned int, unsigned long);
	IMPORT_C static void IMB_Range(unsigned long, unsigned int);
	};

class  DebugSupport
{
public:
IMPORT_C static void ModifyCode(DThread *, unsigned long, int, unsigned int, unsigned int);
IMPORT_C static void RestoreCode(DThread *, unsigned long);
IMPORT_C static void CloseCodeModifier(void);
IMPORT_C static void InitialiseCodeModifier(unsigned int &, int);
};

extern "C"{  //for C++ only

IMPORT_C void kumemget(TAny* aKernAddr, const TAny* aAddr, TInt aLength);
IMPORT_C void kumemget32(TAny* aKernAddr, const TAny* aAddr, TInt aLength);
IMPORT_C void kumemput(TAny* aAddr, const TAny* aKernAddr, TInt aLength);
IMPORT_C void kumemput32(TAny* aAddr, const TAny* aKernAddr, TInt aLength);
IMPORT_C void kumemset(TAny* aAddr, const TUint8 aValue, TInt aLength);
IMPORT_C void umemget(TAny* aKernAddr, const TAny* aUserAddr, TInt aLength);
IMPORT_C void umemget32(TAny* aKernAddr, const TAny* aUserAddr, TInt aLength);
IMPORT_C void umemput(TAny* aUserAddr, const TAny* aKernAddr, TInt aLength);
IMPORT_C void umemput32(TAny* aUserAddr, const TAny* aKernAddr, TInt aLength);
IMPORT_C void umemset(TAny* aUserAddr, const TUint8 aValue, TInt aLength);
IMPORT_C TInt memicmp(const TUint8 *aLeft, TInt aLeftLen, const TUint8 *aRight, TInt aRightLen);

}


#endif /*EKERN_H_*/
