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

std::map<wxString, int>MadCSConv::MadExtCharSetNames;

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
	MadExtCharSetNames[_T("GB18030")]	= MAD_FONTENCODING_GB18030;
	MadExtCharSetNames[_T("IBM1025")]	= MAD_FONTENCODING_IBM1025;
	MadExtCharSetNames[_T("IBM1047")]	= MAD_FONTENCODING_IBM1047;
	MadExtCharSetNames[_T("IBM1097")]	= MAD_FONTENCODING_IBM1097;
	MadExtCharSetNames[_T("IBM1112")]	= MAD_FONTENCODING_IBM1112;
	MadExtCharSetNames[_T("IBM1122")]	= MAD_FONTENCODING_IBM1122;
	MadExtCharSetNames[_T("IBM1123")]	= MAD_FONTENCODING_IBM1123;
	MadExtCharSetNames[_T("IBM1130")]	= MAD_FONTENCODING_IBM1130;
	MadExtCharSetNames[_T("IBM1132")]	= MAD_FONTENCODING_IBM1132;
	MadExtCharSetNames[_T("IBM1137")]	= MAD_FONTENCODING_IBM1137;
	MadExtCharSetNames[_T("IBM1140")]	= MAD_FONTENCODING_IBM1140;
	MadExtCharSetNames[_T("IBM1141")]	= MAD_FONTENCODING_IBM1141;
	MadExtCharSetNames[_T("IBM1142")]	= MAD_FONTENCODING_IBM1142;
	MadExtCharSetNames[_T("IBM1143")]	= MAD_FONTENCODING_IBM1143;
	MadExtCharSetNames[_T("IBM1144")]	= MAD_FONTENCODING_IBM1144;
	MadExtCharSetNames[_T("IBM1145")]	= MAD_FONTENCODING_IBM1145;
	MadExtCharSetNames[_T("IBM1146")]	= MAD_FONTENCODING_IBM1146;
	MadExtCharSetNames[_T("IBM1147")]	= MAD_FONTENCODING_IBM1147;
	MadExtCharSetNames[_T("IBM1148")]	= MAD_FONTENCODING_IBM1148;
	MadExtCharSetNames[_T("IBM1149")]	= MAD_FONTENCODING_IBM1149;
	MadExtCharSetNames[_T("IBM1153")]	= MAD_FONTENCODING_IBM1153;
	MadExtCharSetNames[_T("IBM1154")]	= MAD_FONTENCODING_IBM1154;
	MadExtCharSetNames[_T("IBM1155")]	= MAD_FONTENCODING_IBM1155;
	MadExtCharSetNames[_T("IBM1156")]	= MAD_FONTENCODING_IBM1156;
	MadExtCharSetNames[_T("IBM1157")]	= MAD_FONTENCODING_IBM1157;
	MadExtCharSetNames[_T("IBM1158")]	= MAD_FONTENCODING_IBM1158;
	MadExtCharSetNames[_T("IBM1160")]	= MAD_FONTENCODING_IBM1160;
	MadExtCharSetNames[_T("IBM1164")]	= MAD_FONTENCODING_IBM1164;
	MadExtCharSetNames[_T("IBM1166")]	= MAD_FONTENCODING_IBM1166;
	MadExtCharSetNames[_T("IBM12712")] = MAD_FONTENCODING_IBM12712;
	MadExtCharSetNames[_T("IBM16804")] = MAD_FONTENCODING_IBM16804;
}

size_t MadCSConv::MB2WC (wchar_t *out, const char *in, size_t outLen) const
{
	if((m_Encoding < wxFONTENCODING_MAX) && (m_CSConv)) return m_CSConv->MB2WC(out, in, outLen);

#ifdef __MAD_ENCODING_EXTENDED__
	switch(m_Encoding)
	{
	case MAD_FONTENCODING_GB18030:
		return MB2WC_GB18030(out, in, outLen);
		//break;
	case MAD_FONTENCODING_IBM1025:
		return MB2WC_IBM1025(out, in, outLen);
	case MAD_FONTENCODING_IBM1047:
		return MB2WC_IBM1047(out, in, outLen);
	case MAD_FONTENCODING_IBM1097:
		return MB2WC_IBM1097(out, in, outLen);
	case MAD_FONTENCODING_IBM1112:
		return MB2WC_IBM1112(out, in, outLen);
	case MAD_FONTENCODING_IBM1122:
		return MB2WC_IBM1122(out, in, outLen);
	case MAD_FONTENCODING_IBM1123:
		return MB2WC_IBM1123(out, in, outLen);
	case MAD_FONTENCODING_IBM1130:
		return MB2WC_IBM1130(out, in, outLen);
	case MAD_FONTENCODING_IBM1132:
		return MB2WC_IBM1132(out, in, outLen);
	case MAD_FONTENCODING_IBM1137:
		return MB2WC_IBM1137(out, in, outLen);
	case MAD_FONTENCODING_IBM1140:
		return MB2WC_IBM1140(out, in, outLen);
	case MAD_FONTENCODING_IBM1141:
		return MB2WC_IBM1141(out, in, outLen);
	case MAD_FONTENCODING_IBM1142:
		return MB2WC_IBM1142(out, in, outLen);
	case MAD_FONTENCODING_IBM1143:
		return MB2WC_IBM1143(out, in, outLen);
	case MAD_FONTENCODING_IBM1144:
		return MB2WC_IBM1144(out, in, outLen);
	case MAD_FONTENCODING_IBM1145:
		return MB2WC_IBM1145(out, in, outLen);
	case MAD_FONTENCODING_IBM1146:
		return MB2WC_IBM1146(out, in, outLen);
	case MAD_FONTENCODING_IBM1147:
		return MB2WC_IBM1147(out, in, outLen);
	case MAD_FONTENCODING_IBM1148:
		return MB2WC_IBM1148(out, in, outLen);
	case MAD_FONTENCODING_IBM1149:
		return MB2WC_IBM1149(out, in, outLen);
	case MAD_FONTENCODING_IBM1153:
		return MB2WC_IBM1153(out, in, outLen);
	case MAD_FONTENCODING_IBM1154:
		return MB2WC_IBM1154(out, in, outLen);
	case MAD_FONTENCODING_IBM1155:
		return MB2WC_IBM1155(out, in, outLen);
	case MAD_FONTENCODING_IBM1156:
		return MB2WC_IBM1156(out, in, outLen);
	case MAD_FONTENCODING_IBM1157:
		return MB2WC_IBM1157(out, in, outLen);
	case MAD_FONTENCODING_IBM1158:
		return MB2WC_IBM1158(out, in, outLen);
	case MAD_FONTENCODING_IBM1160:
		return MB2WC_IBM1160(out, in, outLen);
	case MAD_FONTENCODING_IBM1164:
		return MB2WC_IBM1164(out, in, outLen);
	case MAD_FONTENCODING_IBM1166:
		return MB2WC_IBM1166(out, in, outLen);
	case MAD_FONTENCODING_IBM12712:
		return MB2WC_IBM12712(out, in, outLen);
	case MAD_FONTENCODING_IBM16804:
		return MB2WC_IBM16804(out, in, outLen);
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
	case MAD_FONTENCODING_IBM1025:
		return WC2MB_IBM1025(buf, psz, n);
	case MAD_FONTENCODING_IBM1047:
		return WC2MB_IBM1047(buf, psz, n);
	case MAD_FONTENCODING_IBM1097:
		return WC2MB_IBM1097(buf, psz, n);
	case MAD_FONTENCODING_IBM1112:
		return WC2MB_IBM1112(buf, psz, n);
	case MAD_FONTENCODING_IBM1122:
		return WC2MB_IBM1122(buf, psz, n);
	case MAD_FONTENCODING_IBM1123:
		return WC2MB_IBM1123(buf, psz, n);
	case MAD_FONTENCODING_IBM1130:
		return WC2MB_IBM1130(buf, psz, n);
	case MAD_FONTENCODING_IBM1132:
		return WC2MB_IBM1132(buf, psz, n);
	case MAD_FONTENCODING_IBM1137:
		return WC2MB_IBM1137(buf, psz, n);
	case MAD_FONTENCODING_IBM1140:
		return WC2MB_IBM1140(buf, psz, n);
	case MAD_FONTENCODING_IBM1141:
		return WC2MB_IBM1141(buf, psz, n);
	case MAD_FONTENCODING_IBM1142:
		return WC2MB_IBM1142(buf, psz, n);
	case MAD_FONTENCODING_IBM1143:
		return WC2MB_IBM1143(buf, psz, n);
	case MAD_FONTENCODING_IBM1144:
		return WC2MB_IBM1144(buf, psz, n);
	case MAD_FONTENCODING_IBM1145:
		return WC2MB_IBM1145(buf, psz, n);
	case MAD_FONTENCODING_IBM1146:
		return WC2MB_IBM1146(buf, psz, n);
	case MAD_FONTENCODING_IBM1147:
		return WC2MB_IBM1147(buf, psz, n);
	case MAD_FONTENCODING_IBM1148:
		return WC2MB_IBM1148(buf, psz, n);
	case MAD_FONTENCODING_IBM1149:
		return WC2MB_IBM1149(buf, psz, n);
	case MAD_FONTENCODING_IBM1153:
		return WC2MB_IBM1153(buf, psz, n);
	case MAD_FONTENCODING_IBM1154:
		return WC2MB_IBM1154(buf, psz, n);
	case MAD_FONTENCODING_IBM1155:
		return WC2MB_IBM1155(buf, psz, n);
	case MAD_FONTENCODING_IBM1156:
		return WC2MB_IBM1156(buf, psz, n);
	case MAD_FONTENCODING_IBM1157:
		return WC2MB_IBM1157(buf, psz, n);
	case MAD_FONTENCODING_IBM1158:
		return WC2MB_IBM1158(buf, psz, n);
	case MAD_FONTENCODING_IBM1160:
		return WC2MB_IBM1160(buf, psz, n);
	case MAD_FONTENCODING_IBM1164:
		return WC2MB_IBM1164(buf, psz, n);
	case MAD_FONTENCODING_IBM1166:
		return WC2MB_IBM1166(buf, psz, n);
	case MAD_FONTENCODING_IBM12712:
		return WC2MB_IBM12712(buf, psz, n);
	case MAD_FONTENCODING_IBM16804:
		return WC2MB_IBM16804(buf, psz, n);
	default:
		break;
	}
#endif //__MAD_ENCODING_EXTENDED__

	return 0;
}

#ifdef __MAD_ENCODING_EXTENDED__
size_t MadCSConv::MB2WC_GB18030(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = gb18030_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_GB18030(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = gb18030_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1025(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1025_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1025(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1025_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1047(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1047_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1047(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1047_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1097(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1097_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1097(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1097_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1112(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1112_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1112(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1112_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1122(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1122_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1122(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1122_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1123(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1123_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1123(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1123_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1130(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1130_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1130(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1130_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1132(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1132_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1132(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1132_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1137(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1137_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1137(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1137_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1140(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1140_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1140(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1140_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1141(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1141_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1141(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1141_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1142(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1142_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1142(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1142_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1143(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1143_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1143(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1143_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1144(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1144_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1144(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1144_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1145(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1145_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1145(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1145_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1146(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1146_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1146(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1146_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1147(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1147_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1147(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1147_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1148(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1148_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1148(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1148_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1149(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1149_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1149(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1149_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1153(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1153_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1153(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1153_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1154(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1154_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1154(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1154_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1155(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1155_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1155(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1155_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1156(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1156_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1156(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1156_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1157(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1157_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1157(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1157_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1158(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1158_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1158(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1158_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1160(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1160_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1160(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1160_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1164(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1164_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1164(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1164_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM1166(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm1166_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM1166(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm1166_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM12712(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm12712_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM12712(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm12712_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::MB2WC_IBM16804(wchar_t *out, const char *in, size_t outLen) const
{
	int ret = ibm16804_mbtowc ((ucs4_t *)out, (const unsigned char *)in, (int)outLen);
	if(ret < 0) ret = 0;
	return ret;
}

size_t MadCSConv::WC2MB_IBM16804(char *buf, const wchar_t *psz, size_t n) const
{
	int ret = ibm16804_wctomb ((unsigned char *)buf, *((ucs4_t *)psz), (int) n);
	if(ret < 0) ret = 0;
	return ret;
}
#endif //__MAD_ENCODING_EXTENDED__

