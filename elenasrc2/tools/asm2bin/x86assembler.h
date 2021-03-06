//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Assembler Compiler
//
//		This header contains abstract Assembler declarations
//
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef x86assemblerH
#define x86assemblerH

#include "elena.h"
#include "source.h"
#include "assembler.h"
#include "win32\x86helper.h"
#include "x86jumphelper.h"

namespace _ELENA_
{

// --- x86Assembler ---

class x86Assembler : public Assembler
{
   typedef x86Helper::Operand       Operand;
   typedef x86Helper::OperandType   OperandType;

protected:
	Map<const wchar16_t*, size_t> constants;

   struct PrefixInfo
   {
      bool lockMode;

      bool Exists() { return lockMode; }

      void clear()
      {
         lockMode = false;
      }

      PrefixInfo()
      {
         clear();
      }
   };

	struct ProcedureInfo
	{
      _Module* binary;

      ref_t reference;
	   Map<const wchar16_t*, int> parameters;
      bool inlineMode;

      ProcedureInfo(_Module* binary, bool inlineMode)
      {
         this->binary = binary;
         this->reference = 0;
         this->inlineMode = inlineMode;
      }
	};

	void checkComma(TokenInfo& token)
	{
		if (!token.check(_T(",")))
			throw AssemblerException(_T("',' exprected(%d)\n"), token.terminal.row);
	}

	void setOffsetSize(Operand& operand)
	{
		if (abs(operand.offset) <= 0x80 && (size_t)operand.offset != 0x80000000) {
         operand.type = x86Helper::otDB;
		}
		else operand.type = x86Helper::otDD;
	}

   void loadDefaultConstants();

   void readParameterList(TokenInfo& token, ProcedureInfo& info, ReferenceNs& refName);

	int readStReg(TokenInfo& token);
   bool setOffset(Operand& operand, Operand disp);

	Operand defineRegister(TokenInfo& token);
	Operand defineOperand(TokenInfo& token, ProcedureInfo& info, const wchar16_t* err);
   Operand defineDisplacement(TokenInfo& token, ProcedureInfo& info, const wchar16_t* err);

   Operand readDispOperand(TokenInfo& token, ProcedureInfo& info, const wchar16_t* err, OperandType prefix);
	Operand readPtrOperand(TokenInfo& token, ProcedureInfo& info, const wchar16_t* err, OperandType prefix);

	Operand compileOperand(TokenInfo& token, ProcedureInfo& info, const wchar16_t* err);   

	void compileMOV(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileCMP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileADD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileXADD(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileADC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileAND(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileXOR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileOR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileLEA(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSUB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSBB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileTEST(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSHR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileSAR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSHL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileSHLD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileSHRD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileROL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileROR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileRCR(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileRCL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileXCHG(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileMOVZX(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileRET(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileNOP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileCDQ(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileLODSD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileLODSW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileLODSB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSTOSD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSTOSB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileMOVSB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSTOSW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileCMPSB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileCMPXCHG(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSTC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileSAHF(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compilePUSHFD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compilePOPFD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileREP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileREPZ(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compilePUSH(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compilePOP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileDEC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileINC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileNEG(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileNOT(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileMUL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileIMUL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileIDIV(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileDIV(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileCALL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, x86JumpHelper& helper);
	void compileLOOP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, x86JumpHelper& helper);

   void compileJxx(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, int prefix, x86JumpHelper& helper);
	void compileJMP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, x86JumpHelper& helper);

	void fixJump(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, x86JumpHelper& helper);

	void compileFLDZ(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFLDL2T(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFLDLG2(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFMULP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFRNDINT(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileF2XM1(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFLD1(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFADDP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSUBP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFDIVP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSCALE(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFXAM(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFABS(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFSQRT(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFSIN(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFCOS(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFYL2X(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFTST(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);

	void compileFBLD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFCHS(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFILD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFIST(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFISTP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFLD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFADD(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSUB(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFMUL(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFDIV(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFXCH(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSTP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFBSTP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSTSW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFNSTSW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFSTCW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFLDCW(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
	void compileFCOMIP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFCOMP(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFLDPI(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFPREM(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFPATAN(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFLDL2E(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFLDLN2(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileFFREE(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code);
   void compileSETCC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, int postfix);
   void compileCMOVCC(TokenInfo& token, ProcedureInfo& info, MemoryWriter* code, int postfix);

   bool compileCommandA(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandB(TokenInfo& token);
   bool compileCommandC(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer, x86JumpHelper& helper);
   bool compileCommandD(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandE(TokenInfo& token);
   bool compileCommandF(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandG(TokenInfo& token);
   bool compileCommandH(TokenInfo& token);
   bool compileCommandI(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandJ(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer, x86JumpHelper& helper);
   bool compileCommandK(TokenInfo& token);
   bool compileCommandL(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer, x86JumpHelper& helper);
   bool compileCommandM(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandN(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandO(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandP(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandQ(TokenInfo& token);
   bool compileCommandR(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandS(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandT(TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandU(TokenInfo& token);
   bool compileCommandV(TokenInfo& token);
   bool compileCommandW(TokenInfo& token);
   bool compileCommandX(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer);
   bool compileCommandY(TokenInfo& token);
   bool compileCommandZ(TokenInfo& token);
   bool compileCommand(PrefixInfo& prefix, TokenInfo& token, ProcedureInfo& info, MemoryWriter& writer, x86JumpHelper& helper);

   virtual void compileProcedure(TokenInfo& token, _Module* binary, bool inlineMode, bool aligned);
   virtual void compileStructure(TokenInfo& token, _Module* binary, int mask);

public:
	virtual void compile(TextReader* reader, const _path_t* outputPath);

	x86Assembler()
	{
	   loadDefaultConstants();
	}
	virtual ~x86Assembler() {}
};

} // _ELENA_

#endif // x86assemblerH
