//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This file contains ELENA Engine Derivation Tree class implementation
//
//                                              (C)2005-2009, by Alexei Rakov
//---------------------------------------------------------------------------

#include "elena.h"
// --------------------------------------------------------------------------
#include "derivation.h"

using namespace _ELENA_;

typedef String<wchar16_t, 0x100> TempString;

// --- DerivationWriter ---

void DerivationWriter :: writeSymbol(Symbol symbol)
{
   _writer->writeDWord(symbol);
}

void DerivationWriter :: writeTerminal(TerminalInfo terminal)
{
   _writer->writeDWord(terminal.symbol);
   _writer->writeDWord(terminal.disp);
   _writer->writeDWord(terminal.row);
   _writer->writeDWord(terminal.col);
   _writer->writeDWord(terminal.length);

   if (terminal==tsLiteral) {
      // try to use local storage if the quote is not too big
      if (getlength(terminal.value) < 0x100) {
         QuoteTemplate<TempString> quote(terminal.value);

         _writer->writeWideLiteral(quote);
      }
      else {
         QuoteTemplate<DynamicString<wchar16_t> > quote(terminal.value);

         _writer->writeWideLiteral(quote);
      }
   }
   else _writer->writeWideLiteral(terminal.value);
}

// --- DerivationReader ---

DNode DerivationReader :: readRoot()
{
   return Node(this, 0, nsNone);
}

DNode DerivationReader :: readFirstChild(size_t position)
{
   _reader->seek(position);

   Symbol current = (Symbol)_reader->getDWord();
   if (current != nsNone) {
      position = _reader->Position();
      current = (Symbol)_reader->getDWord();
	   if (test(current, mskTerminal)) {
         readTerminalInfo(current);

		   position = _reader->Position();
		   if (_reader->readDWord((int&)current)) {
            return Node(this, position, current);
         }
      }
	   else return Node(this, position, current);
   }
   return Node();
}

DNode DerivationReader :: readNextNode(size_t position)
{
   _reader->seek(position);

   int level = 0;
   Symbol current = nsNone;
   while (_reader->readDWord((int&)current)) {
      if (test(current, mskTerminal)) {
         readTerminalInfo(current);
         continue;
      }
      else if (current==nsNone) {
         if (level > 1) {
            level--;
         }
		   else break;
	   }
      else level++;
   }
   position = _reader->Position();
   if (_reader->readDWord((int&)current)) {
      return Node(this, position, current);
   }
   else return Node();
}

DNode DerivationReader :: seekSymbol(size_t position, Symbol symbol)
{
   _reader->seek(position);

   Symbol current = (Symbol)_reader->getDWord();
   if (!test(current, mskTerminal)) {
      int level = 1;
      while (level > 0) {
         position = _reader->Position();
         current = (Symbol)_reader->getDWord();
         if (test(current, mskTerminal)) {
            readTerminalInfo(current);
         }
         else if (current==nsNone) {
            level--;
         }
         else {
			   if (level==1 && current==symbol)
               return Node(this, position, current);
            level++;
         }
      }
   }
   return Node();
}

TerminalInfo DerivationReader :: readTerminalInfo(Symbol symbol)
{
   TerminalInfo terminal;

   terminal.symbol = symbol;
   _reader->readDWord(terminal.disp);
   _reader->readDWord(terminal.row);
   _reader->readDWord(terminal.col);
   _reader->readDWord(terminal.length);

   terminal.value = _reader->getWideLiteral();

   return terminal;
}

TerminalInfo DerivationReader :: readTerminal(size_t position)
{
   _reader->seek(position);

   Symbol current = (Symbol)_reader->getDWord();
   if (current != nsNone) {
      current = (Symbol)_reader->getDWord();
      if (test(current, mskTerminal)) {
         return readTerminalInfo(current);
      }
   }
   return TerminalInfo();
}
