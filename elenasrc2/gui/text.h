//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//      Text class header
//                                              (C)2005-2013, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef textH
#define textH

#include "guicommon.h"

#define PAGE_SIZE	0x100

#define BM_INVALID  (size_t)-1
#define BM_VALID    (size_t)0
#define BM_EOP      (size_t)1    // end of page
#define BM_EOT      (size_t)2    // end of text

namespace _GUI_
{
// --- forwards ---
class Text;

// --- TextWatcher ---

class _TextWatcher
{
public:
   virtual void onUpdate(size_t position) = 0;
   virtual void onInsert(size_t position, size_t length, const _text_t* line) = 0;
   virtual void onErase(size_t position, size_t length, const _text_t* line) = 0;
};

typedef _ELENA_::List<_TextWatcher*> TextWatchers;

// --- Page ---

struct Page
{
   size_t  used;
   size_t  rows;
   _text_t text[PAGE_SIZE];

   Page()
   {
      used = 0;
      rows = 0;
   }
   Page(int size)
   {
      rows = 0;
      used = size;
   }
   Page(const Page& page)
   {
      rows = page.rows;
      used = page.used;
      _ELENA_::StringHelper::copy(text, page.text, used);
   }
};

typedef _ELENA_::BList<Page>  Pages;

// --- TextBookmark ---

struct TextBookmark
{
   friend class Text;
   friend struct TextScanner;

private:
   _text_t        _LF, _CF;

   // bookmark text position
   size_t         _column;
   size_t         _virtual_column;
   size_t         _row;
   size_t         _length;

   // bookmark stream position
   int             _status;
   size_t          _position;
   int             _offset;
   Pages::Iterator _page;

   void set(Pages* pages);

   void refreshStatus()
   {
      if ((int)(*_page).used == _offset) {
         _status = _page.Last() ? BM_EOT : BM_EOP;
      }
      else _status = BM_VALID;
   }

   bool goToPreviousPage(bool allowEmpty = false); // do not trace caret coordinate
   bool goToNextPage(bool allowEmpty = false);
//   bool skipEmptyPages(bool onlyForward = false);

   void normalize();                                 // make sure _offset is within page text range

   bool go(int disp, bool allowEmpty = false);       // moving without tracing caret coordinate
   bool move(int disp);                              // moving with tracing caret coordinate

   void moveToStart();
   void moveToClosestRow(size_t row);
   void moveToClosestColumn(size_t column);
   bool moveToClosestPosition(size_t position);

   bool moveToPrevBOL();
   bool moveToNextBOL();

   size_t seekEOL();

   bool nextChar(int& disp)
   {
#ifdef _UTF8
      if (_ELENA_::test((*_page).text[_offset], 0x80)) {
         if ((*_page).text[_offset] >= 0xF0) {
            go(3);
            disp += 2;
         }
         else if ((*_page).text[_offset] >= 0xE0) {
            go(2);
            disp += 2;
         }
         else {
            go(1);
            disp++;
         }
      }
      return go(1);
#else
      return go(1);
#endif
   }

   bool prevChar(int& disp)
   {
#ifdef _UTF8
      if (_status == 0 && _ELENA_::test((*_page).text[_offset], 0x80)) {
         while (!_ELENA_::test((*_page).text[_offset], 0xC0)) {
            if (!go(-1))
               return false;

            disp++;
         }
      }
      return go(-1);
#else
      return go(-1);
#endif
   }

public:
   TextBookmark& operator =(const TextBookmark& bookmark)
   {
      this->_status = bookmark._status;
      this->_column = bookmark._column;
      this->_virtual_column = bookmark._virtual_column;
      this->_row = bookmark._row;
      this->_position = bookmark._position;
      this->_page = bookmark._page;
      this->_offset = bookmark._offset;

      this->_length = bookmark._length;
      this->_LF = bookmark._LF;
      this->_CF = bookmark._CF;

      return *this;
   }

   bool isValid() const { return (size_t)_status != BM_INVALID; }

   bool isEOF() const { return _page.Last() && _offset>=(int)(*_page).used; }
   bool isEOL() { return (getLength() <= _column); }

   Point getCaret(bool _virtual = true) const
   {
      return Point(_virtual ? _virtual_column : _column, _row);
   }
   size_t getRow() const { return _row; }
   size_t getColumn(bool _virtual = true) const { return _virtual ? _virtual_column : _column; }

   size_t getLength();

   size_t getPosition() const { return _position + _offset; }

   int getVirtualDiff() const { return _column - _virtual_column; }

   bool moveTo(size_t column, size_t row);
   bool moveOn(int disp);

   void invalidate()
   {
      _status = BM_INVALID;
      _length = BM_INVALID;
   }

   TextBookmark();
   ~TextBookmark();
};

// --- TextScanner ---

struct TextScanner
{
private:
   Text*        _text;
   TextBookmark _bookmark;

public:
   size_t getPosition() const { return _bookmark.getPosition(); }

   const _text_t* getLine(size_t& length);

   bool goTo(int disp)
   {
      return _bookmark.go(disp);
   }

   TextScanner(Text* text);
};

// --- Text ---

class Text
{
   _text_t      _LF, _CF;

   Pages        _pages;
   size_t       _rowCount;

   TextWatchers _watchers;

   int retrieveRowCount();

   void refreshPage(Pages::Iterator page);
   void refreshNextPage(Pages::Iterator page);

   void insert(TextBookmark bookmark, const _text_t* s, size_t length, bool checkRowCount);
   void erase(TextBookmark bookmark, size_t length, bool checkRowCount);

public:
   static int TabSize;

   size_t getRowCount() const { return _rowCount; }
   size_t getRowLength(size_t row);

   void validateBookmark(TextBookmark& bookmark);

   void create();
   bool load(const _path_t* path, int encoding, bool autoDetecting);
   void save(const _path_t* path, int encoding);

   void copyLineTo(TextBookmark& bookmark, _ELENA_::TextWriter& writer, size_t length, bool stopOnEOL);
   void copyTo(TextBookmark bookmark, _text_t* buffer, int length);

   const _text_t* getLine(TextBookmark& bookmark, size_t& length);
   _text_t getChar(TextBookmark& bookmark);

   bool insertChar(TextBookmark& bookmark, _text_t ch);
   bool insertLine(TextBookmark& bookmark, const _text_t* s, size_t length);
   bool insertNewLine(TextBookmark& bookmark);

   bool eraseChar(TextBookmark& bookmark);
   bool eraseLine(TextBookmark& bookmark, size_t length);

   bool compare(TextBookmark bookmark, const _text_t* line, int len, bool matchCase, const _text_t* terminators);
   bool findWord(TextBookmark& bookmark, const _text_t* text, bool matchCase, const _text_t* terminators);

   void attachWatcher(_TextWatcher* watcher);
   void detachWatcher(_TextWatcher* watcher);

   Text();
   virtual ~Text();
};

// --- TextHistory ---

class TextHistory : public _TextWatcher
{
public:
   typedef _ELENA_::MemoryDump Buffer;

   enum Operation { opNone, opInsert, opErase };

private:
   bool     _locking;

   Buffer   _buffer1;
   Buffer   _buffer2;

   size_t    _offset;
   Buffer*   _buffer;
   Buffer*   _previous;

   Operation   _lastOperation;
   size_t      _lastLength;
   size_t      _lastPosition;

   void addRecord(Operation operation, size_t position, size_t length, void* line);

   void switchBuffer();

   void endRecord();

public:
   bool Eof() const;
   bool Bof() const;

   virtual void onUpdate(size_t position)
   {
   }

   virtual void onInsert(size_t position, size_t length, const _text_t* line);
   virtual void onErase(size_t position, size_t length, const _text_t* line);

   void undo(Text* text, TextBookmark& caret);
   void redo(Text* text, TextBookmark& caret);

   TextHistory(int capacity); // at least 20
   virtual ~TextHistory() { /*_ELENA_::freestr(_buffer1); _ELENA_::freestr(_buffer2);*/ }
};

} // _GUI_

#endif // textH
