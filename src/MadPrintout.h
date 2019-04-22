///////////////////////////////////////////////////////////////////////////////
// Name:        MadPrintout.h
// Description: Printing functions
// Author:		madedit@gmail.com
// Maintainer:	minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADPRINTOUT_H_
#define _MADPRINTOUT_H_
#include "MadUtils.h"

#if wxMAJOR_VERSION < 3 || (wxMAJOR_VERSION == 3 && wxMINOR_VERSION < 1)
#define wxOVERRIDE
#endif

class MadPrintout: public wxPrintout
{
    static int s_PrintoutCount;
public:
    explicit MadPrintout(const wxString& title = _("MadEdit-Mod Printout"));
    virtual ~MadPrintout();

    virtual void GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo) wxOVERRIDE;
    virtual bool HasPage(int page);
    virtual bool OnPrintPage(int page);
    
    virtual void OnPreparePrinting();
    
    void CalcPrintInfo(wxPageSetupData *pPageSetupData, double &xScale, double &yScale, wxRect &printRect);
    
    bool m_PrintHeader, m_PrintFooter;
    wxString m_HeaderLeft, m_HeaderCenter, m_HeaderRight;
    wxString m_FooterLeft, m_FooterCenter, m_FooterRight;
    int m_CharHeight, m_HeaderHeight, m_FooterHeight;
};

//extern wxPrintData *g_PrintData;
extern wxPageSetupData* g_PageSetupData;

#endif
