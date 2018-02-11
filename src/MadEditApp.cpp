///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditApp.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEditApp.h"
#include "MadEditFrame.h"
#include "MadOptionsDialog.h"
#include "MadUtils.h"
#include "MadEdit/MadEdit.h"

#include <algorithm>
#include <map>
//#include <boost/bimap.hpp>

#include <wx/display.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/snglinst.h>
#include <wx/cmdline.h>
#include <wx/settings.h>

IMPLEMENT_APP( MadEditApp )

extern int MadMessageBox( const wxString& message,
						  const wxString& caption = wxMessageBoxCaptionStr,
						  long style = wxOK | wxCENTRE,
						  wxWindow *parent = nullptr,
						  int x = wxDefaultCoord, int y = wxDefaultCoord );

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

wxLocale * g_Locale = 0;

wxString g_MadEditAppDir;
wxString g_MadEditHomeDir;
wxString g_MadEditConfigName;

#ifdef __WXMSW__
	wxString g_MadEditRegkeyPath = wxT( "HKEY_CURRENT_USER\\Software\\Classes\\" );
#endif
bool g_DoNotSaveSettings = false;
bool g_ResetAllKeys = false;
bool g_ForcePurgeThisTime = false;

wxArrayString g_LanguageString, g_LocaleDirPrefix;
wxArrayLong g_LanguageId;
//typedef boost::bimap<long, wxString> bm_type;
//typedef bm_type::value_type mad_lang;
//bm_type g_EnhancedLangNameBMap;

std::map<long, wxString> g_EnhancedLangNameMap;
wxString g_MadLanguageFileName = wxT( "madedit-mod" );
void ScanForLocales();

wxChar *g_LanguageStr[] =
{
	wxT( "System Default" ),
	wxT( "\u7B80\u4F53\u4E2D\u6587(Chinese Simplified)" ),
	wxT( "\u6B63\u9AD4\u4E2D\u6587(Chinese Traditional)" ),
	wxT( "English" ),
	wxT( "Deutsch (German)" ),
	wxT( "Italiano (Italian)" ),
	wxT( "\u65E5\u672C\u8A9E (Japanese)" ),
	wxT( "Polski (Polish)" ),
	wxT( "\u0420\u0443\u0441\u0441\u043a\u0438\u0439 (Russian)" ),
	wxT( "Espa\u00F1ol (Spanish)" ),
	wxT( "\u0395\u03BB\u03BB\u03B7\u03BD\u03B9\u03BA\u03AC(Greek)" ),
};
int g_LanguageValue[] =
{
	wxLANGUAGE_DEFAULT,
	wxLANGUAGE_CHINESE_SIMPLIFIED,
	wxLANGUAGE_CHINESE_TRADITIONAL,
	wxLANGUAGE_ENGLISH_US,
	wxLANGUAGE_GERMAN,
	wxLANGUAGE_ITALIAN,
	wxLANGUAGE_JAPANESE,
	wxLANGUAGE_POLISH,
	wxLANGUAGE_RUSSIAN,
	wxLANGUAGE_SPANISH,
	wxLANGUAGE_GREEK,
};
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
	{
		wxCMD_LINE_SWITCH, "h", "help", "Displays help on the command line parameters",
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
	},
	{ wxCMD_LINE_SWITCH, "f", "force", "Edit and save file ignoring the ReadOnly flag" },
	{ wxCMD_LINE_SWITCH, "r", "recursive", "Recursively run on files of subdirectories" },
	{ wxCMD_LINE_SWITCH, "s", "silent", "Disables the GUI" },
	{ wxCMD_LINE_SWITCH, "x", "eXit", "Exit GUI if it was in silent mode" },
	{ wxCMD_LINE_SWITCH, "w", "wildcard", "Enable wildcard support in file name\n(line number would be disabled becasue it used '*')" },
	{ wxCMD_LINE_OPTION, "m", "madpython", "Specify MadPython file to be run on the file" },
	{ wxCMD_LINE_OPTION, "d", "delimiter", "Specify delimiter between file name and line number, default is '*', ie, fname*linenum" },
	{ wxCMD_LINE_PARAM, nullptr, nullptr, "File(s) to be opened", wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },

	{ wxCMD_LINE_NONE }
};

extern const size_t g_LanguageCount = sizeof( g_LanguageValue ) / sizeof( int );
//wxIMPLEMENT_APP(MadEditApp);
#ifdef __WXMSW__
const wxString g_MadServerStr = wxT( "MadMainApp" );
#else  //Linux
const wxString g_MadServerStr = wxT( "/tmp/MadMainApp" );
#endif
const wxString g_MadTopicStr = wxT( "single-instance" );

class MadTranslationHelper : public wxDirTraverser
{
	wxString m_TransName;
	std::map<long, wxString>& m_LanguageIdNameMap;
public:
	MadTranslationHelper( const wxString& transName, std::map<long, wxString>& langIdNameMap ) :
		m_TransName( transName ), m_LanguageIdNameMap( langIdNameMap )
	{}
	~MadTranslationHelper() {};
	virtual wxDirTraverseResult OnFile( const wxString& WXUNUSED( filename ) ) {
		return wxDIR_CONTINUE;
	}
	virtual wxDirTraverseResult OnDir( const wxString& dirName ) {
		const wxLanguageInfo * langinfo;
		langinfo = wxLocale::FindLanguageInfo( dirName.AfterLast( wxFileName::GetPathSeparator() ) );

		if( langinfo != nullptr ) {
			wxLogTrace( wxT("MadTranslationHelper"), _( "SEARCHING FOR %s" ),
						wxString( dirName + wxFileName::GetPathSeparator() +
								  m_TransName + wxT( ".mo" ) ).GetData() );

			if( wxFileExists( dirName + wxFileName::GetPathSeparator() + m_TransName + wxT( ".mo" ) )
					|| ( wxFileExists( dirName + wxFileName::GetPathSeparator() + wxT( "LC_MESSAGES" ) + wxFileName::GetPathSeparator() + m_TransName + wxT( ".mo" ) ) ) ) {
				if( langinfo->Language != wxLANGUAGE_CHINESE )
					m_LanguageIdNameMap[langinfo->Language] = langinfo->Description;
				else
					m_LanguageIdNameMap[wxLANGUAGE_CHINESE_TRADITIONAL] = langinfo->Description;
			}
		}

		return wxDIR_CONTINUE;
	}
};
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
Display *g_Display = nullptr;

static GdkFilterReturn my_gdk_filter( GdkXEvent *xevent,
									  GdkEvent *event,
									  gpointer data )
{
	XEvent *xeve = ( XEvent * )xevent;

	if( xeve->type == PropertyNotify )
	{
		XPropertyEvent *xprop = &xeve->xproperty;

		if( xprop->atom == g_MadEdit_atom )
		{
			Atom actual_type;
			int actual_format;
			u_long nitems, bytes_after;
			char *message;

			if( XGetWindowProperty( g_Display, xprop->window, g_MadEdit_atom, 0, 1024 * 16,
									False, AnyPropertyType, &actual_type, &actual_format,
									&nitems, &bytes_after, ( unsigned char** )&message ) != Success )
			{
				//dbg("err prop");
				return GDK_FILTER_REMOVE;
			}

			const wxWCharBuffer wcstr = wxConvUTF8.cMB2WX( message );
			size_t datalen = wcslen( ( const wchar_t * )wcstr );
			OnReceiveMessage( ( const wchar_t * )wcstr, datalen * sizeof( wchar_t ) );
			XFree( message );
			return GDK_FILTER_REMOVE;
		}
	}

	//if (XFilterEvent(xeve, None) == True)
	//    return GDK_FILTER_REMOVE;
	return GDK_FILTER_CONTINUE;
}

void send_message( Window madedit_win, const wxString &msg )
{
	Window mwin = XCreateSimpleWindow( g_Display, DefaultRootWindow( g_Display ),
									   0, 0, 90, 90, 1, 0, 0 );
	const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( msg );
	size_t datalen_utf8 = strlen( data_utf8 );
	XChangeProperty( g_Display, mwin, g_MadEdit_atom, XA_STRING, 8,
					 PropModeReplace, ( unsigned char* )( const char* )data_utf8, datalen_utf8 + 1 );
	XPropertyEvent eve;
	eve.type = PropertyNotify;
	eve.window = mwin;
	eve.state = PropertyNewValue;
	eve.atom = g_MadEdit_atom;
	XSendEvent( g_Display, madedit_win, False, 0, ( XEvent * )&eve );
	XSync( g_Display, 0 );
	sleep( 1 );
	XDestroyWindow( g_Display, mwin );
}

#endif

void DeleteConfig()
{
	if( g_DoNotSaveSettings == false )
	{
		wxFileConfig *cfg = dynamic_cast< wxFileConfig * >(wxFileConfig::Get( false ));
		if(!cfg)
			return;

		if( g_ResetAllKeys == false )
		{
			// save MadEdit::KeyBindings
			cfg->SetPath( wxT( "/KeyBindings" ) );
			MadEdit::ms_KeyBindings.SaveToConfig_New( cfg );
		}
		else
		{
			cfg->DeleteGroup( wxT( "/KeyBindings" ) );
		}

		cfg->DeleteGroup( wxT( "/EditKeys" ) );
		cfg->DeleteGroup( wxT( "/MenuKeys" ) );
		delete cfg;
	}

	MadEdit::ms_KeyBindings.FreeCommandTextMap();
	FontWidthManager::Save();
	FontWidthManager::FreeMem();
	wxFileConfig::Set( nullptr );
}

wxConnectionBase *MadAppSrv::OnAcceptConnection( const wxString& topic )
{
	if( topic.Lower() == g_MadTopicStr )
	{
		// Check that there are no modal dialogs active
		wxWindowList::Node* node = wxTopLevelWindows.GetFirst();

		while( node )
		{
			wxDialog* dialog = wxDynamicCast( node->GetData(), wxDialog );

			if( dialog && dialog->IsModal() )
			{
				return nullptr;
			}

			node = node->GetNext();
		}

		return new MadAppConn();
	}
	else
		return nullptr;
}

// Opens a file passed from another instance
bool MadAppConn::OnExecute( const wxString& WXUNUSED(topic),
#if wxMAJOR_VERSION < 2 || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION < 9)
	wxChar* data,
	int WXUNUSED(size),
#else
	const void * data,
	size_t size,
#endif
	wxIPCFormat WXUNUSED( format ) )
{
	MadEditFrame* frame = wxDynamicCast( wxGetApp().GetTopWindow(), MadEditFrame );
	if( size == 0 ) return false;
#ifdef __WXMSW__
	wxString filename( ( wxChar* )data );
#else
	wxString filename( wxString::FromUTF8(( const char * )data, size ));
#endif
	if( filename.IsEmpty() )
	{
		// Just raise the main window
		if( frame )
		{
			frame->Restore();    // for minimized frame
			frame->Raise();
		}
	}
	else
	{
		// Check if the filename is already open,
		// and raise that instead.
		OnReceiveMessage( filename.c_str(), ( filename.size() + 1 )*sizeof( wxChar ) );
	}

	return true;
}

bool MadEditApp::OnInit()
{
	// call default behaviour (mandatory)
	m_SilentMode = false;
	m_Exit = false;
	m_ForceEdit = false;
	wxFileName filename( GetExecutablePath() );
	filename.MakeAbsolute();
	g_MadEditAppDir = filename.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
	m_SigleAppChecker = 0;
	m_AppServer = 0;
#ifdef __WXMSW__
	g_MadEditHomeDir = g_MadEditAppDir;
#else //linux: ~/.madedit
	g_MadEditHomeDir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH;

	if( !wxDirExists( g_MadEditHomeDir ) )
	{
		wxLogNull nolog; // no error message
		wxMkdir( g_MadEditHomeDir );
	}

#endif
	//wxLogMessage(g_MadEditAppDir);
	//wxLogMessage(g_MadEditHomeDir);
	// init wxConfig
	g_MadEditConfigName = g_MadEditHomeDir + GetAppName() + wxT( ".cfg" );

	if( !wxApp::OnInit() )
		return false;

	wxFileConfig *cfg = new wxFileConfig( wxEmptyString, wxEmptyString, g_MadEditConfigName, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS );
	cfg->SetExpandEnvVars( false );
	cfg->SetRecordDefaults( true );
	wxFileConfig::Set( cfg );
	bool bSingleInstance = true;
	cfg->Read( wxT( "/Application/SingleInstance" ), &bSingleInstance, true );

	if( bSingleInstance )
	{
		// check SingleInstance and send filenames to previous MadEdit
		wxString name = wxString::Format( wxT( "MadEdit-%s" ), wxGetUserId().GetData() );
		m_SigleAppChecker = new wxSingleInstanceChecker( name );

		// If using a single instance, use IPC to
		// communicate with the other instance
		if( !m_SigleAppChecker->IsAnotherRunning() )
		{
			// Create a new server
			m_AppServer = new MadAppSrv;

			if( !m_AppServer->Create( g_MadServerStr ) )
			{
				ScanForLocales();
				InitLocale();
				wxLogDebug( wxGetTranslation(_( "Failed to create an IPC service." ) ));
				return false;
			}
		}
		else
		{
			wxLogNull logNull;
			// OK, there IS another one running, so try to connect to it
			// and send it any filename before exiting.
			MadAppClnt* client = new MadAppClnt;
			// ignored under DDE, host name in TCP/IP based classes
			wxString hostName = wxT( "localhost" );
			// Create the connection
			wxConnectionBase* connection = client->MakeConnection( hostName, g_MadServerStr, g_MadTopicStr );

			if( connection )
			{
				// Only file names would be send to the instance, ignore other switches, options
				// Ask the other instance to open a file or raise itself
				wxString fnames;

				for( size_t i = 0; i < m_FileNames.GetCount(); ++i )
				{
					//The name is what follows the last \ or /
					fnames +=  m_FileNames[i] + g_MadConfigSeparator;
				}

				if( !m_MadPythonScript.IsEmpty() )
				{
					if( m_SilentMode )
						fnames += wxT( "*s" );

					if( m_Exit )
						fnames += wxT( "*x" );

					if( m_ForceEdit )
						fnames += wxT( "*f" );

					fnames += wxT( "*m" ) + m_MadPythonScript;
				}

#ifdef __WXMSW__
				connection->Execute( fnames );
#else
				const wxScopedCharBuffer buf = fnames.utf8_str();
				connection->Execute(buf, buf.length() + 1, wxIPC_UTF8TEXT);
#endif
				connection->Disconnect();
				delete connection;
			}
			else
			{
				ScanForLocales();
				InitLocale();
				MadMessageBox( wxGetTranslation(_( "Sorry, the existing instance may be too busy too respond.\nPlease close any open dialogs and retry." )),
							   wxT( "MadEdit-Mod" ), wxICON_INFORMATION | wxOK );
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
	cfg->Read( wxT( "/MadEdit/DisableWarningMessage" ), &bDisableWarningMessage, true );

	if( bDisableWarningMessage )
	{
		// redirect "IPP request failed" message to /dev/null
		int fdnull = open( "/dev/null", O_WRONLY, 0 );

		if( fdnull >= 0 )
		{
			dup2( fdnull, STDERR_FILENO );
		}
	}
#endif

	// init locale
	ScanForLocales();
	InitLocale();
	// set colors
	SetHtmlColors();
	wxPoint pos = wxDefaultPosition;
	wxSize size( 1024, 768 );

	for( unsigned int i = 0; i < wxDisplay::GetCount(); ++i )
	{
		wxDisplay dis( i );

		if( dis.IsPrimary() )
		{
			wxRect rect = dis.GetClientArea();
			size.x = std::min( rect.width, size.x );
			size.y = std::min( rect.height, size.y );
			break;
		}
	}

	//if(!maximize)    // removed: gogo, 30.08.2009
	{
		long x = 0, y = 0, w = 0, h = 0;
		cfg->Read( wxT( "/UIView/WindowLeft" ), &x );
		cfg->Read( wxT( "/UIView/WindowTop" ), &y );
		cfg->Read( wxT( "/UIView/WindowWidth" ), &w );
		cfg->Read( wxT( "/UIView/WindowHeight" ), &h );

		if( x + w > 0 && y + h > 0 )
			//if(w>0 && h>0)
		{
			size.x = std::min( w, size.x );
			size.y = std::min( h, size.y );
			pos.x = x;
			pos.y = y;
		}
	}
	// load FontWidth buffers
	cfg->Read( wxT( "/MadEdit/FontWidthBufferMaxCount" ), &FontWidthManager::MaxCount, 10 );

	if( FontWidthManager::MaxCount < 4 ) FontWidthManager::MaxCount = 4;
	else
		if( FontWidthManager::MaxCount > 40 ) FontWidthManager::MaxCount = 40;

	FontWidthManager::Init( g_MadEditHomeDir );
	// create the main frame
	MadEditFrame *myFrame = new MadEditFrame( nullptr, 1, wxEmptyString, pos, size );

	if( !m_SilentMode )
	{
		bool maximize = false;
#ifdef __WXMSW__
		cfg->Read( wxT( "/UIView/WindowMaximize" ), &maximize, false );
#endif

		ShowMainFrame( myFrame, maximize );
	}
	else
	{
		if( !m_FileNames.IsEmpty() && !m_MadPythonScript.IsEmpty() )
		{
			// open the files
			wxTextFile scriptfile( m_MadPythonScript );
			scriptfile.Open( wxConvFile );

			if( scriptfile.IsOpened() )
			{
				wxString str = scriptfile.GetFirstLine() + wxT( "\n" );

				for( ; !scriptfile.Eof(); )
				{
					str << scriptfile.GetNextLine() << wxT( "\n" );
				}

				if( str.IsNull() == false )
				{
					for( size_t i = 0; i < m_FileNames.GetCount(); ++i )
					{
						myFrame->RunScriptWithFile( m_FileNames[i], str, false, true, m_ForceEdit, true );
					}
				}

				scriptfile.Close();
			}
		}

		SetTopWindow(myFrame);
		if( !bSingleInstance || m_Exit)
		{
			OnExit(); // Clean up
			return false;
		}

		// Waiting for -x to close
	}

	if(m_Exit) OnExit(); // Clean up
	return !m_Exit;
}

int MadEditApp::OnExit()
{
	// save settings in FrameClose();
	if( m_SigleAppChecker )
		delete m_SigleAppChecker;
	m_SigleAppChecker = nullptr;

	if( m_AppServer )
	{
		delete m_AppServer;
		m_AppServer = nullptr;
	}

	if( g_Locale )
		wxDELETE( g_Locale );
	g_Locale = nullptr;

	return 0;
}

void MadEditApp::OnInitCmdLine( wxCmdLineParser& cmdParser )
{
	cmdParser.SetDesc( g_cmdLineDesc );
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	cmdParser.SetSwitchChars( wxT( "-" ) );
}

bool MadEditApp::OnCmdLineParsed( wxCmdLineParser& cmdParser )
{
	m_SilentMode = cmdParser.Found( wxT( "s" ) );
	m_Exit = cmdParser.Found( wxT( "x" ) );
	m_ForceEdit  = cmdParser.Found( wxT( "f" ) );
	cmdParser.Found( wxT( "m" ), &m_MadPythonScript );
	cmdParser.Found( wxT( "d" ), &m_Delimiter );
	wxASSERT(m_Delimiter.Length() > 0);
	g_Delimiter = m_Delimiter[0];

	/*if( !m_MadPythonScript.IsEmpty() )
	{
		filename = m_MadPythonScript;

		if( ( filename.GetPath() ).IsEmpty() )
			m_MadPythonScript = g_MadEditHomeDir + wxT( "scripts" ) + wxFILE_SEP_PATH +  m_MadPythonScript;
	}*/

	// parse commandline to filenames, every file is with a trailing char g_MadConfigSeparator, ex: filename1|filename2|
	m_FileNames.Empty();
	// to get at your unnamed parameters use GetParam
	int flags = wxDIR_FILES | wxDIR_HIDDEN;
	wxString fname;
#ifdef __WXMSW__
	wxString escape(wxT("\\\\")), tfname, backslash(wxT("\\"));
#endif

	for( size_t i = 0; i < cmdParser.GetParamCount(); i++ )
	{
		fname = cmdParser.GetParam( i );
#ifdef __WXMSW__
		if( fname.StartsWith(escape, &tfname) )
		{
			tfname.Replace(escape, backslash);
			fname = escape + tfname;
		}
		else
		{
			fname.Replace(escape, backslash);
		}
#endif
		wxFileName filename(fname);

		filename.MakeAbsolute();
		fname = filename.GetFullName();

		if( cmdParser.Found( wxT( "w" ) ) )
		{
			//WildCard
			if( cmdParser.Found( wxT( "r" ) ) ) flags |= wxDIR_DIRS;

			wxArrayString files;
			size_t nums = wxDir::GetAllFiles( filename.GetPath(), &files, fname, flags );

			for( size_t j = 0; j < nums; ++j )
			{
				m_FileNames.Add( files[j] );
			}
		}
		else
		{
			// Support for name*linenum
			m_FileNames.Add( filename.GetFullPath() );
		}
	}

	// and other command line parameters
	// then do what you need with them.
	return true;
}

#if (wxUSE_ON_FATAL_EXCEPTION == 1) && (wxUSE_STACKWALKER == 1)
#include <wx/longlong.h>
void MadStackWalker::OnStackFrame( const wxStackFrame & frame )
{
	if( m_DumpFile )
	{
		wxULongLong address( ( size_t )frame.GetAddress() );
#if defined(__x86_64__) || defined(__LP64__) || defined(_WIN64)
		wxString fmt( wxT( "[%02u]:[%08X%08X] %s(%i)\t%s%s\n" ) );
#else
		wxString fmt( wxT( "[%02u]:[%08X] %s(%i)\t%s%s\n" ) );
#endif
		wxString paramInfo( wxT( "(" ) );
#if defined(_WIN32)
		wxString type, name, value;
		size_t count = frame.GetParamCount(), i = 0;

		while( i < count )
		{
			frame.GetParam( i, &type, &name, &value );
			paramInfo += type + wxT( " " ) + name + wxT( " = " ) + value;

			if( ++i < count ) paramInfo += wxT( ", " );
		}

#endif
		paramInfo += wxT( ")" );
		m_DumpFile->Write( wxString::Format( fmt,
											 ( unsigned )frame.GetLevel(),
#if defined(__x86_64__) || defined(__LP64__) || defined(_WIN64)
											 address.GetHi(),
#endif
											 address.GetLo(),
											 frame.GetFileName().c_str(),
											 ( unsigned )frame.GetLine(),
											 frame.GetName().c_str(),
											 paramInfo.c_str() )
						 );
	}
}

void MadEditApp::OnFatalException()
{
	wxString name = g_MadEditHomeDir + wxString::Format(
						wxT( "%s_%s_%lu.dmp" ),
						wxTheApp ? ( const wxChar* )wxTheApp->GetAppDisplayName().c_str()
						: wxT( "wxwindows" ),
						wxDateTime::Now().Format( wxT( "%Y%m%dT%H%M%S" ) ).c_str(),
#if defined(__WXMSW__)
						::GetCurrentProcessId()
#else
						( unsigned )getpid()
#endif
					);
	wxFile dmpFile( name.c_str(), wxFile::write );

	if( dmpFile.IsOpened() )
	{
		m_StackWalker.SetDumpFile( &dmpFile );
		m_StackWalker.WalkFromException();
		dmpFile.Close();
	}
}
#endif

void MadEditApp::ShowMainFrame( MadEditFrame *mainFrame, bool maximize )
{
	if( mainFrame )
	{
		// reload files previously opened
		wxString files;
		wxConfigBase *cfg = wxConfigBase::Get( false );
		cfg->Read( wxT( "/Application/ReloadFilesList" ), &files );

		if( !files.IsEmpty() )
		{
			// backward comptability
			wxUniChar dm(files.Last());
			if(dm != g_MadConfigSeparator)
			{
				files.Replace(wxString(dm), wxString(g_MadConfigSeparator));
			}

			// use OnReceiveMessage() to open the files
			OnReceiveMessage( files.c_str(), ( files.size() + 1 )*sizeof( wxChar ), false );
			int selid = mainFrame->OpenedFileCount();
			if(selid > 1)
			{
				selid -= 1;
				int nselid = cfg->ReadLong( wxT( "/Application/LastWorkingFile" ), selid );
				if( nselid >= 0 && selid > nselid )
					mainFrame->ActivateFile( nselid );
			}
		}

		files.Empty();

		for( size_t i = 0; i < m_FileNames.GetCount(); ++i )
		{
			//The name is what follows the last \ or /
			files +=  m_FileNames[i] + g_MadConfigSeparator;
		}

		if( !files.IsEmpty() )
		{
			if( !m_MadPythonScript.IsEmpty() )
			{
				files += g_MadEscParameter;
				files += wxT("s");

				if (m_ForceEdit)
				{
					files += g_MadEscParameter;
					files += wxT("f");
				}

				files += g_MadEscParameter;
				files += wxT( "m" ) + m_MadPythonScript;
			}

			// use OnReceiveMessage() to open the files
			OnReceiveMessage( files.c_str(), ( files.size() + 1 )*sizeof( wxChar ), false );
		}

		int pages = mainFrame->OpenedFileCount();
		
		if( pages == 0 )
		{
			mainFrame->OpenFile( wxEmptyString, false );
		}
		SetTopWindow( mainFrame );
#ifdef __WXMSW__
		//if(maximize)    // removed: gogo, 30.08.2009
		{
			WINDOWPLACEMENT wp;
			wp.length = sizeof( WINDOWPLACEMENT );
			GetWindowPlacement( ( HWND )mainFrame->GetHWND(), &wp );
			// changed: gogo, 30.08.2009
			//wp.showCmd=SW_SHOWMAXIMIZED;
			wp.showCmd = maximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
			SetWindowPlacement( ( HWND )mainFrame->GetHWND(), &wp );
		}
#endif
		mainFrame->Show( true );
	}
}

void ScanForLocales()
{
	for( long i = 0; i < g_LanguageCount; ++i )
	{
		g_EnhancedLangNameMap[g_LanguageValue[i]] = wxString( g_LanguageStr[i] );
		//g_EnhancedLangNameBMap.insert(mad_lang(g_LanguageValue[i], wxString(g_LanguageStr[i])));
	}

	g_LanguageString.Empty();
	g_LocaleDirPrefix.Empty();
	// System Default
	g_LanguageString.Add( wxGetTranslation(g_LanguageStr[0]) );
	g_LanguageId.Add( g_LanguageValue[0] );
	// English
	g_LanguageString.Add( g_LanguageStr[3] );
	g_LanguageId.Add( g_LanguageValue[3] );
	std::map<long, wxString> languageIdNameMap;
	MadTranslationHelper langScaner( g_MadLanguageFileName, languageIdNameMap );
	wxASSERT( !g_MadEditAppDir.IsEmpty() );
	wxString searchPath( g_MadEditAppDir + wxT( "locale" ) + wxFILE_SEP_PATH );

	if( wxDir::Exists( searchPath ) )
	{
		wxDir dir( searchPath );
		int flags = wxDIR_DIRS;
		g_LocaleDirPrefix.Add( searchPath );
		dir.Traverse( langScaner, wxEmptyString, flags );
	}

#ifndef __WXMSW__
	wxASSERT( !g_MadEditHomeDir.IsEmpty() );
	searchPath = g_MadEditHomeDir + wxT( "locale" ) + wxFILE_SEP_PATH;

	if( wxDir::Exists( searchPath ) )
	{
		wxDir dir( searchPath );
		int flags = wxDIR_DIRS;
		g_LocaleDirPrefix.Add( searchPath );
		dir.Traverse( langScaner, wxEmptyString, flags );
	}

#if defined (DATA_DIR)
	searchPath = wxT( DATA_DIR"/locale/" );

	if( wxDir::Exists( searchPath ) )
	{
		wxDir dir( searchPath );
		int flags = wxDIR_DIRS;
		g_LocaleDirPrefix.Add( searchPath );
		dir.Traverse( langScaner, wxEmptyString, flags );
	}

#endif
#endif
	std::map<long, wxString>::iterator it = languageIdNameMap.begin();

	while( it != languageIdNameMap.end() )
	{
		std::map<long, wxString>::iterator it_tmp = g_EnhancedLangNameMap.find( it->first );

		if( it_tmp != g_EnhancedLangNameMap.end() )
		{
			g_LanguageString.Add( it_tmp->second );
			g_LanguageId.Add( it_tmp->first );
		}
		else
		{
			g_LanguageString.Add( it->second );
			g_LanguageId.Add( it->first );
		}

		++it;
	}
}

void MadEditApp::InitLocale()
{
	wxString strlang;
	wxConfigBase *cfg = wxConfigBase::Get( false );
	cfg->Read( wxT( "/Application/Language" ), &strlang );
	int lang = g_LanguageValue[0];

	if( !strlang.IsEmpty() )
	{
		strlang.MakeLower();

		for( size_t idx = 1; idx < g_LanguageString.GetCount(); ++idx )
		{
			if( strlang == g_LanguageString[idx].Lower())
			{
				lang = g_LanguageId[idx];
				break;
			}
		}
	}

	if( g_Locale )
	{
		wxDELETE( g_Locale );
		g_Locale = 0;
	}

	g_Locale = new wxLocale( lang );

	// g_Locale.Init(lang);
	for( size_t idx = 0; idx < g_LocaleDirPrefix.GetCount(); ++idx )
	{
		g_Locale->AddCatalogLookupPathPrefix( g_LocaleDirPrefix[idx] );
	}

	g_Locale->AddCatalog( g_MadLanguageFileName );
}

