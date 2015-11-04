///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditApp.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITAPP_H_
#define _MADEDITAPP_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/ipc.h>
#include <wx/stackwalk.h>

class wxFile;
class wxSingleInstanceChecker;

class MadAppConn : public wxConnection
{
public:
    bool OnExecute(const wxString& topic,
#if wxMAJOR_VERSION < 2 || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION < 9)
                            wxChar* data,
                            int WXUNUSED(size),
#else
                            const void * data,
                            size_t WXUNUSED(size),
#endif
                            wxIPCFormat WXUNUSED(format));
};

class MadAppSrv : public wxServer
{
public:
    virtual wxConnectionBase* OnAcceptConnection(const wxString& topic);
};

class MadAppClnt : public wxClient
{
public:
    virtual wxConnectionBase* OnMakeConnection() {return new MadAppConn();}
};

#if (wxUSE_ON_FATAL_EXCEPTION == 1) && (wxUSE_STACKWALKER == 1)
class MadStackWalker : public wxStackWalker
{
    wxFile * m_DumpFile;
public:
    MadStackWalker():m_DumpFile(0) {}
    void SetDumpFile(wxFile * file){m_DumpFile = file;}
protected:
    inline void OnStackFrame(const wxStackFrame & frame);
};
#endif

class MadEditApp:public wxApp
{
    wxSingleInstanceChecker * m_SigleAppChecker;
    MadAppSrv * m_AppServer;
#if (wxUSE_ON_FATAL_EXCEPTION == 1) && (wxUSE_STACKWALKER == 1)
    MadStackWalker m_StackWalker;
#endif
    bool m_SilentMode;
	bool m_Exit;
    bool m_ForceEdit;
    wxArrayString m_FileNames;
    wxString m_MadPythonScript;

public:
    virtual bool OnInit();
    virtual int OnExit();
    virtual void OnInitCmdLine(wxCmdLineParser& cmdParser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& cmdParser);
#if (wxUSE_ON_FATAL_EXCEPTION == 1) && (wxUSE_STACKWALKER == 1)
    void OnFatalException();
#endif
};

 
#endif
