//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler Engine
//
//		This file contains ELENA byte code writer class.
//
//                                              (C)2005-2013, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef bcwriterH
#define bcwriterH 1

#include "bytecode.h"

namespace _ELENA_
{

enum ObjectKind 
{
   okUnknown = 0,

   okExternal,
   okConstant,
   okSymbol,
   okVSelf,
   okSelf,
   okSuper,
   okField,
   okOuter,
   okOuterField,
   okLocal,
   okBlockLocal,
   okCurrent,
   okRegister,
   okRegisterField,
   okCurrentMessage,
   okLocalAddress,
   okBlockLocalAddress
};

enum ObjectType 
{
   otNone,
   otControl,
   otClass,
   otMessage,
   otSignature,
   otRole,

   otByte,
   otInt,
   otLong,
   otReal,
   otLiteral,
   otChar,
   otByteArray,
   otArray,
   otLength,
   otIndex,
   otRef,
   otParams
};

struct ObjectInfo 
{
   ObjectKind kind;
   ObjectType type;
   ref_t      reference;
   ref_t      extraparam;

   ObjectInfo()
   {
      this->kind = okUnknown;
      this->type = otNone;
      this->reference = 0;
      this->extraparam = 0;
   }
   ObjectInfo(ObjectKind kind)
   {
      this->kind = kind;
      this->type = otNone;
      this->reference = 0;
      this->extraparam = 0;
   }
   ObjectInfo(ObjectKind kind, ObjectInfo copy)
   {
      this->kind = kind;
      this->type = copy.type;
      this->reference = copy.reference;
      this->extraparam = copy.extraparam;
   }
   ObjectInfo(ObjectKind kind, ref_t reference)
   {
      this->kind = kind;
      this->type = otNone;
      this->reference = reference;
      this->extraparam = 0;
   }
   ObjectInfo(ObjectKind kind, ObjectType type)
   {
      this->kind = kind;
      this->type = type;
      this->reference = 0;
      this->extraparam = 0;
   }
   ObjectInfo(ObjectKind kind, ObjectType type, ref_t reference)
   {
      this->kind = kind;
      this->type = type;
      this->reference = reference;
      this->extraparam = 0;
   }
   ObjectInfo(ObjectKind kind, ObjectType type, ref_t reference, ref_t extraparam)
   {
      this->kind = kind;
      this->type = type;
      this->reference = reference;
      this->extraparam = extraparam;
   }
   ObjectInfo(ObjectKind kind, ref_t reference, ref_t extraparam)
   {
      this->kind = kind;
      this->type = otNone;
      this->reference = reference;
      this->extraparam = extraparam;
   }
};

// --- ByteCodeWriter class ---
class ByteCodeWriter
{
   ref_t _sourceRef;

   ByteCode peekNext(ByteCodeIterator it)
   {
      it++;

      return (*it).code;
   }
	
   ByteCode peekPrevious(ByteCodeIterator it)
   {
      it--;

      return (*it).code;
   }

   void writeNewStatement(MemoryWriter* debug);
   void writeLocal(MemoryWriter* debug, MemoryWriter* debugStrings, const wchar16_t* localName, int level, int frameLevel  );
   void writeLocal(MemoryWriter* debug, MemoryWriter* debugStrings, const wchar16_t* localName, int level, DebugSymbol symbol, int frameLevel);
   void writeSelfLocal(MemoryWriter* debug, MemoryWriter* debugStrings, int level);
   void writeBreakpoint(ByteCodeIterator& it, MemoryWriter* debug);

   void writeFieldDebugInfo(ClassInfo::FieldMap& fields, MemoryWriter* writer, MemoryWriter* debugStrings);
   void writeClassDebugInfo(_Module* debugModule, MemoryWriter* debug, MemoryWriter* debugStrings, const wchar16_t* className, int flags);
   void writeSymbolDebugInfo(_Module* debugModule, MemoryWriter* debug, MemoryWriter* debugStrings, const wchar16_t* symbolName);
   void writeProcedureDebugInfo(MemoryWriter* writer, ref_t sourceNameRef);
   void writeDebugInfoStopper(MemoryWriter* debug);

   void writeProcedure(ByteCodeIterator& it, MemoryWriter* code, MemoryWriter* debug, MemoryWriter* debugStrings);
   void writeProcedure(ByteCodeIterator& it, MemoryWriter* code)
   {
      writeProcedure(it, code, NULL, NULL);
   }
   void writeVMT(size_t classPosition, MemoryWriter* vmt, MemoryWriter* code, ByteCodeIterator& it, MemoryWriter* debug, MemoryWriter* debugStrings);
//   void writeAction(ref_t reference, ByteCodeIterator& it, _Module* module, _Module* debugModule);
   void writeSymbol(ref_t reference, ByteCodeIterator& it, _Module* module, _Module* debugModule);
//   void writeClassHandler(ref_t reference, ByteCodeIterator& it, _Module* module, _Module* debugModule);
   void writeClass(ref_t reference, ByteCodeIterator& it, _Module* module, _Module* debugModule);

public:
   void declareModule(_Module* debugModule, const wchar16_t* path);
   void declareClass(CommandTape& tape, ref_t reference);
   void declareSymbol(CommandTape& tape, ref_t reference);
   void declareStaticSymbol(CommandTape& tape, ref_t staticReference);
   void declareMethod(CommandTape& tape, ref_t message, bool withNewFrame = true);
   void declareGenericAction(CommandTape& tape, ref_t genericMessage, ref_t message);
   void exclude(CommandTape& tape);
   void declareExternalBlock(CommandTape& tape);
   void declareVariable(CommandTape& tape, ref_t nilReference);
   void declareVariable(CommandTape& tape);
   void declareArgumentList(CommandTape& tape, int count);
   int declareLoop(CommandTape& tape/*, bool threadFriendly*/);  // thread friendly means the loop contains safe point
   void declareThenBlock(CommandTape& tape, bool withStackControl = true);
   void declareThenElseBlock(CommandTape& tape);
   void declareElseBlock(CommandTape& tape);
   void declareSwitchBlock(CommandTape& tape);
   void declareSwitchOption(CommandTape& tape);
   void declareTry(CommandTape& tape);
   void declareCatch(CommandTape& tape);
   void declarePrimitiveCatch(CommandTape& tape);

   void declareLocalInfo(CommandTape& tape, const wchar16_t* localName, int level);
   void declareLocalIntInfo(CommandTape& tape, const wchar16_t* localName, int level);
   void declareLocalLongInfo(CommandTape& tape, const wchar16_t* localName, int level);
   void declareLocalRealInfo(CommandTape& tape, const wchar16_t* localName, int level);
   void declareSelfInfo(CommandTape& tape, int level);
   void declareBreakpoint(CommandTape& tape, int row, int disp, int length, int stepType);
   void declareStatement(CommandTape& tape);

   void nextCatch(CommandTape& tape);

   void newSelf(CommandTape& tape);
   void newFrame(CommandTape& tape);
   void newStructure(CommandTape& tape, int size, ref_t reference);
   void newObject(CommandTape& tape, int fieldCount, ref_t reference, ref_t nilReference);
   void newDynamicObject(CommandTape& tape, ref_t reference, int sizeOffset);
   void newDynamicStructure(CommandTape& tape, int size, ref_t reference, int sizeOffset, int permanentSize = 0);

   void pushObject(CommandTape& tape, ObjectInfo object);
   void swapObject(CommandTape& tape, ObjectKind type, int offset);
   void loadObject(CommandTape& tape, ObjectInfo object);
   void setObject(CommandTape& type, int value);
   void saveObjectLength(CommandTape& tape, int size, int modificator, int offset);
   void saveObject(CommandTape& tape, ObjectInfo object);
   void boxObject(CommandTape& tape, int size, ref_t vmtReference, bool registerMode);
   void popObject(CommandTape& tape, ObjectInfo object);
   void releaseObject(CommandTape& tape, int count = 1);

   void assignLocalObject(CommandTape& tape, int offset);    // light-weight assigning
   void moveLocalObject(CommandTape& tape, int index);

   void setMessage(CommandTape& tape, ref_t message);

   void sendMessage(CommandTape& tape, int paramCount);
   void sendMessage(CommandTape& tape, int targetOffset, int paramCount);
   void sendDirectMessage(CommandTape& tape, int vmtOffset, int paramCount);
   void sendRoleMessage(CommandTape& tape, int paramCount);
   void sendRoleMessage(CommandTape& tape, ref_t classRef, int paramCount);
   void sendSelfMessage(CommandTape& tape, ref_t classRef, ref_t message);
   void invokeMessage(CommandTape& tape, ref_t reference, ref_t message, int targetOffset);
   void invokeConstantRoleMessage(CommandTape& tape, ref_t classRef, ref_t messageRef);
   void dispatchVerb(CommandTape& tape, int verb, int dispatcherOffset, int targetOffset);
   void extendObject(CommandTape& tape, ObjectInfo info);

   void redirectVerb(CommandTape& tape, ref_t message);
   void resend(CommandTape& tape, ObjectInfo info);
   void callBack(CommandTape& tape, int subject_id);
   void callAPI(CommandTape& tape, ref_t reference, bool embedded, int count);
   void callExternal(CommandTape& tape, ref_t functionReference, int paramCount);

   int declareLabel(CommandTape& tape);
   void compare(CommandTape& tape, ref_t trueRetVal, ref_t falseRetVal);
   void jumpIfEqual(CommandTape& tape, ObjectInfo object);
   void jumpIfEqual(CommandTape& tape, ref_t ref);
   void jumpIfNotEqual(CommandTape& tape, ref_t ref);
   void jumpIfNotEqual(CommandTape& tape, ObjectInfo object);
   void jump(CommandTape& tape, bool previousLabel = false);

   void setArrayItem(CommandTape& tape);
   void getArrayItem(CommandTape& tape);

   void throwCurrent(CommandTape& tape);
   void breakLoop(CommandTape& tape, int label);

   void gotoEnd(CommandTape& tape, PseudoArg label);

   void releaseSelf(CommandTape& tape);

   void insertStackAlloc(ByteCodeIterator it, CommandTape& tape, int size);
   void updateStackAlloc(ByteCodeIterator it, CommandTape& tape, int size);
   bool checkIfFrameUsed(ByteCodeIterator it);
   void commentFrame(ByteCodeIterator it);

   void endLabel(CommandTape& tape);
   void endCatch(CommandTape& tape);
   void endPrimitiveCatch(CommandTape& tape);
   void endThenBlock(CommandTape& tape, bool withStackContro = true);
   void endLoop(CommandTape& tape);
   void endExternalBlock(CommandTape& tape);
   void exitGenericAction(CommandTape& tape, int count, int reserved);
   void endGenericAction(CommandTape& tape, int count, int reserved);
   void exitMethod(CommandTape& tape, int count, int reserved, bool withFrame = true);
   void endMethod(CommandTape& tape, int paramCount, int reserved, bool withFrame = true);
   void endIdleMethod(CommandTape& tape);
   void endClass(CommandTape& tape);
   void endSymbol(CommandTape& tape);
   void endStaticSymbol(CommandTape& tape, ref_t staticReference);
   void endSwitchOption(CommandTape& tape);
   void endSwitchBlock(CommandTape& tape);

   //!! TODO: should be refactored
   void saveOutputResult(CommandTape& tape, int destOffset, int sourOffset);
   void saveIntParam(CommandTape& tape, ObjectInfo info);
   void saveLiteralParam(CommandTape& tape, ObjectInfo info, ref_t functionRef);
   void loadLiteralParam(CommandTape& tape, ref_t offset, ref_t functionRef);

   void flush(CommandTape& tape, _Module* module, _Module* debugModule);
};

} // _ELENA_

#endif // bcwriterH