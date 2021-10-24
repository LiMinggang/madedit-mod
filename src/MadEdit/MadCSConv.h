///////////////////////////////////////////////////////////////////////////////
// Name:        MadCSConv.h
// Description: the Encoding Converter of MadEdit
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __MADCSCONV__
#define __MADCSCONV__
#include <map>
#include "../MadUtils.h"

#if CPLUSEPLUSE98
	#include <boost/shared_ptr.hpp>
	using boost::shared_ptr;
#else
	#include <memory>
	using std::shared_ptr;
#endif
#include "wx/fontenc.h"         // for wxFontEncoding

#define __MAD_ENCODING_EXTENDED__

#ifdef __MAD_ENCODING_EXTENDED__
#define MAD_FONTENCODING_GB18030 (wxFONTENCODING_MAX+1)
#define MAD_FONTENCODING_IBM1025 (wxFONTENCODING_MAX+2)
#define MAD_FONTENCODING_IBM1047 (wxFONTENCODING_MAX+3)
#define MAD_FONTENCODING_IBM1097 (wxFONTENCODING_MAX+4)
#define MAD_FONTENCODING_IBM1112 (wxFONTENCODING_MAX+5)
#define MAD_FONTENCODING_IBM1122 (wxFONTENCODING_MAX+6)
#define MAD_FONTENCODING_IBM1123 (wxFONTENCODING_MAX+7)
#define MAD_FONTENCODING_IBM1130 (wxFONTENCODING_MAX+8)
#define MAD_FONTENCODING_IBM1132 (wxFONTENCODING_MAX+9)
#define MAD_FONTENCODING_IBM1137 (wxFONTENCODING_MAX+10)
#define MAD_FONTENCODING_IBM1140 (wxFONTENCODING_MAX+11)
#define MAD_FONTENCODING_IBM1141 (wxFONTENCODING_MAX+12)
#define MAD_FONTENCODING_IBM1142 (wxFONTENCODING_MAX+13)
#define MAD_FONTENCODING_IBM1143 (wxFONTENCODING_MAX+14)
#define MAD_FONTENCODING_IBM1144 (wxFONTENCODING_MAX+15)
#define MAD_FONTENCODING_IBM1145 (wxFONTENCODING_MAX+16)
#define MAD_FONTENCODING_IBM1146 (wxFONTENCODING_MAX+17)
#define MAD_FONTENCODING_IBM1147 (wxFONTENCODING_MAX+18)
#define MAD_FONTENCODING_IBM1148 (wxFONTENCODING_MAX+19)
#define MAD_FONTENCODING_IBM1149 (wxFONTENCODING_MAX+20)
#define MAD_FONTENCODING_IBM1153 (wxFONTENCODING_MAX+21)
#define MAD_FONTENCODING_IBM1154 (wxFONTENCODING_MAX+22)
#define MAD_FONTENCODING_IBM1155 (wxFONTENCODING_MAX+23)
#define MAD_FONTENCODING_IBM1156 (wxFONTENCODING_MAX+24)
#define MAD_FONTENCODING_IBM1157 (wxFONTENCODING_MAX+25)
#define MAD_FONTENCODING_IBM1158 (wxFONTENCODING_MAX+26)
#define MAD_FONTENCODING_IBM1160 (wxFONTENCODING_MAX+27)
#define MAD_FONTENCODING_IBM1164 (wxFONTENCODING_MAX+28)
#define MAD_FONTENCODING_IBM1166 (wxFONTENCODING_MAX+29)
#define MAD_FONTENCODING_IBM12712 (wxFONTENCODING_MAX+30)
#define MAD_FONTENCODING_IBM16804 (wxFONTENCODING_MAX+31)
#endif //__MAD_ENCODING_EXTENDED__

class wxCSConv;

// Simulating proxy pattern
class MadCSConv
{
public:
	explicit MadCSConv(const wxString &charset);
	explicit MadCSConv(int encoding);
	~MadCSConv() {}

	size_t MB2WC (wchar_t *out, const char *in, size_t outLen) const; 
	size_t WC2MB (char *buf, const wchar_t *psz, size_t n) const;
	static void InitCharSetNames();
#ifdef __MAD_ENCODING_EXTENDED__	
	static std::map<wxString, int>MadExtCharSetNames;
#endif //__MAD_ENCODING_EXTENDED__
private:
	wxString m_Charset;
	int m_Encoding;
	shared_ptr< wxCSConv > m_CSConv;
};


#endif //__MADCSCONV__
