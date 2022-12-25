///////////////////////////////////////////////////////////////////////////////
// Name:        MadPrintout.cpp
// Description: Printing functions
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/print.h>
#include <wx/dc.h>
#include <wx/confbase.h>
#include <wx/filename.h>
#include "MadPrintout.h"
#include "MadEdit/MadEdit.h"

extern MadEdit *g_ActiveMadEdit;
bool GetActiveMadEditPathNameOrTitle(wxString &name);

// Global print data, to remember settings during the session
//wxPrintData *g_PrintData = (wxPrintData*) nullptr ;

// Global page setup data
wxPageSetupData *g_PageSetupData = (wxPageSetupData*) nullptr;

int MadPrintout::s_PrintoutCount=0;

MadPrintout::MadPrintout(const wxString& title)
: wxPrintout(title), m_PrintHeader(true), m_PrintFooter(true), m_CharHeight(0), m_HeaderHeight(0), m_FooterHeight(0)
{
    ++s_PrintoutCount;
}

MadPrintout::~MadPrintout()
{
    if (--s_PrintoutCount==0)
    {
        g_ActiveMadEdit->EndPrint();
    }
}

void MadPrintout::OnPreparePrinting()
{
    double xScale, yScale;
    wxRect printRect;
    CalcPrintInfo(g_PageSetupData, xScale, yScale, printRect);

    // get print info
    wxConfigBase *cfg=wxConfigBase::Get(false);
    wxString oldpath=cfg->GetPath();
    cfg->SetPath(wxT("/MadEdit"));

    cfg->Read(wxT("PrintPageHeader"), &m_PrintHeader);
    cfg->Read(wxT("PageHeaderLeft"), &m_HeaderLeft);
    cfg->Read(wxT("PageHeaderCenter"), &m_HeaderCenter);
    cfg->Read(wxT("PageHeaderRight"), &m_HeaderRight);

    cfg->Read(wxT("PrintPageFooter"), &m_PrintFooter);
    cfg->Read(wxT("PageFooterLeft"), &m_FooterLeft);
    cfg->Read(wxT("PageFooterCenter"), &m_FooterCenter);
    cfg->Read(wxT("PageFooterRight"), &m_FooterRight);
    
    cfg->SetPath(oldpath);
    
    wxFont font=g_ActiveMadEdit->GetFont();
    font.SetPointSize(12);      // use 12 PointSize to print Header, Footer
    wxDC *dc=GetDC();
    dc->SetFont(font);
    m_CharHeight = g_ActiveMadEdit->ToPhys(dc->GetCharHeight());
    
    // subtract header,footer rect
    m_HeaderHeight=0;
    if (m_PrintHeader)
    {
        m_HeaderHeight = (m_CharHeight*5)/4 ; // 1.25 times
        printRect.y += m_HeaderHeight;
        printRect.height -= m_HeaderHeight;
    }
    m_FooterHeight=0;
    if (m_PrintFooter)
    {
        m_FooterHeight = (m_CharHeight*5)/4 ; // 1.25 times
        printRect.height -= m_FooterHeight;
    }

    g_ActiveMadEdit->BeginPrint(printRect);
}

void MadPrintout::GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo)
{
    // get info from g_ActiveMadEdit
    *maxPage = *pageTo = g_ActiveMadEdit->GetPageCount();
    if (*maxPage==0)
    {
        *minPage=*pageFrom=0;
    }
    else
    {
        *minPage=*pageFrom=1;
    }
}

bool MadPrintout::HasPage(int page)
{
    // get info from g_ActiveMadEdit
    size_t count=g_ActiveMadEdit->GetPageCount();
    return ((size_t)page>=1 && (size_t)page<=count);
}

/* Header & Footer Print Marks
%f File Name
%p Path Name
%n Page Number
%s Total Pages
%d Date
%t Time
*/
wxString TranslatePrintMark(const wxString &text, int pageNum)
{
    wxString newtext, name;
    wxDateTime now;
    size_t count=text.Len();
    for (size_t i=0;i<count;++i)
    {
        if (text[i]==wxT('%') && (i+1)<count)
        {
            const wxChar wc=text[++i];
            switch (wc)
            {
            case wxT('f'):
                if (GetActiveMadEditPathNameOrTitle(name))
                {
                    wxFileName fn(name);
                    newtext += fn.GetFullName();
                }
                else // title
                {
                    newtext += name;
                }
                break;
            case wxT('p'):
                GetActiveMadEditPathNameOrTitle(name);
                newtext += name;
                break;
            case wxT('n'):
                newtext << pageNum;
                break;
            case wxT('s'):
                newtext << g_ActiveMadEdit->GetPageCount();
                break;
            case wxT('d'):
                now = wxDateTime::Now();
                newtext += now.FormatDate();
                break;
            case wxT('t'):
                now = wxDateTime::Now();
                newtext += now.FormatTime();
                break;
            case wxT('%'):
                newtext += wxT('%');
                break;
            default:
                newtext += wxT('%');
                newtext += text[i];
                break;
            }
        }
        else
        {
            newtext += text[i];
        }
    }

    return newtext;
}

bool MadPrintout::OnPrintPage(int page)
{
    wxDC *dc = GetDC();
    
    double xScale, yScale;
    wxRect rect;
    // calc and set user scale
    CalcPrintInfo(g_PageSetupData, xScale, yScale, rect);
    dc->SetUserScale(xScale, yScale);

    // paint the contents
    g_ActiveMadEdit->PrintPage(dc, page);
    
    // paint Header, Footer
    wxFont font=g_ActiveMadEdit->GetFont();
    font.SetPointSize(12);      // use 12 PointSize to print Header, Footer
    dc->SetFont(font);
    dc->SetTextForeground(*wxBLACK);

    wxString str;
    if (m_PrintHeader)
    {
        if (!m_HeaderLeft.IsEmpty())
        {
            str=TranslatePrintMark(m_HeaderLeft, page);
            dc->DrawText(str, rect.x, rect.y);
        }
        if (!m_HeaderCenter.IsEmpty())
        {
            str=TranslatePrintMark(m_HeaderCenter, page);
			wxSize size = g_ActiveMadEdit->ToPhys(dc->GetTextExtent(str)); 
            dc->DrawText(str, rect.x + (rect.width-size.x)/2, rect.y);
        }
        if (!m_HeaderRight.IsEmpty())
        {
            str=TranslatePrintMark(m_HeaderRight, page);
			wxSize size = g_ActiveMadEdit->ToPhys(dc->GetTextExtent(str)); 
            dc->DrawText(str, rect.x+rect.width-size.x, rect.y);
        }
        
        // draw a line
        dc->SetPen(*wxBLACK_PEN);
        int y=rect.y+m_CharHeight+ (m_HeaderHeight-m_CharHeight)/2;
        dc->DrawLine(rect.x, y, rect.x+rect.width, y);
    }
    
    if (m_PrintFooter)
    {
        int y=rect.y+rect.height-m_CharHeight;
        
        if (!m_FooterLeft.IsEmpty())
        {
            str=TranslatePrintMark(m_FooterLeft, page);
            dc->DrawText(str, rect.x, y);
        }
        if (!m_FooterCenter.IsEmpty())
        {
            str=TranslatePrintMark(m_FooterCenter, page);
			wxSize size = g_ActiveMadEdit->ToPhys(dc->GetTextExtent(str)); 
            dc->DrawText(str, rect.x + (rect.width-size.x)/2, y);
        }
        if (!m_FooterRight.IsEmpty())
        {
            str=TranslatePrintMark(m_FooterRight, page);
			wxSize size = g_ActiveMadEdit->ToPhys(dc->GetTextExtent(str)); 
            dc->DrawText(str, rect.x+rect.width-size.x, y);
        }
        
        // draw a line
        dc->SetPen(*wxBLACK_PEN);
        y -= (m_HeaderHeight-m_CharHeight)/2;
        dc->DrawLine(rect.x, y, rect.x+rect.width, y);
    }

    /*
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->DrawRectangle(rect);
    */
    return true;
}

void MadPrintout::CalcPrintInfo(wxPageSetupData *pPageSetupData, double &xScale, double &yScale, wxRect &printRect)
{
    wxDC *dc = GetDC();
    int dcw, dch;
    dc->GetSize(&dcw, &dch);
    
    int pagesize_x, pagesize_y;
    GetPageSizeMM(&pagesize_x, &pagesize_y);
    wxPoint pttl=pPageSetupData->GetMarginTopLeft();
    wxPoint ptbr=pPageSetupData->GetMarginBottomRight();

    int sx, sy;
    GetPPIScreen(&sx, &sy);

    // pagesize: convert mm to pixel
    double px = double(pagesize_x)/25.4 * double(sx);
    double py = double(pagesize_y)/25.4 * double(sy);

    // calc printRect scale
    double top    = double(pttl.y)/double(pagesize_y);
    double left   = double(pttl.x)/double(pagesize_x);
    double bottom = double(ptbr.y)/double(pagesize_y);
    double right  = double(ptbr.x)/double(pagesize_x);
    
    // calc the pixel-size of printRect
    printRect.x = int(left * px);
    printRect.y = int(top  * py);
    printRect.width  = int(px) - int(right * px) - printRect.x;
    printRect.height = int(py) - int(bottom * py) - printRect.y;

    // calc scaling factor
    xScale = double(dcw) / px;
    yScale = double(dch) / py;
}
