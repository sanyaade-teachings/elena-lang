//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This file contains the base class implementing ELENA Project interface.
//
//                                              (C)2005-2014, by Alexei Rakov
//---------------------------------------------------------------------------

#include "elena.h"
// -------------------------------------------------------
#include "project.h"
#include "errors.h"
#include "module.h"

using namespace _ELENA_;

#define PMODULE_LEN getlength(PACKAGE_MODULE)

inline const char* getLoadError(LoadResult result)
{
   switch(result)
   {
   case lrDuplicate:
      return errDuplicatedModule;
   case lrNotFound:
      return errUnknownModule;
   case lrWrongStructure:
      return errInvalidModule;
   case lrWrongVersion:
      return errInvalidModuleVersion;
   case lrCannotCreate:
      return errCannotCreate;
   default:
      return NULL;
   }
}

// --- Project ---

Project :: Project()
{
   _hasWarning = false;
   _numberOfWarnings = 100;
}

bool Project :: loadOption(_ConfigFile& config, ProjectSetting setting)
{
   ProjectParam param(getOption(config, setting));
   if (!param.isEmpty()) {
      _settings.add(setting, param.clone());

      return true;
   }
   else return false;
}

void Project :: loadIntOption(_ConfigFile& config, ProjectSetting setting)
{
   const char* value = getOption(config, setting);
   if (value) {
      _settings.add(setting, StringHelper::strToInt(value));
   }
}

void Project :: loadHexOption(_ConfigFile& config, ProjectSetting setting)
{
   const char* value = getOption(config, setting);
   if (value) {
      _settings.add(setting, StringHelper::strToLong(value, 16));
   }
}

void Project :: loadIntOption(_ConfigFile& config, ProjectSetting setting, int minValue, int maxValue)
{
   const char* value = getOption(config, setting);
   if (value) {
      int intValue = StringHelper::strToInt(value);
      if (minValue > intValue)
         intValue = minValue;
      else if (maxValue < intValue)
         intValue = maxValue;

      _settings.add(setting, intValue);
   }
}

void Project :: loadAlignedIntOption(_ConfigFile& config, ProjectSetting setting, int alignment)
{
   const char* value = getOption(config, setting);
   if (value) {
      _settings.add(setting, align(StringHelper::strToInt(value), alignment));
   }
}

void Project :: loadBoolOption(_ConfigFile& config, ProjectSetting setting)
{
   const char* value = getOption(config, setting);
   if (value) {
      if (StringHelper::strToInt(value) != 0)
         _settings.add(setting, -1);
   }
}

bool Project :: loadPathOption(_ConfigFile& config, ProjectSetting setting, const _path_t* rootPath)
{
   const char* value = getOption(config, setting);
   if (value) {
      Path path(rootPath, value);

      _settings.add(setting, path.clone());
      return true;
   }
   else return false;
}

void Project :: loadForwardCategory(_ConfigFile& config)
{
   ProjectParam key, value;

   ConfigCategoryIterator it = getCategory(config, opForwards);
   while (!it.Eof()) {
      // copy line key
      key.copy(it.key());

      value.copy((char*)*it);

      // if it is a wildcard
      if (key[getlength(key) - 1] == '*') {
         NamespaceName alias(key);
         NamespaceName module(value);

         _settings.add(opModuleForwards, alias, module.clone());
      }
      else _settings.add(opForwards, key, value.clone());

      it++;
   }
}

void Project :: loadCategory(_ConfigFile& config, ProjectSetting setting, const _path_t* path)
{
   ProjectParam param;
   ProjectParam key;
   _text_t*    dummy = NULL;

   ConfigCategoryIterator it = getCategory(config, setting);
   while (!it.Eof()) {
      // copy line key
      key.copy(it.key());
      key.lower();

      // copy value or key if the value is absent
      const char* value = *it;
      if (emptystr(value))
         value = it.key();

      // add path if provided
      if (!emptystr(path)) {
         Path filePath(path);
         filePath.combine(value);
         filePath.lower();

         _settings.add(setting, key, filePath.clone());
      }
      else {
         param.copy(value);
         param.lower();

         _settings.add(setting, key, param.clone());
      }

      it++;
   }
}

void Project :: loadConfig(_ConfigFile& config, const _path_t* configPath)
{
//   // load entry symbol
//   const _text_t* entry = getOption(config, opStarter);
//   if (entry) {
//      ProjectParam entryRef(entry);
//
//      loadForward(ConstantIdentifier(STARTUP_CLASS), entryRef);
//   }

   // load project settings (if setting is absent previous value is used)
   loadOption(config, opNamespace);
   loadOption(config, opEntry);
   loadPathOption(config, opLibPath, configPath);
   loadPathOption(config, opTarget, configPath);
   loadOption(config, opOutputPath);
   loadBoolOption(config, opWarnOnUnresolved);
   //loadBoolOption(config, opWarnOnSignature);
   loadBoolOption(config, opDebugMode);
   loadOption(config, opVMPath);       // path to virtual machine should be saved as it is, because it is relative to the executable path rather then a config one
   loadOption(config, opTemplate);
   loadBoolOption(config, opEmbeddedSymbolMode);

   // load compiler settings
//   loadOption(config, opJITType);
   loadIntOption(config, opThreadMax, 0, 60);

   // load linker settings
   loadHexOption(config, opGCMGSize);
   loadIntOption(config, opGCObjectSize);
   loadHexOption(config, opGCYGSize);
//   loadIntOption(config, opApplicationType);
   loadIntOption(config, opPlatformType);

   loadIntOption(config, opSizeOfStackReserv);
   loadIntOption(config, opSizeOfStackCommit);
   loadIntOption(config, opSizeOfHeapReserv);
   loadIntOption(config, opSizeOfHeapCommit);
   loadAlignedIntOption(config, opImageBase, 0x400000);

   // load compiler engine options
   loadIntOption(config, opL0);
//   loadIntOption(config, opL1);
//   loadIntOption(config, opL2);
   loadIntOption(config, opL3);

   // load primitives
   loadCategory(config, opPrimitives, configPath);

   // load sources
   loadCategory(config, opSources, configPath);

   // load forwards
   loadForwardCategory(config);
}

void Project :: loadForward(const wchar16_t* forward, const wchar16_t* reference)
{
   ReferenceNs fwd(forward);

   _settings.add(opForwards, fwd, StringHelper::clone(reference));
}

_Module* Project :: loadModule(const wchar16_t* package, bool silentMode)
{
   LoadResult result = lrNotFound;
   _Module* module = _loader.loadModule(package, result);
   if (result != lrSuccessful) {
      if (!silentMode) {
         raiseError(getLoadError(result), package);
      }

      return NULL;
   }
   else return module;
}

_Module* Project :: loadPrimitive(const wchar16_t* package, bool silentMode)
{
   LoadResult result = lrNotFound;
   _Module* module = NULL;
   const _path_t* path = _settings.get(opPrimitives, package, (const _path_t*)NULL);
   if (path) {
      _loader.addPrimitiveAlias(package, path);
      module = _loader.loadPrimitive(package, result);
   }
   else result = lrNotFound;

   if (result != lrSuccessful) {
      if (!silentMode)
         raiseError(getLoadError(result), path);

      return NULL;
   }
   else return module;
}

_Module* Project :: createModule(const _path_t* sourcePath)
{
   Path modulePath;
   modulePath.copyPath(sourcePath);

   // build module namespace
   ReferenceNs name(StrSetting(opNamespace));
   name.pathToName(modulePath);

   LoadResult result = lrNotFound;
   _Module* module = _loader.createModule(name, result);

   if (result != lrSuccessful && result != lrDuplicate) {
      raiseError(getLoadError(result), sourcePath);

      return NULL;
   }
   else return module;
}

_Module* Project :: createDebugModule(const wchar_t* name)
{
   return new Module(name);
}

void Project :: saveModule(_Module* module, const _path_t* extension)
{
   const wchar16_t* name = module->Name();
   Path path;
   _loader.nameToPath(name, path, extension);

   Path outputPath(StrSetting(opProjectPath), StrSetting(opOutputPath));
   Path::create(outputPath, path);

   FileWriter writer(path, feRaw, false);
   if(!module->save(writer)) 
      raiseError(getLoadError(lrCannotCreate), (const _path_t*)path);
}

const wchar16_t* Project :: resolveForward(const wchar16_t* forward)
{
   const wchar16_t* reference = _settings.get(opForwards, forward, DEFAULT_STR);
   // if no forward mapping was found try to resolve on the module level
   if (emptystr(reference)) {
      NamespaceName alias(forward);

      const wchar16_t* module = _settings.get(opModuleForwards, alias, DEFAULT_STR);
      // if there is a module mapping create an appropriate forward
      if (!emptystr(module)) {
         ReferenceName name(forward);
         ReferenceNs newRefeference(module, name);

         _settings.add(opForwards, forward, newRefeference.clone());

         reference = _settings.get(opForwards, forward, DEFAULT_STR);
      }
   }
   return reference;
}

_Module* Project :: resolveModule(const wchar16_t* referenceName, ref_t& reference, bool silentMode)
{
   while (isWeakReference(referenceName)) {
      referenceName = resolveForward(referenceName);
   }

   if (emptystr(referenceName))
      return NULL;

   LoadResult result = lrNotFound;
   _Module* module = NULL;
   if (ConstIdentifier::compare(referenceName, PACKAGE_MODULE, PMODULE_LEN) && referenceName[PMODULE_LEN]=='\'') {
      module = _loader.resolvePrimitive(referenceName, result, reference);
      if (result == lrNotFound) {
         NamespaceName package(referenceName + PMODULE_LEN + 1);

         // add alias to loader
         const _path_t* path = _settings.get(opPrimitives, package, (const _path_t*)NULL);
         if (path) {
            _loader.addPrimitiveAlias(package, path);

            // try to resolve once again
            module = _loader.resolvePrimitive(referenceName, result, reference);
         }
      }
   }
   else module = _loader.resolveModule(referenceName, result, reference);

   if (result != lrSuccessful) {
      if (!silentMode)
         raiseError(getLoadError(result), referenceName);

      return NULL;
   }
   else return module;

}
