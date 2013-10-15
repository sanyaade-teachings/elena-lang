//---------------------------------------------------------------------------
//              E L E N A   p r o j e c t
//                Command line DSA script terminal main file
//                                              (C)2011-2013, by Alexei Rakov
//---------------------------------------------------------------------------

#include "elena.h"
//---------------------------------------------------------------------------
//#include "config.h"
#include "elenavm.h"
#include "elenasm.h"

using namespace _ELENA_;

#define MAX_LINE     256

// global variables
bool  _running = true;
//int   _encoding = feAnsi;
//bool  _tracing = false;

// terminal session
String<wchar16_t, 1024> _script;

//MessageMap _verbs;
//
//// --- commands ---
//
//void printMessage(int messageId, bool withRole)
//{
//   const wchar16_t* message = retrieveKey(_verbs.start(), messageId, (const wchar_t*)NULL);
//
//   if (withRole) {
//      wprintf(_T("@send role.%s\n"), message);
//   }
//   else wprintf(_T("@send %s\n"), message);
//}
//
////void printMerge(const wchar16_t* vmt)
////{
////   if (ConstIdentifier::compare(vmt, GROUP_CLASS)) {
////      wprintf(_T("@group-merge\n"));
////   }
////   else {
////      wprintf(_T("@merge\n"));
////   }
////}
//
//void printTape(void* tape)
//{
//   size_t base = (size_t)tape;
//
//   ByteArray dump(tape, -1);
//   MemoryReader reader(&dump);
//
//   size_t command = reader.getDWord();
//   const wchar16_t* prefix = NULL;
//   bool withRole = false;
//   while (command != 0) {
//      size_t param = reader.getDWord();
//      reader.seek(reader.getDWord());
//
//      switch(command) {
//         //case COPY_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@copy sp[%d] \n"), param);
//         //   break;
//         //case GET_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@copy fp[%d] \n"), param);
//         //   break;
//         //case CALL_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@call %s\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case VERB_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@verb %s\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case SIGN_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@sign %s\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PARAM_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@param %d\n"), param);
//         //   break;
//         ////case PUSH_TAPE_MESSAGE_ID:
//         ////   wprintf(_T("@push %s\n"), (const wchar16_t*)(base + param));
//         ////   break;
//         //case PUSHS_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@push \"%s\"\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PUSHN_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@push %s\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PUSHR_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@push %sr\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PUSHL_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@push %sl\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PUSHM_TAPE_MESSAGE_ID:
//         //case PUSHV_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@push %% %s\n"), (const wchar16_t*)(base + param));
//         //   break;
//         //case PREFIX_TAPE_MESSAGE_ID:
//         //   prefix = (const wchar16_t*)(base + param);
//         //   break;
//         //case POP_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@pop %d\n"), param);
//         //   break;
//         //case PUSH_EMPTY_MESSAGE_ID:
//         //   wprintf(_T("@push <empty>\n"));
//         //   break;
//         //case NEW_TAPE_MESSAGE_ID:
//         //   wprintf(_T("@new[%d] %s\n"), param, prefix);
//         //   prefix = NULL;
//         //   break;
//         //case NEW_ARG_MESSAGE_ID:
//         //   wprintf(_T("@new[] %s\n"), prefix);
//         //   break;
//         //case GROUP_TAPE_MESSAGE_ID:
//         //   printMerge((const wchar16_t*)(base + param));
//         //   break;
//         case USE_VM_MESSAGE_ID:
//            wprintf(_T("@use %s\n"), (const wchar16_t*)(base + param));
//            break;
//         case MAP_VM_MESSAGE_ID:
//            wprintf(_T("@map %s\n"), (const wchar16_t*)(base + param));
//            break;
//         case LOAD_VM_MESSAGE_ID:
//            wprintf(_T("@config %s\n"), (const wchar16_t*)(base + param));
//            break;
//         case START_VM_MESSAGE_ID:
//            wprintf(_T("@start\n"));
//            break;
//         //case POP_ROLE_MESSAGE_ID:
//         //   wprintf(_T("@pop role\n"));
//         //   withRole = true;
//         //   break;
//         default:
//            printMessage(command, withRole);
//            withRole = false;
//            break;
//      }
//      command = reader.getDWord();
//   }
//}
//
////void split(const TCHAR* p1, const TCHAR* &p2, int& p1_len)
////{
////   p1_len = 0;
////
////   while(p1[p1_len] != _T(' ') && p1_len < getlength(p1))
////      p1_len++;
////
////   // argument
////   p2 = p1 + p1_len;
////
////   while (p2[0]==' ')
////      p2++;
////}

const wchar16_t* trim(const wchar16_t* s)
{
   while(s[0]==0x20)s++;

   return s;
}

void printHelp()
{
   printf("-q                   - quit\n");
   printf("-h                   - help\n");
//   printf("-ton                 - trace mode is on\n");
//   printf("-toff                - trace mode is off\n");
//   printf("-n<name>             - set grammar parse mode\n");
//   printf("-Ninline<script>     - execute an inline script\n");
//   printf("-mcf                 - set CF parser mode\n");
//   printf("-moff                - set script mode\n");
//   printf("-l [name=]<path>             - load a script from file\n");
   printf("-i <script>            - execute inline script\n");
   printf("-lc [name=]<path>      - generate CF parser from from file\n");
   printf("<script>               - execute script\n");
//   printf("-Lxc<path>          - generate character-oriented CF parser from from file\n");
//   printf("-lr<path>         - generate LALR parser from from file\n");
//   printf("-lxr<path>         - generate LALR parser from from file\n");
//   printf("-Linline<path>       - load an inline script from file\n");
}

void executeScript(const wchar16_t* ruleSetName, const wchar16_t* script, int mode)
{
   void* tape = TranslateLVMTape(ruleSetName, script, mode);
   if (tape == NULL || (size_t)tape == -1) {
      const wchar16_t* error = GetLSMStatus();
      if (!emptystr(error)) {
         wprintf(_T("\nFailed:%s"), error);
      }
      return;
   }
   /*if (!_tracing)*/else {
      if (InterpretLVM(tape) == 0)
         wprintf(_T("\nFailed:%s"), GetLVMStatus());
   }
//   else printTape(tape);

   FreeLVMTape(tape);
}

void newScriptLine( const wchar16_t* grammarName, const wchar16_t* line, int mode)
{
   _script.append(line);
   if (line[getlength(line) - 1]!='_') {
      executeScript(grammarName, _script, mode);
      _script.clear();
   }
   else _script.append(_T("\r\n"));
}

void loadScript(const wchar16_t* line)
{
   String<wchar16_t, MAX_LINE> grammarName("default");

   int mode = 0;
   if (line[0]=='c') {
      mode = CFGRAMMAR_MODE;

      line++;
   }
   line = trim(line);

   int nameIndex = StringHelper::find(line, '=');
   if (nameIndex >= 0) {
      grammarName.copy(line, nameIndex);

      line = trim(line + nameIndex + 1);
   }

   void* tape = TranslateLVMFile(grammarName, line, feAnsi, true, mode);

   if (tape != NULL && (size_t)tape != -1) {
      //if (_tracing) {
      //   printTape(tape);
      //}
      //else {
         if (InterpretLVM(tape) == 0)
            wprintf(_T("\nFailed:%s"), GetLVMStatus());
//      }

      FreeLVMTape(tape);
   }
   else {
      const wchar16_t* error = GetLSMStatus();
      if (!emptystr(error))
         wprintf(_T("\nFailed:%s"), error);
   }
}

bool executeCommand(const wchar16_t* line/*, int& mode*/)
{
   if (emptystr(line))
      return false;

   // check commands
   if(line[0] == 'q') {
      _running = false;
   }
   else if(line[0] == 'h') {
      printHelp();
   }
   else if(line[0] == 'l') {
      loadScript(line + 1);
   }
   else if(line[0] == 'i') {
      executeScript(ConstIdentifier("inline"), line + 1, 0);
   }
//   else if(ConstantIdentifier::compare(line, "ton")) {
//      _tracing = true;
//   }
//   else if(ConstantIdentifier::compare(line, "toff")) {
//      _tracing = false;
//   }
//   else if(ConstantIdentifier::compare(line, "moff")) {
//      _mode = 0;
//   }
//   else if(ConstantIdentifier::compare(line, "mcf")) {
//      _mode = CFDSARULE_MODE;
//   }
//   else if(ConstantIdentifier::compare(line, "mlalr")) {
//      _mode = LALRDSARULE_MODE;
//   }
//   else if(ConstantIdentifier::compare(line, "Lcf", 3)) {
//      loadScript(line + 3, CFDSARULE_MODE, _grammarName);
//   }
//   else if(ConstantIdentifier::compare(line, "Lxcf", 4)) {
//      loadScript(line + 4, CFDSARULE_MODE | SYMBOLIC_MODE, _grammarName);
//   }
//   else if(ConstantIdentifier::compare(line, "Llalr", 5)) {
//      loadScript(line + 5, LALRDSARULE_MODE, _grammarName);
//   }
//   else if(ConstantIdentifier::compare(line, "Linline", 7)) {
//      loadScript(line + 7, 0, _T("inline"));
//   }
//   else if(ConstantIdentifier::compare(line, "Ninline", 7)) {
//      executeScript(_T("inline"), line + 7, 0);
//   }
//   else if (line[0]=='n') {
//      _grammarName.copy(line + 1);
//   }
//   else if (line[0] == 'l') {
//      loadScript(line + 1, _mode, _grammarName);
//   }
   else return false;

   return true;
}

void runSession()
{
   char                      buffer[MAX_LINE];
   String<wchar_t, MAX_LINE> line;
//   //int                      mode = 0;

   do {
      try {
         printf("\n>");

         // !! fgets is used instead of fgetws, because there is strange bug in fgetws implementation
         fgets(buffer, MAX_LINE, stdin);
         line.copy(buffer, strlen(buffer));

         while (!emptystr(line) && line[getlength(line) - 1]=='\r' || line[getlength(line) - 1]=='\n')
            line[getlength(line) - 1] = 0;

         while (!emptystr(line) && line[getlength(line) - 1]==' ')
            line[getlength(line) - 1] = 0;

         if (line[0]=='-') {
            if(!executeCommand(line + 1/*, mode*/))
               printHelp();
         }
         else if (!emptystr(line)){
            newScriptLine(ConstantIdentifier("default"), line, 0);
         }
         else printHelp();
      }
      catch(...) {
         printf("Invalid operation");
      }
   }
   while(_running);
}

int main(int argc, char* argv[])
{
   printf("ELENA command line VM terminal %d.%d.0 (C)2011-2013 by Alexei Rakov\n", ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION);

//   _grammarName.copy(_T("inline"));
//
//   // load verbs
//   loadVerbs(_verbs);

   // load script passed via command line arguments
   if (argc > 1) {
      for (int i = 1 ; i < argc ; i++) {
         if (argv[i][0] == '-') {
            // check exit command
            if (argv[i][1] == 'q')
               return 0;

            String<wchar_t, 260> param;
            param.copy(argv[i]);

            executeCommand(param + 1);
         }
      }
   }

   runSession();
}