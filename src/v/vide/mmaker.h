#ifndef MMAKER_H
#define MMAKER_H

#include <fstream.h>
#include <v/vutil.h>        //vGetLocalTime
#include <v/vos.h>		// V OS Dependent Utils
#include <v/vslist.h>

class videCmdWindow;

  class makefileMaker
  {
    public:   //----------------------------------------------public

     makefileMaker();
     ~makefileMaker();

     static const int fieldLen; // standard string buffs length

     void setDefaults();
     void eraseAll();

     bool addFile(char *n);
     void saveMakefile();
     void saveProject (char *n);
     void loadProject (char *n);
     bool getSectionEntry(ifstream &prjf, const char* head, char* value);
     bool getSectionList(ifstream &prjf, const char* head, const char* term, vSList& sl);
     bool changed;

     char *makeName;
     char *homeV;
     char *exeName;
     char *ObjDir;
     char *BinDir;
     char *SrcDir;
     char *cFlags;
     char *Libs;
     char *cc;
     char *ctags;

     // lists we need
     vSList projOpts;
     vSList incDirs;
     vSList libDirs;
     vSList objFiles;
     vSList defsPool;
     vSList curDefs;
     vSList curOpts;
     vSList rules;
     vSList userTargets;

     protected: //----------------------------------------------protected

     private:   //----------------------------------------------private

     void fixTabs(char* fixed, char* orig);
     char* stripDir(char* n);
     videCmdWindow* cmdw;
     char* srcN2ObjN(char *, char*);
     void findRule(char *);

     char* fixBCCLibs();
     char *libOptsBCC;
     vSList libsBCC;

  };

#endif
