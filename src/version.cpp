#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

wxString g_MadEdit_Version( wxString(wxT("MadEdit-Mod 0.3.5 "))
#if defined(__WXMSW__)
#ifdef _WIN64
+ wxT("win64 build ") + wxT(__TIME__) + wxT(" ") + wxT(__DATE__)
#else
+ wxT("win32 build ") + wxT(__TIME__) + wxT(" ") + wxT(__DATE__)
#endif
#elif defined(__WXMAC__)
+ wxT("MacOS")
#else //Linux
#if defined(__x86_64__) || defined(__LP64__)
+ wxT("x86_x64")
#else
+ wxT("i686")
#endif
#endif
);

