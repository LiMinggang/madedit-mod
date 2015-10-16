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

#include <algorithm>

#include <wx/display.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/snglinst.h>
#include <wx/cmdline.h>

IMPLEMENT_APP(MadEditApp)

extern int MadMessageBox(const wxString& message,
							 const wxString& caption = wxMessageBoxCaptionStr,
							 long style = wxOK | wxCENTRE,
							 wxWindow *parent = NULL,
							 int x = wxDefaultCoord, int y = wxDefaultCoord);

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
bool g_ForcePurgeThisTime = false;


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
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
	 { wxCMD_LINE_SWITCH, "h", "help", "Displays help on the command line parameters",
		  wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	 { wxCMD_LINE_SWITCH, "f", "force", "Edit and save file ignoring the ReadOnly flag" },
	 { wxCMD_LINE_SWITCH, "s", "silent", "Disables the GUI" },
	 { wxCMD_LINE_OPTION, "m", "madpython", "Specify MadPython file to be run on the file" },
	 { wxCMD_LINE_PARAM, NULL, NULL, "File(s) to be opened", wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE }, 
 
	 { wxCMD_LINE_NONE }
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
	// call default behaviour (mandatory)
	m_SilentMode = false;
	m_ForceEdit = false;
	if (!wxApp::OnInit())
		return false;

	wxFileName filename(GetExecutablePath());
	filename.MakeAbsolute();
	g_MadEditAppDir=filename.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);

	m_SigleAppChecker = 0;
	m_AppServer = 0;
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
				// Only file names would be send to the instance, ignore other switches, options
				// Ask the other instance to open a file or raise itself
				wxString fnames;
				for(size_t i=0; i<m_FileNames.GetCount(); ++i)
				{
					//The name is what follows the last \ or /
					fnames +=  m_FileNames[i] + wxT('|');
				}
				connection->Execute(fnames);
				connection->Disconnect();
				delete connection;
			}
			else
			{
				MadMessageBox(_("Sorry, the existing instance may be too busy too respond.\nPlease close any open dialogs and retry."),
					wxT("MadEdit-Mod"), wxICON_INFORMATION|wxOK);
			}
			g_DoNotSaveSettings = true;
			DeleteConfig();
			delete client;
			return false;
		}
	}

	wxHandleFatalExceptions();

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
#if defined (DATA_DIR)
	g_Locale.AddCatalogLookupPathPrefix(wxT(DATA_DIR"/locale/"));
#endif

#endif
	g_Locale.AddCatalog(wxT("madedit-mod"));

	// set colors
	SetHtmlColors();

	 bool maximize=false;
#ifdef __WXMSW__
	cfg->Read(wxT("/MadEdit/WindowMaximize"), &maximize, false);
#endif
	wxPoint pos=wxDefaultPosition;
	wxSize size(1024, 768);
	for(unsigned int i=0; i<wxDisplay::GetCount(); ++i)
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
	//if(!maximize)    // removed: gogo, 30.08.2009
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

	if(!m_SilentMode)
	{

		SetTopWindow(myFrame);

#ifdef __WXMSW__
		//if(maximize)    // removed: gogo, 30.08.2009
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
		
		for(size_t i=0; i<m_FileNames.GetCount(); ++i)
		{
			//The name is what follows the last \ or /
			files +=  m_FileNames[i] + wxT('|');
		}

		if(!files.IsEmpty())
		{
			// use OnReceiveMessage() to open the files
			OnReceiveMessage(files.c_str(), (files.size()+1)*sizeof(wxChar));
		}

		if(myFrame->OpenedFileCount()==0)
		{
			myFrame->OpenFile(wxEmptyString, false);
		}
	}
	else
	{

		if(!m_FileNames.IsEmpty() && !m_MadPythonScript.IsEmpty())
		{
			// open the files
			for( size_t i = 0; i < m_FileNames.GetCount(); ++i )
			{
				myFrame->RunScriptWithFile(m_FileNames[i], m_MadPythonScript, false, true, m_ForceEdit);
			}
		}
		return false;//exit
	}

#if 0
	// force crash
	wxWindow *w = NULL; w->Show();
#endif
	return true;
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

void MadEditApp::OnInitCmdLine(wxCmdLineParser& cmdParser)
{
	cmdParser.SetDesc (g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	cmdParser.SetSwitchChars (wxT("-"));
}
 
bool MadEditApp::OnCmdLineParsed(wxCmdLineParser& cmdParser)
{
	wxFileName filename;
	m_SilentMode = cmdParser.Found(wxT("s"));
	m_ForceEdit = cmdParser.Found(wxT("f"));
	// to get at your unnamed parameters use
	// parse commandline to filenames, every file is with a trailing char '|', ex: filename1|filename2|
	m_FileNames.Empty();
	for (int i = 0; i < cmdParser.GetParamCount(); i++)
	{
		filename = cmdParser.GetParam(i);
		filename.MakeAbsolute();
		m_FileNames.Add(filename.GetFullPath());
	}

	// and other command line parameters
	cmdParser.Found(wxT("m"), &m_MadPythonScript);
	// then do what you need with them.
 
	return true;
}


#if (wxUSE_ON_FATAL_EXCEPTION == 1) && (wxUSE_STACKWALKER == 1)
#include <wx/longlong.h>
void MadStackWalker::OnStackFrame(const wxStackFrame & frame)
{
	if(m_DumpFile) 
	{
		wxULongLong address((size_t)frame.GetAddress());
#if defined(__x86_64__) || defined(__LP64__) || defined(_WIN64)
		wxString fmt(wxT("[%02u]:[%08X%08X] %s(%i)\t%s%s\n"));
#else
		wxString fmt(wxT("[%02u]:[%08X] %s(%i)\t%s%s\n"));
#endif

		wxString paramInfo(wxT("("));
#if defined(_WIN32)
		wxString type, name, value;  
		size_t count = frame.GetParamCount(), i = 0;
		while(i < count)
		{
			frame.GetParam(i, &type, &name, &value);
			paramInfo += type + wxT(" ") + name + wxT(" = ") + value;    
			if(++i < count) paramInfo += wxT(", ");
		}
#endif
		paramInfo += wxT(")");
		m_DumpFile->Write(wxString::Format(fmt,
			(unsigned)frame.GetLevel(),
#if defined(__x86_64__) || defined(__LP64__) || defined(_WIN64)
			address.GetHi(),
#endif
			address.GetLo(),
			frame.GetFileName().c_str(),
			(unsigned)frame.GetLine(),
			frame.GetName().c_str(),
			paramInfo.c_str())
			);
	}
}

void MadEditApp::OnFatalException()
{
	wxString name = g_MadEditHomeDir + wxString::Format(
															wxT("%s_%s_%lu.dmp"),
															wxTheApp ? (const wxChar*)wxTheApp->GetAppDisplayName().c_str()
																	 : wxT("wxwindows"),
															wxDateTime::Now().Format(wxT("%Y%m%dT%H%M%S")).c_str(),
#if defined(__WXMSW__)
															::GetCurrentProcessId()
#else
															(unsigned)getpid()
#endif
														);
	wxFile dmpFile(name.c_str(), wxFile::write); 
	if(dmpFile.IsOpened())
	{
		m_StackWalker.SetDumpFile(&dmpFile);
		m_StackWalker.WalkFromException();
		dmpFile.Close();
	}
}
#endif


