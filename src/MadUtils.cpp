///////////////////////////////////////////////////////////////////////////////
// Name:		MadUtils.cpp
// Description:	utility	functions. collects	from internet
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence:		GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <locale.h>

#if	defined(__WXMAC__)
#include <wx/mac/private.h>
#include <Processes.h>
#endif

#include "MadUtils.h"
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h" 
#include "MadSortDialog.h"
#include "MadNumberDlg.h"
#include "MadSortDialog.h"
#include "EmbeddedPython.hpp"

extern MadEdit *g_ActiveMadEdit;
extern MadEdit *g_CurrentMadEdit;
extern MadNumberDlg * g_MadNumberDlg;
extern wxString g_MadEditAppDir;
extern wxString g_MadEditHomeDir;
extern wxMenu *g_Menu_MadMacro_Scripts;
extern EmbeddedPython *g_EmbeddedPython;
extern MadEditFrame *g_MainFrame;
extern wxArrayString g_SpellSuggestions;
extern wxString g_MadEditAppDir;
extern wxString g_MadEditHomeDir;
extern wxMenu *g_Menu_MadMacro_Scripts;

extern int MadMessageBox(const wxString& message,
	const wxString& caption = wxMessageBoxCaptionStr,
	long style = wxOK | wxCENTRE,
	wxWindow* parent = nullptr,
	int x = wxDefaultCoord, int y = wxDefaultCoord);

HtmlColor HtmlColorTable[]=
{
	{ wxT("AliceBlue"),			   240,248,255 },
	{ wxT("AntiqueWhite"),		   250,235,215 },
	{ wxT("Aqua"),				   0,255,255 },
	{ wxT("Aquamarine"),		   127,255,212 },
	{ wxT("Azure"),				   240,255,255 },
	{ wxT("Beige"),				   245,245,220 },
	{ wxT("Bisque"),			   255,228,196 },
	{ wxT("Black"),				   0,0,0 },
	{ wxT("BlanchedAlmond"),	   255,235,205 },
	{ wxT("Blue"),				   0,0,255 },
	{ wxT("BlueViolet"),		   138,43,226 },
	{ wxT("Brown"),				   165,42,42 },
	{ wxT("BurlyWood"),			   222,184,135 },
	{ wxT("CadetBlue"),			   95,158,160 },
	{ wxT("Chartreuse"),		   127,255,0 },
	{ wxT("Chocolate"),			   210,105,30 },
	{ wxT("Coral"),				   255,127,80 },
	{ wxT("CornflowerBlue"),	   100,149,237 },
	{ wxT("Cornsilk"),			   255,248,220 },
	{ wxT("Crimson"),			   237,164,61 },
	{ wxT("Cyan"),				   0,255,255 },
	{ wxT("DarkBlue"),			   0,0,139 },
	{ wxT("DarkCyan"),			   0,139,139 },
	{ wxT("DarkGoldenRod"),		   184,134,11 },
	{ wxT("DarkGray"),			   167,167,167 },
	{ wxT("DarkGreen"),			   0,100,0 },
	{ wxT("DarkKhaki"),			   189,183,107 },
	{ wxT("DarkMagenta"),		   139,0,139 },
	{ wxT("DarkOliveGreen"),	   85,107,47 },
	{ wxT("DarkOrchid"),		   153,50,204 },
	{ wxT("DarkRed"),			   139,0,0 },
	{ wxT("DarkSalmon"),		   233,150,122 },
	{ wxT("DarkSeaGreen"),		   143,188,143 },
	{ wxT("DarkSlateBlue"),		   72,61,139 },
	{ wxT("DarkSlateGray"),		   47,79,79	},
	{ wxT("DarkTurquoise"),		   0,206,209 },
	{ wxT("DarkViolet"),		   148,0,211 },
	{ wxT("Darkorange"),		   255,140,0 },
	{ wxT("DeepPink"),			   255,20,147 },
	{ wxT("DeepSkyBlue"),		   0,191,255 },
	{ wxT("DimGray"),			   105,105,105 },
	{ wxT("DodgerBlue"),		   30,144,255 },
	{ wxT("Feldspar"),			   209,146,117 },
	{ wxT("FireBrick"),			   178,34,34 },
	{ wxT("FloralWhite"),		   255,250,240 },
	{ wxT("ForestGreen"),		   34,139,34 },
	{ wxT("Fuchsia"),			   255,0,255 },
	{ wxT("Gainsboro"),			   220,220,220 },
	{ wxT("GhostWhite"),		   248,248,255 },
	{ wxT("Gold"),				   255,215,0 },
	{ wxT("GoldenRod"),			   218,165,32 },
	{ wxT("Gray"),				   190,190,190 },
	{ wxT("Green"),				   0,128,0 },
	{ wxT("GreenYellow"),		   173,255,47 },
	{ wxT("HoneyDew"),			   240,255,240 },
	{ wxT("HotPink"),			   255,105,180 },
	{ wxT("IndianRed"),			   205,92,92 },
	{ wxT("Indigo"),			   75,0,130	},
	{ wxT("Ivory"),				   255,255,240 },
	{ wxT("Khaki"),				   240,230,140 },
	{ wxT("Lavender"),			   230,230,250 },
	{ wxT("LavenderBlush"),		   255,240,245 },
	{ wxT("LawnGreen"),			   124,252,0 },
	{ wxT("LemonChiffon"),		   255,250,205 },
	{ wxT("LightBlue"),			   173,216,230 },
	{ wxT("LightCoral"),		   240,128,128 },
	{ wxT("LightCyan"),			   224,255,255 },
	{ wxT("LightGoldenRodYellow"), 250,250,210 },
	{ wxT("LightGreen"),		   144,238,144 },
	{ wxT("LightGrey"),			   211,211,211 },
	{ wxT("LightPink"),			   255,182,193 },
	{ wxT("LightSalmon"),		   255,160,122 },
	{ wxT("LightSeaGreen"),		   32,178,170 },
	{ wxT("LightSkyBlue"),		   135,206,250 },
	{ wxT("LightSlateBlue"),	   132,112,255 },
	{ wxT("LightSlateGray"),	   119,136,153 },
	{ wxT("LightSteelBlue"),	   176,196,222 },
	{ wxT("LightYellow"),		   255,255,224 },
	{ wxT("Lime"),				   0,255,0 },
	{ wxT("LimeGreen"),			   50,205,50 },
	{ wxT("Linen"),				   250,240,230 },
	{ wxT("Magenta"),			   255,0,255 },
	{ wxT("Maroon"),			   128,0,0 },
	{ wxT("MediumAquaMarine"),	   102,205,170 },
	{ wxT("MediumBlue"),		   0,0,205 },
	{ wxT("MediumOrchid"),		   186,85,211 },
	{ wxT("MediumPurple"),		   147,112,219 },
	{ wxT("MediumSeaGreen"),	   60,179,113 },
	{ wxT("MediumSlateBlue"),	   123,104,238 },
	{ wxT("MediumSpringGreen"),	   0,250,154 },
	{ wxT("MediumTurquoise"),	   72,209,204 },
	{ wxT("MediumVioletRed"),	   199,21,133 },
	{ wxT("MidnightBlue"),		   25,25,112 },
	{ wxT("MintCream"),			   245,255,250 },
	{ wxT("MistyRose"),			   255,228,225 },
	{ wxT("Moccasin"),			   255,228,181 },
	{ wxT("NavajoWhite"),		   255,222,173 },
	{ wxT("Navy"),				   0,0,128 },
	{ wxT("OldLace"),			   253,245,230 },
	{ wxT("Olive"),				   128,128,0 },
	{ wxT("OliveDrab"),			   107,142,35 },
	{ wxT("Orange"),			   255,165,0 },
	{ wxT("OrangeRed"),			   255,69,0	},
	{ wxT("Orchid"),			   218,112,214 },
	{ wxT("PaleGoldenRod"),		   238,232,170 },
	{ wxT("PaleGreen"),			   152,251,152 },
	{ wxT("PaleTurquoise"),		   175,238,238 },
	{ wxT("PaleVioletRed"),		   219,112,147 },
	{ wxT("PapayaWhip"),		   255,239,213 },
	{ wxT("PeachPuff"),			   255,218,185 },
	{ wxT("Peru"),				   205,133,63 },
	{ wxT("Pink"),				   255,192,203 },
	{ wxT("Plum"),				   221,160,221 },
	{ wxT("PowderBlue"),		   176,224,230 },
	{ wxT("Purple"),			   128,0,128 },
	{ wxT("Red"),				   255,0,0 },
	{ wxT("RosyBrown"),			   188,143,143 },
	{ wxT("RoyalBlue"),			   65,105,225 },
	{ wxT("SaddleBrown"),		   139,69,19 },
	{ wxT("Salmon"),			   250,128,114 },
	{ wxT("SandyBrown"),		   244,164,96 },
	{ wxT("SeaGreen"),			   46,139,87 },
	{ wxT("SeaShell"),			   255,245,238 },
	{ wxT("Sienna"),			   160,82,45 },
	{ wxT("Silver"),			   192,192,192 },
	{ wxT("SkyBlue"),			   135,206,235 },
	{ wxT("SlateBlue"),			   106,90,205 },
	{ wxT("SlateGray"),			   112,128,144 },
	{ wxT("Snow"),				   255,250,250 },
	{ wxT("SpringGreen"),		   0,255,127 },
	{ wxT("SteelBlue"),			   70,130,180 },
	{ wxT("Tan"),				   210,180,140 },
	{ wxT("Teal"),				   0,128,128 },
	{ wxT("Thistle"),			   216,191,216 },
	{ wxT("Tomato"),			   255,99,71 },
	{ wxT("Turquoise"),			   64,224,208 },
	{ wxT("Violet"),			   238,130,238 },
	{ wxT("VioletRed"),			   208,32,144 },
	{ wxT("Wheat"),				   245,222,179 },
	{ wxT("White"),				   255,255,255 },
	{ wxT("WhiteSmoke"),		   245,245,245 },
	{ wxT("Yellow"),			   255,255,0 },
	{ wxT("YellowGreen"),		   154,205,50 }
};
const int HtmlColorTableCount= sizeof(HtmlColorTable) / sizeof(HtmlColor);

void SetHtmlColors()
{
	wxColourDatabase *cdb=wxTheColourDatabase;
	HtmlColor *hc=HtmlColorTable;
	for(int	i=0; i<HtmlColorTableCount;	++i, ++hc)
	{
		cdb->AddColour(hc->name, wxColor(hc->red, hc->green, hc->blue));
	}
}

wxString GetExecutablePath()
{
	static bool	found =	false;
	static wxString	path;

	if (!found)	{
#ifdef __WXMSW__

		wxChar buf[512];
		*buf = wxT('\0');
		GetModuleFileName(nullptr, buf, 511);
		path = buf;

#elif defined(__WXMAC__)

		ProcessInfoRec processinfo;
		ProcessSerialNumber	procno ;
		FSSpec fsSpec;

		procno.highLongOfPSN = nullptr;
		procno.lowLongOfPSN	= kCurrentProcess ;
		processinfo.processInfoLength =	sizeof(ProcessInfoRec);
		processinfo.processName	= nullptr;
		processinfo.processAppSpec = &fsSpec;

		GetProcessInformation(&procno , &processinfo)	;
		path = wxMacFSSpec2MacFilename(&fsSpec);
#else
		wxString argv0 = wxTheApp->argv[0];

		if (wxIsAbsolutePath(argv0)) {
			path = argv0;
		}
		else {
			wxPathList pathlist;
			pathlist.AddEnvList(wxT("PATH"));
			path = pathlist.FindAbsoluteValidPath(argv0);
		}

		wxFileName filename(path);
		filename.Normalize();
		path = filename.GetFullPath();
#endif
		found =	true;
	}

	return path;
}

static long MadNewIDRange(long begin, size_t count)
{
	for (size_t i = 1; i<count; ++i)
		wxRegisterId(begin + i);
	return begin + count - 1;
}

MadUniqueIDReserver::MadUniqueIDReserver()
	: fid1(-1), fid20(-1) , rid1(-1), rid20(-1)
{
	fid1 = wxNewId();
	fid20 = MadNewIDRange(fid1, 20);

	rid1 = wxNewId();
	rid20 = MadNewIDRange(rid1, 20);
}

void EditUndo()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && (!g_CurrentMadEdit->IsReadOnly()))
	{
		g_CurrentMadEdit->Undo();
	}

	g_CurrentMadEdit = nullptr;
}

void EditRedo()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && (!g_CurrentMadEdit->IsReadOnly()))
	{
		g_CurrentMadEdit->Redo();
	}

	g_CurrentMadEdit = nullptr;
}

void EditCut()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CutToClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void EditCopy()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CopyToClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void EditPaste()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->PasteFromClipboard();
		g_CurrentMadEdit = nullptr;
	}
}

void EditDelete()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->Delete();
		g_CurrentMadEdit = nullptr;
	}
}

void EditCutLine()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CutLine();
		g_CurrentMadEdit = nullptr;
	}
}

void EditDeleteLine()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->DeleteLine();
		g_CurrentMadEdit = nullptr;
	}
}

void EditSelectAll()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->SelectAll();
		g_CurrentMadEdit = nullptr;
	}
}

void EditStartEndSelction()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->StartEndSelction();
		g_CurrentMadEdit = nullptr;
	}
}

void EditInsertTabChar()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->InsertTabChar();
		g_CurrentMadEdit = nullptr;
	}
}

void EditInsertDateTime()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->InsertDateTime();
		g_CurrentMadEdit = nullptr;
	}
}

void EditSortAscending()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(sfAscending, begin, end);
	}

	g_CurrentMadEdit = nullptr;
}

void EditSortDescending()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(sfDescending, begin, end);
	}

	g_CurrentMadEdit = nullptr;
}

void EditSortAscendingCase()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(sfAscending | sfCaseSensitive, begin, end);
	}

	g_CurrentMadEdit = nullptr;
}

void EditSortDescendingCase()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(sfDescending | sfCaseSensitive, begin, end);
	}

	g_CurrentMadEdit = nullptr;
}

void EditSortByOptions()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		wxConfigBase *m_Config = wxConfigBase::Get(false);
		wxString oldpath = m_Config->GetPath();
		m_Config->SetPath(wxT("/MadEdit"));
		int order;
		bool cs, num, rem;
		m_Config->Read(wxT("SortOrder"), &order, sfAscending);
		m_Config->Read(wxT("SortCaseSensitive"), &cs, false);
		m_Config->Read(wxT("SortNumeric"), &num, false);
		m_Config->Read(wxT("SortRemoveDup"), &rem, false);
		m_Config->SetPath(oldpath);
		MadSortFlags flags = order |
							 (cs ? sfCaseSensitive : 0) |
							 (num ? sfNumericSort : 0) |
							 (rem ? sfRemoveDuplicate : 0) ;
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(flags, begin, end);
	}

	g_CurrentMadEdit = nullptr;
}

void EditSortOptions(wxWindow* parent)
{
	if (!g_CurrentMadEdit || g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit = nullptr;
		return;
	}
	wxConfigBase *m_Config = wxConfigBase::Get(false);
	MadSortDialog dialog(parent);
	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath(wxT("/MadEdit"));
	int order;
	bool cs, num, rem;
	m_Config->Read(wxT("SortOrder"), &order, sfAscending);
	dialog.WxRadioBoxOrder->SetSelection(order);
	m_Config->Read(wxT("SortCaseSensitive"), &cs, false);
	dialog.WxCheckBoxCase->SetValue(cs);
	m_Config->Read(wxT("SortNumeric"), &num, false);
	dialog.WxCheckBoxNumeric->SetValue(num);
	m_Config->Read(wxT("SortRemoveDup"), &rem, false);
	dialog.WxCheckBoxRemoveDup->SetValue(rem);
	// Hide Modaless Dialog
	//HideModalessDialogs();

	if (dialog.ShowModal() == wxID_OK)
	{
		order = dialog.WxRadioBoxOrder->GetSelection();
		cs = dialog.WxCheckBoxCase->GetValue();
		num = dialog.WxCheckBoxNumeric->GetValue();
		rem = dialog.WxCheckBoxRemoveDup->GetValue();
		m_Config->Write(wxT("SortOrder"), order);
		m_Config->Write(wxT("SortCaseSensitive"), cs);
		m_Config->Write(wxT("SortNumeric"), num);
		m_Config->Write(wxT("SortRemoveDup"), rem);
		int flags = order |
					(cs ? sfCaseSensitive : 0) |
					(num ? sfNumericSort : 0) |
					(rem ? sfRemoveDuplicate : 0) ;
		int begin, end;
		g_CurrentMadEdit->GetSelectionLineId(begin, end);
		g_CurrentMadEdit->SortLines(flags, begin, end);
	}

	m_Config->SetPath(oldpath);
	g_CurrentMadEdit = nullptr;
}

void EditCopyAsHexString()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CopyAsHexString(false);
		g_CurrentMadEdit = nullptr;
	}
}

void EditCopyAsHexStringWithSpace()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CopyAsHexString(true);
		g_CurrentMadEdit = nullptr;
	}
}

void EditCopyRevertHex()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		static wxString strDelimiters = wxT(" .,?!@#\t$%^&*()-=_+[]{}\\|;:\"'`<>/~");
		wxString str = wxGetTextFromUser(_("Delimiters:"), _("Revert Hex String"), strDelimiters);

		if (!str.IsEmpty())
		{
			strDelimiters = str;
		}

		g_CurrentMadEdit->CopyRevertHex(str);
		g_CurrentMadEdit = nullptr;
	}
}

void EditIncIndent()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->IncreaseDecreaseIndent(true);
		g_CurrentMadEdit = nullptr;
	}
}
void EditDecIndent()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->IncreaseDecreaseIndent(false);
		g_CurrentMadEdit = nullptr;
	}
}

void EditComment()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CommentUncomment(true);

		g_CurrentMadEdit = nullptr;
	}
}

void EditUncomment()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->CommentUncomment(false);
		g_CurrentMadEdit = nullptr;
	}
}

void EditWordWrapToNewLine()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ConvertWordWrapToNewLine();
		g_CurrentMadEdit = nullptr;
	}
}
void EditNewLineToWordWrap()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ConvertNewLineToWordWrap();
		g_CurrentMadEdit = nullptr;
	}
}

void EditToUpperCase()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ToUpperCase();
		g_CurrentMadEdit = nullptr;
	}
}

void EditToLowerCase()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ToLowerCase();
		g_CurrentMadEdit = nullptr;
	}
}

void EditInvertCase()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->InvertCase();
		g_CurrentMadEdit = nullptr;
	}
}

void EditCapitalize()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->Capitalize();
		g_CurrentMadEdit = nullptr;
	}
}

void EditToHalfWidth()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ToHalfWidth();
		g_CurrentMadEdit = nullptr;
	}
}

void EditToHalfWidthByOptions(wxWindow* parent)
{
	if (g_CurrentMadEdit == g_ActiveMadEdit || !g_CurrentMadEdit) return;

	static wxArrayInt selections;
	static bool inited = false;

	if (!inited)
	{
		selections.Add(0);
		selections.Add(1);
		selections.Add(2);
		selections.Add(3);
		inited = true;
	}

	wxString choices[4] = { _("ASCII characters"), _("Japanese characters"),
							_("Korean characters"), _("other characters")
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices(selections,
										_("Choose the characters you want to convert:"), _("To Halfwidth by Options..."),
										4, choices, parent);
#else
	int sels = wxGetSelectedChoices(selections,
									 _("Choose the characters you want to convert:"), _("To Halfwidth by Options..."),
									 4, choices, parent);
#endif

	if (sels > 0)
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for(size_t i = 0; i < (size_t)sels; ++i)
		{
			switch(selections[i])
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		g_CurrentMadEdit->ToHalfWidth(ascii, japanese, korean, other);
	}

	g_CurrentMadEdit = nullptr;
}

void EditToFullWidth()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ToFullWidth();
		g_CurrentMadEdit = nullptr;
	}
}

void EditToFullWidthByOptions(wxWindow* parent)
{
	if ((g_CurrentMadEdit == g_ActiveMadEdit) ||(!g_CurrentMadEdit)) return;

	static wxArrayInt selections;
	static bool inited = false;

	if (!inited)
	{
		selections.Add(0);
		selections.Add(1);
		selections.Add(2);
		selections.Add(3);
		inited = true;
	}

	wxString choices[4] = { _("ASCII characters"), _("Japanese characters"),
							_("Korean characters"), _("other characters")
						  };
#if (wxMAJOR_VERSION == 2)
	size_t sels = wxGetSelectedChoices(selections,
										_("Choose the characters you want to convert:"), _("To Fullwidth by Options..."),
										4, choices, parent);
#else
	int sels = wxGetSelectedChoices(selections,
									 _("Choose the characters you want to convert:"), _("To Fullwidth by Options..."),
									 4, choices, parent);
#endif

	if (sels > 0)
	{
		bool ascii = false, japanese = false, korean = false, other = false;

		for(size_t i = 0; i < (size_t)sels; ++i)
		{
			switch(selections[i])
			{
			case 0: ascii = true; break;

			case 1: japanese = true; break;

			case 2: korean = true; break;

			case 3: other = true; break;
			}
		}

		g_CurrentMadEdit->ToFullWidth(ascii, japanese, korean, other);
	}

	g_CurrentMadEdit = nullptr;
}

void EditTabToSpace()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ConvertTabToSpace();
		g_CurrentMadEdit = nullptr;
	}

}
void EditSpaceToTab()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{
		g_CurrentMadEdit->ConvertSpaceToTab();
		g_CurrentMadEdit = nullptr;
	}
}

void EditTrimTrailingSpaces()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->TrimTrailingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void EditTrimLeadingSpaces()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->TrimLeadingSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void EditDeleteEmptyLines()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->DeleteEmptyLines();
	}

	g_CurrentMadEdit = nullptr;
}

void EditDeleteEmptyLinesWithSpaces()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->DeleteEmptyLinesWithSpaces();
	}

	g_CurrentMadEdit = nullptr;
}

void EditJoinLines()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->JoinLines();
	}

	g_CurrentMadEdit = nullptr;
}

void EditInsertNumbers(wxWindow* parent)
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && g_CurrentMadEdit->IsColumnMode())
	{
		// Hide Modaless Dialog
		//HideModalessDialogs();

		if (!g_MadNumberDlg) { g_MadNumberDlg = new MadNumberDlg(parent); }

		if (g_MadNumberDlg->ShowModal() == wxID_OK)
		{
			MadNumberingStepType numStepType = mnstLinear;
			MadNumberFormat numFormat = nfDEC;
			MadNumberAlign numAlign = naLeft;
			int sel = g_MadNumberDlg->WxChoiceNumberStepType->GetSelection();

			switch(sel)
			{
			case 1: numStepType = mnstExponential; break;

			default: break;
			}

			sel = g_MadNumberDlg->WxChoiceFormat->GetSelection();

			switch(sel)
			{
			case 1:
				{
					numFormat = nfHEX;
				}
				break;

			case 2:
				{
					numFormat = nfBIN;
				}
				break;

			case 3:
				{
					numFormat = nfOCT;
				}
				break;

			default:
				break;
			}

			sel = g_MadNumberDlg->WxChoiceAlign->GetSelection();

			switch (sel)
			{
			case 1:
				{
					numAlign = naRight;
				}
				break;

			default:
				break;
			}

			long initialNum = 0, numStep = 0, totalChar = 0;
			g_MadNumberDlg->WxEditNumberOfChars->GetValue().ToLong(&totalChar);
			g_MadNumberDlg->WxEditNumberingStep->GetValue().ToLong(&numStep);
			g_MadNumberDlg->WxEditInitialNumber->GetValue().ToLong(&initialNum);
			wxString prefix, postfix;

			if (g_MadNumberDlg->WxCheckPrefix->GetValue())
			{ prefix = g_MadNumberDlg->WxEditPrefix->GetValue(); }

			if (g_MadNumberDlg->WxCheckPostfix->GetValue())
			{ postfix = g_MadNumberDlg->WxEditPostfix->GetValue(); }

			g_CurrentMadEdit->InsertIncrementalNumber(initialNum, numStep, totalChar, numStepType, numFormat, numAlign, g_MadNumberDlg->WxPadChar->GetValue(), prefix, postfix);
			g_CurrentMadEdit->Refresh(false);
		}
	}

	g_CurrentMadEdit = nullptr;
}

void EditColumnAlignLeft()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->ColumnAlignLeft();
	}

	g_CurrentMadEdit = nullptr;
}

void EditColumnAlignRight()
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->ColumnAlignRight();
	}

	g_CurrentMadEdit = nullptr;
}

void EditSpellCheck(wxCommandEvent& event)
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit && !g_CurrentMadEdit->IsHexMode())
	{
		g_CurrentMadEdit->ReplaceWordFromCaretPos(g_SpellSuggestions[event.GetId() - menuSpellOption1]);
	}
	g_CurrentMadEdit = nullptr;
}

void ToolsMadScriptList(wxCommandEvent& event)
{
	if (g_CurrentMadEdit == g_ActiveMadEdit) return;
	if (g_CurrentMadEdit)
	{ 
		wxString scriptdir = g_MadEditAppDir + wxT("scripts") + wxFILE_SEP_PATH;
		int menuId = event.GetId();
		wxString filename = g_Menu_MadMacro_Scripts->GetLabelText(menuId) + wxT(".mpy");
		wxString scripfile = scriptdir + filename;
		if (!wxFileExists(scripfile)) scripfile = g_MadEditHomeDir + wxT("scripts") + wxFILE_SEP_PATH + filename;
		if (!wxFileExists(scripfile)) 
		{
			scripfile = 
#if defined (DATA_DIR)
				wxT(DATA_DIR"/madedit-mod/scripts/") +
#else
				wxT("/usr/share/madedit-mod/scripts/") +
#endif
			filename;
		}

		wxTextFile scriptfile(scripfile);
		scriptfile.Open(wxConvFile);

		if (scriptfile.IsOpened())
		{
			if (!g_EmbeddedPython)
			{
				try
				{
					g_EmbeddedPython = new EmbeddedPython();
				}
				catch(std::bad_alloc &)
				{
					MadMessageBox(_("Memory allocation failed"), _("Error"),  wxOK | wxICON_ERROR);
				}
			}

			if (g_EmbeddedPython)
			{
				wxString str = scriptfile.GetFirstLine() + wxT("\n");

				for(; !scriptfile.Eof();)
				{
					str << scriptfile.GetNextLine() << wxT("\n");
				}

				if (str.IsNull() == false)
				{
					g_MainFrame->SetMacroRunning();
					g_EmbeddedPython->exec(std::string((const char *)(str.ToUTF8().data())));
					g_MainFrame->SetMacroStopped();
				}
			}

			scriptfile.Close();
		}
		g_CurrentMadEdit = nullptr;
	}
}

