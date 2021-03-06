//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     GTK+ TabBar Header File
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef gtktabbarH
#define gtktabbarH

#include "gtkcommon.h"

namespace _GUI_
{

// --- TabBar ---

class TabBar : public Gtk::Notebook
{
protected:
   struct TabPage
   {
      Gtk::Label   label;

      Gtk::Widget* control;

      TabPage(const _text_t* caption)
         : label(caption)
      {
         control = NULL;
      }
   };

   typedef _ELENA_::Map<const _text_t*, TabPage*> TabPages;

   Gtk::Widget* _current;
   TabPages     _tabs;

public:
   int getCount();
   int getCurrentIndex()
   {
      return get_current_page();
   }

   Gtk::Widget* _getTabControl(int index) const;
//   const TCHAR* getTabName(int index);
   int getTabIndex(const _text_t* name);
//   void eraseTab(int index);

   void addTab(const _text_t* name, Gtk::Widget* control);
//   void deleteTab(int index);
   void selectTab(int index);

   virtual void setFocus();

//   virtual void onTabChange(int index);
//
//   void renameTab(int index, const TCHAR* newName);
//   void renameTabCaption(int index, const TCHAR* newName);

   TabBar();
};

//
//// --- TabBarPlus ---
//
//class TabBarPlus : public TabBar
//{
////   void*      _extraparam;
////   _TextView* _current;
////
////   _TextView*(* _newTextView)(_Document*, void*);
////
////   virtual void addTab(const TCHAR* name, void* param);
//
//public:
////   _TextView* getTextView()
////   {
////      if (!_current)
////         _current = (_TextView*)getTabInfo(getCurrentIndex());
////
////      return _current;
////   }
////
////   _Document* getDocument()
////   {
////      if (!_current)
////         _current = (_TextView*)getTabInfo(getCurrentIndex());
////
////      return _current ? _current->getDocument() : NULL;
////   }
////
////   void activate();
////
////   virtual void addDocument(const TCHAR* name, _Document* doc);
////   virtual void closeDocument(int index = -1);
////
////   void markDocument(int index, bool changed);
////
////   void refreshView()
////   {
////      if (!_current)
////         _current = (_TextView*)getTabInfo(getCurrentIndex());
////
////      _current->refreshView();
////   }
////
////   virtual void onTabChange(int index);
//
//   TabBarPlus(/*_TextView*(* newTextView)(_Document*, void*), void* extraparam*/);
//};

} // _GUI_

#endif // gtktabbarH
