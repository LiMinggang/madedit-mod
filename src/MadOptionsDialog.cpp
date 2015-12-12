///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <wx/fileconf.h>
#include <wx/config.h>
#include <wx/dir.h>

#include "MadOptionsDialog.h"

#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEditCommand.h"
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadUtils.h"
#include "MadEdit/MadEditPv.h"
#include "SpellCheckerManager.h"
#include "astyle/astyle.h"
#include "astylepredefinedstyles.h"

#ifdef MADEDIT_ENABLE_STC
enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};
#endif
#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadOptionsDialog)
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/aui/auibook.h>
#include <wx/fileconf.h>
#include <wx/config.h>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include <algorithm>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

MadOptionsDialog *g_OptionsDialog = NULL;
extern wxArrayString g_LanguageString;
extern wxString g_MadEditAppDir;

TreeItemData *g_SelectedCommandItem = NULL;
int g_SelectedKeyId=-1;
TreeItemData *g_CommandItemOfNewKey = NULL;
#define ENABLE_BITMAP_THUARI 0
static wxString bracket_style[aspsCustom + 1] =
{
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar)\n    {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isBar) {\n        bar();\n        return 1;\n    } else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n     if (isBar) {\n          bar();\n          return 1;\n     } else\n          return 0;\n}" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n    if (isBar)\n        {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n    {\n        if (isBar)\n            {\n            bar();\n            return 1;\n            }\n        else\n            return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n        }\n    else\n        return 0;\n    }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n  if (isBar)\n    {\n      bar();\n      return 1;\n    }\n  else\n    return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n        if (isBar) {\n                bar();\n                return 1;\n        } else\n                return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1;\n   }\n   else\n      return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{\n    if (isFoo) {\n        bar();\n        return 1;\n    } else {\n        return 0;\n    }\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar();\n        return 1;\n    }\n    else\n        return 0;\n}\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar)\n{  if (isBar)\n   {  bar();\n      return 1; }\n    else\n      return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
	wxT( "#include <iostream>\n\nint Foo(bool isBar) {\n    if (isBar) {\n        bar()\n        return 1; }\n    else\n        return 0; }\n" ),
};
class KeyTextCtrl : public wxTextCtrl
{
	DECLARE_EVENT_TABLE()
public:
	KeyTextCtrl()
	{
	}
	KeyTextCtrl(wxWindow *parent, wxWindowID id,
			   const wxString& value = wxEmptyString,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize,
			   long style = 0,
			   const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTextCtrlNameStr)
			   :wxTextCtrl(parent, id, value, pos, size, style, validator, name)
	{
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

		g_OptionsDialog->UpdateKeyHint();
		//evt.Skip();
	}
	void OnSetFocus(wxFocusEvent &evt)
	{   // for getting Ctrl-Tab
		g_OptionsDialog->SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
		g_OptionsDialog->AuiNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->AuiNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
		g_OptionsDialog->Panel4->SetWindowStyleFlag(g_OptionsDialog->Panel4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
		g_OptionsDialog->WxButtonCancel->SetId(MadOptionsDialog::ID_WXBUTTONCANCEL);
		evt.Skip();
	}
	void OnKillFocus(wxFocusEvent &evt)
	{   // restore wxTAB_TRAVERSAL
		g_OptionsDialog->SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
		g_OptionsDialog->AuiNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->AuiNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL);
		g_OptionsDialog->Panel4->SetWindowStyleFlag(g_OptionsDialog->Panel4->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
		g_OptionsDialog->WxButtonCancel->SetId(wxID_CANCEL);
		evt.Skip();
	}
};

BEGIN_EVENT_TABLE(KeyTextCtrl,wxTextCtrl)
	EVT_KEY_DOWN(KeyTextCtrl::OnKeyDown)
	EVT_SET_FOCUS(KeyTextCtrl::OnSetFocus)
	EVT_KILL_FOCUS (KeyTextCtrl::OnKillFocus)
END_EVENT_TABLE()

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
const long MadOptionsDialog::ID_WXCOMBOBOXLANGUAGE = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT16 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXSINGLEINSTANCE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRELOADFILES = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXSIZETOLOAD = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT1 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXTEXTFILESIZE = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT2 = wxNewId();
const long MadOptionsDialog::ID_WXCOMBOBOXENCODING = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT13 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRECORDCARETMOVEMENTS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRESTORECARETPOS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXSHOWQSEARCHBAR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXDONOTSAVESETTINGS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPURGEHISTORY = wxNewId();
const long MadOptionsDialog::ID_WSCHECKBOXMSRIGHTCCLICKMENU = wxNewId();
const long MadOptionsDialog::ID_PANEL1 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXLINELENGTH = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT3 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT4 = wxNewId();
const long MadOptionsDialog::ID_WXEDITTABCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT5 = wxNewId();
const long MadOptionsDialog::IID_WXEDITINDENTCOLUMNS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT6 = wxNewId();
const long MadOptionsDialog::ID_WXEDITDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXTDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONDATETIME = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXDATETIMEINENGLISH = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXTABORSPACES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOINDENT = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOCOMPLETEPAIR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXMOUSESELECTTOCOPY = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXWHENPRESSCTRLKEY = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXMIDDLEMOUSETOPASTE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXAUTOFILLCOLUMN = wxNewId();
const long MadOptionsDialog::ID_PANEL2 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTSYNTAX = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTLINENUMBER = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTBOOKMARK = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTENDOFLINE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTTABCHAR = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTSPACECHAR = wxNewId();
const long MadOptionsDialog::ID_WXRADIOBOXPRINTOFFSET = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTPAGEHEADER = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT7 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERLEFT = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON1 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT8 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERCENTER = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON2 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT9 = wxNewId();
const long MadOptionsDialog::ID_WXEDITHEADERRIGHT = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON3 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPRINTPAGEFOOTER = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT10 = wxNewId();
const long MadOptionsDialog::ID_WXEDITFOOTERLEFT = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON4 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT11 = wxNewId();
const long MadOptionsDialog::ID_WXEDITFOOTERLEFTCENTER = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON5 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT12 = wxNewId();
const long MadOptionsDialog::ID_WXEDITFOOTERRIGHT = wxNewId();
const long MadOptionsDialog::ID_WXBUTTON6 = wxNewId();
const long MadOptionsDialog::ID_PANEL3 = wxNewId();
const long MadOptionsDialog::ID_WXREECTRLCMDLIST = wxNewId();
const long MadOptionsDialog::ID_WXSTATICTEXTCOMMANDHINT = wxNewId();
const long MadOptionsDialog::ID_WXEDITHINT = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT14 = wxNewId();
const long MadOptionsDialog::ID_WXLISTBOXKEYS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT15 = wxNewId();
const long MadOptionsDialog::ID_WXEDITKEY = wxNewId();
const long MadOptionsDialog::ID_WXEDITKEYHINT = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONADDKEY = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONDELETEKEY = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONSHOWINMENU = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXRESETALLKEYS = wxNewId();
const long MadOptionsDialog::ID_PANEL4 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXPERSONALDICT = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT17 = wxNewId();
const long MadOptionsDialog::ID_WXCHOICEDICTIONARY = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT18 = wxNewId();
const long MadOptionsDialog::ID_WXEDITDICTIONARYDIR = wxNewId();
const long MadOptionsDialog::ID_WXDICTIONARY_DIR = wxNewId();
const long MadOptionsDialog::ID_PANEL5 = wxNewId();
const long MadOptionsDialog::ID_WXRADIOBOXBRACKETSTYLE = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT19 = wxNewId();
const long MadOptionsDialog::ID_WXTEXTSAMPLE = wxNewId();
const long MadOptionsDialog::ID_PANEL7 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKATTACHCLASSES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKATTACHEXTERNC = wxNewId();
const long MadOptionsDialog::ID_WXCHECKATTACHNAMESPACES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKATTACHINLINES = wxNewId();
const long MadOptionsDialog::ID_PANEL8 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT20 = wxNewId();
const long MadOptionsDialog::ID_WXSPINCTRLINDENTSIZE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKUSETABOVERSPACES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKFORCETABS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTCASE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENETCLASSES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTLABELS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTMODIFIERS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTNS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTSWITCHS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTPREP = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTMULTIPREP = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENTPREPCOND = wxNewId();
const long MadOptionsDialog::ID_WXCHECKINDENETCPPCOMMENTS = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT24 = wxNewId();
const long MadOptionsDialog::ID_WXSPINCTRLMININDENT = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT31 = wxNewId();
const long MadOptionsDialog::ID_WXEDITMAXINSTATEMENTINDENT = wxNewId();
const long MadOptionsDialog::ID_PANEL9 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKCLOSING = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKELSEIFS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKADDBRACKETS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKADDONELINEBRACKETS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBOXREMOVEBRACKETS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKKEEPBLOCKS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKKEEPCOMPLEX = wxNewId();
const long MadOptionsDialog::ID_WXCHECKCONVERTTABS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKCLOSETEMPLATES = wxNewId();
const long MadOptionsDialog::ID_WXCHECKREMOVECOMMENTPREFIX = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKLINES = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT23 = wxNewId();
const long MadOptionsDialog::ID_WXEDITSFMAXLINELENGTH = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKAFTERLOGICAL = wxNewId();
const long MadOptionsDialog::ID_PANEL10 = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKBLOCKS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKBREAKBLOCKSALL = wxNewId();
const long MadOptionsDialog::ID_WXCHECKPADOPERATORS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKPADPARENSOUT = wxNewId();
const long MadOptionsDialog::ID_WXCHECKPADPARENSIN = wxNewId();
const long MadOptionsDialog::ID_WXCHECKPADHEADER = wxNewId();
const long MadOptionsDialog::ID_WXCHECKUNPADPARENS = wxNewId();
const long MadOptionsDialog::ID_WXCHECKDELEMPTYLINE = wxNewId();
const long MadOptionsDialog::ID_WXCHECKFILLEMPTYLINES = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT26 = wxNewId();
const long MadOptionsDialog::ID_WXCHOICEPOINTERALIGN = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT27 = wxNewId();
const long MadOptionsDialog::ID_WXCHOICEREFERENCEALIGN = wxNewId();
const long MadOptionsDialog::ID_PANEL11 = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT28 = wxNewId();
const long MadOptionsDialog::ID_WXEDITXMLVERSION = wxNewId();
const long MadOptionsDialog::ID_STATICTEXT25 = wxNewId();
const long MadOptionsDialog::ID_WXEDITXMLINDENTSIZE = wxNewId();
const long MadOptionsDialog::ID_PANEL12 = wxNewId();
const long MadOptionsDialog::ID_AUINOTEBOOK2 = wxNewId();
const long MadOptionsDialog::ID_PANEL6 = wxNewId();
const long MadOptionsDialog::ID_AUINOTEBOOK1 = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONOK = wxNewId();
const long MadOptionsDialog::ID_WXBUTTONCANCEL = wxNewId();
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

BEGIN_EVENT_TABLE(MadOptionsDialog,wxDialog)
	//(*EventTable(MadOptionsDialog)
	//*)

	EVT_ACTIVATE(MadOptionsDialog::MadOptionsDialogActivate)

	EVT_BUTTON(ID_WXBUTTON1,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON2,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON3,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON4,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON5,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON6,MadOptionsDialog::PrintMarkButtonClick)

	EVT_MENU(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D__M__Y_24_02_2007_1192 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU_MENUITEM1_1170 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A_FULLWEEKDAYNAME_1171 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_ABBREVIATEDMONTHNAME_1172 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_FULLMONTHNAME_1173 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONENAME_1193 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONEABBREVIATION_1194 , MadOptionsDialog::DateTimeMarkClick)

	EVT_MENU(ID_MNU_MENUITEM1_1110 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___P__PATHNAME_1111 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___N_PAGE_NUMBER_1113 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___S__TOTALPAGES_1114 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___D__DATE_1116 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___T__TIME_1117 , MadOptionsDialog::PrintMarkClick)

END_EVENT_TABLE()

#if defined(__WXMSW__) && (wxMAJOR_VERSION >= 3)
#define SET_CONTROLPARENT(pWin) \
    {\
        LONG exStyle = wxGetWindowExStyle((wxWindow *)(pWin));\
        if ( !(exStyle & WS_EX_CONTROLPARENT) )\
        {\
            wxSetWindowExStyle((wxWindow *)(pWin), exStyle | WS_EX_CONTROLPARENT);\
        }\
    }
#else
#define SET_CONTROLPARENT(pWin)
#endif

MadOptionsDialog::MadOptionsDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MadOptionsDialog)
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer6;
	wxGridSizer* GridSizer4;
	wxBoxSizer* BoxSizer29;
	wxBoxSizer* BoxSizer19;
	wxBoxSizer* BoxSizer15;
	wxBoxSizer* BoxSizer20;
	wxBoxSizer* BoxSizer43;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer21;
	wxBoxSizer* BoxSizer13;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer36;
	wxBoxSizer* BoxSizer41;
	wxBoxSizer* BoxSizer42;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer23;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer16;
	wxBoxSizer* BoxSizer44;
	wxStaticBoxSizer* StaticBoxSizer7;
	wxBoxSizer* BoxSizer30;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer28;
	wxBoxSizer* BoxSizer38;
	wxBoxSizer* BoxSizer14;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxGridSizer* GridSizer1;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxGridSizer* GridSizer3;
	wxBoxSizer* BoxSizer27;
	wxBoxSizer* BoxSizer31;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer24;
	wxBoxSizer* BoxSizer26;
	wxBoxSizer* BoxSizer32;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxBoxSizer* BoxSizer47;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer33;
	wxBoxSizer* BoxSizer34;
	wxBoxSizer* BoxSizer22;
	wxBoxSizer* BoxSizer35;
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* BoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxGridSizer* GridSizer2;
	wxBoxSizer* BoxSizer25;

	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(580,640));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer31 = new wxBoxSizer(wxVERTICAL);
	AuiNotebook1 = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
	Panel1 = new wxPanel(AuiNotebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer27->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxLanguage = new wxComboBox(Panel1, ID_WXCOMBOBOXLANGUAGE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXLANGUAGE"));
	BoxSizer27->Add(WxComboBoxLanguage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText16 = new wxStaticText(Panel1, ID_STATICTEXT16, _("Language of User Interface (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	BoxSizer27->Add(StaticText16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer27, 0, wxALL|wxEXPAND, 2);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxSingleInstance = new wxCheckBox(Panel1, ID_WXCHECKBOXSINGLEINSTANCE, _("Single Instance (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSINGLEINSTANCE"));
	WxCheckBoxSingleInstance->SetValue(false);
	BoxSizer7->Add(WxCheckBoxSingleInstance, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxReloadFiles = new wxCheckBox(Panel1, ID_WXCHECKBOXRELOADFILES, _("Reload files previously open on startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRELOADFILES"));
	WxCheckBoxReloadFiles->SetValue(false);
	BoxSizer7->Add(WxCheckBoxReloadFiles, 0, wxALL|wxEXPAND, 2);
	BoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer38->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxSizeToLoad = new wxTextCtrl(Panel1, ID_WXEDITMAXSIZETOLOAD, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXSIZETOLOAD"));
	BoxSizer38->Add(WxEditMaxSizeToLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Max file size to load whole file into memory"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer38->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
	BoxSizer7->Add(BoxSizer38, 1, wxALL|wxEXPAND, 2);
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer17->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxTextFileSize = new wxTextCtrl(Panel1, ID_WXEDITMAXTEXTFILESIZE, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXTEXTFILESIZE"));
	BoxSizer17->Add(WxEditMaxTextFileSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Max file size to load as a text file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer17->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(BoxSizer17, 1, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxEncoding = new wxComboBox(Panel1, ID_WXCOMBOBOXENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXENCODING"));
	BoxSizer4->Add(WxComboBoxEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, _("Use this encoding to create new file or when MadEdit cannot determine the encoding of old file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	BoxSizer4->Add(StaticText13, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxRecordCaretMovements = new wxCheckBox(Panel1, ID_WXCHECKBOXRECORDCARETMOVEMENTS, _("Record caret movements in undo list"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECORDCARETMOVEMENTS"));
	WxCheckBoxRecordCaretMovements->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRecordCaretMovements, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxRestoreCaretPos = new wxCheckBox(Panel1, ID_WXCHECKBOXRESTORECARETPOS, _("Restore previous caret position when files are opened"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRESTORECARETPOS"));
	WxCheckBoxRestoreCaretPos->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRestoreCaretPos, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxShowQSearchBar = new wxCheckBox(Panel1, ID_WXCHECKBOXSHOWQSEARCHBAR, _("Show quick search bar on startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSHOWQSEARCHBAR"));
	WxCheckBoxShowQSearchBar->SetValue(false);
	BoxSizer7->Add(WxCheckBoxShowQSearchBar, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxDoNotSaveSettings = new wxCheckBox(Panel1, ID_WXCHECKBOXDONOTSAVESETTINGS, _("Do not save settings to MadEdit.cfg when MadEdit closed (this session only)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXDONOTSAVESETTINGS"));
	WxCheckBoxDoNotSaveSettings->SetValue(false);
	BoxSizer7->Add(WxCheckBoxDoNotSaveSettings, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxPurgeHistory = new wxCheckBox(Panel1, ID_WXCHECKBOXPURGEHISTORY, _("Purge History while exiting"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPURGEHISTORY"));
	WxCheckBoxPurgeHistory->SetValue(false);
	BoxSizer7->Add(WxCheckBoxPurgeHistory, 0, wxALL|wxEXPAND, 2);
#ifdef __WXMSW__
	WxCheckBoxRightClickMenu = new wxCheckBox(Panel1, ID_WSCHECKBOXMSRIGHTCCLICKMENU, _("Add MadEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WSCHECKBOXMSRIGHTCCLICKMENU"));
	WxCheckBoxRightClickMenu->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRightClickMenu, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer7, 0, wxALL|wxALIGN_LEFT, 2);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
#endif
	Panel2 = new wxPanel(AuiNotebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer9->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxLineLength = new wxTextCtrl(Panel2, ID_WXEDITMAXLINELENGTH, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXLINELENGTH"));
	BoxSizer9->Add(WxEditMaxLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Max line length before Line-Wrap (must restart MadEdit)"), wxDefaultPosition, wxSize(330,13), 0, _T("ID_STATICTEXT3"));
	BoxSizer9->Add(StaticText3, 0, wxALL|wxEXPAND, 5);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxEXPAND, 2);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer10->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxColumns = new wxTextCtrl(Panel2, ID_WXEDITMAXCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXCOLUMNS"));
	BoxSizer10->Add(WxEditMaxColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT4, _("Columns of Wrap-By-Column"), wxDefaultPosition, wxSize(323,13), 0, _T("ID_STATICTEXT4"));
	BoxSizer10->Add(StaticText4, 0, wxALL|wxEXPAND, 5);
	BoxSizer8->Add(BoxSizer10, 0, wxALL|wxEXPAND, 2);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditTabColumns = new wxTextCtrl(Panel2, ID_WXEDITTABCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITTABCOLUMNS"));
	BoxSizer11->Add(WxEditTabColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Columns of Tab"), wxDefaultPosition, wxSize(312,13), 0, _T("ID_STATICTEXT5"));
	BoxSizer11->Add(StaticText5, 0, wxALL|wxEXPAND, 5);
	BoxSizer8->Add(BoxSizer11, 0, wxALL|wxEXPAND, 2);
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer13->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditIndentColumns = new wxTextCtrl(Panel2, IID_WXEDITINDENTCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("IID_WXEDITINDENTCOLUMNS"));
	BoxSizer13->Add(WxEditIndentColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText6 = new wxStaticText(Panel2, ID_STATICTEXT6, _("Columns of Indent"), wxDefaultPosition, wxSize(305,13), 0, _T("ID_STATICTEXT6"));
	BoxSizer13->Add(StaticText6, 0, wxALL|wxEXPAND, 5);
	BoxSizer8->Add(BoxSizer13, 0, wxALL|wxEXPAND, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditDateTime = new wxTextCtrl(Panel2, ID_WXEDITDATETIME, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, wxDefaultValidator, _T("ID_WXEDITDATETIME"));
	BoxSizer5->Add(WxEditDateTime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxStaticTextDateTime = new wxStaticText(Panel2, ID_WXSTATICTEXTDATETIME, _("Format of Date/Time"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTDATETIME"));
	BoxSizer5->Add(WxStaticTextDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxButtonDateTime = new wxButton(Panel2, ID_WXBUTTONDATETIME, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTONDATETIME"));
	BoxSizer5->Add(WxButtonDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBoxDateTimeInEnglish = new wxCheckBox(Panel2, ID_WXCHECKBOXDATETIMEINENGLISH, _("Use English instead of current locale"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXDATETIMEINENGLISH"));
	WxCheckBoxDateTimeInEnglish->SetValue(false);
	BoxSizer5->Add(WxCheckBoxDateTimeInEnglish, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer8->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxTabOrSpaces = new wxCheckBox(Panel2, ID_WXCHECKBOXTABORSPACES, _("Insert Space char instead of Tab char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXTABORSPACES"));
	WxCheckBoxTabOrSpaces->SetValue(false);
	BoxSizer12->Add(WxCheckBoxTabOrSpaces, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxAutoIndent = new wxCheckBox(Panel2, ID_WXCHECKBOXAUTOINDENT, _("Auto Indent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXAUTOINDENT"));
	WxCheckBoxAutoIndent->SetValue(false);
	BoxSizer12->Add(WxCheckBoxAutoIndent, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxAutoCompletePair = new wxCheckBox(Panel2, ID_WXCHECKBOXAUTOCOMPLETEPAIR, _("Auto complete character pair"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXAUTOCOMPLETEPAIR"));
	WxCheckBoxAutoCompletePair->SetValue(false);
	BoxSizer12->Add(WxCheckBoxAutoCompletePair, 0, wxALL|wxEXPAND, 2);
	BoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxMouseSelectToCopy = new wxCheckBox(Panel2, ID_WXCHECKBOXMOUSESELECTTOCOPY, _("Auto copy the mouse-selected text to clipboard"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMOUSESELECTTOCOPY"));
	WxCheckBoxMouseSelectToCopy->SetValue(false);
	BoxSizer28->Add(WxCheckBoxMouseSelectToCopy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWhenPressCtrlKey = new wxCheckBox(Panel2, ID_WXCHECKBOXWHENPRESSCTRLKEY, _("when pressing Ctrl key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWHENPRESSCTRLKEY"));
	WxCheckBoxWhenPressCtrlKey->SetValue(false);
	BoxSizer28->Add(WxCheckBoxWhenPressCtrlKey, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer28, 0, wxALL|wxALIGN_LEFT, 0);
	WxCheckBoxMiddleMouseToPaste = new wxCheckBox(Panel2, ID_WXCHECKBOXMIDDLEMOUSETOPASTE, _("Paste text from clipboard when pressing middle mouse button"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMIDDLEMOUSETOPASTE"));
	WxCheckBoxMiddleMouseToPaste->SetValue(false);
	BoxSizer12->Add(WxCheckBoxMiddleMouseToPaste, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxAutoFillColumnPaste = new wxCheckBox(Panel2, ID_WXCHECKBOXAUTOFILLCOLUMN, _("Auto fill in column paste"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXAUTOFILLCOLUMN"));
	WxCheckBoxAutoFillColumnPaste->SetValue(false);
	BoxSizer12->Add(WxCheckBoxAutoFillColumnPaste, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer12, 0, wxALL|wxEXPAND, 2);
	Panel2->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel2);
	BoxSizer8->SetSizeHints(Panel2);
	Panel3 = new wxPanel(AuiNotebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer15 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Text Mode"));
	WxCheckBoxPrintSyntax = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTSYNTAX, _("Print Syntax Highlighter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTSYNTAX"));
	WxCheckBoxPrintSyntax->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintSyntax, 0, wxALL|wxEXPAND, 1);
	WxCheckBoxPrintLineNumber = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTLINENUMBER, _("Print Line Number"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTLINENUMBER"));
	WxCheckBoxPrintLineNumber->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintLineNumber, 0, wxALL|wxEXPAND, 1);
	WxCheckBoxPrintBookmark = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTBOOKMARK, _("Print Bookmark"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTBOOKMARK"));
	WxCheckBoxPrintBookmark->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintBookmark, 0, wxALL|wxEXPAND, 1);
	WxCheckBoxPrintEndOfLine = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTENDOFLINE, _("Print End of Line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTENDOFLINE"));
	WxCheckBoxPrintEndOfLine->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintEndOfLine, 0, wxALL|wxEXPAND, 1);
	WxCheckBoxPrintTabChar = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTTABCHAR, _("Print Tab Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTTABCHAR"));
	WxCheckBoxPrintTabChar->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintTabChar, 0, wxALL|wxEXPAND, 1);
	WxCheckBoxPrintSpaceChar = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTSPACECHAR, _("Print Space Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTSPACECHAR"));
	WxCheckBoxPrintSpaceChar->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintSpaceChar, 0, wxALL|wxEXPAND, 1);
	BoxSizer15->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Hex Mode"));
	wxString __wxRadioBoxChoices_1[3] =
	{
		_("None"),
		_("First Page Only"),
		_("Every Page")
	};
	WxRadioBoxPrintOffset = new wxRadioBox(Panel3, ID_WXRADIOBOXPRINTOFFSET, _("Print Offset Header"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXPRINTOFFSET"));
	StaticBoxSizer2->Add(WxRadioBoxPrintOffset, 0, wxALL|wxEXPAND, 1);
	BoxSizer15->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer15, 0, wxALL|wxALIGN_TOP, 5);
	BoxSizer16 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Header"));
	WxCheckBoxPrintPageHeader = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTPAGEHEADER, _("Print Page Header"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTPAGEHEADER"));
	WxCheckBoxPrintPageHeader->SetValue(false);
	StaticBoxSizer3->Add(WxCheckBoxPrintPageHeader, 0, wxALL|wxEXPAND, 1);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText7 = new wxStaticText(Panel3, ID_STATICTEXT7, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 1, wxALL|wxEXPAND, 2);
	WxEditHeaderLeft = new wxTextCtrl(Panel3, ID_WXEDITHEADERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERLEFT"));
	FlexGridSizer1->Add(WxEditHeaderLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton1 = new wxButton(Panel3, ID_WXBUTTON1, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON1"));
	FlexGridSizer1->Add(WxButton1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText8 = new wxStaticText(Panel3, ID_STATICTEXT8, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxEXPAND, 2);
	WxEditHeaderCenter = new wxTextCtrl(Panel3, ID_WXEDITHEADERCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERCENTER"));
	FlexGridSizer1->Add(WxEditHeaderCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton2 = new wxButton(Panel3, ID_WXBUTTON2, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON2"));
	FlexGridSizer1->Add(WxButton2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText9 = new wxStaticText(Panel3, ID_STATICTEXT9, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxEXPAND, 2);
	WxEditHeaderRight = new wxTextCtrl(Panel3, ID_WXEDITHEADERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERRIGHT"));
	FlexGridSizer1->Add(WxEditHeaderRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton3 = new wxButton(Panel3, ID_WXBUTTON3, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON3"));
	FlexGridSizer1->Add(WxButton3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer3->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Footer"));
	WxCheckBoxPrintPageFooter = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTPAGEFOOTER, _("Print Page Footer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTPAGEFOOTER"));
	WxCheckBoxPrintPageFooter->SetValue(false);
	StaticBoxSizer4->Add(WxCheckBoxPrintPageFooter, 0, wxALL|wxEXPAND, 1);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText10 = new wxStaticText(Panel3, ID_STATICTEXT10, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(StaticText10, 0, wxALL|wxEXPAND, 2);
	WxEditFooterLeft = new wxTextCtrl(Panel3, ID_WXEDITFOOTERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERLEFT"));
	FlexGridSizer2->Add(WxEditFooterLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton4 = new wxButton(Panel3, ID_WXBUTTON4, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON4"));
	FlexGridSizer2->Add(WxButton4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText11 = new wxStaticText(Panel3, ID_STATICTEXT11, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer2->Add(StaticText11, 0, wxALL|wxEXPAND, 2);
	WxEditFooterCenter = new wxTextCtrl(Panel3, ID_WXEDITFOOTERLEFTCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERLEFTCENTER"));
	FlexGridSizer2->Add(WxEditFooterCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton5 = new wxButton(Panel3, ID_WXBUTTON5, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON5"));
	FlexGridSizer2->Add(WxButton5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText12 = new wxStaticText(Panel3, ID_STATICTEXT12, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer2->Add(StaticText12, 0, wxALL|wxEXPAND, 2);
	WxEditFooterRight = new wxTextCtrl(Panel3, ID_WXEDITFOOTERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERRIGHT"));
	FlexGridSizer2->Add(WxEditFooterRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButton6 = new wxButton(Panel3, ID_WXBUTTON6, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTON6"));
	FlexGridSizer2->Add(WxButton6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer4->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer16, 0, wxALL|wxALIGN_TOP, 2);
	Panel3->SetSizer(BoxSizer14);
	BoxSizer14->Fit(Panel3);
	BoxSizer14->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	WxTreeCtrlCmdList = new wxTreeCtrl(Panel4, ID_WXREECTRLCMDLIST, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_WXREECTRLCMDLIST"));
	BoxSizer19->Add(WxTreeCtrlCmdList, 1, wxALL|wxEXPAND, 3);
	BoxSizer18->Add(BoxSizer19, 2, wxALL|wxEXPAND, 4);
	BoxSizer20 = new wxBoxSizer(wxVERTICAL);
	BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	WxStaticTextCommandHint = new wxStaticText(Panel4, ID_WXSTATICTEXTCOMMANDHINT, _("Hint:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCOMMANDHINT"));
	BoxSizer24->Add(WxStaticTextCommandHint, 0, wxALL|wxEXPAND, 5);
	WxEditCommandHint = new wxTextCtrl(Panel4, ID_WXEDITHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_WXEDITHINT"));
	WxEditCommandHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer24->Add(WxEditCommandHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer20->Add(BoxSizer24, 0, wxALL|wxEXPAND, 2);
	BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer22 = new wxBoxSizer(wxVERTICAL);
	StaticText14 = new wxStaticText(Panel4, ID_STATICTEXT14, _("Assigned Keys:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	BoxSizer22->Add(StaticText14, 0, wxALL|wxEXPAND, 5);
	WxListBoxKeys = new wxListBox(Panel4, ID_WXLISTBOXKEYS, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("ID_WXLISTBOXKEYS"));
	BoxSizer22->Add(WxListBoxKeys, 1, wxALL|wxEXPAND, 2);
	BoxSizer21->Add(BoxSizer22, 0, wxALL|wxEXPAND, 2);
	BoxSizer25 = new wxBoxSizer(wxVERTICAL);
	StaticText15 = new wxStaticText(Panel4, ID_STATICTEXT15, _("New Key:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	BoxSizer25->Add(StaticText15, 0, wxALL|wxEXPAND, 5);
	WxEditKey = new KeyTextCtrl(Panel4, ID_WXEDITKEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxWANTS_CHARS, wxDefaultValidator, _T("ID_WXEDITKEY"));
	BoxSizer25->Add(WxEditKey, 0, wxALL|wxEXPAND, 5);
	WxEditKeyHint = new wxTextCtrl(Panel4, ID_WXEDITKEYHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_WXEDITKEYHINT"));
	WxEditKeyHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer25->Add(WxEditKeyHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer29 = new wxBoxSizer(wxVERTICAL);
	WxButtonAddKey = new wxButton(Panel4, ID_WXBUTTONADDKEY, _("<== Add Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONADDKEY"));
	BoxSizer29->Add(WxButtonAddKey, 0, wxALL|wxEXPAND, 5);
	WxButtonDeleteKey = new wxButton(Panel4, ID_WXBUTTONDELETEKEY, _("==> Delete Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONDELETEKEY"));
	BoxSizer29->Add(WxButtonDeleteKey, 0, wxALL|wxEXPAND, 5);
	WxButtonShowInMenu = new wxButton(Panel4, ID_WXBUTTONSHOWINMENU, _("Show the Key in Menu"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONSHOWINMENU"));
	BoxSizer29->Add(WxButtonShowInMenu, 0, wxALL|wxEXPAND, 5);
	BoxSizer25->Add(BoxSizer29, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer21->Add(BoxSizer25, 5, wxALL|wxEXPAND, 1);
	BoxSizer20->Add(BoxSizer21, 1, wxALL|wxEXPAND, 2);
	BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxResetAllKeys = new wxCheckBox(Panel4, ID_WXCHECKBOXRESETALLKEYS, _("Reset all keys to default (must restart MadEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRESETALLKEYS"));
	WxCheckBoxResetAllKeys->SetValue(false);
	BoxSizer26->Add(WxCheckBoxResetAllKeys, 0, wxALL|wxEXPAND, 5);
	BoxSizer20->Add(BoxSizer26, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer18->Add(BoxSizer20, 3, wxALL|wxEXPAND, 2);
	Panel4->SetSizer(BoxSizer18);
	BoxSizer18->Fit(Panel4);
	BoxSizer18->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	BoxSizer36 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxPersonalDict = new wxCheckBox(Panel5, ID_WXCHECKBOXPERSONALDICT, _("Enable Personal Dictionary"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPERSONALDICT"));
	WxCheckBoxPersonalDict->SetValue(false);
	BoxSizer36->Add(WxCheckBoxPersonalDict, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Language"));
	StaticText17 = new wxStaticText(Panel5, ID_STATICTEXT17, _("Dictionary:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	StaticBoxSizer5->Add(StaticText17, 0, wxALL|wxEXPAND, 5);
	WxChoiceDictionary = new wxChoice(Panel5, ID_WXCHOICEDICTIONARY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_WXCHOICEDICTIONARY"));
	StaticBoxSizer5->Add(WxChoiceDictionary, 0, wxALL|wxEXPAND, 5);
	BoxSizer36->Add(StaticBoxSizer5, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, Panel5, _("Path Setting"));
	StaticText18 = new wxStaticText(Panel5, ID_STATICTEXT18, _("Dictionary:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	StaticBoxSizer6->Add(StaticText18, 0, wxALL|wxEXPAND, 5);
	WxEditDictionaryDir = new wxTextCtrl(Panel5, ID_WXEDITDICTIONARYDIR, wxEmptyString, wxDefaultPosition, wxSize(187,21), 0, wxDefaultValidator, _T("ID_WXEDITDICTIONARYDIR"));
	StaticBoxSizer6->Add(WxEditDictionaryDir, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonDictionaryDir = new wxButton(Panel5, ID_WXDICTIONARY_DIR, _("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXDICTIONARY_DIR"));
	StaticBoxSizer6->Add(WxButtonDictionaryDir, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer36->Add(StaticBoxSizer6, 0, wxALL|wxEXPAND, 5);
	Panel5->SetSizer(BoxSizer36);
	BoxSizer36->Fit(Panel5);
	BoxSizer36->SetSizeHints(Panel5);
	Panel6 = new wxPanel(AuiNotebook1, ID_PANEL6, wxDefaultPosition, wxSize(480,240), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	AuiNotebook2 = new wxAuiNotebook(Panel6, ID_AUINOTEBOOK2, wxDefaultPosition, wxSize(300,360), wxAUI_NB_TOP|wxTAB_TRAVERSAL);
	Panel7 = new wxPanel(AuiNotebook2, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	GridSizer2 = new wxGridSizer(0, 2, 0, 0);
	wxString __wxRadioBoxChoices_2[15] =
	{
		_("Allman(ANSI)"),
		_("Java"),
		_("Kr"),
		_("Stroustrup"),
		_("Whitesmith"),
		_("VTK"),
		_("Banner"),
		_("Gnu"),
		_("Linux"),
		_("Horstmann"),
		_("1TBS"),
		_("Google"),
		_("Pico"),
		_("Lisp"),
		_("Custom")
	};
	WxRadioBoxBracketStyle = new wxRadioBox(Panel7, ID_WXRADIOBOXBRACKETSTYLE, _("Bracket Style"), wxDefaultPosition, wxDefaultSize, 15, __wxRadioBoxChoices_2, 1, 0, wxDefaultValidator, _T("ID_WXRADIOBOXBRACKETSTYLE"));
	GridSizer2->Add(WxRadioBoxBracketStyle, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxVERTICAL, Panel7, _("Sample"));
	StaticText19 = new wxStaticText(Panel7, ID_STATICTEXT19, _("Sample of the bracket style option chosen to use:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	StaticBoxSizer7->Add(StaticText19, 0, wxALL|wxALIGN_LEFT, 5);
#ifdef MADEDIT_ENABLE_MADEDIT
	WxTextSample = new MadEdit(Panel7, ID_WXTEXTSAMPLE , wxDefaultPosition, wxSize(300, 300));
	WxTextSample->SetEncoding(wxT("UTF-32LE"));
	WxTextSample->SetFixedWidthMode(false);
	WxTextSample->SetRecordCaretMovements(false);
	WxTextSample->SetInsertSpacesInsteadOfTab(false);
	WxTextSample->SetWantTab(false);
	WxTextSample->SetMaxLineLength(DEFAULT_MAX_LINELEN);
	WxTextSample->SetReadOnly(true);
	WxTextSample->SetDisplayBookmark(false);
	WxTextSample->SetShowSpaceChar(false);
	WxTextSample->SetShowTabChar(false);
	WxTextSample->SetShowEndOfLine(false);
	WxTextSample->SetSpellCheck(false);
	WxTextSample->SetSyntax(wxT("C/C++"));
	WxTextSample->SetText(bracket_style[aspsAllman]);
#elif MADEDIT_ENABLE_STC
	WxTextSample = new wxStyledTextCtrl( Panel7, ID_WXTEXTSAMPLE, wxPoint( 19, 47 ), wxSize( 219, 300 ) );
	WxTextSample->StyleClearAll();
	WxTextSample->SetLexer( wxSTC_LEX_CPP );
	WxTextSample->SetMarginWidth( MARGIN_LINE_NUMBERS, 50 );
	WxTextSample->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( 75, 75, 75 ) );
	WxTextSample->StyleSetBackground( wxSTC_STYLE_LINENUMBER, wxColour( 220, 220, 220 ) );
	WxTextSample->SetMarginType( MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER );
	// ---- Enable code folding
	WxTextSample->SetMarginType( MARGIN_FOLD, wxSTC_MARGIN_SYMBOL );
	WxTextSample->SetMarginWidth( MARGIN_FOLD, 15 );
	WxTextSample->SetMarginMask( MARGIN_FOLD, wxSTC_MASK_FOLDERS );
	WxTextSample->StyleSetBackground( MARGIN_FOLD, wxColor( 200, 200, 200 ) );
	WxTextSample->SetMarginSensitive( MARGIN_FOLD, true );
	// Properties found from http://www.scintilla.org/SciTEDoc.html
	WxTextSample->SetProperty( wxT( "fold" ),         wxT( "1" ) );
	WxTextSample->SetProperty( wxT( "fold.comment" ), wxT( "1" ) );
	WxTextSample->SetProperty( wxT( "fold.compact" ), wxT( "1" ) );
	wxColor grey( 100, 100, 100 );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_ARROW );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, _T( "WHITE" ) );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, _T( "WHITE" ) );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	WxTextSample->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY );
	WxTextSample->MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	WxTextSample->MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	// ---- End of code folding part
	WxTextSample->SetWrapMode( wxSTC_WRAP_WORD ); // other choice is wxSCI_WRAP_NONE
	WxTextSample->SetText( bracket_style[aspsAllman] );
	WxTextSample->StyleSetForeground( wxSTC_C_STRING,            wxColour( 150, 0, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_PREPROCESSOR,      wxColour( 165, 105, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_IDENTIFIER,        wxColour( 40, 0, 60 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_NUMBER,            wxColour( 0, 150, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_CHARACTER,         wxColour( 150, 0, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_WORD,              wxColour( 0, 0, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_WORD2,             wxColour( 0, 150, 0 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENT,           wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTLINE,       wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOC,        wxColour( 150, 150, 150 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORD, wxColour( 0, 0, 200 ) );
	WxTextSample->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour( 0, 0, 200 ) );
	WxTextSample->StyleSetBold( wxSTC_C_WORD, true );
	WxTextSample->StyleSetBold( wxSTC_C_WORD2, true );
	WxTextSample->StyleSetBold( wxSTC_C_COMMENTDOCKEYWORD, true );
	// a sample list of keywords, I haven't included them all to keep it short...
	WxTextSample->SetKeyWords( 0, wxT( "if else return for while break continue" ) );
	WxTextSample->SetKeyWords( 1, wxT( "const int float void char double bool" ) );
	WxTextSample->SetReadOnly( true );
	WxTextSample->Connect( wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler( MadOptionsDialog::OnMarginClick ), NULL, this );
#else
	WxTextSample = new wxTextCtrl(Panel7, ID_WXTEXTSAMPLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXTEXTSAMPLE"));
	//WxTextSample->SetMaxLength(0);
	WxTextSample->AppendText( bracket_style[aspsAllman] );
#endif
	WxTextSample->SetFocus();
#ifndef MADEDIT_ENABLE_MADEDIT
	WxTextSample->SetInsertionPointEnd();
	SET_CONTROLPARENT( WxTextSample );
#endif
	StaticBoxSizer7->Add(WxTextSample, 0, wxALL|wxEXPAND, 5);
	GridSizer2->Add(StaticBoxSizer7, 1, wxALL|wxEXPAND, 5);
	Panel7->SetSizer(GridSizer2);
	GridSizer2->Fit(Panel7);
	GridSizer2->SetSizeHints(Panel7);
	Panel8 = new wxPanel(AuiNotebook2, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
	BoxSizer34 = new wxBoxSizer(wxVERTICAL);
	WxCheckAttachClasses = new wxCheckBox(Panel8, ID_WXCHECKATTACHCLASSES, _("Attach Classes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKATTACHCLASSES"));
	WxCheckAttachClasses->SetValue(false);
	BoxSizer34->Add(WxCheckAttachClasses, 0, wxALL|wxEXPAND, 5);
	WxCheckAttachExternC = new wxCheckBox(Panel8, ID_WXCHECKATTACHEXTERNC, _("Attach Extern C"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKATTACHEXTERNC"));
	WxCheckAttachExternC->SetValue(false);
	BoxSizer34->Add(WxCheckAttachExternC, 0, wxALL|wxEXPAND, 5);
	WxCheckAttachNamespaces = new wxCheckBox(Panel8, ID_WXCHECKATTACHNAMESPACES, _("Attach Namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKATTACHNAMESPACES"));
	WxCheckAttachNamespaces->SetValue(false);
	BoxSizer34->Add(WxCheckAttachNamespaces, 0, wxALL|wxEXPAND, 5);
	WxCheckAttachInlines = new wxCheckBox(Panel8, ID_WXCHECKATTACHINLINES, _("Attach Inlines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKATTACHINLINES"));
	WxCheckAttachInlines->SetValue(false);
	BoxSizer34->Add(WxCheckAttachInlines, 0, wxALL|wxEXPAND, 5);
	Panel8->SetSizer(BoxSizer34);
	BoxSizer34->Fit(Panel8);
	BoxSizer34->SetSizeHints(Panel8);
	Panel9 = new wxPanel(AuiNotebook2, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	BoxSizer35 = new wxBoxSizer(wxVERTICAL);
	GridSizer4 = new wxGridSizer(0, 2, 0, 0);
	StaticText20 = new wxStaticText(Panel9, ID_STATICTEXT20, _("Indentation size (in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	GridSizer4->Add(StaticText20, 0, wxALL|wxEXPAND, 2);
	WxSpinIndentation = new wxSpinCtrl(Panel9, ID_WXSPINCTRLINDENTSIZE, _T("4"), wxDefaultPosition, wxDefaultSize, 0, 2, 8, 4, _T("ID_WXSPINCTRLINDENTSIZE"));
	WxSpinIndentation->SetValue(_T("4"));
	GridSizer4->Add(WxSpinIndentation, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer35->Add(GridSizer4, 0, wxALL|wxEXPAND, 2);
	WxCheckUseTab = new wxCheckBox(Panel9, ID_WXCHECKUSETABOVERSPACES, _("Use TABs instead of spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKUSETABOVERSPACES"));
	WxCheckUseTab->SetValue(false);
	BoxSizer35->Add(WxCheckUseTab, 0, wxALL|wxEXPAND, 2);
	WxCheckForceUseTabs = new wxCheckBox(Panel9, ID_WXCHECKFORCETABS, _("Force using TABs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKFORCETABS"));
	WxCheckForceUseTabs->SetValue(false);
	BoxSizer35->Add(WxCheckForceUseTabs, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentCase = new wxCheckBox(Panel9, ID_WXCHECKINDENTCASE, _("Indent case: statement in switches (commands under case:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTCASE"));
	WxCheckIndentCase->SetValue(false);
	BoxSizer35->Add(WxCheckIndentCase, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckIndentClasses = new wxCheckBox(Panel9, ID_WXCHECKINDENETCLASSES, _("Indent classes (keywords public:, protected: and private:)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENETCLASSES"));
	WxCheckIndentClasses->SetValue(false);
	BoxSizer35->Add(WxCheckIndentClasses, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentLabels = new wxCheckBox(Panel9, ID_WXCHECKINDENTLABELS, _("Indent labels"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTLABELS"));
	WxCheckIndentLabels->SetValue(false);
	BoxSizer35->Add(WxCheckIndentLabels, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentModifiers = new wxCheckBox(Panel9, ID_WXCHECKINDENTMODIFIERS, _("Indent modifiers"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTMODIFIERS"));
	WxCheckIndentModifiers->SetValue(false);
	BoxSizer35->Add(WxCheckIndentModifiers, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentNamespaces = new wxCheckBox(Panel9, ID_WXCHECKINDENTNS, _("Indent namespaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTNS"));
	WxCheckIndentNamespaces->SetValue(false);
	BoxSizer35->Add(WxCheckIndentNamespaces, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentSwitches = new wxCheckBox(Panel9, ID_WXCHECKINDENTSWITCHS, _("Indent switches"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTSWITCHS"));
	WxCheckIndentSwitches->SetValue(false);
	BoxSizer35->Add(WxCheckIndentSwitches, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentPreprocBlock = new wxCheckBox(Panel9, ID_WXCHECKINDENTPREP, _("Indent preprocessor blocks at bracket level zero, and within a namespace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTPREP"));
	WxCheckIndentPreprocBlock->SetValue(false);
	BoxSizer35->Add(WxCheckIndentPreprocBlock, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentPreprocDefine = new wxCheckBox(Panel9, ID_WXCHECKINDENTMULTIPREP, _("Indent multi-line preprocessor definitions ending with a backslash"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTMULTIPREP"));
	WxCheckIndentPreprocDefine->SetValue(false);
	BoxSizer35->Add(WxCheckIndentPreprocDefine, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentPreprocCond = new wxCheckBox(Panel9, ID_WXCHECKINDENTPREPCOND, _("Indent preprocessor conditionals"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENTPREPCOND"));
	WxCheckIndentPreprocCond->SetValue(false);
	BoxSizer35->Add(WxCheckIndentPreprocCond, 0, wxALL|wxEXPAND, 2);
	WxCheckIndentCol1Comments = new wxCheckBox(Panel9, ID_WXCHECKINDENETCPPCOMMENTS, _("Indent C++ comments beginning in column one"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKINDENETCPPCOMMENTS"));
	WxCheckIndentCol1Comments->SetValue(false);
	BoxSizer35->Add(WxCheckIndentCol1Comments, 0, wxALL|wxEXPAND, 2);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	StaticText24 = new wxStaticText(Panel9, ID_STATICTEXT24, _("Minimal indent added when a header is built of multiple lines:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer3->Add(StaticText24, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxSpinMinConditionalEvent = new wxSpinCtrl(Panel9, ID_WXSPINCTRLMININDENT, _T("2"), wxDefaultPosition, wxDefaultSize, 0, 0, 3, 2, _T("ID_WXSPINCTRLMININDENT"));
	WxSpinMinConditionalEvent->SetValue(_T("2"));
	FlexGridSizer3->Add(WxSpinMinConditionalEvent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText31 = new wxStaticText(Panel9, ID_STATICTEXT31, _("Maximum of # spaces to indent a continuation line (max. 120):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
	FlexGridSizer3->Add(StaticText31, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxEditMaxInStatementIndent = new wxTextCtrl(Panel9, ID_WXEDITMAXINSTATEMENTINDENT, _("40"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXINSTATEMENTINDENT"));
	FlexGridSizer3->Add(WxEditMaxInStatementIndent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer35->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel9->SetSizer(BoxSizer35);
	BoxSizer35->Fit(Panel9);
	BoxSizer35->SetSizeHints(Panel9);
	Panel10 = new wxPanel(AuiNotebook2, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	BoxSizer41 = new wxBoxSizer(wxVERTICAL);
	WxCheckBreakClosing = new wxCheckBox(Panel10, ID_WXCHECKBREAKCLOSING, _("Break closing headers from their immediately preceding closing brackets"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKCLOSING"));
	WxCheckBreakClosing->SetValue(false);
	BoxSizer41->Add(WxCheckBreakClosing, 0, wxALL|wxEXPAND, 2);
	WxCheckBreakElseIfs = new wxCheckBox(Panel10, ID_WXCHECKBREAKELSEIFS, _("Break \'else if()\' header combinations into seperate lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKELSEIFS"));
	WxCheckBreakElseIfs->SetValue(false);
	BoxSizer41->Add(WxCheckBreakElseIfs, 0, wxALL|wxEXPAND, 2);
	WxCheckAddBrackets = new wxCheckBox(Panel10, ID_WXCHECKADDBRACKETS, _("Add brackets to unbracketed one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKADDBRACKETS"));
	WxCheckAddBrackets->SetValue(false);
	BoxSizer41->Add(WxCheckAddBrackets, 0, wxALL|wxEXPAND, 2);
	WxCheckAddOneLineBrackets = new wxCheckBox(Panel10, ID_WXCHECKADDONELINEBRACKETS, _("Add one line brackets to unbracketed one line conditional statements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKADDONELINEBRACKETS"));
	WxCheckAddOneLineBrackets->SetValue(false);
	BoxSizer41->Add(WxCheckAddOneLineBrackets, 0, wxALL|wxEXPAND, 2);
	WxCheckRemoveBrackets = new wxCheckBox(Panel10, ID_WXCHECKBOXREMOVEBRACKETS, _("Remove brackets from conditional statements (single statement on a single line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREMOVEBRACKETS"));
	WxCheckRemoveBrackets->SetValue(false);
	BoxSizer41->Add(WxCheckRemoveBrackets, 0, wxALL|wxEXPAND, 2);
	WxCheckKeepBlocks = new wxCheckBox(Panel10, ID_WXCHECKKEEPBLOCKS, _("Don\'t break one-line blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKKEEPBLOCKS"));
	WxCheckKeepBlocks->SetValue(false);
	BoxSizer41->Add(WxCheckKeepBlocks, 0, wxALL|wxEXPAND, 2);
	WxCheckKeepComplex = new wxCheckBox(Panel10, ID_WXCHECKKEEPCOMPLEX, _("Don\'t break complex statements and multiple statements residing in a single line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKKEEPCOMPLEX"));
	WxCheckKeepComplex->SetValue(false);
	BoxSizer41->Add(WxCheckKeepComplex, 0, wxALL|wxEXPAND, 2);
	WxCheckConvertTabs = new wxCheckBox(Panel10, ID_WXCHECKCONVERTTABS, _("Convert TABs to spaces"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKCONVERTTABS"));
	WxCheckConvertTabs->SetValue(false);
	BoxSizer41->Add(WxCheckConvertTabs, 0, wxALL|wxEXPAND, 2);
	WxCheckCloseTemplates = new wxCheckBox(Panel10, ID_WXCHECKCLOSETEMPLATES, _("Closes whitespace in the angle brackets of template definitions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKCLOSETEMPLATES"));
	WxCheckCloseTemplates->SetValue(false);
	BoxSizer41->Add(WxCheckCloseTemplates, 0, wxALL|wxEXPAND, 2);
	WxCheckRemoveCommentPrefix = new wxCheckBox(Panel10, ID_WXCHECKREMOVECOMMENTPREFIX, _("Remove the preceding \'*\' in a multi-line comment that begins a line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKREMOVECOMMENTPREFIX"));
	WxCheckRemoveCommentPrefix->SetValue(false);
	BoxSizer41->Add(WxCheckRemoveCommentPrefix, 0, wxALL|wxEXPAND, 2);
	WxCheckBreakLines = new wxCheckBox(Panel10, ID_WXCHECKBREAKLINES, _("Enable line breaking"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKLINES"));
	WxCheckBreakLines->SetValue(false);
	BoxSizer41->Add(WxCheckBreakLines, 0, wxALL|wxEXPAND, 2);
	BoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer42->Add(10,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText23 = new wxStaticText(Panel10, ID_STATICTEXT23, _("Break lines after amount of chars (range: 50-200)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	BoxSizer42->Add(StaticText23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxEditSFMaxLineLength = new wxTextCtrl(Panel10, ID_WXEDITSFMAXLINELENGTH, _("200"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITSFMAXLINELENGTH"));
	BoxSizer42->Add(WxEditSFMaxLineLength, 0, wxALL|wxEXPAND, 2);
	BoxSizer41->Add(BoxSizer42, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer43->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBreakAfterLogical = new wxCheckBox(Panel10, ID_WXCHECKBREAKAFTERLOGICAL, _("Break logical conditionals to be placed last on the previous line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKAFTERLOGICAL"));
	WxCheckBreakAfterLogical->SetValue(false);
	BoxSizer43->Add(WxCheckBreakAfterLogical, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer41->Add(BoxSizer43, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel10->SetSizer(BoxSizer41);
	BoxSizer41->Fit(Panel10);
	BoxSizer41->SetSizeHints(Panel10);
	Panel11 = new wxPanel(AuiNotebook2, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	BoxSizer47 = new wxBoxSizer(wxVERTICAL);
	WxCheckBreakBlocks = new wxCheckBox(Panel11, ID_WXCHECKBREAKBLOCKS, _("Pad empty lines around header blocks (e.g. \'if\', \'while\'...)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKBLOCKS"));
	WxCheckBreakBlocks->SetValue(false);
	BoxSizer47->Add(WxCheckBreakBlocks, 0, wxALL|wxEXPAND, 2);
	BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer23->Add(10,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBreakBlocksAll = new wxCheckBox(Panel11, ID_WXCHECKBREAKBLOCKSALL, _("Treat closing header blocks (e.g. \'else\', \'catch\') as stand-alone blocks"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBREAKBLOCKSALL"));
	WxCheckBreakBlocksAll->SetValue(false);
	BoxSizer23->Add(WxCheckBreakBlocksAll, 0, wxALL|wxEXPAND, 2);
	BoxSizer47->Add(BoxSizer23, 0, wxALL|wxEXPAND, 2);
	WxCheckPadOperators = new wxCheckBox(Panel11, ID_WXCHECKPADOPERATORS, _("Insert space padding around operators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKPADOPERATORS"));
	WxCheckPadOperators->SetValue(false);
	BoxSizer47->Add(WxCheckPadOperators, 0, wxALL|wxEXPAND, 2);
	WxCheckPadParensOut = new wxCheckBox(Panel11, ID_WXCHECKPADPARENSOUT, _("Insert space padding around parenthesies on the outside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKPADPARENSOUT"));
	WxCheckPadParensOut->SetValue(false);
	BoxSizer47->Add(WxCheckPadParensOut, 0, wxALL|wxEXPAND, 2);
	WxCheckPadParensIn = new wxCheckBox(Panel11, ID_WXCHECKPADPARENSIN, _("Insert space padding around parenthesies on the inside"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKPADPARENSIN"));
	WxCheckPadParensIn->SetValue(false);
	BoxSizer47->Add(WxCheckPadParensIn, 0, wxALL|wxEXPAND, 2);
	WxCheckPadHeader = new wxCheckBox(Panel11, ID_WXCHECKPADHEADER, _("Insert space padding between a header and the following parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKPADHEADER"));
	WxCheckPadHeader->SetValue(false);
	BoxSizer47->Add(WxCheckPadHeader, 0, wxALL|wxEXPAND, 2);
	WxCheckUnpadParens = new wxCheckBox(Panel11, ID_WXCHECKUNPADPARENS, _("Remove extra space padding around parenthesis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKUNPADPARENS"));
	WxCheckUnpadParens->SetValue(false);
	BoxSizer47->Add(WxCheckUnpadParens, 0, wxALL|wxEXPAND, 2);
	WxCheckDelEmptyLine = new wxCheckBox(Panel11, ID_WXCHECKDELEMPTYLINE, _("Delete empty lines within a function or method"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKDELEMPTYLINE"));
	WxCheckDelEmptyLine->SetValue(false);
	BoxSizer47->Add(WxCheckDelEmptyLine, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckFillEmptyLines = new wxCheckBox(Panel11, ID_WXCHECKFILLEMPTYLINES, _("Fill empty lines with the whitespace of their previous lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKFILLEMPTYLINES"));
	WxCheckFillEmptyLines->SetValue(false);
	BoxSizer47->Add(WxCheckFillEmptyLines, 0, wxALL|wxEXPAND, 2);
	GridSizer3 = new wxGridSizer(0, 4, 0, 0);
	StaticText26 = new wxStaticText(Panel11, ID_STATICTEXT26, _("Reference alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	GridSizer3->Add(StaticText26, 0, wxALL|wxEXPAND, 2);
	WxChoicePointerAlign = new wxChoice(Panel11, ID_WXCHOICEPOINTERALIGN, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_WXCHOICEPOINTERALIGN"));
	WxChoicePointerAlign->SetSelection( WxChoicePointerAlign->Append(_("None")) );
	WxChoicePointerAlign->Append(_("Type"));
	WxChoicePointerAlign->Append(_("Middle"));
	WxChoicePointerAlign->Append(_("Name"));
	GridSizer3->Add(WxChoicePointerAlign, 0, wxALL|wxEXPAND, 2);
	StaticText27 = new wxStaticText(Panel11, ID_STATICTEXT27, _("Reference alignment"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	GridSizer3->Add(StaticText27, 0, wxALL|wxEXPAND, 2);
	WxChoiceReferenceAlign = new wxChoice(Panel11, ID_WXCHOICEREFERENCEALIGN, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_WXCHOICEREFERENCEALIGN"));
	WxChoiceReferenceAlign->SetSelection( WxChoiceReferenceAlign->Append(_("None")) );
	WxChoiceReferenceAlign->Append(_("Type"));
	WxChoiceReferenceAlign->Append(_("Middle"));
	WxChoiceReferenceAlign->Append(_("Name"));
	GridSizer3->Add(WxChoiceReferenceAlign, 0, wxALL|wxEXPAND, 2);
	BoxSizer47->Add(GridSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Panel11->SetSizer(BoxSizer47);
	BoxSizer47->Fit(Panel11);
	BoxSizer47->SetSizeHints(Panel11);
	Panel12 = new wxPanel(AuiNotebook2, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	BoxSizer44 = new wxBoxSizer(wxVERTICAL);
	GridSizer1 = new wxGridSizer(0, 2, 0, 0);
	StaticText28 = new wxStaticText(Panel12, ID_STATICTEXT28, _("XML version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	GridSizer1->Add(StaticText28, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxEditXMLversion = new wxTextCtrl(Panel12, ID_WXEDITXMLVERSION, _("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITXMLVERSION"));
	GridSizer1->Add(WxEditXMLversion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText25 = new wxStaticText(Panel12, ID_STATICTEXT25, _("Indentation size(in spaces):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	GridSizer1->Add(StaticText25, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditXmlIndentSize = new wxTextCtrl(Panel12, ID_WXEDITXMLINDENTSIZE, _("4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITXMLINDENTSIZE"));
	GridSizer1->Add(WxEditXmlIndentSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer44->Add(GridSizer1, 0, wxALL|wxALIGN_LEFT, 5);
	Panel12->SetSizer(BoxSizer44);
	BoxSizer44->Fit(Panel12);
	BoxSizer44->SetSizeHints(Panel12);
	AuiNotebook2->AddPage(Panel7, _("Style"));
	AuiNotebook2->AddPage(Panel8, _("Brackets"));
	AuiNotebook2->AddPage(Panel9, _("Indentation"));
	AuiNotebook2->AddPage(Panel10, _("Formatting"));
	AuiNotebook2->AddPage(Panel11, _("Padding"));
	AuiNotebook2->AddPage(Panel12, _("XML"));
	BoxSizer6->Add(AuiNotebook2, 1, wxALL|wxEXPAND, 5);
	Panel6->SetSizer(BoxSizer6);
	//SetSizer(BoxSizer6);
	//Layout();
	AuiNotebook1->AddPage(Panel1, _("General"));
	AuiNotebook1->AddPage(Panel2, _("Edit"));
	AuiNotebook1->AddPage(Panel3, _("Print"));
	AuiNotebook1->AddPage(Panel4, _("Keys"));
	AuiNotebook1->AddPage(Panel5, _("SpellChecker"));
	AuiNotebook1->AddPage(Panel6, _("Source Formater"));
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
	WxButtonOK = new wxButton(this, ID_WXBUTTONOK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONOK"));
	BoxSizer2->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, ID_WXBUTTONCANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCANCEL"));
	BoxSizer2->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	Layout();
	Center();

	Connect(ID_WXBUTTONDATETIME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonDateTimeClick);
	Connect(ID_WXCHECKBOXMOUSESELECTTOCOPY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::OnMouseAutoCopyClicked);
	Connect(ID_WXREECTRLCMDLIST,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&MadOptionsDialog::WxTreeCtrlCmdListSelectionChanged);
	Connect(ID_WXLISTBOXKEYS,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::WxListBoxKeysSelected);
	Connect(ID_WXBUTTONADDKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonAddKeyClick);
	Connect(ID_WXBUTTONDELETEKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonDeleteKeyClick);
	Connect(ID_WXBUTTONSHOWINMENU,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonShowInMenuClick);
	Connect(ID_WXCHOICEDICTIONARY,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::OnSelectDictionary);
	Connect(ID_WXEDITDICTIONARYDIR,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&MadOptionsDialog::OnWxEditDictionaryDirText);
	Connect(ID_WXDICTIONARY_DIR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonDictionaryDirClick);
	Connect(ID_WXRADIOBOXBRACKETSTYLE,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&MadOptionsDialog::OnWxRadioBoxBracketStyleSelect);
	Connect(ID_WXCHECKBREAKLINES,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::OnFormattingBreakLinesClick);
	Connect(ID_WXCHECKBREAKBLOCKS,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::OnPaddingBreakBlocksClick);
	Connect(ID_WXBUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonOKClick);
	Connect(ID_WXBUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MadOptionsDialog::WxButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MadOptionsDialog::MadOptionsDialogClose);
	//*)

	WxPopupMenuPrintMark.Append(ID_MNU_MENUITEM1_1110, _("[%f] &File Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___P__PATHNAME_1111, _("[%p] &Path Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.AppendSeparator();
	WxPopupMenuPrintMark.Append(ID_MNU___N_PAGE_NUMBER_1113, _("[%n] Page &Number"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___S__TOTALPAGES_1114, _("[%s] Total Page&s"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.AppendSeparator();
	WxPopupMenuPrintMark.Append(ID_MNU___D__DATE_1116, _("[%d] &Date"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___T__TIME_1117, _("[%t] &Time"), _(""), wxITEM_NORMAL);

	WxPopupMenuDateTimeMark.Append(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, _("[%Y/%m/%d %I:%M:%S %p] 2007/02/24 08:30:55 AM"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___D__M__Y_24_02_2007_1192, _("[%d/%m/%Y] 24/02/2007"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, _("[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.AppendSeparator();
	WxPopupMenuDateTimeMark.Append(ID_MNU_MENUITEM1_1170, _("[%a] Abbreviated weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___A_FULLWEEKDAYNAME_1171, _("[%A] Full weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___B_ABBREVIATEDMONTHNAME_1172, _("[%b] Abbreviated month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___B_FULLMONTHNAME_1173, _("[%B] Full month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, _("[%c] Date and time representation appropriate for locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, _("[%d] Day of month as decimal number (01 - 31)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, _("[%H] Hour in 24-hour format (00 - 23)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, _("[%I] Hour in 12-hour format (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, _("[%j] Day of year as decimal number (001 - 366)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, _("[%m] Month as decimal number (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, _("[%M] Minute as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, _("[%p] Current locale's A.M./P.M. indicator for 12-hour clock"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, _("[%S] Second as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, _("[%U] Week of year as decimal number, with Sunday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, _("[%w] Weekday as decimal number (0 - 6; Sunday is 0)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, _("[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, _("[%x] Date representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, _("[%X] Time representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, _("[%y] Year without century, as decimal number (00 - 99)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, _("[%Y] Year with century, as decimal number"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONENAME_1193, _("[%z] Time-zone name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONEABBREVIATION_1194, _("[%Z] Time-zone abbreviation"), _(""), wxITEM_NORMAL);

	wxString systemenc(_("System Default"));
	WxComboBoxEncoding->Append(systemenc);
	size_t cnt = MadEncoding::GetEncodingsCount();
	size_t i;

	for( i = 0; i < cnt; ++i )
	{
		WxComboBoxEncoding->Append( MadEncoding::GetEncodingName( i ) ); //enc+des);
	}

	WxComboBoxEncoding->SetValue( systemenc );
	for(i=0; i<g_LanguageString.GetCount(); ++i)
	{
	    WxComboBoxLanguage->Append(g_LanguageString[i]);
	}
	WxComboBoxLanguage->SetValue(g_LanguageString[0]);

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
	Center();

	// build Command Tree
	WxTreeCtrlCmdList->SetImageList(g_MainFrame->m_ImageList);
	wxTreeItemId root=WxTreeCtrlCmdList->AddRoot(_("Commands"));

	wxTreeItemId menuRoot=WxTreeCtrlCmdList->AppendItem(root, _("Menu"));
	wxTreeItemId editorRoot=WxTreeCtrlCmdList->AppendItem(root, _("Editor"));

	list<wxTreeItemId> tree_stack;
	CommandData *cd = &CommandTable[0];
	wxTreeItemId tid;

	// Menu command
	do
	{
		if(cd->menu_level==0)
		{
			tid=WxTreeCtrlCmdList->AppendItem(menuRoot, FilterChar(wxGetTranslation(cd->text)));
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
			tid=WxTreeCtrlCmdList->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)));
			tree_stack.push_back(tid);
		}
		else if(cd->kind != wxITEM_SEPARATOR)
		{
			TreeItemData *data=new TreeItemData;
			data->cmddata=cd;
			TreeItemDataList.push_back(data);

			tid=WxTreeCtrlCmdList->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)), cd->image_idx, cd->image_idx, data);
			WxTreeCtrlCmdList->SetItemBold(tid, true);
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

		tid=WxTreeCtrlCmdList->AppendItem(editorRoot, MadKeyBindings::CommandToText(cd->command), cd->image_idx, cd->image_idx, data);
		WxTreeCtrlCmdList->SetItemBold(tid, true);
		cd++;
	}
	while(cd->command > 0);

	AuiNotebook1->SetWindowStyleFlag(wxAUI_NB_TOP|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS);

	//WxButtonCancel->SetId(wxID_CANCEL);
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

	g_OptionsDialog = NULL;
	Destroy();
}

void MadOptionsDialog::MadOptionsDialogActivate( wxActivateEvent& event )
{
	if(event.GetActive())
	{
		if( FindFocus() == NULL )
		{
			SetReturnCode(wxID_CANCEL);
			WxButtonCancel->SetFocus();
		}
	}

	event.Skip();
}

void MadOptionsDialog::LoadOptions( void )
{
	wxConfigBase *cfg = wxConfigBase::Get( false );
	wxString oldpath = cfg->GetPath();
	cfg->SetPath( wxT( "/MadEdit" ) );
	long ll;
	bool bb;
	wxString ss;
	// General page
	ss = g_LanguageString[0];
	cfg->Read( wxT( "Language" ), &ss );
	WxComboBoxLanguage->SetValue( ss );
	cfg->Read( wxT( "SingleInstance" ), &bb );
	WxCheckBoxSingleInstance->SetValue( bb );
	cfg->Read( wxT( "RecordCaretMovements" ), &bb );
	WxCheckBoxRecordCaretMovements->SetValue( bb );
	cfg->Read( wxT( "MaxSizeToLoad" ), &ll );
	WxEditMaxSizeToLoad->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxTextFileSize" ), &ll );
	WxEditMaxTextFileSize->SetValue( wxString() << ll );
	ss = _( "System Default" );
	cfg->Read( wxT( "DefaultEncoding" ), &ss );
	WxComboBoxEncoding->SetValue( ss );
#ifdef __WXMSW__
	wxRegKey *pRegKey = new wxRegKey( g_MadEditRegkeyPath + wxT( "*\\shell\\MadEdit-Mod\\command" ) );

	if( pRegKey->Exists() )
	{
		wxString str;

		if( pRegKey->QueryValue( wxEmptyString, str ) )
		{
			wxString exepath = GetExecutablePath();
			WxCheckBoxRightClickMenu->SetValue( str.Upper().Find( exepath.Upper() ) >= 0 );
		}
	}

	delete pRegKey;
#endif
	// Edit page
	cfg->Read( wxT( "MaxLineLength" ), &ll );
	WxEditMaxLineLength->SetValue( wxString() << ll );
	cfg->Read( wxT( "MaxColumns" ), &ll );
	WxEditMaxColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "TabColumns" ), &ll );
	WxEditTabColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "IndentColumns" ), &ll );
	WxEditIndentColumns->SetValue( wxString() << ll );
	cfg->Read( wxT( "DateTimeFormat" ), &ss, wxT( "%c" ) );
	WxEditDateTime->SetValue( ss );
	cfg->Read( wxT( "DateTimeInEnglish" ), &bb, false );
	WxCheckBoxDateTimeInEnglish->SetValue( bb );
	cfg->Read( wxT( "InsertSpacesInsteadOfTab" ), &bb );
	WxCheckBoxTabOrSpaces->SetValue( bb );
	cfg->Read( wxT( "AutoIndent" ), &bb );
	WxCheckBoxAutoIndent->SetValue( bb );
	cfg->Read( wxT( "AutoCompletePair" ), &bb );
	WxCheckBoxAutoCompletePair->SetValue( bb );
	cfg->Read( wxT( "MouseSelectToCopy" ), &bb );
	WxCheckBoxMouseSelectToCopy->SetValue( bb );
	WxCheckBoxWhenPressCtrlKey->Enable( bb );
	cfg->Read( wxT( "MouseSelectToCopyWithCtrlKey" ), &bb );
	WxCheckBoxWhenPressCtrlKey->SetValue( bb );
	cfg->Read( wxT( "MiddleMouseToPaste" ), &bb );
	WxCheckBoxMiddleMouseToPaste->SetValue( bb );
	cfg->Read( wxT( "AutoFillColumnPaste" ), &bb );
	WxCheckBoxAutoFillColumnPaste->SetValue( bb );
	extern bool g_DoNotSaveSettings;
	WxCheckBoxDoNotSaveSettings->SetValue( g_DoNotSaveSettings );
	cfg->Read( wxT( "ReloadFiles" ), &bb, true );
	WxCheckBoxReloadFiles->SetValue( bb );
	cfg->Read( wxT( "PurgeHistory" ), &bb, false );
	WxCheckBoxPurgeHistory->SetValue( bb );
	cfg->Read( wxT( "ShowQSearchBarOnStart" ), &bb, true );
	WxCheckBoxShowQSearchBar->SetValue( bb );
	cfg->Read( wxT( "RestoreCaretPos" ), &bb, true );
	WxCheckBoxRestoreCaretPos->SetValue( bb );
	// Print page
	cfg->Read( wxT( "PrintSyntax" ), &bb );
	WxCheckBoxPrintSyntax->SetValue( bb );
	cfg->Read( wxT( "PrintLineNumber" ), &bb );
	WxCheckBoxPrintLineNumber->SetValue( bb );
	cfg->Read( wxT( "PrintBookmark" ), &bb );
	WxCheckBoxPrintBookmark->SetValue( bb );
	cfg->Read( wxT( "PrintEndOfLine" ), &bb );
	WxCheckBoxPrintEndOfLine->SetValue( bb );
	cfg->Read( wxT( "PrintTabChar" ), &bb );
	WxCheckBoxPrintTabChar->SetValue( bb );
	cfg->Read( wxT( "PrintSpaceChar" ), &bb );
	WxCheckBoxPrintSpaceChar->SetValue( bb );
	cfg->Read( wxT( "PrintOffsetHeader" ), &ll );
	WxRadioBoxPrintOffset->SetSelection( ll );
	cfg->Read( wxT( "PrintPageHeader" ), &bb );
	WxCheckBoxPrintPageHeader->SetValue( bb );
	cfg->Read( wxT( "PageHeaderLeft" ), &ss );
	WxEditHeaderLeft->SetValue( ss );
	cfg->Read( wxT( "PageHeaderCenter" ), &ss );
	WxEditHeaderCenter->SetValue( ss );
	cfg->Read( wxT( "PageHeaderRight" ), &ss );
	WxEditHeaderRight->SetValue( ss );
	cfg->Read( wxT( "PrintPageFooter" ), &bb );
	WxCheckBoxPrintPageFooter->SetValue( bb );
	cfg->Read( wxT( "PageFooterLeft" ), &ss );
	WxEditFooterLeft->SetValue( ss );
	cfg->Read( wxT( "PageFooterCenter" ), &ss );
	WxEditFooterCenter->SetValue( ss );
	cfg->Read( wxT( "PageFooterRight" ), &ss );
	WxEditFooterRight->SetValue( ss );
	bb = SpellCheckerManager::Instance().GetEnablePersonalDictionary();
	WxCheckBoxPersonalDict->SetValue( bb );
	WxEditDictionaryDir->SetValue( SpellCheckerManager::Instance().GetDictionaryPath() );
	InitDictionaryChoice();
	//cfg->Read(wxT("ThesPath"), &ss, dictDir);
	//WxEditThesauriDir->SetValue(ss);
	//cfg->Read(wxT("BitmPath"), &ss, dictDir);
	//WxEditBitMapDir->SetValue(ss);
	extern bool g_ResetAllKeys;
	WxCheckBoxResetAllKeys->SetValue( g_ResetAllKeys );
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

	g_SelectedCommandItem = NULL;
	g_SelectedKeyId = -1;
	WxEditCommandHint->Clear();
	WxListBoxKeys->Clear();
	UpdateKeyHint();
	wxTreeItemId selid = WxTreeCtrlCmdList->GetSelection();

	if( selid.IsOk() )
	{
		WxTreeCtrlCmdList->Unselect();
		WxTreeCtrlCmdList->SelectItem( selid, true );
	}

	cfg->SetPath( wxT( "/astyle" ) );
	WxRadioBoxBracketStyle->SetSelection( cfg->ReadLong( wxT( "style" ), aspsAllman ) );
	WxCheckAttachClasses->SetValue( cfg->ReadBool( wxT( "attach_classes" ), false ) );
	WxCheckAttachExternC->SetValue( cfg->ReadBool( wxT( "attach_extern_c" ), true ) );
	WxCheckAttachNamespaces->SetValue( cfg->ReadBool( wxT( "attach_namespaces" ), true ) );
	WxCheckAttachInlines->SetValue( cfg->ReadBool( wxT( "attach_inlines" ), true ) );
	WxCheckForceUseTabs->SetValue( cfg->ReadBool( wxT( "force_tabs" ), false ) );
	WxSpinIndentation->SetValue( cfg->ReadLong( wxT( "indentation" ), 4 ) );
	WxCheckUseTab->SetValue( cfg->ReadBool( wxT( "use_tabs" ), false ) );
	WxCheckIndentCase->SetValue( cfg->ReadBool( wxT( "indent_case" ), true ) );
	WxCheckIndentClasses->SetValue( cfg->ReadBool( wxT( "indent_classes" ), false ) );
	WxCheckIndentLabels->SetValue( cfg->ReadBool( wxT( "indent_labels" ), false ) );
	WxCheckIndentModifiers->SetValue( cfg->ReadBool( wxT( "indent_modifiers" ), false ) );
	WxCheckIndentNamespaces->SetValue( cfg->ReadBool( wxT( "indent_namespaces" ), true ) );
	WxCheckIndentSwitches->SetValue( cfg->ReadBool( wxT( "indent_switches" ), false ) );
	WxCheckIndentPreprocBlock->SetValue( cfg->ReadBool( wxT( "indent_preproc_block" ), true ) );
	WxCheckIndentPreprocDefine->SetValue( cfg->ReadBool( wxT( "indent_preproc_define" ), false ) );
	WxCheckIndentPreprocCond->SetValue( cfg->ReadBool( wxT( "indent_preproc_cond" ), false ) );
	WxCheckIndentCol1Comments->SetValue( cfg->ReadBool( wxT( "indent_col1_comments" ), true ) );
	WxSpinMinConditionalEvent->SetValue( cfg->ReadLong( wxT( "min_conditional_indent" ), 2 ) );
	WxEditMaxInStatementIndent->SetValue( wxString() << cfg->ReadLong( wxT( "max_instatement_indent" ), 40 ) );
	WxCheckBreakClosing->SetValue( cfg->ReadBool( wxT( "break_closing" ), true ) );
	WxCheckBreakElseIfs->SetValue( cfg->ReadBool( wxT( "break_elseifs" ), true ) );
	WxCheckAddBrackets->SetValue( cfg->ReadBool( wxT( "add_brackets" ), false ) );
	WxCheckAddOneLineBrackets->SetValue( cfg->ReadBool( wxT( "add_one_line_brackets" ), true ) );
	WxCheckKeepComplex->SetValue( cfg->ReadBool( wxT( "keep_complex" ), true ) );
	WxCheckRemoveBrackets->SetValue( cfg->ReadBool( wxT( "remove_brackets" ), false ) );
	WxCheckKeepBlocks->SetValue( cfg->ReadBool( wxT( "keep_blocks" ), true ) );
	WxCheckConvertTabs->SetValue( cfg->ReadBool( wxT( "convert_tabs" ), true ) );
	WxCheckCloseTemplates->SetValue( cfg->ReadBool( wxT( "close_templates" ), false ) );
	WxCheckRemoveCommentPrefix->SetValue( cfg->ReadBool( wxT( "remove_comment_prefix" ), false ) );
	bb = cfg->ReadBool( wxT( "break_lines" ), false );
	WxCheckBreakLines->SetValue( bb );

	if( bb )
	{
		WxEditSFMaxLineLength->Enable( true );
		WxCheckBreakAfterLogical->Enable( true );
		WxEditSFMaxLineLength->SetValue( cfg->Read( wxT( "max_line_length" ), wxString( wxT( "200" ) ) ) );
		WxCheckBreakAfterLogical->SetValue( cfg->ReadBool( wxT( "break_after_mode" ), false ) );
	}
	else
	{
		WxEditSFMaxLineLength->Enable( false );
		WxCheckBreakAfterLogical->Enable( false );
	}

	WxCheckBreakBlocks->SetValue( cfg->ReadBool( wxT( "break_blocks" ), true ) );

	if( WxCheckBreakBlocks->GetValue() )
	{
		WxCheckBreakBlocksAll->Enable( true );
		WxCheckBreakBlocksAll->SetValue( cfg->ReadBool( wxT( "break_blocks_all" ), false ) );
	}
	else
	{
		WxCheckBreakBlocksAll->Enable( false );
	}

	WxCheckPadOperators->SetValue( cfg->ReadBool( wxT( "pad_operators" ), true ) );
	WxCheckPadParensOut->SetValue( cfg->ReadBool( wxT( "pad_parentheses_out" ), false ) );
	WxCheckPadParensIn->SetValue( cfg->ReadBool( wxT( "pad_parentheses_in" ), true ) );
	WxCheckPadHeader->SetValue( cfg->ReadBool( wxT( "pad_header" ), false ) );
	WxCheckUnpadParens ->SetValue( cfg->ReadBool( wxT( "unpad_parentheses" ), true ) );
	WxCheckDelEmptyLine->SetValue( cfg->ReadBool( wxT( "delete_empty_lines" ), true ) );
	WxCheckFillEmptyLines->SetValue( cfg->ReadBool( wxT( "fill_empty_lines" ), false ) );
	wxString pointerAlign = cfg->Read( wxT( "pointer_align" ), wxEmptyString );

	if( pointerAlign == wxT( "Type" ) )
	{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_TYPE ); }
	else
		if( pointerAlign == wxT( "Middle" ) )
		{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_MIDDLE ); }
		else
			if( pointerAlign == wxT( "Name" ) )
			{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NAME ); }
			else
			{ WxChoicePointerAlign->SetSelection( astyle::PTR_ALIGN_NONE ); }

	wxString referenceAlign = cfg->Read( wxT( "reference_align" ), wxEmptyString );

	if( referenceAlign == wxT( "Type" ) )
	{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_TYPE ); }
	else
		if( referenceAlign == wxT( "Middle" ) )
		{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_MIDDLE ); }
		else
			if( referenceAlign == wxT( "Name" ) )
			{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NAME ); }
			else
			{ WxChoiceReferenceAlign->SetSelection( astyle::REF_ALIGN_NONE ); }

	cfg->SetPath( wxT( "/xml" ) );
	WxEditXmlIndentSize->SetValue( wxString() << cfg->ReadLong( wxT( "indentation" ), 4 ) );
	WxEditXMLversion->SetValue( cfg->Read( wxT( "version" ), wxString( wxT( "1.0" ) ) ) );
	cfg->SetPath( oldpath );
}

/*
 * WxButtonOKClick
 */
void MadOptionsDialog::WxButtonOKClick( wxCommandEvent& event )
{
	long lo;
	bool error = false;
	wxString errtext( _( "Invalid value of \"%s(%s)\"" ) );

	if( !WxEditMaxSizeToLoad->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext, StaticText1->GetLabel().c_str(), WxEditMaxSizeToLoad->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxTextFileSize->GetValue().ToLong( &lo ) || lo < 0 )
	{
		wxLogError( errtext, StaticText2->GetLabel().c_str(), WxEditMaxTextFileSize->GetValue().c_str() );
		error = true;
	}

#if PATCH_MAXLINELENGTH == 1

	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 || lo > 4096 )
#else
	if( !WxEditMaxLineLength->GetValue().ToLong( &lo ) || lo < 80 )
#endif
	{
		wxLogError( errtext, StaticText3->GetLabel().c_str(), WxEditMaxLineLength->GetValue().c_str() );
		error = true;
	}

	if( !WxEditMaxColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, StaticText4->GetLabel().c_str(), WxEditMaxColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditTabColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, StaticText5->GetLabel().c_str(), WxEditTabColumns->GetValue().c_str() );
		error = true;
	}

	if( !WxEditIndentColumns->GetValue().ToLong( &lo ) || lo <= 0 )
	{
		wxLogError( errtext, StaticText6->GetLabel().c_str(), WxEditIndentColumns->GetValue().c_str() );
		error = true;
	}

	if( !error ) { EndModal( wxID_OK ); }
}

/*
 * WxButtonCancelClick
 */
void MadOptionsDialog::WxButtonCancelClick( wxCommandEvent& event )
{
	EndModal( wxID_CANCEL );
}

void MadOptionsDialog::PrintMarkButtonClick( wxCommandEvent& event )
{
	ButtonID = event.GetId();
	PopupMenu( &WxPopupMenuPrintMark );
}

void MadOptionsDialog::PrintMarkClick( wxCommandEvent& event )
{
	wxString str = WxPopupMenuPrintMark.GetLabel( event.GetId() );
	wxTextCtrl *edit = NULL;

	if(ButtonID == ID_WXBUTTON1) edit = WxEditHeaderLeft;
	else if (ButtonID == ID_WXBUTTON2) edit = WxEditHeaderCenter;
	else if (ButtonID == ID_WXBUTTON3) edit = WxEditHeaderRight;
	else if (ButtonID == ID_WXBUTTON4) edit = WxEditFooterLeft;
	else if (ButtonID == ID_WXBUTTON5) edit = WxEditFooterCenter;
	else if (ButtonID == ID_WXBUTTON6) edit = WxEditFooterRight;

	if( edit != NULL && str[0] == wxT( '[' ) && str[3] == wxT( ']' ) )
	{
		wxString text = edit->GetValue();
		edit->SetValue( text + str.Mid( 1, 2 ) );
	}

	ButtonID = 0;
}


/*
 * WxTreeCtrlCmdListSelectionChanged
 */
void MadOptionsDialog::WxTreeCtrlCmdListSelectionChanged( wxTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();

	if( !id.IsOk() ) { return; }

	g_SelectedCommandItem = ( TreeItemData* )WxTreeCtrlCmdList->GetItemData( id );
	g_SelectedKeyId = -1;

	if( g_SelectedCommandItem == NULL )
	{
		WxListBoxKeys->Clear();
		WxEditCommandHint->SetValue( _( "Cannot assign key to this item" ) );
	}
	else
	{
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );
		wxString hint = wxT( '[' );

		if( g_SelectedCommandItem->cmddata->menuid_name > 0 )
		{
			hint += wxString( g_SelectedCommandItem->cmddata->menuid_name );
		}

		if( g_SelectedCommandItem->cmddata->command > 0 )
		{
			wxString text = MadKeyBindings::CommandToText( g_SelectedCommandItem->cmddata->command );

			if( !text.IsEmpty() )
			{
				if( hint.Length() > 1 )
				{
					hint += wxT( ',' );
				}

				hint += text;
			}
		}

		hint += wxT( "] " );
		hint += wxString( wxGetTranslation( g_SelectedCommandItem->cmddata->hint ) );
		WxEditCommandHint->SetValue( hint );
	}
}

/*
 * WxListBoxKeysSelected
 */
void MadOptionsDialog::WxListBoxKeysSelected( wxCommandEvent& event )
{
	g_SelectedKeyId = event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* MadOptionsDialog::FindKeyInList( const wxString &key )
{
	list<TreeItemData*>::iterator tidit = TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend = TreeItemDataList.end();
	wxString lkey = key.Lower();

	while( tidit != tiditend )
	{
		TreeItemData *tid = ( *tidit );

		if( !tid->keys.IsEmpty() )
		{
			size_t count = tid->keys.GetCount();
			size_t idx = 0;

			for( ; idx < count; ++idx )
			{
				if( tid->keys[idx].Lower() == lkey )
				{
					return tid;
				}
			}
		}

		++tidit;
	}

	return NULL;
}

bool MadOptionsDialog::FindItemInList( TreeItemData* tid, const list<TreeItemData*> &tlist )
{
	list<TreeItemData*>::const_iterator tidit = tlist.begin();
	list<TreeItemData*>::const_iterator tiditend = tlist.end();

	while( tidit != tiditend )
	{
		if( ( *tidit ) == tid )
		{
			return true;
		}

		++tidit;
	}

	return false;
}

void MadOptionsDialog::UpdateKeyHint()
{
	wxString scstr = WxEditKey->GetValue();
	g_CommandItemOfNewKey = NULL;

	if( scstr.IsEmpty() )
	{
		g_OptionsDialog->WxEditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList( scstr );

		if( tid == NULL )
		{
			g_OptionsDialog->WxEditKeyHint->SetValue( _( "This key is not assigned" ) );
		}
		else
		{
			g_CommandItemOfNewKey = tid;
			wxString cmd;

			if( tid->cmddata->menuid_name > 0 )
			{
				cmd += wxString( tid->cmddata->menuid_name );
			}

			if( tid->cmddata->command > 0 )
			{
				wxString text = MadKeyBindings::CommandToText( tid->cmddata->command );

				if( !text.IsEmpty() )
				{
					if( cmd.Length() > 1 )
					{
						cmd += wxT( ',' );
					}

					cmd += text;
				}
			}

			wxString str = wxString::Format( _( "This key is assigned to [%s]" ), cmd.c_str() );
			g_OptionsDialog->WxEditKeyHint->SetValue( str );
		}
	}
}

/*
 * WxButtonAddKeyClick
 */
void MadOptionsDialog::WxButtonAddKeyClick( wxCommandEvent& event )
{
	if( g_SelectedCommandItem != NULL && g_SelectedCommandItem != g_CommandItemOfNewKey )
	{
		wxString key = WxEditKey->GetValue();

		if( g_CommandItemOfNewKey != NULL ) // new key is assigned to another command
		{
			/*
			wxMessageDialog dlg(this, key +wxT(": ") +WxEditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
			    wxT("MadEdit-Mod"), wxYES_NO|wxICON_QUESTION );
			if(dlg.ShowModal()!=wxID_YES)
			{
			    return;
			}
			*/
			//delete the key from g_CommandItemOfNewKey
			int idx = g_CommandItemOfNewKey->keys.Index( key.c_str(), false );
			wxASSERT( idx >= 0 );
			g_CommandItemOfNewKey->keys.RemoveAt( idx );

			if( FindItemInList( g_CommandItemOfNewKey, ChangedTreeItemDataList ) == false )
			{
				ChangedTreeItemDataList.push_back( g_CommandItemOfNewKey );
			}
		}

		// add the key to g_SelectedCommandItem
		g_SelectedCommandItem->keys.Add( key );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
		UpdateKeyHint();
	}
}

/*
 * WxButtonDeleteKeyClick
 */
void MadOptionsDialog::WxButtonDeleteKeyClick( wxCommandEvent& event )
{
	if( g_SelectedCommandItem != NULL && g_SelectedKeyId >= 0 )
	{
		g_SelectedCommandItem->keys.RemoveAt( g_SelectedKeyId );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
		UpdateKeyHint();
	}
}

/*
 * WxButtonShowInMenuClick
 */
void MadOptionsDialog::WxButtonShowInMenuClick( wxCommandEvent& event )
{
	// move the selected key to first element of WxListBoxKeys
	if( g_SelectedCommandItem != NULL && g_SelectedCommandItem->cmddata->menu_id > 0 && g_SelectedKeyId > 0 )
	{
		wxString key = g_SelectedCommandItem->keys[g_SelectedKeyId];
		g_SelectedCommandItem->keys.RemoveAt( g_SelectedKeyId );
		g_SelectedCommandItem->keys.Insert( key, 0 );
		WxListBoxKeys->Set( g_SelectedCommandItem->keys );

		if( FindItemInList( g_SelectedCommandItem, ChangedTreeItemDataList ) == false )
		{
			ChangedTreeItemDataList.push_back( g_SelectedCommandItem );
		}

		g_SelectedKeyId = -1;
	}
}


/*
 * WxButtonDateTimeClick
 */
void MadOptionsDialog::WxButtonDateTimeClick( wxCommandEvent& event )
{
	PopupMenu( &WxPopupMenuDateTimeMark );
}

void MadOptionsDialog::DateTimeMarkClick( wxCommandEvent& event )
{
	wxString str = WxPopupMenuDateTimeMark.GetLabel( event.GetId() );
	int pos = str.Find( wxT( ']' ) );

	if( pos != wxNOT_FOUND && str[0] == wxT( '[' ) )
	{
		str = str.Mid( 1, pos - 1 );
	}

	wxString text = WxEditDateTime->GetValue();
	WxEditDateTime->SetValue( text + str );
}

#if ENABLE_BITMAP_THUARI
void MadOptionsDialog::WxButtonBitmapDirClick( wxCommandEvent& event )
{
}
void MadOptionsDialog::WxButtonThesauriDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetThesaurusPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		WxEditThesauriDir->SetValue( dlg.GetPath() );
		SpellCheckerManager::Instance().SetThesaurusPath( dlg.GetPath() );
		InitDictionaryChoice();
	}
}
#endif

void MadOptionsDialog::WxButtonDictionaryDirClick( wxCommandEvent& event )
{
	wxString defdir = SpellCheckerManager::Instance().GetDictionaryPath();

	if( defdir.IsEmpty() ) { defdir = wxGetCwd(); }

	wxDirDialog dlg( this, _( "Choose a directory" ), defdir );

	if( dlg.ShowModal() == wxID_OK )
	{
		WxEditDictionaryDir->SetValue( dlg.GetPath() );
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
	WxChoiceDictionary->Clear();

	for( unsigned int i = 0 ; i < dics.size(); i++ )
	{ WxChoiceDictionary->AppendString( SpellCheckerManager::Instance().GetLanguageName( dics[i] ) ); }

	if( sel != -1 )
	{ WxChoiceDictionary->Select( sel ); }
}

void MadOptionsDialog::OnWxEditDictionaryDirText( wxCommandEvent& event )
{
	wxString path = WxEditDictionaryDir->GetValue();

	if( wxDir::Exists( path ) )
	{
		InitDictionaryChoice( path );
	}
	else
	{
		WxChoiceDictionary->Clear();
	}
}

void MadOptionsDialog::OnSelectDictionary( wxCommandEvent& event )
{
	wxString dictDesc = WxChoiceDictionary->GetString( WxChoiceDictionary->GetSelection() );
	wxString dictName = SpellCheckerManager::Instance().GetDictionaryName( dictDesc );

	if( !dictName.IsEmpty() )
	{
		SpellCheckerManager::Instance().SetDictionaryName( dictName );
	}
}

void MadOptionsDialog::OnMouseAutoCopyClicked( wxCommandEvent& event )
{
	WxCheckBoxWhenPressCtrlKey->Enable( WxCheckBoxMouseSelectToCopy->GetValue() );
}

void MadOptionsDialog::OnWxRadioBoxBracketStyleSelect( wxCommandEvent& event )
{
	long style = WxRadioBoxBracketStyle->GetSelection();
#ifdef MADEDIT_ENABLE_STC
	WxTextSample->SetReadOnly( false );
#endif

	switch( style )
	{
	case aspsAllman: // Allman (ANSI)
	case aspsJava: // Java
	case aspsKr: // K&R
	case aspsStroustrup: // Stroustrup
	case aspsWhitesmith: // Whitesmith
	case aspsVTK: // VTK
	case aspsBanner: // Banner
	case aspsGnu: // GNU
	case aspsLinux: // Linux
	case aspsHorstmann: // Horstmann
	case asps1TBS: // 1TBS
	case aspsGoogle: // Google
	case aspsPico: // Pico
	case aspsLisp: // Lisp
#ifndef MADEDIT_ENABLE_MADEDIT
		WxTextSample->SetValue( bracket_style[style] );
#else
		WxTextSample->SetText(bracket_style[style]);
#endif
		break;

	default: // Custom
#ifndef MADEDIT_ENABLE_MADEDIT
		WxTextSample->SetValue(bracket_style[aspsCustom]);
#else
		WxTextSample->SetText(bracket_style[aspsCustom]);
#endif
		break;
	}

#ifdef MADEDIT_ENABLE_STC
	WxTextSample->SetReadOnly( true );
#endif
}

void MadOptionsDialog::OnFormattingBreakLinesClick( wxCommandEvent& event )
{
	if( WxCheckBreakLines->GetValue() )
	{
		WxEditSFMaxLineLength->Enable( true );
		WxCheckBreakAfterLogical->Enable( true );
	}
	else
	{
		WxEditSFMaxLineLength->Enable( false );
		WxCheckBreakAfterLogical->Enable( false );
	}
}

void MadOptionsDialog::OnPaddingBreakBlocksClick( wxCommandEvent& event )
{
	if( WxCheckBreakBlocks->GetValue() )
	{
		WxCheckBreakBlocksAll->Enable( true );
	}
	else
	{
		WxCheckBreakBlocksAll->Enable( false );
	}
}

#ifdef MADEDIT_ENABLE_STC
void MadOptionsDialog::OnMarginClick( wxStyledTextEvent &event )
{
	if( event.GetMargin() == MARGIN_FOLD )
	{
		int lineClick = WxTextSample->LineFromPosition( event.GetPosition() );
		int levelClick = WxTextSample->GetFoldLevel( lineClick );

		if( ( levelClick & wxSTC_FOLDLEVELHEADERFLAG ) > 0 )
		{
			WxTextSample->ToggleFold( lineClick );
		}
	}
}
#endif
