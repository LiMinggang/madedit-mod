///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_highlighting_dialog.cpp
// Description: Syntax Highlight Settings Dialog
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2006-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "MadHighlightingDialog.h"

#include "MadUtils.h"
#include "MadEdit/MadSyntax.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(MadHighlightingDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern int MadMessageBox(const wxString& message,
                             const wxString& caption = wxMessageBoxCaptionStr,
                             long style = wxOK | wxCENTRE,
                             wxWindow *parent = nullptr,
                             int x = wxDefaultCoord, int y = wxDefaultCoord);
extern void ApplySyntaxAttributes(MadSyntax *syn);

MadHighlightingDialog *g_HighlightingDialog=nullptr;

enum
{
    kindSysAttr1, kindSysAttr2/*aeActiveLine, aeBookmark*/, kindRange, kindKeyword
};

struct KeywordInfo
{
    int kind;
    MadAttributes *attr; // for kindSysAttr1, kindSysArrt2, kindKeyword
    wxColour *range_bgcolor; // for kindRange
    KeywordInfo(): kind(0), attr(0), range_bgcolor(0)
    {}
    KeywordInfo(int k, MadAttributes *a, wxColour *bg): kind(k), attr(a), range_bgcolor(bg)
    {}
};
vector<KeywordInfo> g_KeywordInfoTable;
long g_Index=-1;
MadSyntax *g_Syntax=nullptr;
wxColourDialog *g_ColourDialog=nullptr;
int g_DefaultFontSize;

wxColour GetColourFromUser(const wxColour& colInit, const wxString& caption)
{
    if(g_ColourDialog==nullptr)
    {
        wxColourData *data=new wxColourData;
        data->SetChooseFull(true);
        if ( colInit.Ok() )
        {
            data->SetColour((wxColour &)colInit); // const_cast
        }
        g_ColourDialog = new wxColourDialog(g_HighlightingDialog, data);
    }
    else
    {
        if ( colInit.Ok() )
        {
            g_ColourDialog->GetColourData().SetColour(colInit);
        }
    }

    if (!caption.IsEmpty())
        g_ColourDialog->SetTitle(caption);

    wxColour colRet;
    if ( g_ColourDialog->ShowModal() == wxID_OK )
    {
        colRet = g_ColourDialog->GetColourData().GetColour();
    }

    return colRet;
}

void SetItemColour(wxListCtrl* listctrl, long item, const wxColour& fc, const wxColour& bc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetTextColour(fc);
    it.SetBackgroundColour(bc);
    listctrl->SetItem(it);
}

void SetItemTextColour(wxListCtrl* listctrl, long item, const wxColour& fc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetTextColour(fc);
    listctrl->SetItem(it);
}
void SetItemBackgroundColour(wxListCtrl* listctrl, long item, const wxColour& bc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetBackgroundColour(bc);
    listctrl->SetItem(it);
}

void SetFontStyle(wxFont &font, MadFontStyles fs)
{
    if((fs&fsBold)!=0)  font.SetWeight(wxFONTWEIGHT_BOLD);
    else                font.SetWeight(wxFONTWEIGHT_NORMAL);
    if((fs&fsItalic)!=0) font.SetStyle(wxFONTSTYLE_ITALIC);
    else                 font.SetStyle(wxFONTSTYLE_NORMAL);
    font.SetUnderlined((fs&fsUnderline)!=0);
}
wxFont GetItemFont(wxListCtrl *listctrl, long item)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    return it.GetFont();
}
void SetItemFont(wxListCtrl *listctrl, long item, wxFont &font)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    font.SetPointSize(g_DefaultFontSize);
    it.SetFont(font);
    listctrl->SetItem(it);
}

//(*IdInit(MadHighlightingDialog)
//*)

MadHighlightingDialog::MadHighlightingDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
	: m_Syntax(nullptr)
{
	//(*Initialize(MadHighlightingDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, _("Syntax Highlighting Settings"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	WxListBoxSyntax = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(145,380), 0, 0, wxLB_SINGLE|wxLB_HSCROLL, wxDefaultValidator, _T("ID_WXLISTBOXSYNTAX"));
	BoxSizer1->Add(WxListBoxSyntax, 0, wxALL|wxEXPAND, 4);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("Scheme:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer4->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxComboBoxScheme = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_WXCOMBOBOXSCHEME"));
	BoxSizer4->Add(WxComboBoxScheme, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, wxID_ANY, _("You cannot modify the scheme with * sign."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonLoad = new wxButton(this, wxID_ANY, _("&Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONLOAD"));
	BoxSizer6->Add(WxButtonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonSave = new wxButton(this, wxID_ANY, _("&Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONSAVE"));
	BoxSizer6->Add(WxButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonDelete = new wxButton(this, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONDELETE"));
	BoxSizer6->Add(WxButtonDelete, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer2->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	StaticLine1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND, 1);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxListCtrlKeyword = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(145,260), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_WXLISTCTRLKEYWORD"));
	BoxSizer5->Add(WxListCtrlKeyword, 0, wxALL|wxEXPAND, 2);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxBold = new wxCheckBox(this, wxID_ANY, _("Bold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXBOLD"));
	WxCheckBoxBold->SetValue(false);
	BoxSizer9->Add(WxCheckBoxBold, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxItalic = new wxCheckBox(this, wxID_ANY, _("Italic"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXITALIC"));
	WxCheckBoxItalic->SetValue(false);
	BoxSizer9->Add(WxCheckBoxItalic, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxUnderline = new wxCheckBox(this, wxID_ANY, _("Underline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXUNDERLINE"));
	WxCheckBoxUnderline->SetValue(false);
	BoxSizer9->Add(WxCheckBoxUnderline, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxALIGN_LEFT, 2);
	StaticLine3 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	BoxSizer8->Add(StaticLine3, 0, wxALL|wxEXPAND, 1);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11 = new wxBoxSizer(wxVERTICAL);
	WxStaticText3 = new wxStaticText(this, wxID_ANY, _("Foreground/Text Color"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer11->Add(WxStaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxStaticTextFCName = new wxStaticText(this, wxID_ANY, _T("WxStaticTextFCName"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFCNAME"));
	BoxSizer11->Add(WxStaticTextFCName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxPanelFC = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(85,-1), wxSIMPLE_BORDER, _T("ID_WXPANELFC"));
	BoxSizer11->Add(WxPanelFC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxListCtrlFC = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(160,140), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_WXLISTCTRLFC"));
	BoxSizer11->Add(WxListCtrlFC, 0, wxALL|wxEXPAND, 2);
	WxButtonFC = new wxButton(this, wxID_ANY, _("Other Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFC"));
	BoxSizer11->Add(WxButtonFC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer10->Add(BoxSizer11, 0, wxALL|wxEXPAND, 2);
	StaticLine2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxLI_VERTICAL, _T("ID_STATICLINE2"));
	BoxSizer10->Add(StaticLine2, 0, wxALL|wxEXPAND, 1);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	WxStaticText4 = new wxStaticText(this, wxID_ANY, _("Background Color"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer12->Add(WxStaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxStaticTextBCName = new wxStaticText(this, wxID_ANY, _T("WxStaticTextBCName"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTBCNAME"));
	BoxSizer12->Add(WxStaticTextBCName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxPanelBC = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(85,-1), wxSIMPLE_BORDER, _T("ID_WXPANELBC"));
	BoxSizer12->Add(WxPanelBC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxListCtrlBC = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(160,140), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_WXLISTCTRLBC"));
	BoxSizer12->Add(WxListCtrlBC, 0, wxALL|wxEXPAND, 2);
	WxButtonBC = new wxButton(this, wxID_ANY, _("Other Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONBC"));
	BoxSizer12->Add(WxButtonBC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer10->Add(BoxSizer12, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer10, 0, wxALL|wxEXPAND, 2);
	BoxSizer5->Add(BoxSizer8, 0, wxALL|wxEXPAND, 0);
	BoxSizer2->Add(BoxSizer5, 0, wxALL|wxEXPAND, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonClose = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer7->Add(WxButtonClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	WxButtonCancel->SetDefault();
	BoxSizer7->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer2->Add(BoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Bind( wxEVT_COMMAND_LISTBOX_SELECTED, &MadHighlightingDialog::WxListBoxSyntaxSelected, this, WxListBoxSyntax->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadHighlightingDialog::WxButtonLoadClick, this, WxButtonLoad->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadHighlightingDialog::WxButtonSaveClick, this, WxButtonSave->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadHighlightingDialog::WxButtonDeleteClick, this, WxButtonDelete->GetId() );
	Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED, &MadHighlightingDialog::WxListCtrlKeywordSelected, this, WxListCtrlKeyword->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &MadHighlightingDialog::WxCheckBoxBoldClick, this, WxCheckBoxBold->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &MadHighlightingDialog::WxCheckBoxItalicClick, this, WxCheckBoxItalic->GetId() );
	Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &MadHighlightingDialog::WxCheckBoxUnderlineClick, this, WxCheckBoxUnderline->GetId() );
	Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED, &MadHighlightingDialog::WxListCtrlFCSelected, this, WxListCtrlFC->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadHighlightingDialog::WxButtonFCClick, this, WxButtonFC->GetId() );
	Bind( wxEVT_COMMAND_LIST_ITEM_SELECTED, &MadHighlightingDialog::WxListCtrlBCSelected, this, WxListCtrlBC->GetId() );
	Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MadHighlightingDialog::WxButtonBCClick, this, WxButtonBC->GetId() );
	Bind( wxEVT_CLOSE_WINDOW, &MadHighlightingDialog::MadHighlightingDialogClose, this );
	Bind( wxEVT_ACTIVATE, &MadHighlightingDialog::MadHighlightingDialogActivate, this );
	//*)

#ifdef __WXMSW__
	WxListCtrlKeyword->SetWindowStyle(WxListCtrlKeyword->GetWindowStyle() & ~wxALWAYS_SHOW_SB);
#endif

    {   // build scheme list
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;++i)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
            if(i==0) WxComboBoxScheme->SetValue(name);
        }
    }

    {   // build syntax type list
        size_t cnt=MadSyntax::GetSyntaxCount();
        for(size_t i=0;i<cnt;++i)
        {
            WxListBoxSyntax->Append(MadSyntax::GetSyntaxTitle(i));
        }
    }

    g_DefaultFontSize=WxListCtrlKeyword->GetFont().GetPointSize();
    WxListCtrlKeyword->InsertColumn(0, wxT("first column"));

    // build color list
    WxListCtrlFC->Hide();
    WxListCtrlBC->Hide();
    WxListCtrlFC->InsertColumn(0, wxT("first column"));
    WxListCtrlBC->InsertColumn(0, wxT("first column"));

    wxListItem it;
    it.SetColumn(0);
    it.SetId(0);
    it.SetText(_("(Automatic)"));
    it.SetTextColour(wxT("Red"));
    WxListCtrlFC->InsertItem(it);
    WxListCtrlBC->InsertItem(it);

    HtmlColor *hc=HtmlColorTable;
    for(int i=0; i<HtmlColorTableCount; ++i, ++hc)
    {
        it.SetText(wxGetTranslation(hc->name));
        it.SetId(i+1);

        wxColor c(wxColor(hc->red, hc->green, hc->blue));
        it.SetTextColour(c);
        it.SetBackgroundColour(wxNullColour);
        WxListCtrlFC->InsertItem(it);

        if((hc->red+hc->green+hc->blue)/3 >= 128)
            it.SetTextColour(*wxBLACK);
        else
            it.SetTextColour(*wxWHITE);
        it.SetBackgroundColour(c);
        WxListCtrlBC->InsertItem(it);
    }

    WxListCtrlFC->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    WxListCtrlBC->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    WxListCtrlFC->Show();
    WxListCtrlBC->Show();
}

MadHighlightingDialog::~MadHighlightingDialog()
{
	//(*Destroy(MadHighlightingDialog)
	//*)
}


void MadHighlightingDialog::MadHighlightingDialogClose(wxCloseEvent& event)
{
    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }

    FreeSyntax(false);
    g_HighlightingDialog=nullptr;
    if(g_ColourDialog) delete g_ColourDialog;
    Destroy();
}

void MadHighlightingDialog::WxListBoxSyntaxSelected(wxCommandEvent& event)
{
    wxString title=WxListBoxSyntax->GetString(event.GetSelection());
    g_Syntax=GetSyntax(title);

    // build keyword list
    WxListCtrlKeyword->Freeze();
    WxListCtrlKeyword->DeleteAllItems();
    g_KeywordInfoTable.clear();
    int index=0;

    // system attributes
    for(int ae=aeText; ae<aeNone; ++ae)
    {
        long item = WxListCtrlKeyword->InsertItem(index++, MadSyntax::GetAttributeName((MadAttributeElement)ae));
        MadAttributes *attr = g_Syntax->GetAttributes((MadAttributeElement)ae);
        int kind=kindSysAttr1;
        if(ae==aeActiveLine || ae==aeBookmark) kind=kindSysAttr2;
        g_KeywordInfoTable.push_back(KeywordInfo(kind, attr, nullptr));

        if(ae==aeText)
        {
            WxListCtrlKeyword->SetBackgroundColour(attr->bgcolor);
        }

        SetItemColour(WxListCtrlKeyword, item, attr->color, attr->bgcolor);
        wxFont font=GetItemFont(WxListCtrlKeyword, item);
        SetFontStyle(font, attr->style);
        SetItemFont(WxListCtrlKeyword, item, font);
    }
    
    // custom ranges
    size_t i;
    for(i=0; i<g_Syntax->m_CustomRange.size(); ++i)
    {
        wxString text;
        text.Printf(wxT("Range %s %s"), g_Syntax->m_CustomRange[i].begin.c_str(), g_Syntax->m_CustomRange[i].end.c_str());
        long item = WxListCtrlKeyword->InsertItem(index++, text);
        wxColour *bg = &(g_Syntax->m_CustomRange[i].bgcolor);
        g_KeywordInfoTable.push_back( KeywordInfo(kindRange, nullptr, bg) );
        SetItemColour(WxListCtrlKeyword, item, g_KeywordInfoTable[0].attr->color, *bg);
    }
    
    // custom keywords
    for(i=0; i<g_Syntax->m_CustomKeyword.size(); ++i)
    {
        long item = WxListCtrlKeyword->InsertItem(index++, g_Syntax->m_CustomKeyword[i].m_Name);
        MadAttributes *attr = &(g_Syntax->m_CustomKeyword[i].m_Attr);
        g_KeywordInfoTable.push_back(KeywordInfo(kindKeyword, attr, nullptr));

        SetItemColour(WxListCtrlKeyword, item, attr->color, attr->bgcolor);
        wxFont font=GetItemFont(WxListCtrlKeyword, item);
        SetFontStyle(font, attr->style);
        SetItemFont(WxListCtrlKeyword, item, font);
    }

    WxListCtrlKeyword->SetColumnWidth( 0, WxListCtrlKeyword->GetClientSize().x - 4);
    WxListCtrlKeyword->Thaw();

    g_Index=-1;
    wxListEvent e;
    e.m_itemIndex=0;
    WxListCtrlKeywordSelected(e);
}


void MadHighlightingDialog::SetPanelFC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextFCName->SetLabel(wxT("(Automatic)"));
        WxPanelFC->SetBackgroundColour(WxListCtrlFC->GetItemTextColour(0));
        WxPanelFC->ClearBackground();
    }
    else
    {
        wxString cname=wxTheColourDatabase->FindName(color);
        if(cname.IsEmpty())
        {
            cname.Printf(wxT("#%02X%02X%02X"), color.Red(), color.Green(), color.Blue());
        }
        WxStaticTextFCName->SetLabel(cname);
        WxPanelFC->SetBackgroundColour(color);
        WxPanelFC->ClearBackground();
    }
}

void MadHighlightingDialog::SetPanelBC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextBCName->SetLabel(wxT("(Automatic)"));
        WxPanelBC->SetBackgroundColour(WxListCtrlBC->GetItemBackgroundColour(0));
        WxPanelBC->ClearBackground();
    }
    else
    {
        wxString cname=wxTheColourDatabase->FindName(color);
        if(cname.IsEmpty())
        {
            cname.Printf(wxT("#%02X%02X%02X"), color.Red(), color.Green(), color.Blue());
        }
        WxStaticTextBCName->SetLabel(cname);
        WxPanelBC->SetBackgroundColour(color);
        WxPanelBC->ClearBackground();
    }
}

/*
 * WxListCtrlKeywordSelected
 */
void MadHighlightingDialog::WxListCtrlKeywordSelected(wxListEvent& event)
{
    long oldIndex=g_Index;
    g_Index = event.m_itemIndex;

    WxListCtrlKeyword->Freeze();
    WxListCtrlFC->Freeze();

    if(oldIndex!=g_Index)
    {
        wxString str;
        if(oldIndex>=0)
        {
            str = WxListCtrlKeyword->GetItemText(oldIndex);
            if(str[0]==wxT('*')) 
            {
                WxListCtrlKeyword->SetItemText(oldIndex, str.Right(str.Len()-1));
            }
        }

        str = WxListCtrlKeyword->GetItemText(g_Index);
        WxListCtrlKeyword->SetItemText(g_Index, wxString(wxT('*'))+str);

        WxListCtrlKeyword->SetColumnWidth(0, WxListCtrlKeyword->GetClientSize().x - 4);
    }

    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0) // set (Automatic) colors
    {
        SetItemTextColour(WxListCtrlFC, 0, kinfo.attr->color);
        int c = (kinfo.attr->bgcolor.Red()+kinfo.attr->bgcolor.Green()+kinfo.attr->bgcolor.Blue())/3;
        if(c >= 128)
        {
            SetItemColour(WxListCtrlBC, 0, *wxBLACK, kinfo.attr->bgcolor);
        }
        else
        {
            SetItemColour(WxListCtrlBC, 0, *wxWHITE, kinfo.attr->bgcolor);
        }
    }

    wxColour bgc;
    switch(kinfo.kind)
    {
    case kindSysAttr1:
    case kindKeyword:
        WxCheckBoxBold->Enable();
        WxCheckBoxItalic->Enable();
        WxCheckBoxUnderline->Enable();
        WxCheckBoxBold->SetValue((kinfo.attr->style&fsBold)!=0);
        WxCheckBoxItalic->SetValue((kinfo.attr->style&fsItalic)!=0);
        WxCheckBoxUnderline->SetValue((kinfo.attr->style&fsUnderline)!=0);
        SetPanelFC(kinfo.attr->color);
        SetPanelBC(kinfo.attr->bgcolor);
        WxListCtrlFC->Enable();
        WxButtonFC->Enable();
        WxListCtrlBC->Enable();
        WxButtonBC->Enable();
        bgc=kinfo.attr->bgcolor;
        break;
    case kindSysAttr2:
        WxCheckBoxBold->SetValue(false);
        WxCheckBoxItalic->SetValue(false);
        WxCheckBoxUnderline->SetValue(false);
        WxCheckBoxBold->Disable();
        WxCheckBoxItalic->Disable();
        WxCheckBoxUnderline->Disable();
        SetPanelFC(kinfo.attr->color);
        SetPanelBC(wxNullColour);
        WxListCtrlFC->Enable();
        WxButtonFC->Enable();
        WxListCtrlBC->Disable();
        WxButtonBC->Disable();
        bgc=wxNullColour;
        break;
    case kindRange:
        WxCheckBoxBold->SetValue(false);
        WxCheckBoxItalic->SetValue(false);
        WxCheckBoxUnderline->SetValue(false);
        WxCheckBoxBold->Disable();
        WxCheckBoxItalic->Disable();
        WxCheckBoxUnderline->Disable();
        SetPanelFC(wxNullColour);
        SetPanelBC(*kinfo.range_bgcolor);
        WxListCtrlFC->Disable();
        WxButtonFC->Disable();
        WxListCtrlBC->Enable();
        WxButtonBC->Enable();
        bgc=*kinfo.range_bgcolor;
        break;
    }
    if(bgc==wxNullColour) bgc=g_KeywordInfoTable[0].attr->bgcolor;
    WxListCtrlFC->SetBackgroundColour(bgc);
    SetItemBackgroundColour(WxListCtrlFC, 0, bgc);
    this->Layout();

    WxListCtrlKeyword->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
    WxListCtrlKeyword->Thaw();
    WxListCtrlFC->Thaw();
}

/*
 * WxListCtrlFCSelected
 */
void MadHighlightingDialog::WxListCtrlFCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlFC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlFC->GetItemTextColour(event.m_itemIndex);
    WxStaticTextFCName->SetLabel(colorname);
    this->Layout();

    WxPanelFC->SetBackgroundColour(color);
    WxPanelFC->ClearBackground();

    SetAttrFC(color, colorname);
    SetToModifiedSyntax(g_Syntax);

    WxListCtrlFC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

/*
 * WxListCtrlBCSelected
 */
void MadHighlightingDialog::WxListCtrlBCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlBC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlBC->GetItemBackgroundColour(event.m_itemIndex);
    WxStaticTextBCName->SetLabel(colorname);
    this->Layout();

    WxPanelBC->SetBackgroundColour(color);
    WxPanelBC->ClearBackground();

    SetAttrBC(color, colorname);
    SetToModifiedSyntax(g_Syntax);

    WxListCtrlBC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

/*
 * WxCheckBoxBoldClick
 */
void MadHighlightingDialog::WxCheckBoxBoldClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsBold;
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsBold);
    }
    SetFontStyle(font, g_KeywordInfoTable[g_Index].attr->style);
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, WxListCtrlKeyword->GetClientSize().x - 4);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxCheckBoxItalicClick
 */
void MadHighlightingDialog::WxCheckBoxItalicClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsItalic;
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsItalic);
    }
    SetFontStyle(font, g_KeywordInfoTable[g_Index].attr->style);
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, WxListCtrlKeyword->GetClientSize().x - 4);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxCheckBoxUnderlineClick
 */
void MadHighlightingDialog::WxCheckBoxUnderlineClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsUnderline;
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsUnderline);
    }
    SetFontStyle(font, g_KeywordInfoTable[g_Index].attr->style);
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, WxListCtrlKeyword->GetClientSize().x - 4);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxButtonFCClick
 */
void MadHighlightingDialog::WxButtonFCClick(wxCommandEvent& event)
{
    wxColour color=GetColourFromUser(WxListCtrlKeyword->GetItemTextColour(g_Index), WxStaticText3->GetLabel());
    if(color.Ok())
    {
        SetPanelFC(color);
        this->Layout();
        wxString colorname=WxStaticTextFCName->GetLabel();
        SetAttrFC(color, colorname);
        SetToModifiedSyntax(g_Syntax);
    }
}

/*
 * WxButtonBCClick
 */
void MadHighlightingDialog::WxButtonBCClick(wxCommandEvent& event)
{
    wxColour color=GetColourFromUser(WxPanelBC->GetBackgroundColour(), WxStaticText4->GetLabel());
    if(color.Ok())
    {
        SetPanelBC(color);
        this->Layout();
        wxString colorname=WxStaticTextBCName->GetLabel();
        SetAttrBC(color, colorname);
        SetToModifiedSyntax(g_Syntax);
    }
}

/*
 * MadHighlightingDialogActivate
 */
void MadHighlightingDialog::MadHighlightingDialogActivate(wxActivateEvent& event)
{
    if(!m_InitSetting.IsEmpty() && event.GetActive())
    {
        g_Index=-1;

        int i=WxListBoxSyntax->FindString(m_InitSetting);
        if(i==wxNOT_FOUND)
        {
            i=WxListBoxSyntax->GetSelection();
            if(i==wxNOT_FOUND) i=0;
        }
        WxListBoxSyntax->SetSelection(i);

        wxCommandEvent e;
        e.SetInt(i);
        WxListBoxSyntaxSelected(e);

        m_InitSetting.Empty();

        if(FindFocus()==nullptr)
        {
            WxButtonCancel->SetFocus();
        }
    }
}

MadSyntax *MadHighlightingDialog::GetSyntax(const wxString &title)
{
    if(m_Syntax && m_Syntax->m_Title.CmpNoCase(title)==0)
        return m_Syntax;

    for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        if(m_ModifiedSyntax[i]->m_Title.CmpNoCase(title)==0)
            return m_ModifiedSyntax[i];
    }

    if(m_Syntax) delete m_Syntax;
    m_Syntax=MadSyntax::GetSyntaxByTitle(title);
	wxASSERT( m_Syntax != 0 );
    return m_Syntax;
}

void MadHighlightingDialog::SetToModifiedSyntax(MadSyntax *syn)
{
    if(syn==m_Syntax)
    {
        m_ModifiedSyntax.push_back(syn);
        m_Syntax=nullptr;
    }
    ApplySyntaxAttributes(syn);// apply syntax attributes to editor
}

void MadHighlightingDialog::SetAttrFC(const wxColor &color, const wxString &colorname)
{
    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0)
    {
        SetItemTextColour(WxListCtrlFC, 0, color);
        kinfo.attr->color=color;
    }
    else
    {
        wxASSERT(kinfo.kind!=kindRange);
        if(colorname==wxT("(Automatic)"))
        {
            kinfo.attr->color=wxNullColour;
        }
        else
        {
            kinfo.attr->color=color;
        }
    }
    RepaintKeyword();
}

void MadHighlightingDialog::SetAttrBC(const wxColor &color, const wxString &colorname)
{
    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0)
    {
        SetItemBackgroundColour(WxListCtrlBC, 0, color);
        WxListCtrlKeyword->SetBackgroundColour(color);
        kinfo.attr->bgcolor=color;
    }
    else
    {
        wxASSERT(kinfo.kind!=kindSysAttr2);
        switch(kinfo.kind)
        {
        case kindSysAttr1:
        case kindKeyword:
            if(colorname==wxT("(Automatic)")) kinfo.attr->bgcolor=wxNullColour;
            else                              kinfo.attr->bgcolor=color;
            break;
        case kindRange:
            if(colorname==wxT("(Automatic)")) *kinfo.range_bgcolor=wxNullColour;
            else                              *kinfo.range_bgcolor=color;
            break;
        }
    }
    RepaintKeyword();

    SetItemBackgroundColour(WxListCtrlFC, 0, color);
    WxListCtrlFC->SetBackgroundColour(color);
    WxListCtrlFC->Refresh();
}

void MadHighlightingDialog::RepaintKeyword()
{
    vector<KeywordInfo>::iterator it=g_KeywordInfoTable.begin();
    vector<KeywordInfo>::iterator itend=g_KeywordInfoTable.end();
    long idx=0;
    wxFont font;
    wxColour &fc0=it->attr->color;
    wxColour &bc0=it->attr->bgcolor;
    wxColour fc, bc;
    WxListCtrlKeyword->Freeze();
    do
    {
        switch(it->kind)
        {
        case kindSysAttr1:
        case kindSysAttr2:
        case kindKeyword:
            fc=it->attr->color;
            bc=it->attr->bgcolor;
            font=GetItemFont(WxListCtrlKeyword, idx);
            SetFontStyle(font, it->attr->style);
            SetItemFont(WxListCtrlKeyword, idx, font);
            break;
        case kindRange:
            fc=wxNullColour;
            bc=*it->range_bgcolor;
            break;
        }
        if(fc==wxNullColour) fc=fc0;
        if(bc==wxNullColour) bc=bc0;
        SetItemColour(WxListCtrlKeyword, idx, fc, bc);
        ++idx;
    }
    while(++it != itend);
    WxListCtrlKeyword->SetColumnWidth(0, WxListCtrlKeyword->GetClientSize().x - 4);
    WxListCtrlKeyword->Thaw();
}

void MadHighlightingDialog::FreeSyntax(bool restore)
{
    if(restore) // restore the original syntax
    {
        for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            MadSyntax *syn=MadSyntax::GetSyntaxByTitle(m_ModifiedSyntax[i]->m_Title);
            ApplySyntaxAttributes(syn);
            delete syn;
        }
    }
    else // write the modified syntax back
    {
        for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            m_ModifiedSyntax[i]->SaveAttributes();
        }
    }

    if(m_Syntax) 
    {
        delete m_Syntax;
        m_Syntax=nullptr;
    }

    for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        delete m_ModifiedSyntax[i];
    }
    m_ModifiedSyntax.clear();
}


/*
 * WxButtonLoadClick
 */
void MadHighlightingDialog::WxButtonLoadClick(wxCommandEvent& event)
{
    if(MadSyntax::LoadScheme(WxComboBoxScheme->GetValue(), g_Syntax))
    {
        WxListCtrlKeyword->SetBackgroundColour(g_KeywordInfoTable[0].attr->bgcolor);
        RepaintKeyword();
        wxListEvent e;
        e.m_itemIndex=g_Index;
        WxListCtrlKeywordSelected(e);
        
        SetToModifiedSyntax(g_Syntax);
    }
    else
    {
        MadMessageBox(_("Cannot load this scheme."), wxT("MadEdit-Mod"), wxICON_WARNING|wxOK);
    }
}

/*
 * WxButtonSaveClick
 */
void MadHighlightingDialog::WxButtonSaveClick(wxCommandEvent& event)
{
    wxString schname = WxComboBoxScheme->GetValue();
    if(MadSyntax::SaveScheme(schname, g_Syntax))
    {
        WxComboBoxScheme->Clear();
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;++i)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
        }
        WxComboBoxScheme->SetValue(schname);
    }
    else
    {
        MadMessageBox(_("Cannot save to the scheme."), wxT("MadEdit-Mod"), wxICON_WARNING|wxOK);
    }
}

/*
 * WxButtonDeleteClick
 */
void MadHighlightingDialog::WxButtonDeleteClick(wxCommandEvent& event)
{
    if(MadSyntax::DeleteScheme(WxComboBoxScheme->GetValue()))
    {
        WxComboBoxScheme->Clear();
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;++i)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
            if(i==0) WxComboBoxScheme->SetValue(name);
        }
    }
    else
    {
        MadMessageBox(_("Cannot delete this scheme."), wxT("MadEdit-Mod"), wxICON_WARNING|wxOK);
    }
}
