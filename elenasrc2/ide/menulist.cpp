//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//      MenuList class implementation
//                                              (C)2005-2011, by Alexei Rakov
//---------------------------------------------------------------------------

#include "menulist.h"

using namespace _GUI_;
using namespace _ELENA_;

// --- MenuHistoryList

MenuHistoryList :: MenuHistoryList(int maxCount, int menuBaseId, bool withSeparator)
   : _list(NULL, freestr)
{
   _maxCount = maxCount;
   _menuBaseId = menuBaseId;
   _menu = NULL;
   _menuSize = 0;
   _withSeparator = withSeparator;
}

int MenuHistoryList :: getIndex(const _text_t* item)
{
   int index = 0;
   List<_text_t*>::Iterator it = _list.start();
   while (!it.Eof()) {
      if (StringHelper::compare(item, *it)) {
         return index;
      }
      index++;
      it++;
   }

   return -1;
}

bool MenuHistoryList :: erase(const _text_t* item)
{
   List<_text_t*>::Iterator it = _list.start();
   while (!it.Eof()) {
      if (StringHelper::compare(item, *it)) {
         _list.cut(it);

         return true;
      }
      it++;
   }

   return false;
}

void MenuHistoryList :: eraseLast()
{
   _list.cut(_list.end());
}

const _text_t* MenuHistoryList :: get(int id)
{
   _ELENA_::List<_text_t*>::Iterator it = _list.get(id - _menuBaseId - 1);

   return !it.Eof() ? *it : NULL;
}

void MenuHistoryList :: add(const _text_t* item)
{
   _text_t* itemCopy = StringHelper::clone(item);

   erase(item);

   _list.insert(itemCopy);

   if (_list.Count() >= _maxCount)
      eraseLast();

   refresh();
}

void MenuHistoryList :: clear()
{
   #ifdef _WIN32
   _list.clear();

   refresh();
   if (_withSeparator) {
      _menu->eraseItemById(_menuBaseId);
      _menu->enableItemById(_menuBaseId + _maxCount + 1, false);
   }
   #endif
}

void MenuHistoryList :: refresh()
{
   #ifdef _WIN32
   if (_menuSize == 0 && _list.Count() > 0 && _withSeparator) {
      // insert separator before clear command
      _menu->insertSeparatorById(_menuBaseId + _maxCount + 1, _menuBaseId);
      _menu->enableItemById(_menuBaseId + _maxCount + 1, true);
   }

   while (_list.Count() > _menuSize) {
      _menuSize++;
      _menu->insertItemById(_menuBaseId, _menuBaseId + _menuSize, _T("none"));
   }

   while (_list.Count() < _menuSize) {
      _menu->eraseItemById(_menuBaseId + _menuSize);
      _menuSize--;
   }

   Path caption;
   int index = 1;
   List<_text_t*>::Iterator it = _list.start();
   while (!it.Eof()) {
      caption.clear();

      caption.append(_menu->getMnemonicAccKey());
      caption.appendInt(index);
      caption.append(_T(": "));
      caption.append(*it);

      _menu->renameItemById(index + _menuBaseId, caption);

      it++;
      index++;
   }
#endif
}
