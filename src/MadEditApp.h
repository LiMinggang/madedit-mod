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
    virtual bool OnExecute(const wxString& WXUNUSED(topic), wxChar* WXUNUSED(data),
        int WXUNUSED(size), wxIPCFormat WXUNUSED(format));
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
