//---------------------------------------------------------------------------
//              E L E N A   p r o j e c t
//                Command line syntax generator main file
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#include "sg.h"

using namespace _ELENA_;
using namespace _ELENA_TOOL_;

// !! code duplication (syntax.h)
const int mskAnySymbolMask             = 0x07000;               // masks
const int mskTraceble                  = 0x01000;

int last_id = 0;

int _registerSymbol(ParserTable& table, const wchar16_t* symbol, int new_id)
{
   int id = (int)table.defineSymbol(symbol);
   if (id == 0) {
      id = new_id;

      if ((symbol[0]<'A')||(symbol[0]>'Z'))
         id |= ParserTable::mskTerminal;

      table.registerSymbol(id, symbol);

      if (last_id < (id & ~mskAnySymbolMask)) last_id = id & ~mskAnySymbolMask;

      if (last_id >= mskTraceble) {
         printLine("WARNING: symbol id is overflown %d", last_id);
      }
   }
   return id;
}

int registerSymbol(ParserTable& table, const wchar16_t* symbol, int new_id)
{
   if (ConstIdentifier::compare(symbol, "||")) {
      return _registerSymbol(table, ConstIdentifier("|"), new_id);
   }
   else if (ConstIdentifier::compare(symbol, "&|")) {
      return _registerSymbol(table, ConstIdentifier("||"), new_id);
   }
   else if (ConstIdentifier::compare(symbol, "-->")) {
      return _registerSymbol(table, ConstIdentifier("->"), new_id);
   }
   else return _registerSymbol(table, symbol, new_id);
}

int main(int argc, char* argv[])
{
   printf("ELENA command line syntax generator %d.%d.0 (C)2005-2013 by Alexei Rakov\n", ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION);
   if (argc < 2 || argc > 3) {
      printLine("sg <syntax_file> [-cp<codepage>]");
      return 0;
   }
   try {
      int encoding = DEFAULT_ENCODING;

      if (argc==3) {
         if (StringHelper::compare(argv[2], "-cp", 3)) {
            encoding = StringHelper::strToInt(argv[2] + 3);
         }
         else {
            printLine("sg <syntax_file> [-cp<codepage>]");
            return 0;
         }
      }

      Path path;
		path.copy(argv[1], strlen(argv[1]));
      TextFileReader   sourceFile(path, encoding, true);
      TextSourceReader source(4, &sourceFile);
      ParserTable      table;
      LineInfo         info(0, 0, 0);
      wchar16_t        token[IDENTIFIER_LEN + 1];
      int              rule[20];
      int              rule_len = 0;
      bool             arrayCheck = false;

      table.registerSymbol(ParserTable::nsEps, ConstIdentifier("eps"));

      while (true) {
         info = source.read(token, IDENTIFIER_LEN);

         if (info.state == dfaEOF) break;

         if (ConstIdentifier::compare(token, "__define")) {
            source.read(token, IDENTIFIER_LEN);

            wchar16_t number[10];
            source.read(number, 10);

            registerSymbol(table, token, StringHelper::strToInt(number));
         }
         else if (ConstIdentifier::compare(token, "->") && !arrayCheck) {
            if (rule_len > 2) {
               table.registerRule(rule[0], rule + 1, rule_len - 2);

               rule[0] = rule[rule_len - 1];
               rule_len = 1;
            }
            arrayCheck = true;
         }
         else if (ConstIdentifier::compare(token, "|") && rule_len != 1) {
            arrayCheck = false;
            table.registerRule(rule[0], rule + 1, rule_len - 1);

            rule_len = 1;
         }
         else {
            arrayCheck = false;
            rule[rule_len++] = registerSymbol(table, token, last_id + 1);
            if (ConstIdentifier::compare(token, "|"))
               source.read(token, IDENTIFIER_LEN);
         }
      }
      table.registerRule(rule[0], rule + 1, rule_len - 1);

      printLine("generating...\n");

      int ambigous = table.generate();
      if (ambigous) {
         printLine("error:ambigous rule %s\n", table.retrieveSymbol(ambigous));
         return -1;
      }

      printLine("saving...\n");

      path.changeExtension(ConstIdentifier("dat"));

      FileWriter file(path, feRaw, false);
      table.save(&file);
   }
   catch(_ELENA_::InvalidChar& e) {
      printLine("(%d:%d): Invalid char %c\n", e.row, e.column, e.ch);
   }
   return 0;
}

