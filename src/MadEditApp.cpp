///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditApp.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEditApp.h"
#include "MadEditFrame.h"
#include "MadOptionsDialog.h"
#include "MadUtils.h"

#include "MadEdit/MadEdit.h"

#include <wx/display.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/snglinst.h>
#include <algorithm>

IMPLEMENT_APP(MadEditApp)


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

wxLocale g_Locale;

wxString g_MadEditAppDir;
wxString g_MadEditHomeDir;
wxString g_MadEditConfigName;
#ifdef __WXMSW__
wxString g_MadEditRegkeyPath = wxT("HKEY_CURRENT_USER\\Software\\Classes\\");
#endif
bool g_DoNotSaveSettings=false;
bool g_ResetAllKeys=false;

wxChar *g_LanguageString[]=
{
    wxT("System Default"),
    wxT("\u7B80\u4F53\u4E2D\u6587(Chinese Simplified)"),
    wxT("\u6B63\u9AD4\u4E2D\u6587(Chinese Traditional)"),
    wxT("English"),
    wxT("Italiano(Italian)"),
    wxT("\u65E5\u6587(Japanese)"),
    wxT("Espa\u00F1ol(Spanish)"),
    wxT("\u0440\u0443\u0441\u0441\u043a\u0438\u0439(Russian)"),
};
int g_LanguageValue[]=
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_CHINESE_SIMPLIFIED,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_ENGLISH_US,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_JAPANESE,
    wxLANGUAGE_SPANISH,
    wxLANGUAGE_RUSSIAN,
};
extern const size_t g_LanguageCount = sizeof(g_LanguageValue)/sizeof(int);
//wxIMPLEMENT_APP(MadEditApp);
const wxString g_MadServerStr = wxT("MadMainApp");
const wxString g_MadTopicStr = wxT("single-instance");

#ifdef __WXGTK__

// the codes of SingleInstance checking and
// SendMessage to previous instance under GTK+
// are from gcin (http://www.csie.nctu.edu.tw/~cp76/gcin/)

#include <X11/Xatom.h>
#define GSocket GlibGSocket 
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#undef GSocket
#if wxMAJOR_VERSION < 2 || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION < 9)
#include <wx/gtk/win_gtk.h>
//#else
//#include <wx/gtk/private/win_gtk.h>
#endif

Atom g_MadEdit_atom;
Display *g_Display=NULL;

static GdkFilterReturn my_gdk_filter(GdkXEvent *xevent,
                                     GdkEvent *event,
                                     gpointer data)
{
    XEvent *xeve = (XEvent *)xevent;

    if (xeve->type == PropertyNotify)
    {
        XPropertyEvent *xprop = &xeve->xproperty;

        if (xprop->atom == g_MadEdit_atom)
        {
            Atom actual_type;
            int actual_format;
            u_long nitems,bytes_after;
            char *message;

            if (XGetWindowProperty(g_Display, xprop->window, g_MadEdit_atom, 0, 1024*16,
                False, AnyPropertyType, &actual_type, &actual_format,
                &nitems,&bytes_after,(unsigned char**)&message) != Success)
            {
                //dbg("err prop");
                return GDK_FILTER_REMOVE;
            }

            const wxWCharBuffer wcstr = wxConvUTF8.cMB2WX(message);
            size_t datalen = wcslen((const wchar_t *)wcstr);

            OnReceiveMessage((const wchar_t *)wcstr, datalen*sizeof(wchar_t));

            XFree(message);
            return GDK_FILTER_REMOVE;
        }
    }

    //if (XFilterEvent(xeve, None) == True)
    //    return GDK_FILTER_REMOVE;

    return GDK_FILTER_CONTINUE;
}

void send_message(Window madedit_win, const wxString &msg)
{
    Window mwin = XCreateSimpleWindow(g_Display, DefaultRootWindow(g_Display),
                    0,0,90,90,1,0,0);

    const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( msg );
    size_t datalen_utf8 = strlen(data_utf8);

    XChangeProperty(g_Display, mwin , g_MadEdit_atom, XA_STRING, 8,
        PropModeReplace, (unsigned char*)(const char*)data_utf8, datalen_utf8 + 1);

    XPropertyEvent eve;

    eve.type=PropertyNotify;
    eve.window=mwin;
    eve.state=PropertyNewValue;
    eve.atom=g_MadEdit_atom;
    XSendEvent(g_Display, madedit_win, False, 0, (XEvent *)&eve);
    XSync(g_Display,0);
    sleep(1);

    XDestroyWindow(g_Display, mwin);
}

#endif

void DeleteConfig()
{
    if(g_DoNotSaveSettings==false)
    {
        wxFileConfig *cfg=(wxFileConfig *)wxFileConfig::Get(false);

        if(g_ResetAllKeys==false)
        {
            // save MadEdit::KeyBindings
            cfg->SetPath(wxT("/KeyBindings"));
            MadEdit::ms_KeyBindings.SaveToConfig_New(cfg);
        }
        else
        {
            cfg->DeleteGroup(wxT("/KeyBindings"));
        }
        cfg->DeleteGroup(wxT("/EditKeys"));
        cfg->DeleteGroup(wxT("/MenuKeys"));

        delete cfg;
    }

    MadEdit::ms_KeyBindings.FreeCommandTextMap();

    FontWidthManager::Save();
    FontWidthManager::FreeMem();

    wxFileConfig::Set(NULL);
}

wxConnectionBase *MadAppSrv::OnAcceptConnection(const wxString& topic)
{
    if (topic.Lower() == g_MadTopicStr)
    {
        // Check that there are no modal dialogs active
        wxWindowList::Node* node = wxTopLevelWindows.GetFirst();
        while (node)
        {
            wxDialog* dialog = wxDynamicCast(node->GetData(), wxDialog);
            if (dialog && dialog->IsModal())
            {
                return NULL;
            }
            node = node->GetNext();
        }
        return new MadAppConn();
    }
    else
        return NULL;
}

// Opens a file passed from another instance
bool MadAppConn::OnExecute(const wxString& topic,
#if wxMAJOR_VERSION < 2 || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION < 9)
                        wxChar* data,
                        int WXUNUSED(size),
#else
                        const void * data,
                        size_t WXUNUSED(size),
#endif
                        wxIPCFormat WXUNUSED(format))
{
    MadEditFrame* frame = wxDynamicCast(wxGetApp().GetTopWindow(), MadEditFrame);
    wxString filename((wxChar*)data);
    if (filename.IsEmpty())
    {
        // Just raise the main window
        if (frame)
        {
            frame->Restore();    // for minimized frame
            frame->Raise();
        }
    }
    else
    {
        // Check if the filename is already open,
        // and raise that instead.
        OnReceiveMessage(filename.c_str(), (filename.size()+1)*sizeof(wxChar));
    }
    return true;
}

bool MadEditApp::OnInit()
{
    wxFileName filename(GetExecutablePath());
    filename.MakeAbsolute();
    g_MadEditAppDir=filename.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    
    m_SigleAppChecker = 0;
#ifdef __WXMSW__
    g_MadEditHomeDir=g_MadEditAppDir;
#else //linux: ~/.madedit
    g_MadEditHomeDir=wxStandardPaths::Get().GetUserDataDir() +wxFILE_SEP_PATH;
    if(!wxDirExists(g_MadEditHomeDir))
    {
        wxLogNull nolog; // no error message
        wxMkdir(g_MadEditHomeDir);
    }
#endif

    //wxLogMessage(g_MadEditAppDir);
    //wxLogMessage(g_MadEditHomeDir);

    // parse commandline to filenames, every file is with a trailing char '|', ex: filename1|filename2|
    wxString filenames;
    for(int i=1;i<argc;++i)
    {
        wxFileName filename(argv[i]);
        filename.MakeAbsolute();
        filenames += filename.GetFullPath() + wxT('|');
    }


    // init wxConfig
    g_MadEditConfigName=g_MadEditHomeDir+ GetAppName()+ wxT(".cfg");

    wxFileConfig *cfg=new wxFileConfig(wxEmptyString, wxEmptyString, g_MadEditConfigName, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
    cfg->SetExpandEnvVars(false);
    cfg->SetRecordDefaults(true);
    wxFileConfig::Set(cfg);

    bool bSingleInstance=true;
    cfg->Read(wxT("/MadEdit/SingleInstance"), &bSingleInstance, true);

    if(bSingleInstance)
    {
        // check SingleInstance and send filenames to previous MadEdit
        wxString name = wxString::Format(wxT("MadEdit-%s"), wxGetUserId().GetData());

        m_SigleAppChecker = new wxSingleInstanceChecker(name);
        // If using a single instance, use IPC to
        // communicate with the other instance
        if (!m_SigleAppChecker->IsAnotherRunning())
        {
            // Create a new server
            m_AppServer = new MadAppSrv;
            if ( !m_AppServer->Create(g_MadServerStr))
            {
                wxLogDebug(_("Failed to create an IPC service."));
            }
        }
        else
        {
            wxLogNull logNull;
            // OK, there IS another one running, so try to connect to it
            // and send it any filename before exiting.
            MadAppClnt* client = new MadAppClnt;
            // ignored under DDE, host name in TCP/IP based classes
            wxString hostName = wxT("localhost");
            // Create the connection
            wxConnectionBase* connection = client->MakeConnection(hostName, g_MadServerStr, g_MadTopicStr);
            if (connection)
            {
                // Ask the other instance to open a file or raise itself
                connection->Execute(filenames);
                connection->Disconnect();
                delete connection;
            }
            else
            {
                wxMessageBox(_("Sorry, the existing instance may be too busy too respond.\nPlease close any open dialogs and retry."),
                    wxT("MadEdit"), wxICON_INFORMATION|wxOK);
            }
            g_DoNotSaveSettings = true;
            DeleteConfig();
            delete client;
            return false;
        }
    }


#ifdef __WXGTK__
    bool bDisableWarningMessage = true;
    cfg->Read(wxT("/MadEdit/DisableWarningMessage"), &bDisableWarningMessage, true);
    if(bDisableWarningMessage)
    {
        // redirect "IPP request failed" message to /dev/null
        int fdnull = open ("/dev/null", O_WRONLY, 0);
        if(fdnull >= 0)
        {
            dup2(fdnull, STDERR_FILENO);
        }
    }
#endif

    // init locale
    wxString strlang;
    cfg->Read(wxT("/MadEdit/Language"), &strlang);
    int lang=g_LanguageValue[0];
    if(!strlang.IsEmpty())
    {
        strlang.MakeLower();
        for(size_t idx=1; idx<g_LanguageCount; ++idx)
        {
            if(strlang == wxString(g_LanguageString[idx]).Lower())
            {
                lang=g_LanguageValue[idx];
                break;
            }
        }
    }

    g_Locale.Init(lang);
    g_Locale.AddCatalogLookupPathPrefix(wxT("./locale/"));
    g_Locale.AddCatalogLookupPathPrefix(g_MadEditAppDir+wxT("locale/"));
#ifndef __WXMSW__
    g_Locale.AddCatalogLookupPathPrefix(g_MadEditHomeDir+wxT("locale/"));
#   if defined (DATA_DIR)
    g_Locale.AddCatalogLookupPathPrefix(wxT(DATA_DIR"/locale/"));
#   endif

#endif
    g_Locale.AddCatalog(wxT("madedit"));

    // set colors
    SetHtmlColors();

    bool maximize=false;
#ifdef __WXMSW__
    cfg->Read(wxT("/MadEdit/WindowMaximize"), &maximize, false);
#endif
    wxPoint pos=wxDefaultPosition;
    wxSize size(1024, 768);
    for(int i=0; i<wxDisplay::GetCount(); ++i)
    {
        wxDisplay dis(i);
        if(dis.IsPrimary())
        {
            wxRect rect = dis.GetClientArea();
            size.x = std::min(rect.width, size.x);
            size.y = std::min(rect.height, size.y);
            break;
        }
    }
    //if(!maximize)	// removed: gogo, 30.08.2009
    {
        long x=0,y=0,w=0,h=0;
        cfg->Read(wxT("/MadEdit/WindowLeft"), &x);
        cfg->Read(wxT("/MadEdit/WindowTop"), &y);
        cfg->Read(wxT("/MadEdit/WindowWidth"), &w);
        cfg->Read(wxT("/MadEdit/WindowHeight"), &h);

        if(x+w>0 && y+h>0)
        //if(w>0 && h>0)
        {
            size.x=w;
            size.y=h;

            pos.x=x;
            pos.y=y;
        }
    }


    // load FontWidth buffers
    cfg->Read(wxT("/MadEdit/FontWidthBufferMaxCount"), &FontWidthManager::MaxCount, 10);
    if(FontWidthManager::MaxCount < 4) FontWidthManager::MaxCount=4;
    else if(FontWidthManager::MaxCount>40) FontWidthManager::MaxCount=40;
    FontWidthManager::Init(g_MadEditHomeDir);


    // create the main frame
    MadEditFrame *myFrame = new MadEditFrame(NULL, 1 , wxEmptyString, pos, size);
    SetTopWindow(myFrame);

#ifdef __WXMSW__
    //if(maximize)	// removed: gogo, 30.08.2009
    {
        WINDOWPLACEMENT wp;
        wp.length=sizeof(WINDOWPLACEMENT);
        GetWindowPlacement((HWND)myFrame->GetHWND(), &wp);

       // changed: gogo, 30.08.2009
       //wp.showCmd=SW_SHOWMAXIMIZED;
       wp.showCmd = maximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;

       SetWindowPlacement((HWND)myFrame->GetHWND(), &wp);
    }
#endif

    myFrame->Show(true);

    // reload files previously opened
    wxString files;
    cfg->Read(wxT("/MadEdit/ReloadFilesList"), &files);
    if(!files.IsEmpty())
    {
        filenames = files + filenames;
    }

    if(!filenames.IsEmpty())
    {
        // use OnReceiveMessage() to open the files
        OnReceiveMessage(filenames.c_str(), (filenames.size()+1)*sizeof(wxChar));
    }

    if(myFrame->OpenedFileCount()==0)
    {
        myFrame->OpenFile(wxEmptyString, false);
    }

    return TRUE;
}

int MadEditApp::OnExit()
{
    // save settings in FrameClose();
    if(m_SigleAppChecker)
        delete m_SigleAppChecker;
    if(m_AppServer)
        delete m_AppServer;
    
    return 0;
}
