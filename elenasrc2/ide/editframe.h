//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     EditFrame container File
//                                              (C)2005-2013, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef editframeH
#define editframeH

#include "idecommon.h"
#include "document.h"
#include "settings.h"

namespace _GUI_
{

// --- _EditFrame ---

class _EditFrame
{
protected:
   typedef _ELENA_::List<Document*> Documents;
   typedef _ELENA_::Map<const _path_t*, int> DocMapping;

   DocMapping _mappings;
   Documents  _documents;
   Document*  _currentDoc;

   virtual int getDocumentTabCount() = 0;
   virtual void selectDocumentTab(int index) = 0;
   virtual int addDocumentTab(const _path_t* path, Document* doc) = 0;
   virtual void eraseDocumentTab(int index) = 0;
   virtual void renameDocumentTab(int index, const _text_t* newName) = 0;
   virtual void refreshDocument() = 0;

public:
   int getDocumentCount()
   {
      return _documents.Count();
   }

   virtual void showContextMenu(int x, int y) = 0;
   virtual void reloadSettings() = 0;

   void selectDocument(const _text_t* path);
   void selectDocument(int index);
   const wchar16_t* getDocumentPath(int index);
   int getDocumentIndex(const _path_t* path);

   Document* openDocument(const _text_t* path, Text* text, _GUI_::LexicalStyler* styler, int encoding);
   Document* getDocument(int index);
   void renameDocument(int index, const _path_t* path);

   void closeDocument(int index);

   bool isReadOnly() const
   {
      return (_currentDoc != NULL) && _currentDoc->status.readOnly;
   }

   void setReadOnlyMode(bool mode);

   void addDocumentMarker(int index, HighlightInfo info, int bandStyle, int style);
   void removeDocumentMarker(int index, int row, int bandStyle);
   void removeAllDocumentMarker(int bandStyle);

   bool isDocumentIncluded(int index = -1);
   bool isDocumentModified(int index = -1);
   bool isDocumentUnnamed(int index = -1);

   bool isAnyModified();

   void markDocumentAsIncluded(int index = -1);
   void markDocumentAsExcluded(int index = -1);

   void saveDocument(const _path_t* path, int index = -1);

   virtual int getCurrentDocumentIndex() = 0;

   void undo();
   void redo();
   void selectAll();
   void trim();
   void swap();
   void duplicateLine();
   void eraseLine();
   void commentBlock();
   void uncommentBlock();
   void toUppercase();
   void toLowercase();

   bool findText(SearchOption& option);
   bool replaceText(SearchOption& option);

   FrameState getState();

   _EditFrame()
      : _documents(NULL, _ELENA_::freeobj), _mappings(-1)
   {
   }
   virtual ~_EditFrame()
   {
   }
};

} // _GUI_

#endif // editframeH
