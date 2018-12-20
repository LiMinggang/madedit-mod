///////////////////////////////////////////////////////////////////////////////
// Name:        MadCSConv.h
// Description: the Encoding Converter of MadEdit
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __MADCSCONV__
#define __MADCSCONV__
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
private:
#ifdef __MAD_ENCODING_EXTENDED__
	size_t MB2WC_GB18030 (wchar_t *out, const char *in, size_t outLen) const; 
	size_t WC2MB_GB18030 (char *buf, const wchar_t *psz, size_t n) const;
#endif //__MAD_ENCODING_EXTENDED__
private:
	wxString m_Charset;
	int m_Encoding;
	shared_ptr< wxCSConv > m_CSConv;
};


#endif //__MADCSCONV__
