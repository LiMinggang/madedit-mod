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


class MadEditApp:public wxApp
{
    wxSingleInstanceChecker * m_SigleAppChecker;
    MadAppSrv * m_AppServer;
public:
    bool OnInit();
    int OnExit();
};

 
#endif
