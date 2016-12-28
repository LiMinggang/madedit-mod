///////////////////////////////////////////////////////////////////////////////
// Name:        MadCSConv.cpp
// Description: the Encoding Converter of MadEdit
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/strconv.h>
#include <wx/fontmap.h>

#include "MadCSConv.h"
#ifdef __MAD_ENCODING_EXTENDED__
#include "ExtEncodings/converters.h"
#endif //__MAD_ENCODING_EXTENDED__

static const wxString CHARSET_GB18030(_T("GB18030"));

MadCSConv::MadCSConv(const wxString &charset):m_Charset(charset.Upper())
{

	m_Encoding = wxFontMapperBase::GetEncodingFromName(charset);
	if(m_Encoding != wxFONTENCODING_SYSTEM)
	{
		m_CSConv.reset(new wxCSConv( (wxFontEncoding)m_Encoding ));
	}
#ifdef __MAD_ENCODING_EXTENDED__
	else if(m_Charset.IsSameAs(CHARSET_GB18030))
	{
		m_Encoding = MAD_FONTENCODING_GB18030;
		//m_CSConv.reset(new wxCSConv( wxFONTENCODING_CP936));
	}
#endif //__MAD_ENCODING_EXTENDED__
	else
	{
		m_Encoding = wxFONTENCODING_SYSTEM;
	}
}

MadCSConv::MadCSConv(int encoding):m_Charset(_T("UNKNOWN_CHARSET")), m_Encoding(encoding)
{
	if(m_Encoding < wxFONTENCODING_MAX)
	{
		m_CSConv.reset(new wxCSConv( (wxFontEncoding)m_Encoding ));
	}
#ifdef __MAD_ENCODING_EXTENDED__
	else if(m_Encoding == MAD_FONTENCODING_GB18030)
	{
		;//m_CSConv.reset(new wxCSConv( wxFONTENCODING_CP936));
	}
#endif //__MAD_ENCODING_EXTENDED__
	else
	{
		m_Encoding = wxFONTENCODING_SYSTEM;
	}
}

size_t MadCSConv::MB2WC (wchar_t *out, const char *in, size_t outLen) const
{
	if((m_Encoding < wxFONTENCODING_MAX) && (m_CSConv)) return m_CSConv->MB2WC(out, in, outLen);

#ifdef __MAD_ENCODING_EXTENDED__
	switch(m_Encoding)
	{
	case MAD_FONTENCODING_GB18030:
		return MB2WC_GB18030 (out, in, outLen);
		//break;
	default:
		break;
	}
#endif //__MAD_ENCODING_EXTENDED__

	return 0;
}

size_t MadCSConv::WC2MB (char *buf, const wchar_t *psz, size_t n) const
{
	if((m_Encoding < wxFONTENCODING_MAX) && (m_CSConv)) return m_CSConv->WC2MB(buf, psz, n);

#ifdef __MAD_ENCODING_EXTENDED__
	switch(m_Encoding)
	{
	case MAD_FONTENCODING_GB18030:
		return WC2MB_GB18030 (buf, psz, n);
		//break;
	default:
		break;
	}
#endif //__MAD_ENCODING_EXTENDED__

	return 0;
}

#ifdef __MAD_ENCODING_EXTENDED__
size_t MadCSConv::MB2WC_GB18030 (wchar_t *out, const char *in, size_t outLen) const
{
	int ret = gb18030_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_GB18030 (char *buf, const wchar_t *psz, size_t n) const
{
	int ret = gb18030_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}
#endif //__MAD_ENCODING_EXTENDED__

