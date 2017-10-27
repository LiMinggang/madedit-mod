///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "MadOptionsDialog.h"


#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadOptionsDialog)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/aui/auibook.h>
#include <wx/fileconf.h>
#include <wx/config.h>

#include <wx/dir.h>
#include <wx/aui/auibook.h>

#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEditCommand.h"
#include "MadEdit/MadEdit.h"
#include "MadEdit/MadSyntax.h"
#include "MadEditFrame.h"
#include "MadUtils.h"
#include "MadEdit/MadEditPv.h"
#include "SpellCheckerManager.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

MadOptionsDialog *g_OptionsDialog = nullptr;
extern wxArrayString g_LanguageString;
extern wxString g_MadEditAppDir;
extern wxArrayString g_FontNames;

TreeItemData *g_SelectedCommandItem = nullptr;
int g_SelectedKeyId = -1;
TreeItemData *g_CommandItemOfNewKey = nullptr;
#define ENABLE_BITMAP_THUARI 0

class KeyTextCtrl : public wxTextCtrl
{
	MadOptionsDialog * m_OptDlg;
public:
	KeyTextCtrl() : m_OptDlg(nullptr)
	{
	}
	KeyTextCtrl(wxWindow *parent, wxWindowID id,
			   const wxString& value = wxEmptyString,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize,
			   long style = 0,
			   const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTextCtrlNameStr)
			   :wxTextCtrl(parent, id, value, pos, size, style, validator, name), m_OptDlg(nullptr)
	{
		Bind( wxEVT_KEY_DOWN, &KeyTextCtrl::OnKeyDown, this );
		Bind( wxEVT_SET_FOCUS, &KeyTextCtrl::OnSetFocus, this );
		Bind( wxEVT_KILL_FOCUS, &KeyTextCtrl::OnKillFocus, this );
	}

	void BindDialog(MadOptionsDialog * optDlg)
	{
		m_OptDlg = optDlg;
	}

	void OnKeyDown(wxKeyEvent& evt)
	{
		int flags=wxACCEL_NORMAL;
		int key=evt.GetKeyCode();

		if(evt.ControlDown()) flags|=wxACCEL_CTRL;
		if(evt.AltDown())     flags|=wxACCEL_ALT;
		if(evt.ShiftDown())   flags|=wxACCEL_SHIFT;

		MadEditShortCut sc=ShortCut(flags, key);
		static MadEditShortCut prevsc=0;

		if(sc==prevsc) return;
		prevsc=sc;

		wxString scstr=ShortCutToString(sc);
		SetValue(scstr);

		if(m_OptDlg)
			m_OptDlg->UpdateKeyHint();
		//evt.Skip();
	}
	
	void OnSetFocus(wxFocusEvent &evt)
	{   // for getting Ctrl-Tab
		if(m_OptDlg)
		{
			m_OptDlg->SetWindowStyleFlag(m_OptDlg->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->AuiNotebook1->wxControl::SetWindowStyleFlag(m_OptDlg->AuiNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->Panel4->SetWindowStyleFlag(m_OptDlg->Panel4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
			m_OptDlg->ButtonCancel->SetId(MadOptionsDialog::ID_BUTTONCANCEL);
		}
		evt.Skip();
	}
	
	void OnKillFocus(wxFocusEvent &evt)
	{   // restore wxTAB_TRAVERSAL
		if(m_OptDlg)
		{
			m_OptDlg->SetWindowStyleFlag(m_OptDlg->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->AuiNotebook1->wxControl::SetWindowStyleFlag(m_OptDlg->AuiNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->Panel4->SetWindowStyleFlag(m_OptDlg->Panel4->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
			m_OptDlg->ButtonCancel->SetId(wxID_CANCEL);
		}
		evt.Skip();
	}
};

// filter '&' and '.'
wxString FilterChar(const wxChar *ws)
{
	wxString str;
	wxChar wc;

	while((wc=*ws) != wxT('\0'))
	{
		if(wc!=wxT('&') && wc!=wxT('.'))
		{
			str << wc;
		}
		++ws;
	}

	return str;
}

//(*IdInit(MadOptionsDialog)
const long MadOptionsDialog::ID_COMBOBOXLANGUAGE = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT16 = wxNewId();
const long MadOptionsDialog::ID_COMBOBOXENCODING = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT13 = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXSKIPAUTOENCODING = wxNewId();
const long MadOptionsDialog::ID_COMBOBOXDEFAULTFONT = wxNewId();
const long MadOptionsDialog::ID_TEXTCTRLDEFAULTFONTSIZE = wxNewId();
const long MadOptionsDialog::ID_EDITMAXSIZETOLOAD = wxNewId();
const long MadOptionsDialog::ID_EDITMAXTEXTFILESIZE = wxNewId();
const long MadOptionsDialog::ID_MAXDISPLAYSIZE = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXSINGLEINSTANCE = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXRELOADFILES = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXRECORDCARETMOVEMENTS = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXRESTORECARETPOS = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXDONOTSAVESETTINGS = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPURGEHISTORY = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXENABLEAUTOSAVE = wxNewId();
const long MadOptionsDialog::ID_TEXTCTRLAUTOSAVETIMEOUT = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXAUTOBACKUP = wxNewId();
const long MadOptionsDialog::ID_WSCHECKBOXMSRIGHTCCLICKMENU = wxNewId();
const long MadOptionsDialog::ID_PANEL1 = wxNewId();
const long MadOptionsDialog::ID_EDITMAXLINELENGTH = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT3 = wxNewId();
const long MadOptionsDialog::ID_EDITMAXCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT4 = wxNewId();
const long MadOptionsDialog::ID_EDITTABCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT5 = wxNewId();
const long MadOptionsDialog::ID_EDITINDENTCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT6 = wxNewId();
const long MadOptionsDialog::ID_EDITDATETIME = wxNewId();
const long MadOptionsDialog::ID_STATICTEXTDATETIME = wxNewId();
const long MadOptionsDialog::ID_BUTTONDATETIME = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXDATETIMEINENGLISH = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXTABORSPACES = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXAUTOINDENT = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXAUTOCOMPLETEPAIR = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXINSERTPAIRFORSELECTION = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXMOUSESELECTTOCOPY = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXWHENPRESSCTRLKEY = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXMIDDLEMOUSETOPASTE = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXAUTOFILLCOLUMN = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXDCLICKHIGHLIGHT = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXLOCKCARETYPOS = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXFIXWIDTHMODE = wxNewId();
const long MadOptionsDialog::ID_PANEL2 = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCLINEENDINGCRLF = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCLINEENDINGLF = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCLINEENDINGCR = wxNewId();
const long MadOptionsDialog::ID_COMBOBOXNEWDOCSYNTAX = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCENCUTF8 = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXNEWDOCENCUTF8WITHBOM = wxNewId();
const long MadOptionsDialog::ID_RADIOBUTTONNEWDOCENCOTHER = wxNewId();
const long MadOptionsDialog::ID_COMBOBOXNEWDOCENCOTHER = wxNewId();
const long MadOptionsDialog::ID_COMBOBOXNEWDOCFONT = wxNewId();
const long MadOptionsDialog::ID_TEXTCTRLNEWDOCFONTSIZE = wxNewId();
const long MadOptionsDialog::ID_PANEL6 = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTSYNTAX = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTLINENUMBER = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTBOOKMARK = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTENDOFLINE = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTTABCHAR = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTSPACECHAR = wxNewId();
const long MadOptionsDialog::ID_RADIOBOXPRINTOFFSET = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTPAGEHEADER = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT7 = wxNewId();
const long MadOptionsDialog::ID_EDITHEADERLEFT = wxNewId();
const long MadOptionsDialog::ID_BUTTON1 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT8 = wxNewId();
const long MadOptionsDialog::ID_EDITHEADERCENTER = wxNewId();
const long MadOptionsDialog::ID_BUTTON2 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT9 = wxNewId();
const long MadOptionsDialog::ID_EDITHEADERRIGHT = wxNewId();
const long MadOptionsDialog::ID_BUTTON3 = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPRINTPAGEFOOTER = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT10 = wxNewId();
const long MadOptionsDialog::ID_EDITFOOTERLEFT = wxNewId();
const long MadOptionsDialog::ID_BUTTON4 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT11 = wxNewId();
const long MadOptionsDialog::ID_EDITFOOTERLEFTCENTER = wxNewId();
const long MadOptionsDialog::ID_BUTTON5 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT12 = wxNewId();
const long MadOptionsDialog::ID_EDITFOOTERRIGHT = wxNewId();
const long MadOptionsDialog::ID_BUTTON6 = wxNewId();
const long MadOptionsDialog::ID_PANEL3 = wxNewId();
const long MadOptionsDialog::ID_TREECTRL1 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXTCOMMANDHINT = wxNewId();
const long MadOptionsDialog::ID_EDITHINT = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT14 = wxNewId();
const long MadOptionsDialog::ID_LISTBOXKEYS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT15 = wxNewId();
const long MadOptionsDialog::ID_EDITKEY = wxNewId();
const long MadOptionsDialog::ID_EDITKEYHINT = wxNewId();
const long MadOptionsDialog::ID_BUTTONADDKEY = wxNewId();
const long MadOptionsDialog::ID_BUTTONDELETEKEY = wxNewId();
const long MadOptionsDialog::ID_BUTTONSHOWINMENU = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXRESETALLKEYS = wxNewId();
const long MadOptionsDialog::ID_PANEL4 = wxNewId();
const long MadOptionsDialog::ID_CHECKBOXPERSONALDICT = wxNewId();
const long MadOptionsDialog::ID_CHOICEDICTIONARY = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT17 = wxNewId();
const long MadOptionsDialog::ID_EDITDICTIONARYDIR = wxNewId();
const long MadOptionsDialog::ID_DICTIONARY_DIR = wxNewId();
const long MadOptionsDialog::ID_PANEL5 = wxNewId();
const long MadOptionsDialog::ID_AUINOTEBOOK1 = wxNewId();
const long MadOptionsDialog::ID_BUTTONOK = wxNewId();
const long MadOptionsDialog::ID_BUTTONCANCEL = wxNewId();
//*)

const long MadOptionsDialog:: ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 = wxNewId();
const long MadOptionsDialog:: ID_MNU___D__M__Y_24_02_2007_1192 = wxNewId();
const long MadOptionsDialog:: ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 = wxNewId();
const long MadOptionsDialog:: ID_MNU_MENUITEM1_1170 = wxNewId();
const long MadOptionsDialog:: ID_MNU___A_FULLWEEKDAYNAME_1171 = wxNewId();
const long MadOptionsDialog:: ID_MNU___B_ABBREVIATEDMONTHNAME_1172 = wxNewId();
const long MadOptionsDialog:: ID_MNU___B_FULLMONTHNAME_1173 = wxNewId();
const long MadOptionsDialog:: ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 = wxNewId();
const long MadOptionsDialog:: ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 = wxNewId();
const long MadOptionsDialog:: ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 = wxNewId();
const long MadOptionsDialog:: ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 = wxNewId();
const long MadOptionsDialog:: ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 = wxNewId();
const long MadOptionsDialog:: ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 = wxNewId();
const long MadOptionsDialog:: ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 = wxNewId();
const long MadOptionsDialog:: ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 = wxNewId();
const long MadOptionsDialog:: ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 = wxNewId();
const long MadOptionsDialog:: ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 = wxNewId();
const long MadOptionsDialog:: ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 = wxNewId();
const long MadOptionsDialog:: ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 = wxNewId();
const long MadOptionsDialog:: ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 = wxNewId();
const long MadOptionsDialog:: ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 = wxNewId();
const long MadOptionsDialog:: ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 = wxNewId();
const long MadOptionsDialog:: ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 = wxNewId();
const long MadOptionsDialog:: ID_MNU___Z_TIME_ZONENAME_1193 = wxNewId();
const long MadOptionsDialog:: ID_MNU___Z_TIME_ZONEABBREVIATION_1194 = wxNewId();

const long MadOptionsDialog:: ID_MNU_MENUITEM1_1110 = wxNewId();
const long MadOptionsDialog:: ID_MNU___P__PATHNAME_1111 = wxNewId();
const long MadOptionsDialog:: ID_MNU_MENUITEM3_1112 = wxNewId();
const long MadOptionsDialog:: ID_MNU___N_PAGE_NUMBER_1113 = wxNewId();
const long MadOptionsDialog:: ID_MNU___S__TOTALPAGES_1114 = wxNewId();
const long MadOptionsDialog:: ID_MNU_MENUITEM6_1115 = wxNewId();
const long MadOptionsDialog:: ID_MNU___D__DATE_1116 = wxNewId();
const long MadOptionsDialog:: ID_MNU___T__TIME_1117 = wxNewId();


MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_menu_evt_map[] = 
{
	{ ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___D__M__Y_24_02_2007_1192, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU_MENUITEM1_1170, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___A_FULLWEEKDAYNAME_1171, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___B_ABBREVIATEDMONTHNAME_1172, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___B_FULLMONTHNAME_1173, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Z_TIME_ZONENAME_1193, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU___Z_TIME_ZONEABBREVIATION_1194, &MadOptionsDialog::DateTimeMarkClick },
	{ ID_MNU_MENUITEM1_1110, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___P__PATHNAME_1111, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___N_PAGE_NUMBER_1113, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___S__TOTALPAGES_1114, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___D__DATE_1116, &MadOptionsDialog::PrintMarkClick },
	{ ID_MNU___T__TIME_1117, &MadOptionsDialog::PrintMarkClick },
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_button_evt_map[] = 
{
	{ ID_BUTTON1, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTON2, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTON3, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTON4, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTON5, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTON6, &MadOptionsDialog::PrintMarkButtonClick },
	{ ID_BUTTONCANCEL, &MadOptionsDialog::ButtonCancelClick },
	{ ID_BUTTONOK, &MadOptionsDialog::ButtonOKClick },
	//  {ID_BITMAP_DIR,&MadOptionsDialog::ButtonBitmapDirClick},
	//  {ID_THESAURI_DIR,&MadOptionsDialog::ButtonThesauriDirClick},
	{ ID_DICTIONARY_DIR, &MadOptionsDialog::ButtonDictionaryDirClick },
	{ ID_BUTTONSHOWINMENU, &MadOptionsDialog::ButtonShowInMenuClick },
	{ ID_BUTTONDELETEKEY, &MadOptionsDialog::ButtonDeleteKeyClick },
	{ ID_BUTTONADDKEY, &MadOptionsDialog::ButtonAddKeyClick },
	{ ID_BUTTONDATETIME, &MadOptionsDialog::ButtonDateTimeClick },
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_checkbox_evt_map[] =
{
	{ ID_CHECKBOXMOUSESELECTTOCOPY, &MadOptionsDialog::CheckBoxMouseSelectToCopyClick },

	{ ID_CHECKBOXAUTOCOMPLETEPAIR, &MadOptionsDialog::CheckBoxAutoCompletePairClick },
	{ ID_CHECKBOXENABLEAUTOSAVE, &MadOptionsDialog::EnableAutoSaveClick },
};

MadOptionsDialog::wxCmdEvtHandlerMap_t MadOptionsDialog::m_radiobutton_evt_map[] = 
{
	{ ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT, &MadOptionsDialog::RadioButtonNewDocEncSelect },
	{ ID_RADIOBUTTONNEWDOCENCUTF8, &MadOptionsDialog::RadioButtonNewDocEncSelect },
	{ ID_RADIOBUTTONNEWDOCENCOTHER, &MadOptionsDialog::RadioButtonNewDocEncSelect },
	{ ID_RADIOBUTTONNEWDOCLINEENDINGCRLF, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect },
	{ ID_RADIOBUTTONNEWDOCLINEENDINGLF, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect },
	{ ID_RADIOBUTTONNEWDOCLINEENDINGCR, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect },
	{ ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT, &MadOptionsDialog::RadioButtonNewDocLineEndingSelect },
};


#if defined(__WXMSW__) && (wxMAJOR_VERSION >= 3)
#define ADD2CONTROLS(ctrls, pWin) (ctrls).push_back((wxWindow *)(pWin)); 

#define SET_CONTROLPARENT(pWin) \
    {\
        LONG exStyle = wxGetWindowExStyle((wxWindow *)(pWin));\
        if ( !(exStyle & WS_EX_CONTROLPARENT) )\
        {\
            wxSetWindowExStyle((wxWindow *)(pWin), exStyle | WS_EX_CONTROLPARENT);\
        }\
    }
#else
#define ADD2CONTROLS(ctrls, pWin)
#define SET_CONTROLPARENT(pWin)
#endif

MadOptionsDialog::MadOptionsDialog(wxWindow* parent,wxWindowID id)
{
#if defined(__WXMSW__) && (wxMAJOR_VERSION >= 3)
	std::vector< wxWindow * > controls;
#endif
	//(*Initialize(MadOptionsDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer13;
	wxBoxSizer* BoxSizer14;
	wxBoxSizer* BoxSizer15;
	wxBoxSizer* BoxSizer16;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer19;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer20;
	wxBoxSizer* BoxSizer21;
	wxBoxSizer* BoxSizer22;
	wxBoxSizer* BoxSizer23;
	wxBoxSizer* BoxSizer24;
	wxBoxSizer* BoxSizer25;
	wxBoxSizer* BoxSizer26;
	wxBoxSizer* BoxSizer27;
	wxBoxSizer* BoxSizer28;
	wxBoxSizer* BoxSizer29;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer30;
	wxBoxSizer* BoxSizer31;
	wxBoxSizer* BoxSizer32;
	wxBoxSizer* BoxSizer33;
	wxBoxSizer* BoxSizer34;
	wxBoxSizer* BoxSizer35;
	wxBoxSizer* BoxSizer36;
	wxBoxSizer* BoxSizer37;
	wxBoxSizer* BoxSizer38;
	wxBoxSizer* BoxSizer39;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer40;
	wxBoxSizer* BoxSizer42;
	wxBoxSizer* BoxSizer43;
	wxBoxSizer* BoxSizer47;
	wxBoxSizer* BoxSizer49;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer50;
	wxBoxSizer* BoxSizer51;
	wxBoxSizer* BoxSizer52;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxGridSizer* GridSizer3;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxStaticBoxSizer* StaticBoxSizer9;
	wxStaticText* StaticText18;
	wxStaticText* StaticText19;
	wxStaticText* StaticText20;
	wxStaticText* StaticText23;
	wxStaticText* StaticText24;
	wxStaticText* StaticText25;
	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(900,400));

	SetMaxSize(wxSize(-1,-1));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer31 = new wxBoxSizer(wxVERTICAL);
	AuiNotebook1 = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
	Panel1 = new wxPanel(AuiNotebook1, ID_PANEL1, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer27->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	wxASSERT( g_LanguageString.GetCount() != 0 );
	ComboBoxLanguage = new wxComboBox( Panel1, ID_COMBOBOXLANGUAGE, wxEmptyString, wxDefaultPosition, wxSize(160,-1), g_LanguageString,  wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, wxT( "ID_COMBOBOXLANGUAGE" ) );
	ComboBoxLanguage->SetValue( g_LanguageString[0] );
	ADD2CONTROLS(controls, ComboBoxLanguage);
	BoxSizer27->Add(ComboBoxLanguage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText16 = new wxStaticText(Panel1, ID_STATICTEXT16, _("Language of User Interface (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	BoxSizer27->Add(StaticText16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer27, 0, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ComboBoxEncoding = new wxComboBox(Panel1, ID_COMBOBOXENCODING, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXENCODING"));
	ADD2CONTROLS(controls, ComboBoxEncoding);
	BoxSizer4->Add(ComboBoxEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, _("Use this encoding to create new file or when MadEdit cannot determine the encoding of old file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	BoxSizer4->Add(StaticText13, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxEXPAND, 2);
	BoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer40->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	CheckBoxSkipAutoEncoding = new wxCheckBox(Panel1, ID_CHECKBOXSKIPAUTOENCODING, _("Enforce default encoding"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXSKIPAUTOENCODING"));
	ADD2CONTROLS(controls, CheckBoxSkipAutoEncoding);
	CheckBoxSkipAutoEncoding->SetValue(false);
	BoxSizer40->Add(CheckBoxSkipAutoEncoding, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer40, 0, wxALL|wxEXPAND, 2);
	BoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer35->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ComboBoxDefaultFont = new wxComboBox(Panel1, ID_COMBOBOXDEFAULTFONT, wxEmptyString, wxDefaultPosition, wxSize(160,-1), g_FontNames, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXDEFAULTFONT"));
	ADD2CONTROLS(controls, ComboBoxDefaultFont);
	BoxSizer35->Add(ComboBoxDefaultFont, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText19 = new wxStaticText(Panel1, wxID_ANY, _("Default font"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer35->Add(StaticText19, 0, wxALL|wxEXPAND, 2);
	BoxSizer3->Add(BoxSizer35, 0, wxALL|wxEXPAND, 2);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	BoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer39->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditDefaultFontSize = new wxTextCtrl(Panel1, ID_TEXTCTRLDEFAULTFONTSIZE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLDEFAULTFONTSIZE"));
	ADD2CONTROLS(controls, EditDefaultFontSize);
	EditDefaultFontSize->SetMaxLength(2);
	BoxSizer39->Add(EditDefaultFontSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText20 = new wxStaticText(Panel1, wxID_ANY, _("Default font size"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer39->Add(StaticText20, 0, wxALL|wxEXPAND, 2);
	BoxSizer7->Add(BoxSizer39, 0, wxALL|wxEXPAND, 2);
	BoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer38->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxSizeToLoad = new wxTextCtrl(Panel1, ID_EDITMAXSIZETOLOAD, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITMAXSIZETOLOAD"));
	EditMaxSizeToLoad->SetMaxLength(128);
	ADD2CONTROLS(controls, EditMaxSizeToLoad);
	BoxSizer38->Add(EditMaxSizeToLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText1 = new wxStaticText(Panel1, wxID_ANY, _("Max file size to load whole file into memory"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer38->Add(StaticText1, 0, wxALL|wxEXPAND, 2);
	BoxSizer7->Add(BoxSizer38, 0, wxALL|wxEXPAND, 2);
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer17->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxTextFileSize = new wxTextCtrl(Panel1, ID_EDITMAXTEXTFILESIZE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITMAXTEXTFILESIZE"));
	EditMaxTextFileSize->SetMaxLength(128);
	ADD2CONTROLS(controls, EditMaxTextFileSize);
	BoxSizer17->Add(EditMaxTextFileSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText2 = new wxStaticText(Panel1, wxID_ANY, _("Max file size to load as a text file"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer17->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer17, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxDisplaySize = new wxTextCtrl(Panel1, ID_MAXDISPLAYSIZE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_MAXDISPLAYSIZE"));
	EditMaxDisplaySize->SetMaxLength(5);
	ADD2CONTROLS(controls, EditMaxDisplaySize);
	BoxSizer6->Add(EditMaxDisplaySize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText18 = new wxStaticText(Panel1, wxID_ANY, _("Max chars displayed in search results"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer6->Add(StaticText18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(BoxSizer6, 0, wxALL|wxEXPAND, 2);
	CheckBoxSingleInstance = new wxCheckBox(Panel1, ID_CHECKBOXSINGLEINSTANCE, _("Single Instance (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXSINGLEINSTANCE"));
	CheckBoxSingleInstance->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxSingleInstance);
	BoxSizer7->Add(CheckBoxSingleInstance, 0, wxALL|wxEXPAND, 2);
	CheckBoxReloadFiles = new wxCheckBox(Panel1, ID_CHECKBOXRELOADFILES, _("Reload files previously open on startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRELOADFILES"));
	CheckBoxReloadFiles->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxReloadFiles);
	BoxSizer7->Add(CheckBoxReloadFiles, 0, wxALL|wxEXPAND, 2);
	CheckBoxRecordCaretMovements = new wxCheckBox(Panel1, ID_CHECKBOXRECORDCARETMOVEMENTS, _("Record caret movements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECORDCARETMOVEMENTS"));
	CheckBoxRecordCaretMovements->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxRecordCaretMovements);
	BoxSizer7->Add(CheckBoxRecordCaretMovements, 0, wxALL|wxEXPAND, 2);
	CheckBoxRestoreCaretPos = new wxCheckBox(Panel1, ID_CHECKBOXRESTORECARETPOS, _("Restore previous caret position when files are opened"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRESTORECARETPOS"));
	CheckBoxRestoreCaretPos->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxRestoreCaretPos);
	BoxSizer7->Add(CheckBoxRestoreCaretPos, 0, wxALL|wxEXPAND, 2);
	CheckBoxDoNotSaveSettings = new wxCheckBox(Panel1, ID_CHECKBOXDONOTSAVESETTINGS, _("Do not save settings to MadEdit.cfg when MadEdit closed (this session only)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXDONOTSAVESETTINGS"));
	CheckBoxDoNotSaveSettings->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxDoNotSaveSettings);
	BoxSizer7->Add(CheckBoxDoNotSaveSettings, 0, wxALL|wxEXPAND, 2);
	CheckBoxPurgeHistory = new wxCheckBox(Panel1, ID_CHECKBOXPURGEHISTORY, _("Purge History while exiting"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPURGEHISTORY"));
	CheckBoxPurgeHistory->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPurgeHistory);
	BoxSizer7->Add(CheckBoxPurgeHistory, 0, wxALL|wxEXPAND, 2);
	CheckBoxEnableAutoSave = new wxCheckBox(Panel1, ID_CHECKBOXENABLEAUTOSAVE, _("Auto save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXENABLEAUTOSAVE"));
	CheckBoxEnableAutoSave->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxEnableAutoSave);
	BoxSizer7->Add(CheckBoxEnableAutoSave, 0, wxALL|wxEXPAND, 2);
	BoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer37->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditAutoSaveTimeout = new wxTextCtrl(Panel1, ID_TEXTCTRLAUTOSAVETIMEOUT, _T("10"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLAUTOSAVETIMEOUT"));
	EditAutoSaveTimeout->SetMaxLength(2);
	ADD2CONTROLS(controls, EditAutoSaveTimeout);
	BoxSizer37->Add(EditAutoSaveTimeout, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticTextAutoSaveTimeout = new wxStaticText(Panel1, wxID_ANY, _("Timeout(M)"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer37->Add(StaticTextAutoSaveTimeout, 0, wxALL|wxEXPAND, 2);
	BoxSizer7->Add(BoxSizer37, 0, wxALL|wxEXPAND, 2);
	CheckBoxEnableAutoBackup = new wxCheckBox(Panel1, ID_CHECKBOXAUTOBACKUP, _("Auto backup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXAUTOBACKUP"));
	CheckBoxEnableAutoBackup->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxEnableAutoBackup);
	BoxSizer7->Add(CheckBoxEnableAutoBackup, 0, wxALL|wxEXPAND, 2);
#ifdef __WXMSW__
	CheckBoxRightClickMenu = new wxCheckBox(Panel1, ID_WSCHECKBOXMSRIGHTCCLICKMENU, _("Add MadEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WSCHECKBOXMSRIGHTCCLICKMENU"));
	CheckBoxRightClickMenu->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxRightClickMenu);
	BoxSizer7->Add(CheckBoxRightClickMenu, 0, wxALL|wxEXPAND, 2);
#endif
	BoxSizer3->Add(BoxSizer7, 0, wxALL|wxEXPAND, 2);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
	Panel2 = new wxPanel(AuiNotebook1, ID_PANEL2, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer9->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxLineLength = new wxTextCtrl(Panel2, ID_EDITMAXLINELENGTH, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITMAXLINELENGTH"));
	ADD2CONTROLS(controls, EditMaxLineLength);
	BoxSizer9->Add(EditMaxLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Max line length before Line-Wrap (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer9->Add(StaticText3, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxEXPAND, 2);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer10->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditMaxColumns = new wxTextCtrl(Panel2, ID_EDITMAXCOLUMNS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITMAXCOLUMNS"));
	ADD2CONTROLS(controls, EditMaxColumns);
	BoxSizer10->Add(EditMaxColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT4, _("Columns of Wrap-By-Column"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer10->Add(StaticText4, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer10, 0, wxALL|wxEXPAND, 2);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditTabColumns = new wxTextCtrl(Panel2, ID_EDITTABCOLUMNS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITTABCOLUMNS"));
	ADD2CONTROLS(controls, EditTabColumns);
	BoxSizer11->Add(EditTabColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Columns of Tab"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer11->Add(StaticText5, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer11, 0, wxALL|wxEXPAND, 2);
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer13->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditIndentColumns = new wxTextCtrl(Panel2, ID_EDITINDENTCOLUMNS, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITINDENTCOLUMNS"));
	ADD2CONTROLS(controls, EditIndentColumns);
	BoxSizer13->Add(EditIndentColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText6 = new wxStaticText(Panel2, ID_STATICTEXT6, _("Columns of Indent"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer13->Add(StaticText6, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer13, 0, wxALL|wxEXPAND, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	EditDateTime = new wxTextCtrl(Panel2, ID_EDITDATETIME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_EDITDATETIME"));
	ADD2CONTROLS(controls, EditDateTime);
	BoxSizer5->Add(EditDateTime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticTextDateTime = new wxStaticText(Panel2, ID_STATICTEXTDATETIME, _("Format of Date/Time"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTDATETIME"));
	BoxSizer5->Add(StaticTextDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	ButtonDateTime = new wxButton(Panel2, ID_BUTTONDATETIME, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTONDATETIME"));
	ADD2CONTROLS(controls, ButtonDateTime);
	BoxSizer5->Add(ButtonDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxDateTimeInEnglish = new wxCheckBox(Panel2, ID_CHECKBOXDATETIMEINENGLISH, _("Use English instead of current locale"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXDATETIMEINENGLISH"));
	CheckBoxDateTimeInEnglish->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxDateTimeInEnglish);
	BoxSizer5->Add(CheckBoxDateTimeInEnglish, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	CheckBoxTabOrSpaces = new wxCheckBox(Panel2, ID_CHECKBOXTABORSPACES, _("Insert Space char instead of Tab char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXTABORSPACES"));
	CheckBoxTabOrSpaces->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxTabOrSpaces);
	BoxSizer12->Add(CheckBoxTabOrSpaces, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoIndent = new wxCheckBox(Panel2, ID_CHECKBOXAUTOINDENT, _("Auto Indent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXAUTOINDENT"));
	CheckBoxAutoIndent->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxAutoIndent);
	BoxSizer12->Add(CheckBoxAutoIndent, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoCompletePair = new wxCheckBox(Panel2, ID_CHECKBOXAUTOCOMPLETEPAIR, _("Auto complete character pair"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXAUTOCOMPLETEPAIR"));
	CheckBoxAutoCompletePair->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxAutoCompletePair);
	BoxSizer12->Add(CheckBoxAutoCompletePair, 0, wxALL|wxEXPAND, 2);
	BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer23->Add(10,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxInsertPairForSelection = new wxCheckBox(Panel2, ID_CHECKBOXINSERTPAIRFORSELECTION, _("Insert pair instead of replacing selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXINSERTPAIRFORSELECTION"));
	CheckBoxInsertPairForSelection->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxInsertPairForSelection);
	BoxSizer23->Add(CheckBoxInsertPairForSelection, 1, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer23, 1, wxALL|wxEXPAND, 0);
	BoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	CheckBoxMouseSelectToCopy = new wxCheckBox(Panel2, ID_CHECKBOXMOUSESELECTTOCOPY, _("Auto copy the mouse-selected text to clipboard"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXMOUSESELECTTOCOPY"));
	CheckBoxMouseSelectToCopy->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxMouseSelectToCopy);
	BoxSizer28->Add(CheckBoxMouseSelectToCopy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	CheckBoxCtrlWithMouseToSelect = new wxCheckBox(Panel2, ID_CHECKBOXWHENPRESSCTRLKEY, _("when pressing Ctrl key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXWHENPRESSCTRLKEY"));
	CheckBoxCtrlWithMouseToSelect->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxCtrlWithMouseToSelect);
	BoxSizer28->Add(CheckBoxCtrlWithMouseToSelect, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer28, 0, wxALL|wxALIGN_LEFT, 0);
	CheckBoxMiddleMouseToPaste = new wxCheckBox(Panel2, ID_CHECKBOXMIDDLEMOUSETOPASTE, _("Paste text from clipboard when pressing middle mouse button"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXMIDDLEMOUSETOPASTE"));
	CheckBoxMiddleMouseToPaste->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxMiddleMouseToPaste);
	BoxSizer12->Add(CheckBoxMiddleMouseToPaste, 0, wxALL|wxEXPAND, 2);
	CheckBoxAutoFillColumnPaste = new wxCheckBox(Panel2, ID_CHECKBOXAUTOFILLCOLUMN, _("Auto fill in column paste"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXAUTOFILLCOLUMN"));
	CheckBoxAutoFillColumnPaste->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxAutoFillColumnPaste);
	BoxSizer12->Add(CheckBoxAutoFillColumnPaste, 0, wxALL|wxEXPAND, 2);
	CheckBoxLDClickHighlight = new wxCheckBox(Panel2, ID_CHECKBOXDCLICKHIGHLIGHT, _("Left double click to highlight"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXDCLICKHIGHLIGHT"));
	CheckBoxLDClickHighlight->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxLDClickHighlight);
	BoxSizer12->Add(CheckBoxLDClickHighlight, 0, wxALL|wxEXPAND, 2);
	CheckBoxTypewriterMode = new wxCheckBox(Panel2, ID_CHECKBOXLOCKCARETYPOS, _("Typewriter mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXLOCKCARETYPOS"));
	CheckBoxTypewriterMode->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxTypewriterMode);
	BoxSizer12->Add(CheckBoxTypewriterMode, 0, wxALL|wxEXPAND, 2);
	CheckBoxFixWidthMode = new wxCheckBox(Panel2, ID_CHECKBOXFIXWIDTHMODE, _("Fix width mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXFIXWIDTHMODE"));
	CheckBoxFixWidthMode->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxFixWidthMode);
	BoxSizer12->Add(CheckBoxFixWidthMode, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer12, 0, wxALL|wxEXPAND, 2);
	Panel2->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel2);
	BoxSizer8->SetSizeHints(Panel2);
	Panel6 = new wxPanel(AuiNotebook1, ID_PANEL6, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	BoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer43 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxVERTICAL, Panel6, _("Line Ending"));
	RadioButtonNewDocLineEndingDefault = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT, _("System Default"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT"));
	ADD2CONTROLS(controls, RadioButtonNewDocLineEndingDefault);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingDefault, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingCRLF = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCLINEENDINGCRLF, _T("Windows(CR LF)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCLINEENDINGCRLF"));
	ADD2CONTROLS(controls, RadioButtonNewDocLineEndingCRLF);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingCRLF, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingLF = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCLINEENDINGLF, _T("Unix(LF)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCLINEENDINGLF"));
	ADD2CONTROLS(controls, RadioButtonNewDocLineEndingLF);
	//ADD2CONTROLS(controls, RadioBoxLineEnding);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingLF, 0, wxALL|wxEXPAND, 2);
	RadioButtonNewDocLineEndingCR = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCLINEENDINGCR, _T("Macintosh"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCLINEENDINGCR"));
	ADD2CONTROLS(controls, RadioButtonNewDocLineEndingCR);
	StaticBoxSizer9->Add(RadioButtonNewDocLineEndingCR, 0, wxALL|wxEXPAND, 2);
	BoxSizer43->Add(StaticBoxSizer9, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer49 = new wxBoxSizer(wxVERTICAL);
	StaticText23 = new wxStaticText(Panel6, wxID_ANY, _("Syntax:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer49->Add(StaticText23, 0, wxALL|wxEXPAND, 2);
	ComboBoxNewDocSyntax = new wxComboBox(Panel6, ID_COMBOBOXNEWDOCSYNTAX, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXNEWDOCSYNTAX"));
	ADD2CONTROLS(controls, ComboBoxNewDocSyntax);
	BoxSizer49->Add(ComboBoxNewDocSyntax, 0, wxALL|wxEXPAND, 2);
	BoxSizer43->Add(BoxSizer49, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer42->Add(BoxSizer43, 0, wxALL|wxALIGN_TOP, 5);
	BoxSizer47 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxVERTICAL, Panel6, _("Encoding"));
	RadioButtonNewDocEncSystemDefault = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT, _("System Default"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT"));
	ADD2CONTROLS(controls, RadioButtonNewDocEncSystemDefault);
	StaticBoxSizer10->Add(RadioButtonNewDocEncSystemDefault, 0, wxALL|wxEXPAND, 2);
	GridSizer3 = new wxGridSizer(0, 2, 0, 0);
	RadioButtonNewDocEncUTF8 = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCENCUTF8, _T("UTF8"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCENCUTF8"));
	ADD2CONTROLS(controls, RadioButtonNewDocEncUTF8);
	GridSizer3->Add(RadioButtonNewDocEncUTF8, 0, wxALL|wxEXPAND, 2);
	CheckBoxNewDocEncUTF8WithBOM = new wxCheckBox(Panel6, ID_CHECKBOXNEWDOCENCUTF8WITHBOM, _("with BOM"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXNEWDOCENCUTF8WITHBOM"));
	CheckBoxNewDocEncUTF8WithBOM->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxNewDocEncUTF8WithBOM);
	GridSizer3->Add(CheckBoxNewDocEncUTF8WithBOM, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer10->Add(GridSizer3, 1, wxALL|wxALIGN_LEFT, 0);
	BoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
	RadioButtonNewDocEncOther = new wxRadioButton(Panel6, ID_RADIOBUTTONNEWDOCENCOTHER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONNEWDOCENCOTHER"));
	ADD2CONTROLS(controls, RadioButtonNewDocEncOther);
	BoxSizer50->Add(RadioButtonNewDocEncOther, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ComboBoxNewDocEncOther = new wxComboBox(Panel6, ID_COMBOBOXNEWDOCENCOTHER, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXNEWDOCENCOTHER"));
	ADD2CONTROLS(controls, ComboBoxNewDocEncOther);
	BoxSizer50->Add(ComboBoxNewDocEncOther, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer10->Add(BoxSizer50, 0, wxALL|wxEXPAND, 0);
	BoxSizer47->Add(StaticBoxSizer10, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer51 = new wxBoxSizer(wxVERTICAL);
	StaticText24 = new wxStaticText(Panel6, wxID_ANY, _("Font:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer51->Add(StaticText24, 0, wxALL|wxEXPAND, 2);
	ComboBoxNewDocFont = new wxComboBox(Panel6, ID_COMBOBOXNEWDOCFONT, wxEmptyString, wxDefaultPosition, wxSize(160,-1), g_FontNames, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXNEWDOCFONT"));
	ADD2CONTROLS(controls, ComboBoxNewDocFont);
	BoxSizer51->Add(ComboBoxNewDocFont, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer47->Add(BoxSizer51, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer52 = new wxBoxSizer(wxVERTICAL);
	StaticText25 = new wxStaticText(Panel6, wxID_ANY, _("Font Size:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	BoxSizer52->Add(StaticText25, 0, wxALL|wxEXPAND, 2);
	EditNewDocFontSize = new wxTextCtrl(Panel6, ID_TEXTCTRLNEWDOCFONTSIZE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLNEWDOCFONTSIZE"));
	EditNewDocFontSize->SetMaxLength(2);
	ADD2CONTROLS(controls, EditNewDocFontSize);
	BoxSizer52->Add(EditNewDocFontSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer47->Add(BoxSizer52, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer42->Add(BoxSizer47, 0, wxALL|wxALIGN_TOP, 5);
	Panel6->SetSizer(BoxSizer42);
	BoxSizer42->Fit(Panel6);
	BoxSizer42->SetSizeHints(Panel6);
	Panel3 = new wxPanel(AuiNotebook1, ID_PANEL3, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer15 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Text Mode"));
	CheckBoxPrintSyntax = new wxCheckBox(Panel3, ID_CHECKBOXPRINTSYNTAX, _("Print Syntax Highlighter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTSYNTAX"));
	CheckBoxPrintSyntax->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintSyntax);
	StaticBoxSizer1->Add(CheckBoxPrintSyntax, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintLineNumber = new wxCheckBox(Panel3, ID_CHECKBOXPRINTLINENUMBER, _("Print Line Number"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTLINENUMBER"));
	CheckBoxPrintLineNumber->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintLineNumber);
	StaticBoxSizer1->Add(CheckBoxPrintLineNumber, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintBookmark = new wxCheckBox(Panel3, ID_CHECKBOXPRINTBOOKMARK, _("Print Bookmark"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTBOOKMARK"));
	CheckBoxPrintBookmark->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintBookmark);
	StaticBoxSizer1->Add(CheckBoxPrintBookmark, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintEndOfLine = new wxCheckBox(Panel3, ID_CHECKBOXPRINTENDOFLINE, _("Print End of Line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTENDOFLINE"));
	CheckBoxPrintEndOfLine->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintEndOfLine);
	StaticBoxSizer1->Add(CheckBoxPrintEndOfLine, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintTabChar = new wxCheckBox(Panel3, ID_CHECKBOXPRINTTABCHAR, _("Print Tab Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTTABCHAR"));
	CheckBoxPrintTabChar->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintTabChar);
	StaticBoxSizer1->Add(CheckBoxPrintTabChar, 0, wxALL|wxALIGN_LEFT, 2);
	CheckBoxPrintSpaceChar = new wxCheckBox(Panel3, ID_CHECKBOXPRINTSPACECHAR, _("Print Space Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTSPACECHAR"));
	CheckBoxPrintSpaceChar->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintSpaceChar);
	StaticBoxSizer1->Add(CheckBoxPrintSpaceChar, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer15->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Hex Mode"));
	wxString __wxRadioBoxChoices_1[3] =
	{
		_("None"),
		_("First Page Only"),
		_("Every Page")
	};
	RadioBoxPrintOffset = new wxRadioBox(Panel3, ID_RADIOBOXPRINTOFFSET, _("Print Offset Header"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_RADIOBOXPRINTOFFSET"));
	ADD2CONTROLS(controls, RadioBoxPrintOffset);
	StaticBoxSizer2->Add(RadioBoxPrintOffset, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer15->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer15, 0, wxALL|wxALIGN_TOP, 2);
	BoxSizer16 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Header"));
	CheckBoxPrintPageHeader = new wxCheckBox(Panel3, ID_CHECKBOXPRINTPAGEHEADER, _("Print Page Header"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTPAGEHEADER"));
	CheckBoxPrintPageHeader->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintPageHeader);
	StaticBoxSizer3->Add(CheckBoxPrintPageHeader, 0, wxALL|wxALIGN_LEFT, 1);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText7 = new wxStaticText(Panel3, ID_STATICTEXT7, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderLeft = new wxTextCtrl(Panel3, ID_EDITHEADERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITHEADERLEFT"));
	ADD2CONTROLS(controls, EditHeaderLeft);
	FlexGridSizer1->Add(EditHeaderLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button1 = new wxButton(Panel3, ID_BUTTON1, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	ADD2CONTROLS(controls, Button1);
	FlexGridSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText8 = new wxStaticText(Panel3, ID_STATICTEXT8, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderCenter = new wxTextCtrl(Panel3, ID_EDITHEADERCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITHEADERCENTER"));
	ADD2CONTROLS(controls, EditHeaderCenter);
	FlexGridSizer1->Add(EditHeaderCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button2 = new wxButton(Panel3, ID_BUTTON2, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	ADD2CONTROLS(controls, Button2);
	FlexGridSizer1->Add(Button2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText9 = new wxStaticText(Panel3, ID_STATICTEXT9, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditHeaderRight = new wxTextCtrl(Panel3, ID_EDITHEADERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITHEADERRIGHT"));
	ADD2CONTROLS(controls, EditHeaderRight);
	FlexGridSizer1->Add(EditHeaderRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button3 = new wxButton(Panel3, ID_BUTTON3, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	ADD2CONTROLS(controls, Button3);
	FlexGridSizer1->Add(Button3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer3->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Footer"));
	CheckBoxPrintPageFooter = new wxCheckBox(Panel3, ID_CHECKBOXPRINTPAGEFOOTER, _("Print Page Footer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTPAGEFOOTER"));
	CheckBoxPrintPageFooter->SetValue(false);
	ADD2CONTROLS(controls, CheckBoxPrintPageFooter);
	StaticBoxSizer4->Add(CheckBoxPrintPageFooter, 0, wxALL|wxALIGN_LEFT, 2);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText10 = new wxStaticText(Panel3, ID_STATICTEXT10, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(StaticText10, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterLeft = new wxTextCtrl(Panel3, ID_EDITFOOTERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITFOOTERLEFT"));
	ADD2CONTROLS(controls, EditFooterLeft);
	FlexGridSizer2->Add(EditFooterLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button4 = new wxButton(Panel3, ID_BUTTON4, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	ADD2CONTROLS(controls, Button4);
	FlexGridSizer2->Add(Button4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText11 = new wxStaticText(Panel3, ID_STATICTEXT11, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer2->Add(StaticText11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterCenter = new wxTextCtrl(Panel3, ID_EDITFOOTERLEFTCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITFOOTERLEFTCENTER"));
	ADD2CONTROLS(controls, EditFooterCenter);
	FlexGridSizer2->Add(EditFooterCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button5 = new wxButton(Panel3, ID_BUTTON5, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	ADD2CONTROLS(controls, Button5);
	FlexGridSizer2->Add(Button5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText12 = new wxStaticText(Panel3, ID_STATICTEXT12, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer2->Add(StaticText12, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	EditFooterRight = new wxTextCtrl(Panel3, ID_EDITFOOTERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_EDITFOOTERRIGHT"));
	ADD2CONTROLS(controls, EditFooterRight);
	FlexGridSizer2->Add(EditFooterRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button6 = new wxButton(Panel3, ID_BUTTON6, _T(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	ADD2CONTROLS(controls, Button6);
	FlexGridSizer2->Add(Button6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer4->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer16, 0, wxALL|wxALIGN_TOP, 2);
	Panel3->SetSizer(BoxSizer14);
	BoxSizer14->Fit(Panel3);
	BoxSizer14->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, ID_PANEL4, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	TreeCtrl1 = new wxTreeCtrl(Panel4, ID_TREECTRL1, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
	ADD2CONTROLS(controls, TreeCtrl1);
	BoxSizer19->Add(TreeCtrl1, 1, wxALL|wxEXPAND, 3);
	BoxSizer18->Add(BoxSizer19, 2, wxALL|wxEXPAND, 4);
	BoxSizer20 = new wxBoxSizer(wxVERTICAL);
	BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	StaticTextCommandHint = new wxStaticText(Panel4, ID_STATICTEXTCOMMANDHINT, _("Hint:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTCOMMANDHINT"));
	BoxSizer24->Add(StaticTextCommandHint, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	EditCommandHint = new wxTextCtrl(Panel4, ID_EDITHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_EDITHINT"));
	ADD2CONTROLS(controls, EditCommandHint);
	EditCommandHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer24->Add(EditCommandHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer20->Add(BoxSizer24, 0, wxALL|wxEXPAND, 2);
	BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer22 = new wxBoxSizer(wxVERTICAL);
	StaticText14 = new wxStaticText(Panel4, ID_STATICTEXT14, _("Assigned Keys:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	BoxSizer22->Add(StaticText14, 0, wxALL|wxALIGN_LEFT, 5);
	ListBoxKeys = new wxListBox(Panel4, ID_LISTBOXKEYS, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("ID_LISTBOXKEYS"));
	ADD2CONTROLS(controls, ListBoxKeys);
	BoxSizer22->Add(ListBoxKeys, 1, wxALL|wxEXPAND, 2);
	BoxSizer21->Add(BoxSizer22, 0, wxALL|wxEXPAND, 2);
	BoxSizer25 = new wxBoxSizer(wxVERTICAL);
	StaticText15 = new wxStaticText(Panel4, ID_STATICTEXT15, _("New Key:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	BoxSizer25->Add(StaticText15, 0, wxALL|wxALIGN_LEFT, 5);
	EditKey = new KeyTextCtrl(Panel4, ID_EDITKEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxWANTS_CHARS, wxDefaultValidator, _T("ID_EDITKEY"));
	ADD2CONTROLS(controls, EditKey);
	BoxSizer25->Add(EditKey, 0, wxALL|wxEXPAND, 5);
	EditKeyHint = new wxTextCtrl(Panel4, ID_EDITKEYHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_EDITKEYHINT"));
	ADD2CONTROLS(controls, EditKeyHint);
	EditKeyHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer25->Add(EditKeyHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer29 = new wxBoxSizer(wxVERTICAL);
	ButtonAddKey = new wxButton(Panel4, ID_BUTTONADDKEY, _("<== Add Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONADDKEY"));
	ADD2CONTROLS(controls, ButtonAddKey);
	BoxSizer29->Add(ButtonAddKey, 0, wxALL|wxEXPAND, 5);
	ButtonDeleteKey = new wxButton(Panel4, ID_BUTTONDELETEKEY, _("==> Delete Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDELETEKEY"));
	ADD2CONTROLS(controls, ButtonDeleteKey);
	BoxSizer29->Add(ButtonDeleteKey, 0, wxALL|wxEXPAND, 5);
	ButtonShowInMenu = new wxButton(Panel4, ID_BUTTONSHOWINMENU, _("Show the Key in Menu"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSHOWINMENU"));
	ADD2CONTROLS(controls, ButtonShowInMenu);
	BoxSizer29->Add(ButtonShowInMenu, 0, wxALL|wxEXPAND, 5);
	BoxSizer25->Add(BoxSizer29, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer21->Add(BoxSizer25, 5, wxALL|wxEXPAND, 1);
	BoxSizer20->Add(BoxSizer21, 1, wxALL|wxEXPAND, 2);
	BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	CheckBoxResetAllKeys = new wxCheckBox(Panel4, ID_CHECKBOXRESETALLKEYS, _("Reset all keys to default (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRESETALLKEYS"));
	ADD2CONTROLS(controls, CheckBoxResetAllKeys);
	CheckBoxResetAllKeys->SetValue(false);
	BoxSizer26->Add(CheckBoxResetAllKeys, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer20->Add(BoxSizer26, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer18->Add(BoxSizer20, 3, wxALL|wxEXPAND, 2);
	Panel4->SetSizer(BoxSizer18);
	BoxSizer18->Fit(Panel4);
	BoxSizer18->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, ID_PANEL5, wxDefaultPosition, wxSize(792,400), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	BoxSizer36 = new wxBoxSizer(wxVERTICAL);
	BoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer34->Add(3,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CheckBoxPersonalDict = new wxCheckBox(Panel5, ID_CHECKBOXPERSONALDICT, _("Enable Personal Dictionary"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPERSONALDICT"));
	ADD2CONTROLS(controls, CheckBoxPersonalDict);
	CheckBoxPersonalDict->SetValue(false);
	BoxSizer34->Add(CheckBoxPersonalDict, 0, wxALL|wxEXPAND, 2);
	BoxSizer36->Add(BoxSizer34, 0, wxALL|wxALIGN_LEFT, 2);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Language"));
	ChoiceDictionary = new wxChoice(Panel5, ID_CHOICEDICTIONARY, wxDefaultPosition, wxSize(200,-1), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEDICTIONARY"));
	StaticBoxSizer5->Add(ChoiceDictionary, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ADD2CONTROLS(controls, ChoiceDictionary);
	StaticText17 = new wxStaticText(Panel5, ID_STATICTEXT17, _("Dictionary"), wxDefaultPosition, wxSize(200,-1), 0, _T("ID_STATICTEXT17"));
	StaticBoxSizer5->Add(StaticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(StaticBoxSizer5, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Path Setting"));
	EditDictionaryDir = new wxTextCtrl(Panel5, ID_EDITDICTIONARYDIR, wxEmptyString, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("ID_EDITDICTIONARYDIR"));
	ADD2CONTROLS(controls, EditDictionaryDir);
	StaticBoxSizer6->Add(EditDictionaryDir, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ButtonDictionaryDir = new wxButton(Panel5, ID_DICTIONARY_DIR, _T("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DICTIONARY_DIR"));
	ADD2CONTROLS(controls, ButtonDictionaryDir);
	StaticBoxSizer6->Add(ButtonDictionaryDir, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(StaticBoxSizer6, 0, wxALL|wxEXPAND, 2);
	Panel5->SetSizer(BoxSizer36);
	BoxSizer36->Fit(Panel5);
	BoxSizer36->SetSizeHints(Panel5);
	AuiNotebook1->AddPage(Panel1, _("General"));
	AuiNotebook1->AddPage(Panel2, _("Edit"));
	AuiNotebook1->AddPage(Panel6, _("New Document"));
	AuiNotebook1->AddPage(Panel3, _("Print"));
	AuiNotebook1->AddPage(Panel4, _("Keys"));
	AuiNotebook1->AddPage(Panel5, _("SpellChecker"));
	BoxSizer31->Add(AuiNotebook1, 1, wxALL|wxEXPAND, 0);
	BoxSizer32 = new wxBoxSizer(wxVERTICAL);
	BoxSizer32->Add(450,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer31->Add(BoxSizer32, 0, wxALL|wxEXPAND, 0);
	BoxSizer30->Add(BoxSizer31, 1, wxALL|wxEXPAND, 0);
	BoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer33->Add(0,260,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer30->Add(BoxSizer33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(BoxSizer30, 1, wxALL|wxEXPAND, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOK = new wxButton(this, ID_BUTTONOK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	BoxSizer2->Add(ButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTONCANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer2->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
#if 0
	Layout();
	Center();
	Connect(ID_CHECKBOXENABLEAUTOSAVE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::EnableAutoSaveClick);
	Connect(ID_BUTTONDATETIME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDateTimeClick);
	Connect(ID_CHECKBOXAUTOCOMPLETEPAIR,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::CheckBoxAutoCompletePairClick);
	Connect(ID_CHECKBOXMOUSESELECTTOCOPY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::CheckBoxMouseSelectToCopyClick);
	Connect(ID_RADIOBUTTONNEWDOCLINEENDINGDEFAULT,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(ID_RADIOBUTTONNEWDOCLINEENDINGCRLF,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(ID_RADIOBUTTONNEWDOCLINEENDINGLF,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(ID_RADIOBUTTONNEWDOCLINEENDINGCR,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocLineEndingSelect);
	Connect(ID_RADIOBUTTONNEWDOCENCSYSTEMDEFAULT,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(ID_RADIOBUTTONNEWDOCENCUTF8,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(ID_RADIOBUTTONNEWDOCENCOTHER,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::RadioButtonNewDocEncSelect);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&MadOptionsDialog::TreeCtrl1SelChanged);
	Connect(ID_LISTBOXKEYS,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::ListBoxKeysSelected);
	Connect(ID_BUTTONADDKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonAddKeyClick);
	Connect(ID_BUTTONDELETEKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDeleteKeyClick);
	Connect(ID_BUTTONSHOWINMENU,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonShowInMenuClick);
	Connect(ID_CHOICEDICTIONARY,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::ChoiceDictionarySelect);
	Connect(ID_EDITDICTIONARYDIR,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&MadOptionsDialog::EditDictionaryDirTextEnter);
	Connect(ID_DICTIONARY_DIR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonDictionaryDirClick);
	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonOKClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::ButtonCancelClick);
#endif
	//*)

	for(size_t i = 0; i < sizeof(m_menu_evt_map)/sizeof(m_menu_evt_map[0]); ++i)
	{
		Bind( wxEVT_MENU, m_menu_evt_map[i].method, this, m_menu_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_button_evt_map)/sizeof(m_button_evt_map[0]); ++i)
	{
		Bind( wxEVT_BUTTON, m_button_evt_map[i].method, this, m_button_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_checkbox_evt_map)/sizeof(m_checkbox_evt_map[0]); ++i)
	{
		Bind( wxEVT_CHECKBOX, m_checkbox_evt_map[i].method, this, m_checkbox_evt_map[i].evtTag );
	}

	for(size_t i = 0; i < sizeof(m_radiobutton_evt_map)/sizeof(m_radiobutton_evt_map[0]); ++i)
	{
		Bind( wxEVT_COMMAND_RADIOBUTTON_SELECTED, m_radiobutton_evt_map[i].method, this, m_radiobutton_evt_map[i].evtTag );
	}

	Bind( wxEVT_CLOSE_WINDOW, &MadOptionsDialog::MadOptionsDialogClose , this );
	Bind( wxEVT_ACTIVATE, &MadOptionsDialog::MadOptionsDialogActivate , this );

	Bind( wxEVT_TEXT_ENTER, &MadOptionsDialog::EditDictionaryDirTextEnter, this, ID_EDITDICTIONARYDIR );
	Bind( wxEVT_CHOICE, &MadOptionsDialog::ChoiceDictionarySelect, this, ID_CHOICEDICTIONARY );
	Bind( wxEVT_LISTBOX, &MadOptionsDialog::ListBoxKeysSelected , this, ID_LISTBOXKEYS );

	Bind( wxEVT_TREE_SEL_CHANGED, &MadOptionsDialog::TreeCtrl1SelChanged , this, ID_TREECTRL1 );

	PopupMenuPrintMark.Append(ID_MNU_MENUITEM1_1110, _("[%f] &File Name"), _T(""), wxITEM_NORMAL);
	PopupMenuPrintMark.Append(ID_MNU___P__PATHNAME_1111, _("[%p] &Path Name"), _T(""), wxITEM_NORMAL);
	PopupMenuPrintMark.AppendSeparator();
	PopupMenuPrintMark.Append(ID_MNU___N_PAGE_NUMBER_1113, _("[%n] Page &Number"), _T(""), wxITEM_NORMAL);
	PopupMenuPrintMark.Append(ID_MNU___S__TOTALPAGES_1114, _("[%s] Total Page&s"), _T(""), wxITEM_NORMAL);
	PopupMenuPrintMark.AppendSeparator();
	PopupMenuPrintMark.Append(ID_MNU___D__DATE_1116, _("[%d] &Date"), _T(""), wxITEM_NORMAL);
	PopupMenuPrintMark.Append(ID_MNU___T__TIME_1117, _("[%t] &Time"), _T(""), wxITEM_NORMAL);

	PopupMenuDateTimeMark.Append(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, _("[%Y/%m/%d %I:%M:%S %p] 2007/02/24 08:30:55 AM"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___D__M__Y_24_02_2007_1192, _("[%d/%m/%Y] 24/02/2007"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, _("[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.AppendSeparator();
	PopupMenuDateTimeMark.Append(ID_MNU_MENUITEM1_1170, _("[%a] Abbreviated weekday name"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___A_FULLWEEKDAYNAME_1171, _("[%A] Full weekday name"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___B_ABBREVIATEDMONTHNAME_1172, _("[%b] Abbreviated month name"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___B_FULLMONTHNAME_1173, _("[%B] Full month name"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, _("[%c] Date and time representation appropriate for locale"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, _("[%d] Day of month as decimal number (01 - 31)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, _("[%H] Hour in 24-hour format (00 - 23)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, _("[%I] Hour in 12-hour format (01 - 12)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, _("[%j] Day of year as decimal number (001 - 366)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, _("[%m] Month as decimal number (01 - 12)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, _("[%M] Minute as decimal number (00 - 59)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, _("[%p] Current locale's A.M./P.M. indicator for 12-hour clock"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, _("[%S] Second as decimal number (00 - 59)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, _("[%U] Week of year as decimal number, with Sunday as first day of week (00 - 53)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, _("[%w] Weekday as decimal number (0 - 6; Sunday is 0)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, _("[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, _("[%x] Date representation for current locale"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, _("[%X] Time representation for current locale"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, _("[%y] Year without century, as decimal number (00 - 99)"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, _("[%Y] Year with century, as decimal number"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONENAME_1193, _("[%z] Time-zone name"), _T(""), wxITEM_NORMAL);
	PopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONEABBREVIATION_1194, _("[%Z] Time-zone abbreviation"), _T(""), wxITEM_NORMAL);

	wxString systemenc(_("System Default"));
	ComboBoxEncoding->Append(systemenc);
	size_t cnt = MadEncoding::GetEncodingsCount();
	size_t i;

	for (i = 0; i < cnt; ++i)
	{
		ComboBoxEncoding->Append(MadEncoding::GetEncodingName(i)); //enc+des);
		ComboBoxNewDocEncOther->Append(MadEncoding::GetEncodingName(i));
	}

	ComboBoxEncoding->SetValue(systemenc);

	/*for(i=0; i<g_LanguageString.GetCount(); ++i)
	{
	ComboBoxLanguage->Append(g_LanguageString[i]);
	}
	ComboBoxLanguage->SetValue(g_LanguageString[0]);*/

	cnt=MadSyntax::GetSyntaxCount();
	for(size_t i=0; i<cnt; ++i)
	{
		ComboBoxNewDocSyntax->Append(wxGetTranslation(MadSyntax::GetSyntaxTitle(i)));
	}
	int index = ComboBoxNewDocSyntax->FindString(wxGetTranslation(_T("Plain Text")));
	if(index != wxNOT_FOUND)
		ComboBoxNewDocSyntax->SetSelection(index);
	m_NewDocEncoding = _("System Default");
	m_NewDocLineEnding = nltDefault;
	index = ComboBoxNewDocSyntax->FindString(wxGetTranslation(_T("Plain Text")));

#ifdef __WXMSW__
	wxString fontname( wxT( "Courier New" ) );
#elif defined(__APPLE__) && defined(__MACH__)
	wxString fontname( wxT( "Monaco" ) );
#else
	wxString fontname( wxT( "Monospace" ) );
#endif
	index = ComboBoxDefaultFont->FindString(fontname);
	if(index != wxNOT_FOUND) ComboBoxDefaultFont->SetSelection(index);
	index = ComboBoxNewDocFont->FindString(fontname);
	if(index != wxNOT_FOUND) ComboBoxNewDocFont->SetSelection(index);

	wxSize sz1 = BoxSizer3->CalcMin();
	wxSize sz2 = BoxSizer8->CalcMin();
	wxSize sz3 = BoxSizer14->CalcMin();
	wxSize sz4 = BoxSizer18->CalcMin();

	int ww = std::max(std::max(sz1.x, sz2.x), std::max(sz3.x, sz4.x));
	int hh = std::max(std::max(sz1.y, sz2.y), std::max(sz3.y, sz4.y));

	BoxSizer33->Add(0, hh + AuiNotebook1->GetTabCtrlHeight() + 2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer32->Add(ww + 2, 0, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Layout();
	Center();

	// build Command Tree
	TreeCtrl1->SetImageList(g_MainFrame->m_ImageList);
	wxTreeItemId root=TreeCtrl1->AddRoot(_("Commands"));

	wxTreeItemId menuRoot=TreeCtrl1->AppendItem(root, _("Menu"));
	wxTreeItemId editorRoot=TreeCtrl1->AppendItem(root, _("Editor"));

	list<wxTreeItemId> tree_stack;
	CommandData *cd = &CommandTable[0];
	wxTreeItemId tid;

	// Menu command
	do
	{
		if(cd->menu_level==0)
		{
			tid=TreeCtrl1->AppendItem(menuRoot, FilterChar(wxGetTranslation(cd->text)));
			tree_stack.clear();
			tree_stack.push_back(tid);
			++cd;
			continue;
		}
		else if(cd->menu_level < int(tree_stack.size()))
		{
			do
			{
				tree_stack.pop_back();
			}
			while(cd->menu_level < int(tree_stack.size()));
		}

		if(cd->menu_ptr != 0)
		{
			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)));
			tree_stack.push_back(tid);
		}
		else if(cd->kind != wxITEM_SEPARATOR)
		{
			TreeItemData *data=new TreeItemData;
			data->cmddata=cd;
			TreeItemDataList.push_back(data);

			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)), cd->image_idx, cd->image_idx, data);
			TreeCtrl1->SetItemBold(tid, true);
		}

		++cd;
	}
	while(cd->menu_level>=0);

	// Editor command
	do
	{
		TreeItemData *data=new TreeItemData;
		data->cmddata=cd;
		TreeItemDataList.push_back(data);

		tid=TreeCtrl1->AppendItem(editorRoot, MadKeyBindings::CommandToText(cd->command), cd->image_idx, cd->image_idx, data);
		TreeCtrl1->SetItemBold(tid, true);
		cd++;
	}
	while(cd->command > 0);

	TreeCtrl1->Expand(menuRoot);
	TreeCtrl1->Expand(editorRoot);
	TreeCtrl1->Expand(root);

	AuiNotebook1->SetWindowStyleFlag(wxAUI_NB_TOP|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS);

#if defined(__WXMSW__) && (wxMAJOR_VERSION >= 3)
	std::vector< wxWindow * >::iterator it = controls.begin();
	while(it != controls.end())
	{
		wxWindow *pWin = *it;
		wxASSERT(pWin != nullptr);
        LONG exStyle = wxGetWindowExStyle(pWin);
        if ( !(exStyle & WS_EX_CONTROLPARENT) )
        {
            wxSetWindowExStyle((pWin), exStyle | WS_EX_CONTROLPARENT);
        }
        ++it;
    }
#endif
	ButtonCancel->SetId(wxID_CANCEL);
}

MadOptionsDialog::~MadOptionsDialog()
{
	//(*Destroy(MadOptionsDialog)
	//*)
}

void MadOptionsDialog::MadOptionsDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_OptionsDialog=nullptr;
	Destroy();
}

void MadOptionsDialog::MadOptionsDialogActivate(wxActivateEvent& event)
{
	if(event.GetActive())
	{
		if(FindFocus()==nullptr)
		{
			SetReturnCode(wxID_CANCEL);
			ButtonCancel->SetFocus();
		}
	}

	event.Skip();
}

void MadOptionsDialog::LoadOptions(void)
{
	wxConfigBase *cfg = wxConfigBase::Get( false );
	wxString oldpath = cfg->GetPath();
	cfg->SetPath( wxT( "/MadEdit" ) );
	long ll;
	bool bb;
	wxString ss;
#ifdef __WXMSW__
	wxString fontname( wxT( "Courier New" ) );
#elif defined(__APPLE__) && defined(__MACH__)
	wxString fontname( wxT( "Monaco" ) );
#else
	wxString fontname( wxT( "Monospace" ) );
#endif
	// General page
	ss = g_LanguageString[0];
	cfg->Read( wxT( "/Application/Language" ), &ss );
	ComboBoxLanguage->SetValue( ss );
	cfg->Read( wxT( "/Application/SingleInstance" ), &bb, false );
	CheckBoxSingleInstance->SetValue( bb );
	cfg->Read( wxT( "RecordCaretMovements" ), &bb, true );
	CheckBoxRecordCaretMovements->SetValue( bb );
	cfg->Read( wxT( "MaxSizeToLoad" ), &ll );
	EditMaxSizeToLoad->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxTextFileSize" ), &ll );
	EditMaxTextFileSize->SetValue( wxString() << ll );
	cfg->Read( wxT( "/UIView/MaxDisplaySize" ), &ll );
	EditMaxDisplaySize->SetValue( wxString() << ll );
	ss = _( "System Default" );
	cfg->Read( wxT( "DefaultEncoding" ), &ss );
	ComboBoxEncoding->SetValue( ss );
	cfg->Read( wxT( "EnforceDefaultEncoding" ), &bb, false );
	CheckBoxSkipAutoEncoding->SetValue( bb );

	cfg->Read( wxT( "DefaultTextFont" ), &fontname );
	int index = ComboBoxDefaultFont->FindString(fontname);
	if(index != wxNOT_FOUND) ComboBoxDefaultFont->SetSelection(index);
	cfg->Read( wxT( "DefaultTextFontSize" ), &ll, DEFAULT_FONT_SIZE );
	if(ll < 0) ll = DEFAULT_FONT_SIZE;
	if(ll > MAX_FONT_SIZE) ll = MAX_FONT_SIZE;
	EditDefaultFontSize->SetValue( wxString() << ll );

	cfg->Read( wxT( "/Application/AutoSaveTimeout" ), &ll, 0 );
	bb = ((ll >= 10) && (ll <= 30));
	CheckBoxEnableAutoSave->SetValue( bb );
	if(!bb) ll = 10;
	EditAutoSaveTimeout->Enable(bb);
	EditAutoSaveTimeout->SetValue( wxString() << ll );
	cfg->Read( wxT( "/Application/AutoBackup" ), &bb, false );
	CheckBoxEnableAutoBackup->SetValue( bb );
#ifdef __WXMSW__
	wxRegKey *pRegKey = new wxRegKey( g_MadEditRegkeyPath + wxT( "*\\shell\\MadEdit-Mod\\command" ) );

	if( pRegKey->Exists() )
	{
		wxString str;

		if( pRegKey->QueryValue( wxEmptyString, str ) )
		{
			wxString exepath = GetExecutablePath();
			CheckBoxRightClickMenu->SetValue( str.Upper().Find( exepath.Upper() ) >= 0 );
		}
	}

	delete pRegKey;
#endif

	// Edit page
	cfg->Read( wxT( "MaxLineLength" ), &ll );
	EditMaxLineLength->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxColumns" ), &ll );
	EditMaxColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "TabColumns" ), &ll );
	EditTabColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "IndentColumns" ), &ll );
	EditIndentColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "DateTimeFormat" ), &ss, wxT( "%c" ) );
	EditDateTime->SetValue( ss );
	cfg->Read( wxT( "DateTimeInEnglish" ), &bb, false );
	CheckBoxDateTimeInEnglish->SetValue( bb );
	cfg->Read( wxT( "InsertSpacesInsteadOfTab" ), &bb, false );
	CheckBoxTabOrSpaces->SetValue( bb );
	cfg->Read( wxT( "AutoIndent" ), &bb, false );
	CheckBoxAutoIndent->SetValue( bb );
	cfg->Read( wxT( "AutoCompletePair" ), &bb, false );
	CheckBoxAutoCompletePair->SetValue( bb );
	CheckBoxInsertPairForSelection->Enable(bb);
	cfg->Read( wxT( "InsertPairForSelction" ), &bb, true );
	CheckBoxInsertPairForSelection->SetValue( bb );
	cfg->Read( wxT( "MouseSelectToCopy" ), &bb, false );
	CheckBoxMouseSelectToCopy->SetValue( bb );
	CheckBoxCtrlWithMouseToSelect->Enable( bb );
	cfg->Read( wxT( "MouseSelectToCopyWithCtrlKey" ), &bb, true );
	CheckBoxCtrlWithMouseToSelect->SetValue( bb );
	cfg->Read( wxT( "MiddleMouseToPaste" ), &bb, false );
	CheckBoxMiddleMouseToPaste->SetValue( bb );
	cfg->Read( wxT( "AutoFillColumnPaste" ), &bb, true );
	CheckBoxAutoFillColumnPaste->SetValue( bb );
	cfg->Read( wxT( "LDoubleClickHighlight" ), &bb, true );
	CheckBoxLDClickHighlight->SetValue( bb );
	cfg->Read( wxT( "TypewriterMode" ), &bb, false );
	CheckBoxTypewriterMode->SetValue( bb );
	cfg->Read( wxT( "FixedWidthMode" ), &bb, false );
	CheckBoxFixWidthMode->SetValue( bb );
	extern bool g_DoNotSaveSettings;
	CheckBoxDoNotSaveSettings->SetValue( g_DoNotSaveSettings );
	cfg->Read( wxT( "/Application/ReloadFiles" ), &bb, true );
	CheckBoxReloadFiles->SetValue( bb );
	cfg->Read( wxT( "PurgeHistory" ), &bb, false );
	CheckBoxPurgeHistory->SetValue( bb );
	cfg->Read( wxT( "RestoreCaretPos" ), &bb, true );
	CheckBoxRestoreCaretPos->SetValue( bb );

	// New Document
	cfg->Read( wxT( "NewDocumentLineEnding" ), &m_NewDocLineEnding, nltDefault );
	switch(m_NewDocLineEnding)
	{
	case nltDefault:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(true);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	case nltDOS:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(true);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	case nltUNIX:
		RadioButtonNewDocLineEndingCR->SetValue(false);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(true);
		break;
	case nltMAC:
		RadioButtonNewDocLineEndingCR->SetValue(true);
		RadioButtonNewDocLineEndingCRLF->SetValue(false);
		RadioButtonNewDocLineEndingDefault->SetValue(false);
		RadioButtonNewDocLineEndingLF->SetValue(false);
		break;
	default:
		wxASSERT(0);
	}

	ss = wxGetTranslation(wxT("Plain Text"));
	cfg->Read( wxT( "NewDocumentSyntax" ), &ss );
	index = ComboBoxNewDocSyntax->FindString(ss);
	if(index == wxNOT_FOUND) index = 0;
	ComboBoxNewDocSyntax->SetSelection(index);

	m_NewDocEncoding = _( "System Default" );
	cfg->Read( wxT( "NewDocumentEncoding" ), &m_NewDocEncoding );
	wxFontEncoding enc = wxFontMapper::GetEncodingFromName( m_NewDocEncoding );
	index = ComboBoxNewDocEncOther->FindString(m_NewDocEncoding);
	if (wxFONTENCODING_MAX == enc || wxFONTENCODING_DEFAULT == enc || index == wxNOT_FOUND)
		enc = wxFONTENCODING_SYSTEM;

	if(wxFONTENCODING_UTF8 == enc)
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(true);
		ComboBoxNewDocEncOther->Enable(false);
		
		RadioButtonNewDocEncOther->SetValue(false);
		RadioButtonNewDocEncSystemDefault->SetValue(false);
		RadioButtonNewDocEncUTF8->SetValue(true);
	}
	else
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(false);
		RadioButtonNewDocEncUTF8->SetValue(false);
		if(wxFONTENCODING_SYSTEM == enc)
		{
			ComboBoxNewDocEncOther->Enable(false);
			RadioButtonNewDocEncOther->SetValue(false);
			RadioButtonNewDocEncSystemDefault->SetValue(true);
		}
		else
		{
			ComboBoxNewDocEncOther->Enable(true);
			ComboBoxNewDocEncOther->SetSelection(index);
			RadioButtonNewDocEncOther->SetValue(true);
			RadioButtonNewDocEncSystemDefault->SetValue(false);
		}
	}

	cfg->Read( wxT( "NewDocumentEncodingUTF8WithBOM" ), &bb, false );
	CheckBoxNewDocEncUTF8WithBOM->SetValue( bb );

#ifdef __WXMSW__
	fontname = wxString( wxT( "Courier New" ) );
#elif defined(__APPLE__) && defined(__MACH__)
	fontname = wxString( wxT( "Monaco" ) );
#else
	fontname = wxString( wxT( "Monospace" ) );
#endif
	cfg->Read( wxT( "NewDocumentTextFont" ), &fontname );
	index = ComboBoxNewDocFont->FindString(fontname);
	if(index != wxNOT_FOUND) ComboBoxNewDocFont->SetSelection(index);

	cfg->Read( wxT( "NewDocumentTextFontSize" ), &ll, DEFAULT_FONT_SIZE );
	if(ll < 0) ll = DEFAULT_FONT_SIZE;
	if(ll > MAX_FONT_SIZE) ll = MAX_FONT_SIZE;
	EditNewDocFontSize->SetValue( wxString() << ll );

	// Print page
	cfg->Read( wxT( "PrintSyntax" ), &bb );
	CheckBoxPrintSyntax->SetValue( bb );
	cfg->Read( wxT( "PrintLineNumber" ), &bb );
	CheckBoxPrintLineNumber->SetValue( bb );
	cfg->Read( wxT( "PrintBookmark" ), &bb );
	CheckBoxPrintBookmark->SetValue( bb );
	cfg->Read( wxT( "PrintEndOfLine" ), &bb );
	CheckBoxPrintEndOfLine->SetValue( bb );
	cfg->Read( wxT( "PrintTabChar" ), &bb );
	CheckBoxPrintTabChar->SetValue( bb );
	cfg->Read( wxT( "PrintSpaceChar" ), &bb );
	CheckBoxPrintSpaceChar->SetValue( bb );
	cfg->Read( wxT( "PrintOffsetHeader" ), &ll );
	RadioBoxPrintOffset->SetSelection( ll );
	cfg->Read( wxT( "PrintPageHeader" ), &bb );
	CheckBoxPrintPageHeader->SetValue( bb );
	cfg->Read( wxT( "PageHeaderLeft" ), &ss );
	EditHeaderLeft->SetValue( ss );
	cfg->Read( wxT( "PageHeaderCenter" ), &ss );
	EditHeaderCenter->SetValue( ss );
	cfg->Read( wxT( "PageHeaderRight" ), &ss );
	EditHeaderRight->SetValue( ss );
	cfg->Read( wxT( "PrintPageFooter" ), &bb );
	CheckBoxPrintPageFooter->SetValue( bb );
	cfg->Read( wxT( "PageFooterLeft" ), &ss );
	EditFooterLeft->SetValue( ss );
	cfg->Read( wxT( "PageFooterCenter" ), &ss );
	EditFooterCenter->SetValue( ss );
	cfg->Read( wxT( "PageFooterRight" ), &ss );
	EditFooterRight->SetValue( ss );
	bb = SpellCheckerManager::Instance().GetEnablePersonalDictionary();
	CheckBoxPersonalDict->SetValue( bb );
	EditDictionaryDir->SetValue( SpellCheckerManager::Instance().GetDictionaryPath() );
	InitDictionaryChoice();
	//cfg->Read(wxT("ThesPath"), &ss, dictDir);
	//EditThesauriDir->SetValue(ss);
	//cfg->Read(wxT("BitmPath"), &ss, dictDir);
	//EditBitMapDir->SetValue(ss);
	extern bool g_ResetAllKeys;
	CheckBoxResetAllKeys->SetValue( g_ResetAllKeys );
	ChangedTreeItemDataList.clear();
	// get all keys of each command and srore them in TreeItemDataList
	list<TreeItemData*>::iterator tidit = TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend = TreeItemDataList.end();

	do
	{
		CommandData *cd = ( *tidit )->cmddata;
		( *tidit )->keys.Clear();
		MadEdit::ms_KeyBindings.GetKeys( cd->menu_id, cd->command, ( *tidit )->keys );
	}
	while( ++tidit != tiditend );

	g_SelectedCommandItem = nullptr;
	g_SelectedKeyId = -1;
	EditCommandHint->Clear();
	ListBoxKeys->Clear();
	UpdateKeyHint();
	wxTreeItemId selid = TreeCtrl1->GetSelection();

	if( selid.IsOk() )
	{
		TreeCtrl1->Unselect();
		TreeCtrl1->SelectItem( selid, true );
	}

	cfg->SetPath( oldpath );
}

void MadOptionsDialog::ButtonOKClick(wxCommandEvent& event)
{
	long lo;
	bool error = false;
	wxString errtext( _( "Invalid value of \"%s(%s)\"" ) );

	if( !EditMaxSizeToLoad->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext + _(": Should be greater than zero"), StaticText1->GetLabel().c_str(), EditMaxSizeToLoad->GetValue().c_str() );
		error = true;
	}

	if( !EditMaxTextFileSize->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext + _(": Should be greater than zero"), StaticText2->GetLabel().c_str(), EditMaxTextFileSize->GetValue().c_str() );
		error = true;
	}

#if PATCH_MAXLINELENGTH == 1
	if( !EditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 || lo > 4096 )
#else
	if( !EditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 )
#endif
	{
		wxLogError( errtext + _(": Should be 1~4096"), StaticText3->GetLabel().c_str(), EditMaxLineLength->GetValue().c_str() );
		error = true;
	}

	if( !EditMaxColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 4096 )
	{
		wxLogError( errtext + _(": Should be 1~4096"), StaticText4->GetLabel().c_str(), EditMaxColumns->GetValue().c_str() );
		error = true;
	}

	if( !EditTabColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 256 )
	{
		wxLogError( errtext + _(": Should be 1~256"), StaticText5->GetLabel().c_str(), EditTabColumns->GetValue().c_str() );
		error = true;
	}

	if( !EditIndentColumns->GetValue().ToLong( &lo ) || lo <= 0 || lo > 256 )
	{
		wxLogError( errtext + _(": Should be 1~256"), StaticText6->GetLabel().c_str(), EditIndentColumns->GetValue().c_str() );
		error = true;
	}

	if( !EditAutoSaveTimeout->GetValue().ToLong( &lo ) || ( lo < 10 || lo > 30 ))
	{
		wxLogError( errtext + _(": Should be 10~30"), StaticTextAutoSaveTimeout->GetLabel().c_str(), EditAutoSaveTimeout->GetValue().c_str() );
		error = true;
	}

	if( !error ) { EndModal( wxID_OK ); }
}

void MadOptionsDialog::ButtonCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void MadOptionsDialog::PrintMarkButtonClick(wxCommandEvent& event)
{
	ButtonID=event.GetId();
	PopupMenu(&PopupMenuPrintMark);
}

void MadOptionsDialog::PrintMarkClick(wxCommandEvent& event)
{
	wxString str = PopupMenuPrintMark.GetLabel( event.GetId() );
	wxTextCtrl *edit = nullptr;
	if (ButtonID == ID_BUTTON1) { edit = EditHeaderLeft; }
	else if (ButtonID == ID_BUTTON2) { edit = EditHeaderCenter; }
	else if (ButtonID == ID_BUTTON3) { edit = EditHeaderRight; }
	else if (ButtonID == ID_BUTTON4) { edit = EditFooterLeft; }
	else if (ButtonID == ID_BUTTON5) { edit = EditFooterCenter; }
	else if (ButtonID == ID_BUTTON6) { edit = EditFooterRight; }
	else { ; }

	if(edit!=nullptr && str[0]==wxT('[') && str[3]==wxT(']'))
	{
		wxString text=edit->GetValue();
		edit->SetValue(text+ str.Mid(1, 2));
	}
	ButtonID=0;
}

void MadOptionsDialog::TreeCtrl1SelChanged(wxTreeEvent& event)
{
	wxTreeItemId id=event.GetItem();
	if(!id.IsOk()) return;

	g_SelectedCommandItem=(TreeItemData*)TreeCtrl1->GetItemData(id);
	g_SelectedKeyId=-1;

	if(g_SelectedCommandItem==nullptr)
	{
		ListBoxKeys->Clear();
		EditCommandHint->SetValue(_("Cannot assign key to this item"));
	}
	else
	{
		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		wxString hint=wxT('[');
		if(g_SelectedCommandItem->cmddata->menuid_name > 0)
		{
			hint+= wxString(g_SelectedCommandItem->cmddata->menuid_name);
		}
		if(g_SelectedCommandItem->cmddata->command > 0)
		{
			wxString text=MadKeyBindings::CommandToText(g_SelectedCommandItem->cmddata->command);
			if(!text.IsEmpty())
			{
				if(hint.Length()>1)
				{
					hint+= wxT(',');
				}
				hint+= text;
			}
		}
		hint+= wxT("] ");
		hint+= wxString(wxGetTranslation(g_SelectedCommandItem->cmddata->hint));

		EditCommandHint->SetValue(hint);
	}
}

void MadOptionsDialog::ListBoxKeysSelected(wxCommandEvent& event)
{
	g_SelectedKeyId=event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* MadOptionsDialog::FindKeyInList(const wxString &key)
{
	list<TreeItemData*>::iterator tidit=TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend=TreeItemDataList.end();

	wxString lkey=key.Lower();

	while(tidit!=tiditend)
	{
		TreeItemData *tid = (*tidit);
		if(!tid->keys.IsEmpty())
		{
			size_t count=tid->keys.GetCount();
			size_t idx=0;

			for( ; idx < count; ++idx )
			{
				if(tid->keys[idx].Lower() == lkey)
				{
					return tid;
				}
			}
		}

		++tidit;
	}
	return nullptr;
}

bool MadOptionsDialog::FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist)
{
	list<TreeItemData*>::const_iterator tidit=tlist.begin();
	list<TreeItemData*>::const_iterator tiditend=tlist.end();

	while(tidit!=tiditend)
	{
		if((*tidit) == tid)
		{
			return true;
		}
		++tidit;
	}
	return false;
}

void MadOptionsDialog::UpdateKeyHint()
{
	wxString scstr=EditKey->GetValue();
	g_CommandItemOfNewKey=nullptr;

	if(scstr.IsEmpty())
	{
		g_OptionsDialog->EditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList(scstr);
		if(tid==nullptr)
		{
			g_OptionsDialog->EditKeyHint->SetValue(_("This key is not assigned"));
		}
		else
		{
			g_CommandItemOfNewKey=tid;
			wxString cmd;

			if(tid->cmddata->menuid_name > 0)
			{
				cmd+= wxString(tid->cmddata->menuid_name);
			}
			if(tid->cmddata->command > 0)
			{
				wxString text=MadKeyBindings::CommandToText(tid->cmddata->command);
				if(!text.IsEmpty())
				{
					if(cmd.Length()>1)
					{
						cmd+= wxT(',');
					}
					cmd+= text;
				}
			}

			wxString str=wxString::Format(_("This key is assigned to [%s]"), cmd.c_str());
			g_OptionsDialog->EditKeyHint->SetValue(str);
		}
	}
}

void MadOptionsDialog::ButtonAddKeyClick(wxCommandEvent& event)
{
	if(g_SelectedCommandItem!=nullptr && g_SelectedCommandItem!=g_CommandItemOfNewKey)
	{
		wxString key=EditKey->GetValue();
		if(g_CommandItemOfNewKey!=nullptr) // new key is assigned to another command
		{
			/*
			wxMessageDialog dlg(this, key +wxT(": ") +EditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
				wxT("MadEdit"), wxYES_NO|wxICON_QUESTION );
			if(dlg.ShowModal()!=wxID_YES)
			{
				return;
			}
			*/

			//delete the key from g_CommandItemOfNewKey
			int idx=g_CommandItemOfNewKey->keys.Index(key.c_str(), false);
			wxASSERT(idx>=0);
			g_CommandItemOfNewKey->keys.RemoveAt(idx);
			if(FindItemInList(g_CommandItemOfNewKey, ChangedTreeItemDataList)==false)
			{
				ChangedTreeItemDataList.push_back(g_CommandItemOfNewKey);
			}
		}

		// add the key to g_SelectedCommandItem
		g_SelectedCommandItem->keys.Add(key);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void MadOptionsDialog::ButtonDeleteKeyClick(wxCommandEvent& event)
{
	if(g_SelectedCommandItem!=nullptr && g_SelectedKeyId >= 0)
	{
		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void MadOptionsDialog::ButtonShowInMenuClick(wxCommandEvent& event)
{
	// move the selected key to first element of ListBoxKeys
	if(g_SelectedCommandItem!=nullptr && g_SelectedCommandItem->cmddata->menu_id>0 && g_SelectedKeyId > 0)
	{
		wxString key=g_SelectedCommandItem->keys[g_SelectedKeyId];

		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);
		g_SelectedCommandItem->keys.Insert(key, 0);

		ListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
	}
}

void MadOptionsDialog::ButtonDateTimeClick(wxCommandEvent& event)
{
	PopupMenu(&PopupMenuDateTimeMark);
}

void MadOptionsDialog::DateTimeMarkClick(wxCommandEvent& event)
{
	wxString str=PopupMenuDateTimeMark.GetLabel(event.GetId());
	int pos=str.Find(wxT(']'));
	if(pos != wxNOT_FOUND && str[0]==wxT('['))
	{
		str = str.Mid(1, pos-1);
	}

	wxString text=EditDateTime->GetValue();
	EditDateTime->SetValue(text+str);
}

#if ENABLE_BITMAP_THUARI
void MadOptionsDialog::ButtonBitmapDirClick( wxCommandEvent& event )
{
}
void MadOptionsDialog::ButtonThesauriDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetThesaurusPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		EditThesauriDir->SetValue( dlg.GetPath() );
		SpellCheckerManager::Instance().SetThesaurusPath( dlg.GetPath() );
		InitDictionaryChoice();
	}
}
#endif

void MadOptionsDialog::ButtonDictionaryDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetDictionaryPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		EditDictionaryDir->SetValue( dlg.GetPath() );
		InitDictionaryChoice( dlg.GetPath() );
	}
}

void MadOptionsDialog::InitDictionaryChoice( const wxString &path/* = wxEmptyString*/ )
{
	if( !path.IsEmpty() )
	{
		SpellCheckerManager::Instance().SetDictionaryPath( path );
		SpellCheckerManager::Instance().ScanForDictionaries();
	}

	std::vector<wxString> dics = SpellCheckerManager::Instance().GetPossibleDictionaries();
	int sel = SpellCheckerManager::Instance().GetSelectedDictionaryNumber();
	ChoiceDictionary->Clear();

	for( unsigned int i = 0 ; i < dics.size(); i++ )
	{ ChoiceDictionary->AppendString( SpellCheckerManager::Instance().GetLanguageName( dics[i] ) ); }

	if( sel != -1 )
	{ ChoiceDictionary->Select( sel ); }
}

void MadOptionsDialog::EditDictionaryDirTextEnter( wxCommandEvent& event )
{
	wxString path = EditDictionaryDir->GetValue();

	if( wxDir::Exists( path ) )
	{
		InitDictionaryChoice( path );
	}
	else
	{
		ChoiceDictionary->Clear();
	}
}

void MadOptionsDialog::ChoiceDictionarySelect( wxCommandEvent& event )
{
	wxString dictDesc = ChoiceDictionary->GetString( ChoiceDictionary->GetSelection() );
	wxString dictName = SpellCheckerManager::Instance().GetDictionaryName( dictDesc );

	if( !dictName.IsEmpty() )
	{
		SpellCheckerManager::Instance().SetDictionaryName( dictName );
	}
}

void MadOptionsDialog::CheckBoxMouseSelectToCopyClick( wxCommandEvent& event )
{
	CheckBoxCtrlWithMouseToSelect->Enable( CheckBoxMouseSelectToCopy->GetValue() );
}

void MadOptionsDialog::CheckBoxAutoCompletePairClick( wxCommandEvent& event )
{
	CheckBoxInsertPairForSelection->Enable( CheckBoxAutoCompletePair->GetValue() );
}

void MadOptionsDialog::EnableAutoSaveClick( wxCommandEvent& event )
{
	EditAutoSaveTimeout->Enable( CheckBoxEnableAutoSave->GetValue() );
}

#ifdef MADEDIT_ENABLE_STC
void MadOptionsDialog::OnMarginClick( wxStyledTextEvent &event )
{
	if( event.GetMargin() == MARGIN_FOLD )
	{
		int lineClick = TextSample->LineFromPosition( event.GetPosition() );
		int levelClick = TextSample->GetFoldLevel( lineClick );
		if( ( levelClick & wxSTC_FOLDLEVELHEADERFLAG ) > 0 )
		{
			TextSample->ToggleFold( lineClick );
		}
	}
}
#endif

void MadOptionsDialog::RadioButtonNewDocEncSelect(wxCommandEvent& event)
{
	if(ID_RADIOBUTTONNEWDOCENCUTF8 == event.GetId())
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(true);
		ComboBoxNewDocEncOther->Enable(false);
		m_NewDocEncoding = wxFontMapper::GetEncodingName(wxFONTENCODING_UTF8);
	}
	else
	{
		CheckBoxNewDocEncUTF8WithBOM->Enable(false);
		if(ID_RADIOBUTTONNEWDOCENCOTHER == event.GetId())
		{
			ComboBoxNewDocEncOther->Enable(true);
			m_NewDocEncoding = ComboBoxNewDocEncOther->GetString(ComboBoxNewDocEncOther->GetSelection());
		}
		else
		{
			ComboBoxNewDocEncOther->Enable(false);
			m_NewDocEncoding = _("System Default");
		}
	}
}

void MadOptionsDialog::RadioButtonNewDocLineEndingSelect(wxCommandEvent& event)
{
	if (event.GetId() == ID_RADIOBUTTONNEWDOCLINEENDINGCRLF)
	{
		m_NewDocLineEnding = nltDOS;
	}
	else if (event.GetId() == ID_RADIOBUTTONNEWDOCLINEENDINGLF)
	{
		m_NewDocLineEnding = nltUNIX;
	}
	else if (event.GetId() == ID_RADIOBUTTONNEWDOCLINEENDINGCR)
	{
		m_NewDocLineEnding = nltMAC;
	}
	else
	{
		m_NewDocLineEnding = nltDefault;
	}
}
