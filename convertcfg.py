import sys, os, stat, string

app = ["SingleInstance","ReloadFilesList","Language","AutoBackup","FindListFileOnly","FindOpenedFiles","FindDir","FindSubDir","AutoSaveTimeout",]
view = ["TBStandardPos","TBEditorPos","TBSearchReplacePos","TBTextviewPos","TBEditModePos","TBMacroPos","TBFontEncodingPos","WindowMaximize","MaxDisplaySize","WindowLeft","WindowTop","WindowWidth","WindowHeight","QuickSearchBarStatus","InfoWindowStatus", "ShowQSearchBarOnStart"]
qsearch = [ "QuickSearchWholeWord", "QuickSearchCaseSensitive", "QuickSearchRegEx", "QuickSearchDotMatchNewLine"]
delet = ["SearchListFileOnly", "ShowToolbarStandard", "ShowToolbarEditor", "ShowToolbarSearchReplace", "ShowToolbarTextview", "ShowToolbarEditMode", "ShowToolbarMacro"]

newhdrs = ["[Application]", "[UIView]", "[QuickSearch]"]

if (__name__=="__main__"):
    name = "MadEdit.cfg"
    try:
        fd = open(name, "r") 
        #fd = os.open(name,os.O_RDONLY)
        data = fd.read(os.stat(name)[stat.ST_SIZE])
        fd.close()
        data = data.split('\n')
        # Function header
        newcfg = ""
        appcfg = "[Application]\n"
        apphds = []
        viewcfg = "[UIView]\n"
        viewhds = []
        qsearchcfg = "[QuickSearch]\n"
        qshdrs = []
        for line in data:
            tags = line.split('=')
            if len(tags) > 1:
                if tags[0] in app:
                    if tags[0] not in apphds:
                        appcfg += line + '\n'
                        apphds.append(tags[0])
                elif tags[0] in view:
                    if tags[0] not in viewhds:
                        viewcfg += line + '\n'
                        viewhds.append(tags[0])
                elif tags[0] in qsearch:
                    if tags[0] not in qshdrs:
                        qsearchcfg += line + '\n'
                        qshdrs.append(tags[0])
                elif tags[0] in delet:
                    continue
                else:
                    newcfg += line + '\n'
            else:
                if line not in newhdrs:
                    if len(line) > 2:
                        newcfg += line + '\n'
        newcfg += appcfg + viewcfg
        #print newcfg
        fd = open(name,"wb")
        fd.write(newcfg)
        fd.close()
    except (OSError,IOError,KeyboardInterrupt):
        print '** Halted **' 