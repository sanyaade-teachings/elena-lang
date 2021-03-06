//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Common Library
//
//		This file contains ELENA Engine File class implementations.
//
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#include "common.h"
// -------------------------------------------------------
#include "files.h"

#ifdef _WIN32

#include <windows.h>
#include <direct.h>
#include <io.h>

#else

#include <unistd.h>
#include <sys/stat.h>

#endif

#define TEMP_SIZE 0x100

using namespace _ELENA_;

// --- Path ---

#ifdef _WIN32

inline int checkDir(const wchar_t* name, int mode)
{
   return _waccess(name, mode);
}

inline void createDir(const wchar_t* path)
{
   _wmkdir(path);
}

bool Path :: isRelative(const wchar_t* path, size_t length)
{
   if (path[0]!='/') {
      for (size_t i = 0 ; i < length - 1; i++) {
         if (path[i] == ':' && path[i + 1] == '/') {
            return false;
         }
      }
      return true;
   }
   else return false;
}

bool Path :: isRelative(const char* path, size_t length)
{
   if (path[0]!='/') {
      for (size_t i = 0 ; i < length - 1; i++) {
         if (path[i] == ':' && path[i + 1] == '/') {
            return false;
         }
      }
      return true;
   }
   else return false;
}

bool Path :: create(const wchar_t* root, const wchar_t* path)
{
   Path dirPath;
   dirPath.copyPath(path);

   if (checkDir(dirPath, 0)!=0) {
      if (!emptystr(dirPath) && !dirPath.compare(root)) {
         create(root, dirPath);
      }
      createDir(dirPath);

      return true;
   }
   else return false;
}

#else

bool Path :: isRelative(const char* path, size_t length)
{
   // !! temporal
   if (path[0]!='/') {
      return true;
   }
   else return false;
}

bool Path :: isRelative(const wchar16_t* path, size_t length)
{
   // !! temporal
   if (path[0]!='/') {
      return true;
   }
   else return false;
}

inline int checkDir(const char* name, int mode)
{
   return access(name, mode);
}

inline void createDir(const char* path)
{
   mkdir(path, S_IRWXO | S_IRWXG);
}

bool Path :: create(const char* root, const char* path)
{
   Path dirPath;
   dirPath.copyPath(path);

   if (checkDir(dirPath, 0)!=0) {
      if (!emptystr(dirPath) && !dirPath.compare(root)) {
         create(root, dirPath);
      }
      createDir(dirPath);

      return true;
   }
   else return false;
}

#endif

// --- File ---

File :: File(const char* path, const char* mode, int encoding, bool withBOM)
{
   _file = fopen(path, mode);

   if (isOpened()) {
      _encoding = encoding;
      if (withBOM) {
         detectEncoding();
      }
   }
}

#ifdef _WIN32

File :: File(const wchar_t* path, const wchar_t* mode, int encoding, bool withBOM)
{
   _file = _wfopen(path, mode);

   if (isOpened()) {
      _encoding = encoding;
      if (withBOM) {
         detectEncoding();
      }
   }
}

bool File :: readLine(wchar_t* s, size_t length)
{
   if (_encoding==feUTF16 || _encoding==feRaw) {
      return (fgetws(s, length, _file) != NULL);
   }
   else if (_encoding==feUTF8) {
      char temp[TEMP_SIZE];
      size_t count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         if (fgets(temp, count, _file) == NULL)
            return false;

         count = strlen(temp);
         if (count < (TEMP_SIZE - 1)) {
            StringHelper::copy(s, temp, count);
            s[count] = 0;
            break;
         }
         else StringHelper::copy(s, temp, count);

         length -= count;
         s += count;
      }
      return true;
   }
   else {
      char temp[TEMP_SIZE];
      size_t count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         if (fgets(temp, count, _file) == NULL)
            return false;

         count = strlen(temp);

         MultiByteToWideChar(_encoding, MB_PRECOMPOSED, temp, count, s, length);

         if (count < (TEMP_SIZE - 1)) {
            s[count] = 0;
            break;
         }
         length -= count;
         s += count;
      }
      return true;
   }
}

bool File :: readLiteral(wchar16_t* s, size_t length, size_t& wasread)
{
   if (_encoding==feUTF16 || _encoding==feRaw) {
      wasread = fread((char*)s, 2, length, _file);
      return (wasread > 0);
   }
   else if (_encoding==feUTF8) {
      wasread = 0;

      char temp[0x100];
      size_t count;
      while (length > 0) {
         count = (length > 0x100) ? 0x100 : length;

         fread(temp, 1, count, _file);

         length -= count;
         StringHelper::copy(s, temp, count);
         wasread += count;
         s += count;
      }
      return (wasread > 0);
   }
   else {
      wasread = 0;

      char temp[0x100];
      int count;
      while (length > 0) {
         count = (length > 0x100) ? 0x100 : length;

         wasread += fread(temp, 1, count, _file);

         MultiByteToWideChar(_encoding, MB_PRECOMPOSED, temp, count, s, length);

         length -= count;
         s += count;
      }
      return (wasread > 0);
   }
}

bool File :: readLiteral(char* s, size_t length, size_t& wasread)
{
   if (_encoding >= feUTF8) {
      wasread = fread((char*)s, 1, length, _file);
      return (wasread > 0);
   }
   else return 0; // !! temporal
}

bool File :: writeLiteral(const wchar16_t* s, size_t length)
{
   if (_encoding==feUTF16 || _encoding==feRaw) {
      return (fwrite((const char*)s, 2, length, _file) == length);
   }
   else if (_encoding==feUTF8) {
      char temp[TEMP_SIZE * 4];
      size_t count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         size_t utfCount = count;
         StringHelper::copy(temp, s, utfCount);

         if (fwrite(temp, 1, utfCount, _file) <= 0)
            return false;

         length -= count;
         s += count;
      }
      return true;
   }
   else {
      char temp[TEMP_SIZE];
      int count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         BOOL withError = 0;
         WideCharToMultiByte(_encoding, WC_COMPOSITECHECK, s, count, temp, count, "?", &withError);

         if (fwrite(temp, 1, count, _file) <= 0)
            return false;

         length -= count;
         s += count;
      }
      return true;
   }
}

bool File :: writeLiteral(const char* s, size_t length)
{
   if (_encoding >= feUTF8) {
      return (fwrite(s, 1, length, _file) == length);
   }
   else {
      //char temp[TEMP_SIZE];
      //int count;
      //while (length > 0) {
      //   count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

      //   BOOL withError = 0;
      //   WideCharToMultiByte(_encoding, WC_COMPOSITECHECK, s, count, temp, count, "?", &withError);

      //   if (fwrite(temp, 1, count, _file) <= 0)
      //      return false;

      //   length -= count;
      //   s += count;
      //}
      //return true;

      return false; // !! temporal
   }
}

bool File :: readLine(char* s, size_t length)
{
   if (_encoding >= feUTF8) {
      return (fgets(s, length, _file) != NULL);
   }
   else return false; // !! temporal
}

bool File :: writeNewLine()
{
   return writeLiteral("\r\n", 2);
}

#else

bool File :: readLine(char* s, size_t length)
{
   if (_encoding==feUTF8 || _encoding==feRaw) {
      return (fgets(s, length, _file) != NULL);
   }
   else return false; // !! temporal
}

bool File :: readLine(unsigned short* s, size_t length)
{
   if (_encoding==feUTF16 || _encoding==feRaw) {
      return (fgets((char*)s, length << 1, _file) != NULL);
   }
   else {
      char temp[TEMP_SIZE];
      size_t count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         if (fgets(temp, count, _file) == NULL)
            return false;

         count = strlen(temp);

         StringHelper::copy(s, temp, count);

         if (count < (TEMP_SIZE - 1)) {
            s[count] = 0;
            break;
         }
         length -= count;
         s += count;
      }
      return true;
   }
}

bool File :: writeLiteral(const char* s, size_t length)
{
   if (_encoding >= feRaw) {
      return (fwrite(s, 1, length, _file) == length);
   }
   else {
      // !! temporal
      //char temp[TEMP_SIZE];
      //int count;
      //while (length > 0) {
      //   count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

      //   BOOL withError = 0;
      //   WideCharToMultiByte(_encoding, WC_COMPOSITECHECK, s, count, temp, count, "?", &withError);

      //   if (fwrite(temp, 1, count, _file) <= 0)
      //      return false;

      //   length -= count;
      //   s += count;
      //}
      //return true;

      return false; // !! temporal
   }
}

bool File :: writeLiteral(const unsigned short* s, size_t length)
{
   if (_encoding==feUTF16 || _encoding==feRaw) {
      return (fwrite((const char*)s, 2, length, _file) == length);
   }
   else {
      char temp[TEMP_SIZE];
      int count;
      while (length > 0) {
         count = (length > TEMP_SIZE) ? TEMP_SIZE : length;

         StringHelper::copy(temp, s, count);

         if (fwrite(temp, 1, count, _file) <= 0)
            return false;

         length -= count;
         s += count;
      }
      return true;
   }
}

bool File :: writeNewLine()
{
   return writeLiteral("\n", 1);
}

bool File :: readLiteral(char* s, size_t length, size_t& wasread)
{
   if (_encoding >= feUTF8) {
      wasread = fread((char*)s, 1, length, _file);
      return (wasread > 0);
   }
   else return 0; // !! temporal
}

#endif

void File :: detectEncoding()
{
   unsigned short signature = 0;
   fread(&signature, 1, 2, _file);
   if (signature==0xFEFF) {
      _encoding = feUTF16;
   }
   else if (signature==0xBBEF) {
      unsigned char ch;
      fread(&ch, 1, 1, _file);
      if (ch==0xBF) {
         _encoding = feUTF8;
      }
      else rewind();
   }
   else rewind();
}

File :: ~File()
{
   if (_file != NULL) {
      fclose(_file);
      _file = NULL;
   }
}

long File :: Position() const
{
   return ftell(_file);
}

long File :: Length()
{
   long position = ftell(_file);
   fseek(_file, 0, SEEK_END);

   long length = ftell(_file);
   fseek(_file, position, SEEK_SET);

   return length;
}

bool File :: Eof()
{
   if (_file) {
      int pos = ftell(_file);
      fgetc(_file);
      bool eof = (feof(_file)!=0);
      fseek(_file, pos, SEEK_SET);

      return eof;
   }
   else return true;
}

bool File :: seek(long position)
{
   return fseek(_file, position, SEEK_SET) == 0;
}

bool File :: read(void* s, size_t length)
{
   return (fread(s, 1, length, _file) > 0);
}

bool File :: write(const void* s, size_t length)
{
   return (fwrite((const char*)s, 1, length, _file) > 0);
}

void File :: rewind()
{
   ::rewind(_file);
}

// --- TextFileReader ---

TextFileReader :: TextFileReader(const _path_t* path, int encoding, bool withBOM)
   : _file(path, _T("rb"), encoding, withBOM)
{
}

bool TextFileReader :: read(wchar16_t* s, size_t length)
{
   return _file.readLine(s, length);
}

bool TextFileReader :: read(char* s, size_t length)
{
   return _file.readLine(s, length);
}

// --- FileReader ---

FileReader :: FileReader(const _path_t* path, int encoding, bool withBOM)
   : _file(path, _T("rb+"), encoding, withBOM)
{
}

FileReader :: FileReader(const _path_t* path, const _path_t* mode, int encoding, bool withBOM)
   : _file(path, mode, encoding, withBOM)
{
}

bool FileReader :: read(void* s, size_t length)
{
   return _file.read(s, length);
}

// --- FileWriter ---

FileWriter :: FileWriter(const _path_t* path, int encoding, bool withBOM)
   : _file(path, _T("wb+"), encoding, withBOM)
{
   if (encoding == feUTF16 && isOpened()) {
      unsigned short signature = 0xFEFF;
      _file.write((void*)&signature, 2);
   }
}

bool FileWriter :: write(const void* s, size_t length)
{
   return _file.write(s, length);
}

void FileWriter :: align(int alignment)
{
   int len = ::align(_file.Position(), alignment) - _file.Position();

   writeBytes('\0', len);
}

// --- TextFileWriter ---

TextFileWriter :: TextFileWriter(const _path_t* path, int encoding, bool withBOM)
   : _file(path, _T("wb+"), encoding, withBOM)
{
   if (withBOM) {
      if (encoding == feUTF16 && isOpened()) {
         unsigned short signature = 0xFEFF;
         _file.write((void*)&signature, 2);
      }
      else if (encoding == feUTF8 && isOpened()) {
         int signature = 0xBFBBEF;
         _file.write((void*)&signature, 3);
      }
   }
}

bool TextFileWriter :: write(const wchar16_t* s, size_t length)
{
   return _file.writeLiteral(s, length);
}

bool TextFileWriter :: write(const char* s, size_t length)
{
   return _file.writeLiteral(s, length);
}

bool TextFileWriter :: writeNewLine()
{
   return _file.writeNewLine();
}
