///////////////////////////////////////////////////////////////////////////////
// Name:        MadCSConv.cpp
// Description: the Encoding Converter of MadEdit
// Author:      minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/intl.h> 
#include <wx/strconv.h>
#include <wx/fontmap.h>

#include "MadCSConv.h"
#ifdef __MAD_ENCODING_EXTENDED__
#include "ExtEncodings/converters.h"

std::map<wxString, int>MadCSConv::MadExtCharSetNames;
std::map<wxString, wxString>MadCSConv::MadExtCharSetFullNames;
#endif //__MAD_ENCODING_EXTENDED__

MadCSConv::MadCSConv(const wxString &charset):m_Charset(charset.Upper())
{

	m_Encoding = wxFontMapperBase::GetEncodingFromName(charset);
	if(m_Encoding != wxFONTENCODING_SYSTEM && m_Encoding < wxFONTENCODING_MAX)
	{
		m_CSConv.reset(new wxCSConv( (wxFontEncoding)m_Encoding ));
	}
	else
	{
		m_Encoding = wxFONTENCODING_SYSTEM;
#ifdef __MAD_ENCODING_EXTENDED__
		std::map<wxString, int>::iterator it = MadExtCharSetNames.find(m_Charset);
		if(MadExtCharSetNames.end() != it)
			m_Encoding = it->second;
#endif //__MAD_ENCODING_EXTENDED__
	}
}

MadCSConv::MadCSConv(int encoding):m_Charset(_T("UNKNOWN_CHARSET")), m_Encoding(encoding)
{
	if(m_Encoding < wxFONTENCODING_MAX)
	{
		m_CSConv.reset(new wxCSConv( (wxFontEncoding)m_Encoding ));
	}
	else
	{
#ifdef __MAD_ENCODING_EXTENDED__
		std::map<wxString, int>::iterator it = MadExtCharSetNames.begin();
		for(; it != MadExtCharSetNames.end(); ++it)
			if(it->second == m_Encoding)
				break;
		if (it == MadExtCharSetNames.end())		
#endif //__MAD_ENCODING_EXTENDED__
			m_Encoding = wxFONTENCODING_SYSTEM;
	}
}

void MadCSConv::InitCharSetNames()
{
#ifdef __MAD_ENCODING_EXTENDED__
	MadExtCharSetNames[_T("GB18030")]  = MAD_FONTENCODING_GB18030;
	MadExtCharSetNames[_T("IBM1025")]  = MAD_FONTENCODING_IBM1025;
	MadExtCharSetNames[_T("IBM1047")]  = MAD_FONTENCODING_IBM1047;
	MadExtCharSetNames[_T("IBM1097")]  = MAD_FONTENCODING_IBM1097;
	MadExtCharSetNames[_T("IBM1112")]  = MAD_FONTENCODING_IBM1112;
	MadExtCharSetNames[_T("IBM1122")]  = MAD_FONTENCODING_IBM1122;
	MadExtCharSetNames[_T("IBM1123")]  = MAD_FONTENCODING_IBM1123;
	MadExtCharSetNames[_T("IBM1130")]  = MAD_FONTENCODING_IBM1130;
	MadExtCharSetNames[_T("IBM1132")]  = MAD_FONTENCODING_IBM1132;
	MadExtCharSetNames[_T("IBM1137")]  = MAD_FONTENCODING_IBM1137;
	MadExtCharSetNames[_T("IBM1140")]  = MAD_FONTENCODING_IBM1140;
	MadExtCharSetNames[_T("IBM1141")]  = MAD_FONTENCODING_IBM1141;
	MadExtCharSetNames[_T("IBM1142")]  = MAD_FONTENCODING_IBM1142;
	MadExtCharSetNames[_T("IBM1143")]  = MAD_FONTENCODING_IBM1143;
	MadExtCharSetNames[_T("IBM1144")]  = MAD_FONTENCODING_IBM1144;
	MadExtCharSetNames[_T("IBM1145")]  = MAD_FONTENCODING_IBM1145;
	MadExtCharSetNames[_T("IBM1146")]  = MAD_FONTENCODING_IBM1146;
	MadExtCharSetNames[_T("IBM1147")]  = MAD_FONTENCODING_IBM1147;
	MadExtCharSetNames[_T("IBM1148")]  = MAD_FONTENCODING_IBM1148;
	MadExtCharSetNames[_T("IBM1149")]  = MAD_FONTENCODING_IBM1149;
	MadExtCharSetNames[_T("IBM1153")]  = MAD_FONTENCODING_IBM1153;
	MadExtCharSetNames[_T("IBM1154")]  = MAD_FONTENCODING_IBM1154;
	MadExtCharSetNames[_T("IBM1155")]  = MAD_FONTENCODING_IBM1155;
	MadExtCharSetNames[_T("IBM1156")]  = MAD_FONTENCODING_IBM1156;
	MadExtCharSetNames[_T("IBM1157")]  = MAD_FONTENCODING_IBM1157;
	MadExtCharSetNames[_T("IBM1158")]  = MAD_FONTENCODING_IBM1158;
	MadExtCharSetNames[_T("IBM1160")]  = MAD_FONTENCODING_IBM1160;
	MadExtCharSetNames[_T("IBM1164")]  = MAD_FONTENCODING_IBM1164;
	MadExtCharSetNames[_T("IBM1166")]  = MAD_FONTENCODING_IBM1166;
	MadExtCharSetNames[_T("IBM12712")] = MAD_FONTENCODING_IBM12712;
	MadExtCharSetNames[_T("IBM16804")] = MAD_FONTENCODING_IBM16804;

	MadExtCharSetFullNames[_T("GB18030")]  = _("Chinese Simplified(UTF-PRC)");
	MadExtCharSetFullNames[_T("IBM1025")]  = _("IBM Cyrillic");
	MadExtCharSetFullNames[_T("IBM1047")]  = _("IBM Latin-1");
	MadExtCharSetFullNames[_T("IBM1097")]  = _("IBM Farsi Bilingual");
	MadExtCharSetFullNames[_T("IBM1112")]  = _("IBM Baltic");
	MadExtCharSetFullNames[_T("IBM1122")]  = _("IBM Estonia");
	MadExtCharSetFullNames[_T("IBM1123")]  = _("IBM Cyrillic Ukraine");
	MadExtCharSetFullNames[_T("IBM1130")]  = _("IBM Vietnamese");
	MadExtCharSetFullNames[_T("IBM1132")]  = _("IBM Lao");
	MadExtCharSetFullNames[_T("IBM1137")]  = _("IBM Devanagari");
	MadExtCharSetFullNames[_T("IBM1140")]  = _("IBM USA");
	MadExtCharSetFullNames[_T("IBM1141")]  = _("IBM Austria");
	MadExtCharSetFullNames[_T("IBM1142")]  = _("IBM Denmark");
	MadExtCharSetFullNames[_T("IBM1143")]  = _("IBM Finland");
	MadExtCharSetFullNames[_T("IBM1144")]  = _("IBM Italy");
	MadExtCharSetFullNames[_T("IBM1145")]  = _("IBM Spain");
	MadExtCharSetFullNames[_T("IBM1146")]  = _("IBM UK");
	MadExtCharSetFullNames[_T("IBM1147")]  = _("IBM France");
	MadExtCharSetFullNames[_T("IBM1148")]  = _("IBM International");
	MadExtCharSetFullNames[_T("IBM1149")]  = _("IBM Icelandic");
	MadExtCharSetFullNames[_T("IBM1153")]  = _("IBM Latin-2");
	MadExtCharSetFullNames[_T("IBM1154")]  = _("IBM Cyrillic Multilingual");
	MadExtCharSetFullNames[_T("IBM1155")]  = _("IBM Turkey");
	MadExtCharSetFullNames[_T("IBM1156")]  = _("IBM Baltic");
	MadExtCharSetFullNames[_T("IBM1157")]  = _("IBM Estonia with euro");
	MadExtCharSetFullNames[_T("IBM1158")]  = _("IBM Cyrillic Ukraine");
	MadExtCharSetFullNames[_T("IBM1160")]  = _("IBM Thai");
	MadExtCharSetFullNames[_T("IBM1164")]  = _("IBM Vietnamese with euro");
	MadExtCharSetFullNames[_T("IBM1166")]  = _("IBM Cyrillic Kazakh");
	MadExtCharSetFullNames[_T("IBM12712")] = _("IBM Hebrew");
	MadExtCharSetFullNames[_T("IBM16804")] = _("IBM Arabic");
#endif
}

size_t MadCSConv::MB2WC (wchar_t *out, const char *in, size_t outLen) const
{
	int ret = 0;
	if((m_Encoding < wxFONTENCODING_MAX) && (m_CSConv)) return m_CSConv->MB2WC(out, in, outLen);

#ifdef __MAD_ENCODING_EXTENDED__
	ucs4_t *pwc = (ucs4_t *)out;
	const unsigned char *s = (const unsigned char *)in;
	int n = (int)outLen;

	switch(m_Encoding)
	{
		case MAD_FONTENCODING_GB18030:
			ret = gb18030_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1025:
			ret = ibm1025_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1047:
			ret = ibm1047_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1097:
			ret = ibm1097_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1112:
			ret = ibm1112_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1122:
			ret = ibm1122_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1123:
			ret = ibm1123_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1130:
			ret = ibm1130_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1132:
			ret = ibm1132_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1137:
			ret = ibm1137_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1140:
			ret = ibm1140_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1141:
			ret = ibm1141_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1142:
			ret = ibm1142_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1143:
			ret = ibm1143_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1144:
			ret = ibm1144_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1145:
			ret = ibm1145_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1146:
			ret = ibm1146_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1147:
			ret = ibm1147_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1148:
			ret = ibm1148_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1149:
			ret = ibm1149_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1153:
			ret = ibm1153_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1154:
			ret = ibm1154_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1155:
			ret = ibm1155_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1156:
			ret = ibm1156_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1157:
			ret = ibm1157_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1158:
			ret = ibm1158_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1160:
			ret = ibm1160_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1164:
			ret = ibm1164_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM1166:
			ret = ibm1166_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM12712:
			ret = ibm12712_mbtowc(pwc, s, n);
			break;
		case MAD_FONTENCODING_IBM16804:
			ret = ibm16804_mbtowc(pwc, s, n);
		default:
			break;
	}

	if(ret < 0) ret = 0;
#endif //__MAD_ENCODING_EXTENDED__

	return ret;
}

size_t MadCSConv::WC2MB (char *buf, const wchar_t *psz, size_t n) const
{
	int ret = 0;
	if((m_Encoding < wxFONTENCODING_MAX) && (m_CSConv)) return m_CSConv->WC2MB(buf, psz, n);

#ifdef __MAD_ENCODING_EXTENDED__
	unsigned char *r = (unsigned char *)buf;
	ucs4_t wc = *((ucs4_t *)psz);

	switch(m_Encoding)
	{
		case MAD_FONTENCODING_GB18030:
			ret = gb18030_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1025:
			ret = ibm1025_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1047:
			ret = ibm1047_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1097:
			ret = ibm1097_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1112:
			ret = ibm1112_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1122:
			ret = ibm1122_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1123:
			ret = ibm1123_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1130:
			ret = ibm1130_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1132:
			ret = ibm1132_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1137:
			ret = ibm1137_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1140:
			ret = ibm1140_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1141:
			ret = ibm1141_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1142:
			ret = ibm1142_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1143:
			ret = ibm1143_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1144:
			ret = ibm1144_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1145:
			ret = ibm1145_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1146:
			ret = ibm1146_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1147:
			ret = ibm1147_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1148:
			ret = ibm1148_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1149:
			ret = ibm1149_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1153:
			ret = ibm1153_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1154:
			ret = ibm1154_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1155:
			ret = ibm1155_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1156:
			ret = ibm1156_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1157:
			ret = ibm1157_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1158:
			ret = ibm1158_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1160:
			ret = ibm1160_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1164:
			ret = ibm1164_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM1166:
			ret = ibm1166_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM12712:
			ret = ibm12712_wctomb(r, wc, (int)n);
			break;
		case MAD_FONTENCODING_IBM16804:
			ret = ibm16804_wctomb(r, wc, (int)n);
		default:
			break;
	}
	
	if(ret < 0) ret = 0;
#endif //__MAD_ENCODING_EXTENDED__

	return ret;
}

