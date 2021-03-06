//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Engine
//               
//		This file contains the Debugger class and its helpers header
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef debuggerH
#define debuggerH

#include <windows.h>
#include "..\debugging.h"

// --- EventManager ---

#define DEBUG_CLOSE	   0
#define DEBUG_SUSPEND   1
#define DEBUG_RESUME    2
#define DEBUG_ACTIVE	   3
#define MAX_DEBUG_EVENT 4

namespace _ELENA_
{

class DebugEventManager
{
   HANDLE _events[MAX_DEBUG_EVENT];

public:
   void init();
   void resetEvent(int event);
   void setEvent(int event);
   int  waitForAnyEvent();
   bool waitForEvent(int event, int timeout);
   void close();
      
   DebugEventManager()
   {
      for (int i = 0 ; i < MAX_DEBUG_EVENT ; i++)
         _events[i] = NULL;
   }   
   ~DebugEventManager()
   {
      close();                
   }
};

// --- ThreadBreakpoint ---

struct ThreadBreakpoint
{
   bool   software;
   bool   hardware;
   size_t next;
   size_t stackLevel;

   void clearSoftware()
   {
      software = false;
      next = 0;
   }

   ThreadBreakpoint()
   {
      hardware = software = false;
      stackLevel = next = 0;
   }
};

// --- ProcessException ---

struct ProcessException
{
   int code;
   int address;

   wchar_t* Text();

   ProcessException()
   {
      code = 0;
   }
};

// --- ThreadContext ---

struct ThreadContext
{
   friend class Debugger;
   friend struct BreakpointContext;

protected:
   void*   state;
   HANDLE  hProcess;
   HANDLE  hThread;
   CONTEXT context;

public:
   ThreadBreakpoint breakpoint;

   bool atCheckPoint;
   bool checkFailed;
   bool autoStep;

   void* State() const { return state; }
   size_t EIP() const { return context.Eip; }
   size_t Self() const { return context.Edi; }
   size_t Local(int offset) { return context.Ebp - offset * 4; }
   size_t Current(int offset) { return context.Esp + offset * 4; }
   size_t ClassVMT(size_t address);
   size_t VMTFlags(size_t address);
   size_t ObjectPtr(size_t address);
   size_t LocalPtr(int offset) { return ObjectPtr(Local(offset)); }
   size_t CurrentPtr(int offset) { return ObjectPtr(Current(offset)); }

   void readDump(size_t address, char* dump, size_t length);
   void writeDump(size_t address, char* dump, size_t length);

   size_t readDWord(size_t address)
   {
      size_t word = 0;
      readDump(address, (char*)&word, 4);

      return word;
   }

   void writeDWord(size_t address, size_t word)
   {
      writeDump(address, (char*)&word, 4);
   }

   void refresh();

   void setCheckPoint();
   void setTrapFlag();
   void resetTrapFlag();
   void setHardwareBreakpoint(size_t breakpoint);
   unsigned char setSoftwareBreakpoint(size_t breakpoint);
   void setEIP(size_t address);

   void clearHardwareBreakpoint();
   void clearSoftwareBreakpoint(size_t breakpoint, char substitute);

   ThreadContext(HANDLE hProcess, HANDLE hThread);
};

// --- BreakpointContext ---

struct BreakpointContext
{
   Map<size_t, char> breakpoints;   

   void addBreakpoint(size_t address, ThreadContext* context, bool started);
   void removeBreakpoint(size_t address, ThreadContext* context, bool started);
   void setSoftwareBreakpoints(ThreadContext* context);
   void setHardwareBreakpoint(size_t address, ThreadContext* context, bool withStackLevelControl);

   bool processStep(ThreadContext* context, bool stepMode);
   bool processBreakpoint(ThreadContext* context);

   void clear();

   BreakpointContext();
};

// --- Debugger ---

class Debugger
{
   typedef Map<int, ThreadContext*> ThreadContextes;

   DWORD             threadId;

   bool              started;
   bool              trapped;
   bool              stepMode;
   bool              needToHandle;
   bool              exitCheckPoint;
   size_t            vm_breakpoint;

   BreakpointContext breakpoints;

   ThreadContextes   threads;
   ThreadContext*    current;

   DWORD             dwCurrentProcessId;
   DWORD             dwCurrentThreadId;

   size_t            minAddress, maxAddress;

   MemoryMap<int, void*> steps;

   ProcessException exception;

   bool startProcess(const wchar_t* exePath, const wchar_t* cmdLine);
   void processEvent(size_t timeout);
   void processException(EXCEPTION_DEBUG_INFO* exception);
   void continueProcess();

   void processStep();

public:
   bool isStarted() const { return started; }
   bool isTrapped() const { return trapped; }
   bool isVMBreakpoint() const { return vm_breakpoint == current->context.Eip; }
   bool checkAutoStep();

   ThreadContext* Context() { return current; }
   ProcessException* Exception() { return exception.code == 0 ? NULL : &exception; }

   void resetException() { exception.code = 0; }

   void addStep(size_t address, void* state);

   void addBreakpoint(size_t address);
   void removeBreakpoint(size_t address);
   void clearBreakpoints();

   void setStepMode();
   void setBreakpoint(size_t address, bool withStackLevelControl);
   void setCheckMode();
   void setAutoStepMode();

   bool startThread(_Controller* controller);

   bool start(const wchar_t* exePath, const wchar_t* cmdLine);
   void run();
   bool proceed(size_t timeout);
   void stop();

   void processVirtualStep(void* step);
   bool proceedCheckPoint();

   void reset();

   void activate();

   void setVMHook() { vm_breakpoint = -1; }

   Debugger();
};

// --- setForegroundWindow() ---
inline void setForegroundWindow(HWND hwnd)
{
   DWORD dwTimeoutMS;
   // Get the current lock timeout.
   ::SystemParametersInfo (0x2000, 0, &dwTimeoutMS, 0);

   // Set the lock timeout to zero
   ::SystemParametersInfo (0x2001, 0, 0, 0);

   // Perform the SetForegroundWindow
   ::SetForegroundWindow (hwnd);

   // Set the timeout back
   ::SystemParametersInfo (0x2001, 0, (LPVOID)dwTimeoutMS, 0);   //HWND hCurrWnd;
}

} // _ELENA_

#endif // debuggerH
