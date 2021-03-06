//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//    WinAPI: Static dialog implementations
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#include "windialogs.h"
#include "..\settings.h"
#include "elenaconst.h"

using namespace _GUI_;

// --- FileDialog ---

wchar_t* FileDialog :: ProjectFilter = _T("ELENA Project file\0*.prj\0All types\0*.*\0\0");
wchar_t* FileDialog :: SourceFilter = _T("ELENA source file\0*.l\0All types\0*.*\0\0");

typedef _ELENA_::String<_text_t, 255> ParamString;
typedef _ELENA_::String<char, 255> SettingString;

FileDialog :: FileDialog(Control* owner, const wchar_t* filter, const wchar_t* caption, const wchar_t* initialDir)
{
   _fileName[0] = '\0';

   ZeroMemory(&_struct, sizeof(_struct));
   _struct.lStructSize = sizeof(_struct);
   _struct.hwndOwner = owner->getHandle();
   _struct.hInstance = owner->_getInstance();
   _struct.lpstrCustomFilter = (LPTSTR) NULL;
   _struct.nMaxCustFilter = 0L;
   _struct.nFilterIndex = 1L;
   _struct.lpstrFilter = filter;
   _struct.lpstrFile = _fileName;
   _struct.nMaxFile = sizeof(_fileName);
   _struct.lpstrFileTitle = NULL;
   _struct.nMaxFileTitle = 0;
   _struct.lpstrInitialDir = _ELENA_::emptystr(initialDir) ? NULL : initialDir;
   _struct.lpstrTitle = caption;
   _struct.nFileOffset  = 0;
   _struct.nFileExtension = 0;
   _struct.lpstrDefExt = NULL;
   _struct.lCustData = 0;
   _struct.lpfnHook = NULL;
   _struct.lpTemplateName = NULL;

   _defaultFlags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_LONGNAMES | DS_CENTER | OFN_HIDEREADONLY;
}

bool FileDialog :: openFiles(_ELENA_::List<wchar_t*>& files)
{
   _struct.Flags = _defaultFlags | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

   files.clear();
   if (::GetOpenFileName(&_struct)) {
      if (_ELENA_::emptystr(_fileName + _ELENA_::getlength(_fileName) + 1)) {
         // !! always case insensitive?
         files.add(wcslwr(wcsdup(_fileName)));
      }
      else {
         _ELENA_::Path path;
         const wchar_t* p = _fileName + _ELENA_::getlength(_fileName) + 1;

         while (!_ELENA_::emptystr(p)) {
            path.copy(_fileName);
            path.combine(p);

            // !! always case insensitive?
            files.add(wcslwr(wcsdup(path)));

            p += _ELENA_::getlength(p) + 1;
         }
      }
      return true;
   }
   else return false;
}

const wchar_t* FileDialog :: openFile()
{
   _struct.Flags = _defaultFlags;

   if (::GetOpenFileName(&_struct)) {
      // !! always case insensitive?
      wcslwr(_fileName);

      return _fileName;
   }
   else return NULL;
}

bool FileDialog :: saveFile(const wchar_t* defaultExt, _ELENA_::Path& path)
{
   _struct.Flags = _defaultFlags | OFN_PATHMUSTEXIST;
   _struct.lpstrDefExt = defaultExt;

   if (::GetSaveFileName(&_struct)) {
      path.copy(_fileName);

      return true;
   }
   else return false;
}

// --- MessageBox ---

int MsgBox :: showError(const wchar_t* message, const wchar_t* param)
{
   return showError(NULL, message, param);
}

int MsgBox :: showError(HWND owner, const wchar_t* message, const wchar_t* param)
{
   _ELENA_::String<_text_t, 255> string(message);
   string.append(param);

   return show(owner, string, MB_OK | MB_ICONERROR);
}

int MsgBox :: showQuestion(HWND owner, const wchar_t* message)
{
   return show(owner, message, MB_YESNOCANCEL | MB_ICONQUESTION);
}

int MsgBox :: showQuestion(HWND owner, const wchar_t* message, const wchar_t* param)
{
   _ELENA_::String<_text_t, 255> string(message);
   string.append(param);

   return show(owner, string, MB_YESNOCANCEL | MB_ICONQUESTION);
}

int MsgBox :: showQuestion(HWND owner, const wchar_t* message, const wchar_t* param1, const wchar_t* param2)
{
   _ELENA_::String<_text_t, 255> string(message);
   string.append(param1);
   string.append(param2);

   return show(owner, string, MB_YESNOCANCEL | MB_ICONQUESTION);
}

int MsgBox :: show(HWND owner, const wchar_t* message, int type)
{
   return ::MessageBox(owner, message, APP_NAME, type);
}

// --- Dialogs ---

Dialog :: Dialog(Control* owner)
{
   _handle = NULL;
   _owner = owner;
}

int Dialog :: showModal()
{
   return ::DialogBoxParam(_owner->_getInstance(), MAKEINTRESOURCE(_getDialogID()),
	   _owner->getHandle(), (DLGPROC)DialogProc, (LPARAM)this);
}

void Dialog :: enable(int id, bool enabled)
{
   ::EnableWindow(::GetDlgItem(_handle, id), enabled? TRUE : FALSE);
}

void Dialog :: getText(int id, wchar_t** text, int length)
{
   ::SendDlgItemMessage(_handle, id, WM_GETTEXT, length, (LPARAM)text);
}

int Dialog :: getIntText(int id)
{
   wchar_t s[13];

   ::SendDlgItemMessage(_handle, id, WM_GETTEXT, 12, (LPARAM)s);

   return _ELENA_::StringHelper::strToInt(s);
}

bool Dialog :: getCheckState(int id)
{
   return _ELENA_::test(::SendDlgItemMessage(_handle, id, BM_GETCHECK, 0, 0), BST_CHECKED);
}

int Dialog :: getComboBoxIndex(int id)
{
   return ::SendDlgItemMessage(_handle, id, CB_GETCURSEL, 0, 0);
}

int Dialog :: getListCount(int id)
{
   return ::SendDlgItemMessage(_handle, id, LB_GETCOUNT, 0, 0);
}

void Dialog :: getListItem(int id, int index, wchar_t** text)
{
   ::SendDlgItemMessage(_handle, id, LB_GETTEXT, index, (LPARAM)text);
}

int Dialog :: getListIndex(int id)
{
   return ::SendDlgItemMessage(_handle, id, LB_GETCURSEL, 0, 0);
}

void Dialog :: setText(int id, const wchar_t* text)
{
   ::SendDlgItemMessage(_handle, id, WM_SETTEXT, 0, (LPARAM)text);
}

void Dialog :: setIntText(int id, int value)
{
   _ELENA_::String<_text_t, 15> s;
   s.appendInt(value);

   ::SendDlgItemMessage(_handle, id, WM_SETTEXT, 0, (LPARAM)((const _text_t*)s));
}

void Dialog :: setCheckState(int id, bool value)
{
   ::SendDlgItemMessage(_handle, id, BM_SETCHECK, value ? BST_CHECKED : BST_UNCHECKED, 0);
}

void Dialog :: setComboBoxIndex(int id, int index)
{
   ::SendDlgItemMessage(_handle, id, CB_SETCURSEL, index, 0);
}

void Dialog :: setListIndex(int id, int index)
{
   ::SendDlgItemMessage(_handle, id, LB_SETCURSEL, index, 0);
}

void Dialog :: setTextLimit(int id, int maxLength)
{
   ::SendDlgItemMessage(_handle, id, EM_SETLIMITTEXT, maxLength, 0);
}

void Dialog :: addComboBoxItem(int id, const wchar_t* text)
{
   ::SendDlgItemMessage(_handle, id, CB_ADDSTRING, 0, (LPARAM)text);
}

void Dialog :: addListItem(int id, const wchar_t* text)
{
   ::SendDlgItemMessage(_handle, id, LB_ADDSTRING, 0, (LPARAM)text);
}

void Dialog :: insertListItem(int id, int index, const wchar_t* text)
{
   ::SendDlgItemMessage(_handle, id, LB_INSERTSTRING, index, (LPARAM)text);
}

void Dialog :: removeListItem(int id, int index)
{
   ::SendDlgItemMessage(_handle, id, LB_DELETESTRING, index, (LPARAM)0);
}

int  Dialog :: getListSelCount(int id)
{
   return ::SendDlgItemMessage(_handle, id, LB_GETSELCOUNT, 0, 0);
}

void Dialog :: getListSelected(int id, int count, int* selected)
{
   ::SendDlgItemMessage(_handle, id, LB_GETSELITEMS, count, (LPARAM)selected);
}

void Dialog :: setListSelected(int id, int index, bool toggle)
{
   ::SendDlgItemMessage(_handle, id, LB_SETSEL, toggle ? TRUE : FALSE, index);
}

void Dialog :: doCommand(int id, int command)
{
   switch (id) {
      case IDOK:
         onOK();
         ::EndDialog(_handle, -1);
         break;
      case IDCANCEL:
         ::EndDialog(_handle, 0);
         break;
   }
}

BOOL CALLBACK Dialog :: DialogProc(HWND hWnd, size_t message, WPARAM wParam, LPARAM lParam)
{
   Dialog* dialog = (Dialog*)::GetWindowLong(hWnd, GWL_USERDATA);
   switch (message) {
      case WM_INITDIALOG:
         dialog = (Dialog*)lParam;
         dialog->_handle = hWnd;
         ::SetWindowLong(hWnd, GWL_USERDATA, (long)lParam);

         dialog->onCreate();

         return 0;
      case WM_COMMAND:
         dialog->doCommand(LOWORD(wParam), HIWORD(wParam));
         return TRUE;
      default:
         return FALSE;
   }
}

// --- ProjectSettingsDialog ---

void ProjectSettingsDialog :: loadTemplateList()
{
   _ELENA_::Path configPath(Paths::appPath, _T("elc.cfg"));
   _ELENA_::IniConfigFile config;
   if (!config.load(configPath, _ELENA_::feAnsi))
      return;

   const char* curTemplate = Project::getTemplate();

   int current = 0;
   for (_ELENA_::ConfigCategoryIterator it = config.getCategoryIt("templates") ; !it.Eof() ; it++, current++) {
      ParamString value(it.key());

      addComboBoxItem(IDC_SETTINGS_TEPMPLATE, value);

      if (_ELENA_::StringHelper::compare(curTemplate, it.key()))
         setComboBoxIndex(IDC_SETTINGS_TEPMPLATE, current);
   }
}

void ProjectSettingsDialog :: onCreate()
{
   setTextLimit(IDC_SETTINGS_PACKAGE, IDENTIFIER_LEN);

   setText(IDC_SETTINGS_PACKAGE, ParamString(Project::getPackage()));
   setText(IDC_SETTINGS_TARGET, ParamString(Project::getTarget()));
   setText(IDC_SETTINGS_OUTPUT, ParamString(Project::getOutputPath()));
   setText(IDC_SETTINGS_ARGUMENT, ParamString(Project::getArguments()));
   setText(IDC_SETTINGS_OPTIONS, ParamString(Project::getOptions()));
   setText(IDC_SETTINGS_VMPATH, ParamString(Project::getVMPath()));

   //setCheckState(IDC_SETTINGS_DEBUG, );
   addComboBoxItem(IDC_SETTINGS_DEBUG, _T("Disabled"));
   addComboBoxItem(IDC_SETTINGS_DEBUG, _T("Enabled"));
   addComboBoxItem(IDC_SETTINGS_DEBUG, _T("Enabled for VM Client"));

   int mode = Project::getDebugMode();
   if (mode == -1) {
      setComboBoxIndex(IDC_SETTINGS_DEBUG, 1);
   }
   else if (mode == -2) {
      setComboBoxIndex(IDC_SETTINGS_DEBUG, 2);
   }
   else setComboBoxIndex(IDC_SETTINGS_DEBUG, 0);

   setCheckState(IDC_SETTINGS_WARN_REF, Project::getBoolSetting("warn:unresolved"));

   loadTemplateList();

   //onProjectTypeChanged();
}

void ProjectSettingsDialog :: onOK()
{
   wchar_t path[MAX_PATH + 1];
   getText(IDC_SETTINGS_TARGET, (wchar_t**)(&path), MAX_PATH);

   if (!_ELENA_::emptystr(path)) {
      if (_ELENA_::Path::checkExtension(path, NULL)) {
         wcsncat(path, _T(".exe"), 4);
      }
      Project::setTarget(SettingString(path));
   }
   else Project::setTarget(NULL);

   getText(IDC_SETTINGS_ARGUMENT, (wchar_t**)(&path), MAX_PATH);
   Project::setArguments(SettingString(path));

   getText(IDC_SETTINGS_OUTPUT, (wchar_t**)(&path), MAX_PATH);
   Project::setOutputPath(SettingString(path));

   getText(IDC_SETTINGS_VMPATH, (wchar_t**)(&path), MAX_PATH);
   if (!_ELENA_::emptystr(path))
      Project::setVMPath(SettingString(path));

   getText(IDC_SETTINGS_OPTIONS, (wchar_t**)(&path), MAX_PATH);
   Project::setOptions(SettingString(path));

   wchar_t name[IDENTIFIER_LEN + 1] ;
   getText(IDC_SETTINGS_PACKAGE, (wchar_t**)(&name), IDENTIFIER_LEN);
   Project::setPackage(SettingString(name));

   if (getComboBoxIndex(IDC_SETTINGS_TEPMPLATE) != -1) {
      getText(IDC_SETTINGS_TEPMPLATE, (wchar_t**)(&name), IDENTIFIER_LEN);
      Project::setTemplate(SettingString(name));
   }

   int index = getComboBoxIndex(IDC_SETTINGS_DEBUG);
   if (index == 1) {
      Project::setDebugMode(_ELENA_::dbmStandalone);
   }
   else if (index == 2) {
      Project::setDebugMode(_ELENA_::dbmElenaVM);
   }
   else Project::setDebugMode(_ELENA_::dbmNone);

   Project::setBoolSetting("warn:unresolved", getCheckState(IDC_SETTINGS_WARN_REF));
}

// --- ProjectForwardsDialog ---

bool ProjectForwardsDialog :: validateItem(wchar_t* &text)
{
   // trim space
   while (text[0]==' ') text++;
   while (_ELENA_::getlength(text) > 0 && text[_ELENA_::getlength(text) - 1]==' ') text[_ELENA_::getlength(text) - 1] = 0;

   if (_ELENA_::emptystr(text))
      return false;
   else if (_ELENA_::StringHelper::find(text, '=')==-1) {
      MsgBox::show(_owner->getHandle(), _T("The forward should have the following structure: <forward name>=<full class name>\n(e.g. 'integer=std'basic'integer)"), MB_ICONERROR);
	  return false;
   }
   else return true;
}

void ProjectForwardsDialog :: addItem()
{
   wchar_t item[IDENTIFIER_LEN * 2 + 1];

   getText(IDC_FORWARDS_EDIT, (wchar_t**)(&item), IDENTIFIER_LEN * 2);

   wchar_t* s = item;
   if (validateItem(s)) {
      addListItem(IDC_FORWARDS_LIST, s);
	  _changed = true;
   }
   setText(IDC_FORWARDS_EDIT, NULL);
   _current = -1;
   _changed = true;
}

void ProjectForwardsDialog :: getItem()
{
   _current = getListIndex(IDC_FORWARDS_LIST);

   wchar_t item[IDENTIFIER_LEN * 2 + 1];

   getListItem(IDC_FORWARDS_LIST, _current, (wchar_t**)(&item));
   setText(IDC_FORWARDS_EDIT, item);
}

void ProjectForwardsDialog :: editItem()
{
   if (_current != -1) {
      wchar_t item[IDENTIFIER_LEN * 2 + 1];

      getText(IDC_FORWARDS_EDIT, (wchar_t**)(&item), IDENTIFIER_LEN * 2);

      wchar_t* s = item;
      if (validateItem(s)) {
         removeListItem(IDC_FORWARDS_LIST, _current);
         insertListItem(IDC_FORWARDS_LIST, _current, s);
	     _changed = true;
      }
      setText(IDC_FORWARDS_EDIT, NULL);
      _current = -1;
	  _changed = true;
   }
}

void ProjectForwardsDialog :: deleteItem()
{
   int index = _current = getListIndex(IDC_FORWARDS_LIST);

   removeListItem(IDC_FORWARDS_LIST, index);
   setText(IDC_FORWARDS_EDIT, NULL);

   _current = -1;
   _changed = true;
}


void ProjectForwardsDialog :: onCreate()
{
   _ELENA_::ConfigCategoryIterator forwards = Project::Forwards();
   _ELENA_::String<_text_t, IDENTIFIER_LEN> item;
   while (!forwards.Eof()) {
      ParamString fwd(forwards.key());
      ParamString value((char*)*forwards);

      item.copy(fwd);
      item.append('=');
      item.append(value);

      addListItem(IDC_FORWARDS_LIST, item);

      forwards++;
   }
}

void ProjectForwardsDialog :: onOK()
{
   if (_changed) {
      Project::clearForwards();

      int count = getListCount(IDC_FORWARDS_LIST);
      _text_t item[IDENTIFIER_LEN * 2 + 1];
      _ELENA_::String<_text_t, IDENTIFIER_LEN> name;
      for (int i = 0 ; i < count ; i++) {
         getListItem(IDC_FORWARDS_LIST, i, (wchar_t**)(&item));

         int pos = _ELENA_::StringHelper::find(item, '=');
         name.copy(item, pos);

         Project::addForward(name, item + pos + 1);
      }
   }
}

void ProjectForwardsDialog :: doCommand(int id, int command)
{
   switch (id) {
      case IDC_FORWARDS_ADD:
         addItem();
         break;
      case IDC_FORWARDS_REPLACE:
         editItem();
         break;
      case IDC_FORWARDS_DELETE:
         deleteItem();
         break;
      case IDC_FORWARDS_LIST:
         if (command==LBN_DBLCLK) {
            getItem();
         }
         break;
	  case IDC_FORWARDS_SAVE:
	     onOK();
         ::EndDialog(_handle, -1);
	     break;
	  case IDOK:
	     break;
	  default:
	     Dialog::doCommand(id, command);
   }
}

// --- WindowsDialog ---

WindowsDialog :: WindowsDialog(Control* owner)
   : Dialog(owner)
{
}

void WindowsDialog :: doCommand(int id, int command)
{
   switch (id) {
      case IDC_WINDOWS_LIST:
         if (command==LBN_SELCHANGE) {
            onListChange();
         }
         break;
      case IDC_WINDOWS_CLOSE:
         onClose();
         ::EndDialog(_handle, -2);
         break;
      default:
         Dialog::doCommand(id, command);
   }
}

void WindowsDialog :: onListChange()
{
    enable(IDOK, (getListSelCount(IDC_WINDOWS_LIST) == 1));
}

// --- EditorSettings ---

inline int encodingToIndex(int encoding)
{
   switch (encoding) {
      case _ELENA_::feAnsi:
         return 0;
      case 1250:
         return 1;
      case 1251:
         return 2;
      case 1252:
         return 3;
      case _ELENA_::feUTF16:
         return 5;
      case _ELENA_::feUTF8:
         return 6;
      default:
         return 4;
   }
}

inline int indexToEncoding(int encoding)
{
   switch (encoding) {
      case 0:
         return _ELENA_::feAnsi;
      case 1:
         return 1250;
      case 2:
         return 1251;
      case 3:
         return 1252;
      case 5:
         return _ELENA_::feUTF16;
      case 6:
         return _ELENA_::feUTF8;
      default:
         return CP_OEMCP;
   }
}

void EditorSettings :: onCreate()
{
   addComboBoxItem(IDC_EDITOR_COLORSCHEME, TEXT("Default"));
   addComboBoxItem(IDC_EDITOR_COLORSCHEME, TEXT("Classic"));

   // populate font size combo box
   _ELENA_::String<wchar16_t, 4> size;
   for(int i = 8 ; i < 25 ; i++) {
      size.appendInt(i);
      addComboBoxItem(IDC_EDITOR_FONTSIZE, size);
      size.clear();
   }   

   setComboBoxIndex(IDC_EDITOR_COLORSCHEME, Settings::scheme);
   setComboBoxIndex(IDC_EDITOR_FONTSIZE, Settings::font_size - 8);

   setCheckState(IDC_EDITOR_LINENUMBERFLAG, Settings::lineNumberVisible);
   setCheckState(IDC_EDITOR_USETAB, Settings::tabCharUsing);
   setCheckState(IDC_EDITOR_HIGHLIGHSYNTAXFLAG, Settings::highlightSyntax);   

   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("ASCII"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("Win 1250"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("Win 1251"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("Win 1252"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("OEM"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("UTF-16"));
   addComboBoxItem(IDC_EDITOR_ENCODING, TEXT("UTF-8"));

   setComboBoxIndex(IDC_EDITOR_ENCODING, encodingToIndex(Settings::defaultEncoding));

   if (!Settings::highlightSyntax)
      enable(IDC_EDITOR_COLORSCHEME, false);

   size.appendInt(Settings::tabSize);
   setText(IDC_EDITOR_TABSIZE, size);

   setCheckState(IDC_EDITOR_REMEMBERPATH, Settings::lastPathRemember);
   setCheckState(IDC_EDITOR_REMEMBERPROJECT, Settings::lastProjectRemember);
}

void EditorSettings :: doCommand(int id, int msg)
{
   if (id==IDC_EDITOR_HIGHLIGHSYNTAXFLAG) {
      onEditorHighlightSyntaxChanged();
   }
   else Dialog::doCommand(id, msg);
}

void EditorSettings :: onEditorHighlightSyntaxChanged()
{
   enable(IDC_EDITOR_COLORSCHEME, getCheckState(IDC_EDITOR_HIGHLIGHSYNTAXFLAG));
}

void EditorSettings :: onOK()
{
   Settings::scheme = getComboBoxIndex(IDC_EDITOR_COLORSCHEME);
   Settings::font_size = 8 + getComboBoxIndex(IDC_EDITOR_FONTSIZE);
   Settings::lineNumberVisible = getCheckState(IDC_EDITOR_LINENUMBERFLAG);
   Settings::tabCharUsing = getCheckState(IDC_EDITOR_USETAB);
   Settings::highlightSyntax = getCheckState(IDC_EDITOR_HIGHLIGHSYNTAXFLAG);
   Settings::defaultEncoding = indexToEncoding(getComboBoxIndex(IDC_EDITOR_ENCODING));
   Settings::lastPathRemember = getCheckState(IDC_EDITOR_REMEMBERPATH);
   Settings::lastProjectRemember = getCheckState(IDC_EDITOR_REMEMBERPROJECT);

   wchar_t size[12];
   getText(IDC_EDITOR_TABSIZE, (wchar_t**)(&size), 11);
   Settings::tabSize = _ELENA_::StringHelper::strToInt(size);
   if (Settings::tabSize <= 0 && Settings::tabSize > 20) {
      Settings::tabSize = 4;
   }
}

// --- DebuggerSettings ---

void DebuggerSettings :: onCreate()
{
   setText(IDC_DEBUGGER_SRCPATH, Paths::packageRoot);
   setText(IDC_DEBUGGER_LIBPATH, Paths::libraryRoot);
}

void DebuggerSettings :: onOK()
{
   wchar_t path[MAX_PATH + 1];   

   getText(IDC_DEBUGGER_SRCPATH, (wchar_t**)(&path), MAX_PATH);
   Settings::addPackagePath("default", path);

   getText(IDC_DEBUGGER_LIBPATH, (wchar_t**)(&path), MAX_PATH);
   Settings::addLibraryPath("default", path);
}

// --- GoToLineDialog ---

void GoToLineDialog :: onCreate()
{
   setIntText(IDC_GOTOLINE_LINENUMBER, _number);
}

void GoToLineDialog :: onOK()
{
   _number = getIntText(IDC_GOTOLINE_LINENUMBER);
}

// --- FindDialog ---

void FindDialog :: copyHistory(int id, SearchHistory* history)
{
   SearchHistory::Iterator it = history->start();
   while (!it.Eof()) {
      addComboBoxItem(id, *it);

      it++;
   }
}

void FindDialog :: onCreate()
{
   setText(IDC_FIND_TEXT, _option->text);
   if (_replaceMode) {
      setText(IDC_REPLACE_TEXT, _option->newText);
   }
   setCheckState(IDC_FIND_CASE, _option->matchCase);
   setCheckState(IDC_FIND_WHOLE, _option->wholeWord);

   if (_searchHistory) {
      copyHistory(IDC_FIND_TEXT, _searchHistory);
   }

   if (_replaceHistory) {
      copyHistory(IDC_REPLACE_TEXT, _replaceHistory);
   }
}

void FindDialog :: onOK()
{
   wchar_t s[200];

   getText(IDC_FIND_TEXT, (wchar_t**)(&s), 200);
   _option->text.copy(s);

   if (_replaceMode) {
      s[0] = 0;
      getText(IDC_REPLACE_TEXT, (wchar_t**)(&s), 200);
      _option->newText.copy(s);
   }
   _option->matchCase = getCheckState(IDC_FIND_CASE);
   _option->wholeWord = getCheckState(IDC_FIND_WHOLE);
}

// --- AboutDialog ---

void AboutDialog :: onCreate()
{
   setText(IDC_ABOUT_LICENCE_TEXT, APACHE_LICENSE2);
   setText(IDC_ABOUT_HOME, ELENA_HOMEPAGE);
   setText(IDC_ABOUT_BLOG, ELENA_BLOG);
}
