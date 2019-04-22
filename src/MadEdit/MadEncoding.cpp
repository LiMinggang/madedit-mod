///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEncoding.cpp
// Description: define the Encodings which are supported by MadEdit
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <set>
#include <algorithm>
#include <wx/config.h>
#include "MadEncoding.h"
#include "uchardet.h"
#include "MadCSConv.h"

using std::vector;
using std::map;
using std::set;

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif
// Pure C instead of Map of Map, but map with MadEncodingGrp
// ISO_8859_1(NA)  ==> (ENCG_ISO8859)&&(ENCG_WESTERNEUROPE))
// ISO_8859_2  ==> (ENCG_ISO8859)&&(ENCG_CENTRALEUROPE))
// ISO_8859_3  ==> (ENCG_ISO8859)&&(ENCG_SOUTHEUROPE))
// ISO_8859_4  ==> (ENCG_ISO8859)&&(ENCG_NORTHEUROPE))
// ISO_8859_5  ==> (ENCG_ISO8859)&&(ENCG_CYRILLIC))
// ISO_8859_6  ==> (ENCG_ISO8859)&&(ENCG_ARABIC))
// ISO_8859_7  ==> (ENCG_ISO8859)&&(ENCG_GREEK))
// ISO_8859_8  ==> (ENCG_ISO8859)&&(ENCG_HEBREW))
// ISO_8859_9  ==> (ENCG_ISO8859)&&(ENCG_TURKISH))
// ISO_8859_10 ==> (ENCG_ISO8859)&&(ENCG_NORTHEUROPE))
// ISO_8859_11 ==> (ENCG_ISO8859)&&(ENCG_SOUTHEASTASIA))
// ISO_8859_13 ==> (ENCG_ISO8859)&&(ENCG_BALTIC))
// ISO_8859_14 ==> (ENCG_ISO8859)&&(ENCG_NORTHEUROPE))
// ISO_8859_15 ==> (ENCG_ISO8859)&&(ENCG_WESTERNEUROPE))
// ISO_8859_16(NA) ==> (ENCG_ISO8859)&&(ENCG_SOUTHEUROPE))
// Windows
// Windows_874  ==> (ENCG_WINDOWS)&&(ENCG_SOUTHEASTASIA))
// Windows_1250 ==> (ENCG_WINDOWS)&&(ENCG_CENTRALEUROPE))
// Windows_1251 ==> (ENCG_WINDOWS)&&(ENCG_CYRILLIC))
// Windows_1252 ==> (ENCG_WINDOWS)&&(ENCG_WESTERNEUROPE))
// Windows_1253 ==> (ENCG_WINDOWS)&&(ENCG_GREEK))
// Windows_1254 ==> (ENCG_WINDOWS)&&(ENCG_TURKISH))
// Windows_1255 ==> (ENCG_WINDOWS)&&(ENCG_HEBREW))
// Windows_1256 ==> (ENCG_WINDOWS)&&(ENCG_ARABIC))
// Windows_1257 ==> (ENCG_WINDOWS)&&(ENCG_BALTIC))
// Windows_1258(NA) ==> (ENCG_WINDOWS)&&(ENCG_SOUTHEASTASIA))
// OEM
// CP437 ==> (ENCG_OEM))
// CP850(NA) ==> (ENCG_OEM))
// CP852(NA) ==> (ENCG_OEM))
// CP855(NA) ==> (ENCG_OEM)&&(ENCG_CYRILLIC))
// CP866(NA) ==> (ENCG_OEM)&&(ENCG_CYRILLIC))
// Cyrillic
// KOI8_R(NA) ==> (ENCG_CYRILLIC))
// KOI8_U(NA) ==> (ENCG_CYRILLIC))
// Windows & East Asia
// MS932 ==> (ENCG_WINDOWS)&&(ENCG_EASTASIA))
// MS936 ==> (ENCG_WINDOWS)&&(ENCG_EASTASIA))
// MS949 ==> (ENCG_WINDOWS)&&(ENCG_EASTASIA))
// MS950 ==> (ENCG_WINDOWS)&&(ENCG_EASTASIA))
// CP20932(NA) ==> (ENCG_WINDOWS)&&(ENCG_EASTASIA))
// Unicode
// GB18030(NA) ==> (ENCG_UNICODE)&&(ENCG_EASTASIA))
// UTF_8    ==> (ENCG_UNICODE))
// UTF_16LE ==> (ENCG_UNICODE))
// UTF_16BE ==> (ENCG_UNICODE))
// UTF_32LE ==> (ENCG_UNICODE))
// UTF_32BE ==> (ENCG_UNICODE))

static vector<MadEncodingInfo> EncodingsTable;
map<int, wxString>MadEncoding::MadEncodingGrpName;
set<int> EncodingsSet;

class EncSort  
{  
public:  
  
	// Return whether first element is less than the second  
    bool operator () (const MadEncodingInfo& a, const MadEncodingInfo& b) const  
	{  
		 return (a.m_Name.Cmp(b.m_Name) < 0);  
	};  
};  
  

wxChar TestEncoding( const wxChar *name, wxFontEncoding enc, wxByte *mb )
{
#if defined(__WXGTK__)
	wxCSConv conv( name );
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
	name;
	wxCSConv conv( enc );
#endif
	wxChar wcs[4] = {0};
	conv.MB2WC( wcs, ( char* )mb, 4 );
	return wcs[0];
}
#ifdef PYMADEDIT_DLL
	#ifdef __WXMSW__
		wxString g_MadEditRegkeyPath = wxT( "HKEY_CURRENT_USER\\Software\\Classes\\" );
	#endif
#endif

#ifdef __WXMSW__
#define MSW_GET_FONT_NAME(cp,fn) MSW_GetFontName(cp,fn)
extern wxString g_MadEditRegkeyPath;

// get fontname from registry mime database
void MSW_GetFontName( wxChar *codepage, wxString &fontname )
{
	const wxString MIMEDB( g_MadEditRegkeyPath + wxT( "MIME\\Database\\Codepage\\" ) );
	wxRegKey *pRegKey = new wxRegKey( MIMEDB + codepage );

	if( !pRegKey->Exists() )
	{
		delete pRegKey;
		return;
	}

	long cp;

	if( pRegKey->QueryValue( wxT( "Family" ), &cp ) )
	{
		delete pRegKey;
		pRegKey = new wxRegKey( MIMEDB + wxString::Format( wxT( "%s" ), ( wxLongLong( cp ).ToString() ).c_str() ) );
	}

	pRegKey->QueryValue( wxT( "FixedWidthFont" ), fontname );
	delete pRegKey;
}

#else
#define MSW_GET_FONT_NAME(cp,fn)
#endif

size_t MadEncoding::ms_SystemEncodingIndex = 0;
MadEncoding *MadEncoding::ms_SystemEncoding = nullptr;

void MadEncoding::InitEncodings()
{
	wxLogNull nolog;
	MadEncodingGrpName[ENCG_WESTERNEUROPE] = _( "Western European" );
	MadEncodingGrpName[ENCG_CENTRALEUROPE] = _( "Central European" );
	MadEncodingGrpName[ENCG_SOUTHEUROPE  ] = _( "South European" );
	MadEncodingGrpName[ENCG_NORTHEUROPE  ] = _( "North European" );
	MadEncodingGrpName[ENCG_CYRILLIC     ] = _( "Cyrillic" );
	MadEncodingGrpName[ENCG_ARABIC       ] = _( "Arabic" );
	MadEncodingGrpName[ENCG_GREEK        ] = _( "Greek" );
	MadEncodingGrpName[ENCG_HEBREW       ] = _( "Hebrew" );
	MadEncodingGrpName[ENCG_TURKISH      ] = _( "Turkish" );
	MadEncodingGrpName[ENCG_BALTIC       ] = _( "Baltic" );
	MadEncodingGrpName[ENCG_EASTASIA     ] = _( "East Asian" );
	MadEncodingGrpName[ENCG_SOUTHEASTASIA] = _( "Southeast Asian" );
	MadEncodingGrpName[ENCG_UNICODE      ] = _( "Unicode" );
	MadEncodingGrpName[ENCG_ISO8859      ] = _( "ISO-8859" );
	MadEncodingGrpName[ENCG_WINDOWS      ] = _( "Windows" );
	MadEncodingGrpName[ENCG_MACINTOSH    ] = _( "Macintosh" );
	MadEncodingGrpName[ENCG_OEM          ] = _( "OEM" );
	MadEncodingGrpName[ENCG_DEFAULT      ] = _( "Other" );
	MadEncodingGrpName[ENCG_MAX          ] = _( "Invalid" );
	wxFontEncoding sysenc = wxLocale::GetSystemEncoding();
	size_t count = wxFontMapper::GetSupportedEncodingsCount();
	std::vector<int> encGrp;
#ifdef __WXMSW__
	wxString fontname( wxT( "Courier New" ) );
#elif defined(__APPLE__) && defined(__MACH__)
	wxString fontname( wxT( "Monaco" ) );
#else
	wxString fontname( wxT( "Monospace" ) );
#endif

	wxConfigBase *cfg = wxConfigBase::Get( false );
	cfg->Read( wxT( "/MadEdit/DefaultTextFont" ), &fontname );

	for( size_t idx = 0; idx < count; ++idx )
	{
		wxFontEncoding enc = wxFontMapper::GetEncoding( idx );
		wxString name = wxFontMapper::GetEncodingName( enc );
		wxString desc = wxFontMapper::GetEncodingDescription( enc );
		MadEncodingType type = etSingleByte;
		bool ignore = false;
		bool dotest = true;    // test the encoding if is supported in this machine
		wxChar testwc = 0;
		wxByte testmb[8] = {0};
		encGrp.clear();

		switch( enc )
		{
		case wxFONTENCODING_CP932:          // Japanese (shift-JIS)
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "SHIFT-JIS" );
					type = etDoubleByte;
					testwc = 0x3000;
					testmb[0] = 0x81;
					testmb[1] = 0x40;
					MSW_GET_FONT_NAME( wxT( "932" ), fontname );
					added = true;
					encGrp.push_back( ENCG_OEM );
					encGrp.push_back( ENCG_EASTASIA );
				}
				else
				{
					dotest = false;
					ignore = true;
				}
			}
			break;

		case wxFONTENCODING_CP936:          // Chinese simplified (GB)
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "GBK" );
					type = etDoubleByte;
					testwc = 0x3000;
					testmb[0] = 0xA1;
					testmb[1] = 0xA1;
					MSW_GET_FONT_NAME( wxT( "936" ), fontname );
					added = true;
					encGrp.push_back( ENCG_OEM );
					encGrp.push_back( ENCG_EASTASIA );
				}
				else
				{
					dotest = false;
					ignore = true;
				}
			}
			break;

		case wxFONTENCODING_CP949:          // Korean (Hangul charset)
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "EUC-KR" );
					type = etDoubleByte;
					testwc = 0x3000;
					testmb[0] = 0xA1;
					testmb[1] = 0xA1;
					MSW_GET_FONT_NAME( wxT( "949" ), fontname );
					added = true;
					encGrp.push_back( ENCG_OEM );
					encGrp.push_back( ENCG_EASTASIA );
				}
				else
				{
					dotest = false;
					ignore = true;
				}
			}
			break;

		case wxFONTENCODING_CP950:          // Chinese (traditional - Big5)
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "BIG5" );
					type = etDoubleByte;
					testwc = 0x3000;
					testmb[0] = 0xA1;
					testmb[1] = 0x40;
					MSW_GET_FONT_NAME( wxT( "950" ), fontname );
					added = true;
					encGrp.push_back( ENCG_OEM );
					encGrp.push_back( ENCG_EASTASIA );
				}
				else
				{
					dotest = false;
					ignore = true;
				}
			}
			break;

		case wxFONTENCODING_EUC_JP:         // Extended Unix Codepage for Japanese
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "EUC-JP" );
					type = etDoubleByte;
					testwc = 0x3000;
					testmb[0] = 0xA1;
					testmb[1] = 0xA1;
					MSW_GET_FONT_NAME( wxT( "51932" ), fontname );
					added = true;
					encGrp.push_back( ENCG_WINDOWS );
					encGrp.push_back( ENCG_EASTASIA );
				}
				else
				{
					dotest = false;
					ignore = true;
				}
			}
			break;

		case wxFONTENCODING_CP437:
			name = wxT( "CP437" );
			testwc = 0xA0;
			testmb[0] = 0xFF;
			encGrp.push_back( ENCG_OEM );
			break;

		case wxFONTENCODING_UTF7:    // not support
			ignore = true;
			dotest = false;
			//encGrp.push_back(ENCG_UNICODE);
			break;

		case wxFONTENCODING_UTF8:
			type = etUTF8;
			dotest = false;
			encGrp.push_back( ENCG_UNICODE );
			break;

		case wxFONTENCODING_UTF16BE:
			type = etUTF16BE;
			dotest = false;
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "utf-16be" );
					desc = wxT( "Unicode 16 bit Big Endian (UTF-16BE)" );
					added = true;
					encGrp.push_back( ENCG_UNICODE );
				}
				else ignore = true;
			}
			break;

		case wxFONTENCODING_UTF16LE:
			type = etUTF16LE;
			dotest = false;
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "utf-16le" );
					desc = wxT( "Unicode 16 bit Little Endian (UTF-16LE)" );
					added = true;
					encGrp.push_back( ENCG_UNICODE );
				}
				else ignore = true;
			}
			break;

		case wxFONTENCODING_UTF32BE:
			type = etUTF32BE;
			dotest = false;
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "utf-32be" );
					desc = wxT( "Unicode 32 bit Big Endian (UTF-32BE)" );
					added = true;
					encGrp.push_back( ENCG_UNICODE );
				}
				else ignore = true;
			}
			break;

		case wxFONTENCODING_UTF32LE:
			type = etUTF32LE;
			dotest = false;
			{
				static bool added = false;

				if( !added )
				{
					name = wxT( "utf-32le" );
					desc = wxT( "Unicode 32 bit Little Endian (UTF-32LE)" );
					added = true;
					encGrp.push_back( ENCG_UNICODE );
				}
				else ignore = true;
			}
			break;

		case wxFONTENCODING_KOI8:
			name = wxT( "KOI8-R" );
			testwc = 0x2553;
			testmb[0] = 0xA4;
			encGrp.push_back( ENCG_CYRILLIC );
			break;

		case wxFONTENCODING_KOI8_U:
			name = wxT( "KOI8-U" );
			testwc = 0x0454;
			testmb[0] = 0xA4;
			encGrp.push_back( ENCG_CYRILLIC );
			break;

		case wxFONTENCODING_CP866:
			name = wxT( "CP866" );
			testwc = 0x0434;
			testmb[0] = 0xA4;
			encGrp.push_back( ENCG_OEM );
			encGrp.push_back( ENCG_CYRILLIC );
			break;

		case wxFONTENCODING_CP855:
			name = wxT( "CP855" );
			testwc = 0x0446;
			testmb[0] = 0xA4;
			encGrp.push_back( ENCG_OEM );
			encGrp.push_back( ENCG_CYRILLIC );
			break;

		case wxFONTENCODING_CP874:
			name = wxT( "CP874" );
			testwc = 0x0E5B;
			testmb[0] = 0xFB;
			encGrp.push_back( ENCG_OEM );
			encGrp.push_back( ENCG_SOUTHEASTASIA );
			break;

		default:
			if( enc >= wxFONTENCODING_ISO8859_1 && enc <= wxFONTENCODING_ISO8859_15 )
			{
				static wxChar wctable[] = {0x00A1, 0x0102, 0x0108, 0x0100, 0x00a7, 0x060c, 0x03ce, 0x00d7, 0x011e, 0x0100, 0x0e01,    0, 0x00c6, 0x010a, 0x0152};
				static wxByte mbtable[] = {0xA1,   0xc3,   0xc6,   0xc0,   0xfd,   0xac,   0xfe,   0xaa,   0xd0,   0xc0,   0xa1,      0, 0xaf,   0xa4,   0xbc};
				testwc    = wctable[enc - wxFONTENCODING_ISO8859_1];
				testmb[0] = mbtable[enc - wxFONTENCODING_ISO8859_1];
				encGrp.push_back( ENCG_ISO8859 );

				switch( enc )
				{
				case wxFONTENCODING_ISO8859_1:
					encGrp.push_back( ENCG_WESTERNEUROPE );
					break;

				case wxFONTENCODING_ISO8859_2:
					encGrp.push_back( ENCG_CENTRALEUROPE );
					break;

				case wxFONTENCODING_ISO8859_3:
					encGrp.push_back( ENCG_SOUTHEUROPE );
					break;

				case wxFONTENCODING_ISO8859_4:
					encGrp.push_back( ENCG_NORTHEUROPE );
					break;

				case wxFONTENCODING_ISO8859_5:
					encGrp.push_back( ENCG_CYRILLIC );
					break;

				case wxFONTENCODING_ISO8859_6:
					encGrp.push_back( ENCG_ARABIC );
					break;

				case wxFONTENCODING_ISO8859_7:
					encGrp.push_back( ENCG_GREEK );
					break;

				case wxFONTENCODING_ISO8859_8:
					encGrp.push_back( ENCG_HEBREW );
					break;

				case wxFONTENCODING_ISO8859_9:
					encGrp.push_back( ENCG_TURKISH );
					break;

				case wxFONTENCODING_ISO8859_10:
					encGrp.push_back( ENCG_NORTHEUROPE );
					break;

				case wxFONTENCODING_ISO8859_11:
					encGrp.push_back( ENCG_SOUTHEASTASIA );
					break;

				case wxFONTENCODING_ISO8859_13:
					encGrp.push_back( ENCG_BALTIC );
					break;

				case wxFONTENCODING_ISO8859_14:
					encGrp.push_back( ENCG_NORTHEUROPE );
					break;

				case wxFONTENCODING_ISO8859_15:
					encGrp.push_back( ENCG_WESTERNEUROPE );
					break;

				default:// Not support yet
					break;
				}
			}
			else
				if( enc >= wxFONTENCODING_CP1250 && enc <= wxFONTENCODING_CP1258 )
				{
					static wxChar wctable[] = {0x0102, 0x0401, 0x0152, 0x0192, 0x011e, 0x00d7, 0x0152, 0x00a8, 0x20AB};
					static wxByte mbtable[] = {0xc3,   0xa8,   0x8c,   0x83,   0xd0,   0xaa,   0x8c,   0x8d,   0xFE};
					testwc    = wctable[enc - wxFONTENCODING_CP1250];
					testmb[0] = mbtable[enc - wxFONTENCODING_CP1250];
					encGrp.push_back( ENCG_WINDOWS );

					switch( enc )
					{
					case wxFONTENCODING_CP1250:
						encGrp.push_back( ENCG_CENTRALEUROPE );
						break;

					case wxFONTENCODING_CP1251:
						encGrp.push_back( ENCG_CYRILLIC );
						break;

					case wxFONTENCODING_CP1252:
						encGrp.push_back( ENCG_WESTERNEUROPE );
						break;

					case wxFONTENCODING_CP1253:
						encGrp.push_back( ENCG_GREEK );
						break;

					case wxFONTENCODING_CP1254:
						encGrp.push_back( ENCG_TURKISH );
						break;

					case wxFONTENCODING_CP1255:
						encGrp.push_back( ENCG_HEBREW );
						break;

					case wxFONTENCODING_CP1256:
						encGrp.push_back( ENCG_ARABIC );
						break;

					case wxFONTENCODING_CP1257:
						encGrp.push_back( ENCG_BALTIC );
						break;

					case wxFONTENCODING_CP1258:
						encGrp.push_back( ENCG_SOUTHEASTASIA );
						break;

					default:
						break;
					}
				}
				else
					if( name == wxT( "windows-874" ) )
					{
						testwc = 0x0e50;
						testmb[0] = 0xf0;
						MSW_GET_FONT_NAME( wxT( "874" ), fontname );
						encGrp.push_back( ENCG_OEM );
						encGrp.push_back( ENCG_WINDOWS );
						encGrp.push_back( ENCG_SOUTHEASTASIA );
					}
					else if( enc >= wxFONTENCODING_MACMIN && enc <= wxFONTENCODING_MACMAX )
					{
						encGrp.push_back( ENCG_MACINTOSH );

						switch( enc )
						{
							case wxFONTENCODING_MACROMAN:
								testwc    = 0x00B0;
								testmb[0] = 0xA1;
								break;
							case wxFONTENCODING_MACJAPANESE:
								type = etDoubleByte;
								testwc    = 0x4E9C;
								testmb[0] = 0x88;
								testmb[1] = 0x9F;
								encGrp.push_back( ENCG_EASTASIA );
								break;
							case wxFONTENCODING_MACCHINESETRAD:
								type = etDoubleByte;
								testwc    = 0x3000;
								testmb[0] = 0xA1;
								testmb[1] = 0x40;
								encGrp.push_back( ENCG_EASTASIA );
								break;
							case wxFONTENCODING_MACKOREAN:
								type = etDoubleByte;
								testwc    = 0x3000;
								testmb[0] = 0xA1;
								testmb[1] = 0xA1;
								encGrp.push_back( ENCG_EASTASIA );
								break;
							case wxFONTENCODING_MACCHINESESIMP:
								type = etDoubleByte;
								testwc    = 0x3000;
								testmb[0] = 0xA1;
								testmb[1] = 0xA1;
								encGrp.push_back( ENCG_EASTASIA );
								break;
							case wxFONTENCODING_MACARABIC:
								encGrp.push_back( ENCG_ARABIC );
								testwc    = 0x0621;
								testmb[0] = 0xC1;
								break;
							case wxFONTENCODING_MACHEBREW:
								encGrp.push_back( ENCG_HEBREW );
								testwc    = 0x00C4;
								testmb[0] = 0x80;
								break;
							case wxFONTENCODING_MACGREEK:
								encGrp.push_back( ENCG_GREEK );
								testwc    = 0x0393;
								testmb[0] = 0xA1;
								break;
							case wxFONTENCODING_MACCYRILLIC:
								encGrp.push_back( ENCG_CYRILLIC );
								testwc    = 0x042F;
								testmb[0] = 0x9F;
								break;
							case wxFONTENCODING_MACTHAI:
								encGrp.push_back( ENCG_SOUTHEASTASIA );
								testwc    = 0x0E01;
								testmb[0] = 0xA1;
								break;
							case wxFONTENCODING_MACCENTRALEUR:
								encGrp.push_back( ENCG_CENTRALEUROPE );
								testwc    = 0x012B;
								testmb[0] = 0xB4;
								break;
							case wxFONTENCODING_MACCROATIAN:
								encGrp.push_back( ENCG_SOUTHEUROPE );
								testwc    = 0x25CA;
								testmb[0] = 0xD7;
								break;
							case wxFONTENCODING_MACICELANDIC:
								encGrp.push_back( ENCG_NORTHEUROPE );
								testwc    = 0x2014;
								testmb[0] = 0xD1;
								break;
							case wxFONTENCODING_MACROMANIAN:
								encGrp.push_back( ENCG_SOUTHEUROPE );
								testwc    = 0x2014;
								testmb[0] = 0xD1;
								break;

							default:
								break;
						}
					}
					else
						if((enc >= wxFONTENCODING_CP437) && ((enc < wxFONTENCODING_CP12_MAX)))
						{
							encGrp.push_back( ENCG_OEM );
						}
						else
							if( name == wxT( "default" ) || name == wxGetTranslation( wxT( "default" ) ) ) // unnecessary
							{
								ignore = true;
								dotest = false;
							}
		}

		// test the encoding is supported in the system
		if( dotest )
		{
			if( testwc != 0 )
			{
				ignore = ( testwc != TestEncoding( name.c_str(), enc, testmb ) );
			}

#ifdef __WXMSW__
			else
			{
				ignore = ! wxFontMapper::Get()->IsEncodingAvailable( enc );
			}
#endif
		}

		if( !ignore )
		{
			EncodingsTable.push_back( MadEncodingInfo( enc, name.Upper(), desc, type, fontname, encGrp ) );
			EncodingsSet.insert(enc);
		}
	}

#ifdef __MAD_ENCODING_EXTENDED__
	{
		encGrp.clear();
		wxString name = wxT( "GB18030" );
//		wxString fontname( wxT( "Courier New" ) );
		wxString desc(_("Chinese Simplified(UTF-PRC)"));
		MadEncodingType type = etGB18030;
		MSW_GET_FONT_NAME( wxT( "54936" ), fontname );
		encGrp.push_back( ENCG_EASTASIA );
		EncodingsTable.push_back( MadEncodingInfo( MAD_FONTENCODING_GB18030, name.Upper(), desc, type, fontname, encGrp ) );
		EncodingsSet.insert(MAD_FONTENCODING_GB18030);
	}
#endif //__MAD_ENCODING_EXTENDED__

	std::sort(EncodingsTable.begin(), EncodingsTable.end(), EncSort());

	for( size_t idx = 0; idx < EncodingsTable.size(); ++idx )
	{
		if( EncodingsTable[idx].m_Encoding == sysenc )
		{
			ms_SystemEncodingIndex = idx;
			break;
		}
	}
}

void MadEncoding::FreeEncodings()
{
	if( ms_SystemEncoding != nullptr )
	{
		delete ms_SystemEncoding;
	}

	vector<MadEncodingInfo>::iterator it = EncodingsTable.begin();
	vector<MadEncodingInfo>::iterator itend = EncodingsTable.end();

	while( it != itend )
	{
		if( it->m_CSConv ) delete it->m_CSConv;

		if( it->m_MBtoWC_Table ) delete []it->m_MBtoWC_Table;

		if( it->m_WCtoMB_Table ) delete []it->m_WCtoMB_Table;

		if( it->m_LeadByte_Table ) delete []it->m_LeadByte_Table;

		++it;
	}
}

void MadEncoding::UpdateEncodingDefaultFont(const wxString & font)
{
	vector<MadEncodingInfo>::iterator it = EncodingsTable.begin();
	vector<MadEncodingInfo>::iterator itend = EncodingsTable.end();

	while( it != itend )
	{
		it->m_FontName = font;
		++it;
	}
}

size_t MadEncoding::GetEncodingsCount()
{
	return EncodingsTable.size();
}

wxString MadEncoding::GetEncodingName( size_t idx )
{
	wxASSERT( idx < EncodingsTable.size() );
	//return wxFontMapper::GetEncodingName( EncodingsTable[idx].m_Encoding );
	return EncodingsTable[idx].m_Name;
}

wxString MadEncoding::GetEncodingDescription( size_t idx )
{
	wxASSERT( idx < EncodingsTable.size() );
	//return wxFontMapper::GetEncodingDescription( EncodingsTable[idx].m_Encoding );
	return EncodingsTable[idx].m_Description;
}

wxString MadEncoding::GetEncodingFontName( size_t idx )
{
	wxASSERT( idx < EncodingsTable.size() );
	return EncodingsTable[idx].m_FontName;
}

const std::vector<int>& MadEncoding::GetEncodingGrps( size_t idx )
{
	wxASSERT( idx < EncodingsTable.size() );
	return EncodingsTable[idx].m_GrpIdVec;
}

wxString MadEncoding::EncodingToName( int enc )
{
	size_t idx;

	for( idx = 0; idx < EncodingsTable.size(); ++idx )
	{
		if( EncodingsTable[idx].m_Encoding == enc )
		{
			return EncodingsTable[idx].m_Name;
		}
	}

	return EncodingsTable[ms_SystemEncodingIndex].m_Name;
}

int MadEncoding::NameToEncoding( const wxString &name )
{
	size_t idx;
	wxString uname( name.Upper() );

	for( idx = 0; idx < EncodingsTable.size(); ++idx )
	{
		if( EncodingsTable[idx].m_Name == uname )
		{
			return EncodingsTable[idx].m_Encoding;
		}
	}

	return EncodingsTable[ms_SystemEncodingIndex].m_Encoding;
}

MadEncoding *MadEncoding::GetSystemEncoding()
{
	if( ms_SystemEncoding == nullptr )
	{
		ms_SystemEncoding = new MadEncoding( ms_SystemEncodingIndex );
	}

	return ms_SystemEncoding;
}

MadEncoding::MadEncoding( size_t idx ) : m_Info(nullptr), m_CSConv(nullptr), m_MBtoWC_Table(nullptr), m_WCtoMB_Table(nullptr), m_LeadByte_Table(nullptr)
{
	Create( idx );
}

MadEncoding::MadEncoding( int enc ) : m_Info(nullptr), m_CSConv(nullptr), m_MBtoWC_Table(nullptr), m_WCtoMB_Table(nullptr), m_LeadByte_Table(nullptr)
{
	size_t idx;

	for( idx = 0; idx < EncodingsTable.size(); ++idx )
	{
		if( EncodingsTable[idx].m_Encoding == enc )
		{
			Create( idx );
			break;
		}
	}

	if( idx == EncodingsTable.size() )
	{
		Create( ms_SystemEncodingIndex );
	}
}

MadEncoding::MadEncoding( const wxString &name ) : m_Info(nullptr), m_CSConv(nullptr), m_MBtoWC_Table(nullptr), m_WCtoMB_Table(nullptr), m_LeadByte_Table(nullptr)
{
	size_t idx;
	wxString uname( name.Upper() );

	for( idx = 0; idx < EncodingsTable.size(); ++idx )
	{
		if( EncodingsTable[idx].m_Name == uname )
		{
			Create( idx );
			break;
		}
	}

	if( idx == EncodingsTable.size() )
	{
		Create( ms_SystemEncodingIndex );
	}
}

void MadEncoding::Create( size_t idx )
{
	wxASSERT( idx < EncodingsTable.size() );
	m_Info = &EncodingsTable[idx];
	m_CSConv = m_Info->m_CSConv;

	if( m_CSConv != nullptr )
	{
		m_MBtoWC_Table = m_Info->m_MBtoWC_Table;
		m_WCtoMB_Table = m_Info->m_WCtoMB_Table;
		m_LeadByte_Table = m_Info->m_LeadByte_Table;
	}
	else
	{
#if defined(__WXGTK__)
		m_CSConv = new MadCSConv( m_Info->m_Name.c_str() );
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
		m_CSConv = new MadCSConv( m_Info->m_Encoding );
#endif
		m_Info->m_CSConv = m_CSConv;

		switch( m_Info->m_Type )
		{
		case etSingleByte:
			m_MBtoWC_Table = new ucs2_t[256];
			memset( m_MBtoWC_Table, 0, sizeof( ucs2_t ) * 256 );
			m_WCtoMB_Table = new wxWord[65536];
			memset( m_WCtoMB_Table, 0, sizeof( wxWord ) * 65536 );
			{
				// cache the results of Single-Byte <==> Wide-Char
				wxByte singlebyte[2] = {0, 0};
				wchar_t wc[2];

				for( wxWord i = 0; i < 256; ++i )
				{
					singlebyte[0] = i;

					if( m_CSConv->MB2WC( wc, ( char* )singlebyte, 2 ) == 1 )
					{
						m_MBtoWC_Table[i] = wc[0];
					}
					else
					{
						m_MBtoWC_Table[i] = i;
						wc[0] = i;
					}

					m_WCtoMB_Table[ wc[0] ] = ( i << 8 ) ;
				}
			}
			m_Info->m_MBtoWC_Table = m_MBtoWC_Table;
			m_Info->m_WCtoMB_Table = m_WCtoMB_Table;
			break;

		case etDoubleByte:
			m_MBtoWC_Table = new ucs2_t[65536];
			// value: 0x0000, indicate the column isn't a valid Double-Byte char
			memset( m_MBtoWC_Table, 0, sizeof( ucs2_t ) * 256 );
			// value: 0xFFFF, indicate the column is non-cached
			memset( &m_MBtoWC_Table[256], 0xFF, sizeof( ucs2_t ) * ( 65536 - 256 ) );
			m_WCtoMB_Table = new wxWord[65536];
			memset( m_WCtoMB_Table, 0xFF, sizeof( wxWord ) * 65536 );
			m_LeadByte_Table = new wxByte[256];
			memset( m_LeadByte_Table, 0, 256 );
			m_LeadByte_Table[0] = 0xFF;
			m_Info->m_MBtoWC_Table = m_MBtoWC_Table;
			m_Info->m_WCtoMB_Table = m_WCtoMB_Table;
			m_Info->m_LeadByte_Table = m_LeadByte_Table;
			break;

		default:
			break;
		}
	}

	// set member function pointer: UCS4toMultiByte
	switch( m_Info->m_Type )
	{
	case etUTF8:
		UCS4toMultiByte = &MadEncoding::UCS4toUTF8;
		break;

	case etUTF16LE:
		UCS4toMultiByte = &MadEncoding::UCS4toUTF16LE;
		break;

	case etUTF16BE:
		UCS4toMultiByte = &MadEncoding::UCS4toUTF16BE;
		break;

	case etUTF32LE:
		UCS4toMultiByte = &MadEncoding::UCS4toUTF32LE;
		break;

	case etUTF32BE:
		UCS4toMultiByte = &MadEncoding::UCS4toUTF32BE;
		break;

#ifdef __MAD_ENCODING_EXTENDED__
	case etGB18030:
		UCS4toMultiByte = &MadEncoding::UCS4toGB18030;
		break;
#endif
	default:
		//case etSingleByte:
		//case etDoubleByte:
		UCS4toMultiByte = &MadEncoding::UCS4toMB;
		break;
	}
}

MadEncoding::~MadEncoding()
{
}


size_t MadEncoding::UCS4toMB( ucs4_t ucs4, wxByte *buf )
{
	if( ucs4 > 0xFFFF )
		return 0;

	wxWord mb = m_WCtoMB_Table[ucs4];

	if( mb == 0 )    // invalid MB char
	{
		return 0;
	}

	size_t len;

	if( mb == 0xFFFF ) // non-cached
	{
		wxByte mbs[4];
		wchar_t wc[2] = {wchar_t( ucs4 ), 0};
		len = m_CSConv->WC2MB( ( char* )mbs, wc, 4 );

		if( len == 0 || len == ( size_t ) - 1 )
		{
			m_WCtoMB_Table[ucs4] = 0;
			return 0;
		}

		wxASSERT( len <= 2 );
		//mbs[len]=0;
		mb = ( ( ( wxWord )mbs[0] ) << 8 ) | mbs[1];
		m_WCtoMB_Table[ucs4] = mb;
		buf[0] = mbs[0];

		if( len == 2 ) buf[1] = mbs[1];

		return len;
	}

	buf[0] = mb >> 8;
	len = 1;

	if( ( mb & 0xFF ) != 0 )
	{
		buf[1] = mb & 0xFF;
		++len;//len=2
	}

	return len;
}

size_t MadEncoding::UCS4toUTF8( ucs4_t ucs4, wxByte *buf )
{
	/***  from rfc3629
	Char. number range  |        UTF-8 octet sequence
	(hexadecimal)       |              (binary)
	--------------------+---------------------------------------------
	0000 0000-0000 007F | 0xxxxxxx
	0000 0080-0000 07FF | 110xxxxx 10xxxxxx
	0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
	0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

	UTF8-octets = *( UTF8-char )
	UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
	UTF8-1    = %x00-7F
	UTF8-2    = %xC2-DF UTF8-tail
	UTF8-3    = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
	              %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
	UTF8-4    = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
	              %xF4 %x80-8F 2( UTF8-tail )
	UTF8-tail   = %x80-BF
	***/
	if( ucs4 < 0 )
		return 0;

	if( ucs4 <= 0x7F )
	{
		buf[0] = ucs4;
		return 1;
	}

	if( ucs4 <= 0x7FF )
	{
		buf[0] = 0xC0 | ( ucs4 >> 6 );
		buf[1] = 0x80 | ( ucs4 & 0x3F );
		return 2;
	}

	if( ucs4 <= 0xFFFF )
	{
		buf[0] = 0xE0 | ( ucs4 >> 12 );
		buf[1] = 0x80 | ( ( ucs4 >> 6 ) & 0x3F );
		buf[2] = 0x80 | ( ucs4 & 0x3F );
		return 3;
	}

	if( ucs4 <= 0x10FFFF )
	{
		buf[0] = 0xF0 | ( ucs4 >> 18 );
		buf[1] = 0x80 | ( ( ucs4 >> 12 ) & 0x3F );
		buf[2] = 0x80 | ( ( ucs4 >> 6 ) & 0x3F );
		buf[3] = 0x80 | ( ucs4 & 0x3F );
		return 4;
	}

	return 0;
}

size_t MadEncoding::UCS4toUTF16LE_U10000( ucs4_t ucs4, wxByte *buf )
{
	//ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
	//if(ucs4>0x10FFFF) return 0;
	//wxASSERT(ucs4>=0x10000 && ucs4<=0x10FFFF);
	ucs4 -= 0x10000;
	ucs2_t high = ( ucs4 >> 10 ) + 0xD800; // high surrogate
	ucs2_t low = ( ucs4 & 0x3FF ) + 0xDC00; // low surrogate
	buf[0] = high;
	buf[1] = high >> 8;
	buf[2] = low;
	buf[3] = low >> 8;
	return 4;
}

size_t MadEncoding::UCS4toUTF16LE( ucs4_t ucs4, wxByte *buf )
{
	if( ucs4 >= 0x10000 ) // to unicode surrogates
	{
		if( ucs4 > 0x10FFFF ) return 0;

		return UCS4toUTF16LE_U10000( ucs4, buf );
	}

	buf[0] = ucs4;
	buf[1] = ucs4 >> 8;
	return 2;
}
size_t MadEncoding::UCS4toUTF16BE( ucs4_t ucs4, wxByte *buf )
{
	if( ucs4 >= 0x10000 ) // to unicode surrogates
	{
		if( ucs4 > 0x10FFFF ) return 0;

		ucs4 -= 0x10000;
		ucs2_t high = ( ucs4 >> 10 ) + 0xD800; // high surrogate
		ucs2_t low = ( ucs4 & 0x3FF ) + 0xDC00; // low surrogate
		buf[0] = high >> 8;
		buf[1] = high;
		buf[2] = low >> 8;
		buf[3] = low;
		return 4;
	}

	buf[0] = ucs4 >> 8;
	buf[1] = ucs4;
	return 2;
}

size_t MadEncoding::UCS4toUTF32LE( ucs4_t ucs4, wxByte *buf )
{
	wxASSERT( ucs4 >= 0 && ucs4 <= 0x10FFFF );
#if wxBYTE_ORDER == wxBIG_ENDIAN
	wxByte *p = ( wxByte* )&ucs4;
	buf[0] = p[3];
	buf[1] = p[2];
	buf[2] = p[1];
	buf[3] = p[0];
#else
	*( ( ucs4_t* )buf ) = ucs4;
#endif
	return 4;
}
size_t MadEncoding::UCS4toUTF32BE( ucs4_t ucs4, wxByte *buf )
{
	wxASSERT( ucs4 >= 0 && ucs4 <= 0x10FFFF );
#if wxBYTE_ORDER == wxBIG_ENDIAN
	*( ( ucs4_t* )buf ) = ucs4;
#else
	wxByte *p = ( wxByte* )&ucs4;
	buf[0] = p[3];
	buf[1] = p[2];
	buf[2] = p[1];
	buf[3] = p[0];
#endif
	return 4;
}

size_t MadEncoding::UCS4toGB18030(ucs4_t ucs4, wxByte *buf)
{
	return m_CSConv->WC2MB( ( char* )buf, (wchar_t *)(&ucs4), 4 );
}

ucs4_t MadEncoding::SBtoUCS4( wxByte b1 )    // Single-Byte to UCS4
{
	return m_MBtoWC_Table[ b1 ];
}

// return 0 if it is not a valid DB char
ucs4_t MadEncoding::DBtoUCS4( wxByte *buf )  // Double-Byte to UCS4
{
	if( m_LeadByte_Table[ buf[0] ] == 0 )
		IsLeadByte( buf[0] );

	__REGISTER unsigned int w = ( ( ( unsigned int )buf[0] ) << 8 ) | buf[1];
	return m_MBtoWC_Table[w];
}

ucs4_t MadEncoding::GB18030toUCS4(wxByte *buf, size_t &len)
{
	ucs4_t ucs4 = 0;
	len = m_CSConv->MB2WC ((wchar_t *)(&ucs4), (const char *)buf, 4);
	return ucs4;
}

bool MadEncoding::IsLeadByte( wxByte byte )
{
	if( m_LeadByte_Table[byte] == 0 )
	{
		wxWord db = ( ( wxWord )byte ) << 8 ;
		wxByte dbs[4] = {byte, 0, 0, 0};
		wchar_t wc[4];

		// check first byte
		if( m_CSConv->MB2WC( wc, ( char* )dbs, 4 ) == 1 )
		{
			m_MBtoWC_Table[ db ] = wc[0];
			m_WCtoMB_Table[ wc[0] ] = db;
		}
		else
		{
			m_MBtoWC_Table[ db ] = 0;
		}

		++db;
		++dbs[1];

		for( int i = 1; i <= 0xFF; ++i, ++db, ++dbs[1] )
		{
			if( m_CSConv->MB2WC( wc, ( char* )dbs, 4 ) == 1 )
			{
				m_MBtoWC_Table[ db ]    = wc[0];
				m_WCtoMB_Table[ wc[0] ] = db;
				m_LeadByte_Table[byte]  = 1;       // is lead-byte
			}
			else
			{
				m_MBtoWC_Table[ db ] = 0;
			}
		}

		if( m_LeadByte_Table[byte] == 0 )
		{
			m_LeadByte_Table[byte] = 0xFF;
			return false;
		}
	}

	return m_LeadByte_Table[byte] == 1;
}

//==================================================

bool IsTextUTF32LE( wxByte *text, int size )
{
	// check BOM
	if( size >= 4 && text[0] == 0xFF && text[1] == 0xFE && text[2] == 0 && text[3] == 0 )
		return true;

	size >>= 2; // to count

	if( size < 4 ) return false;

	ucs4_t ucs4, *p = ( ucs4_t * )text;

	for( int i = 0; i < size; ++i, ++p )
	{
		ucs4 = wxINT32_SWAP_ON_BE( *p );

		if( ucs4 <= 0 || ucs4 > 0x10FFFF ) return false;
	}

	return true;
}

bool IsTextUTF32BE( wxByte *text, int size )
{
	// check BOM
	if( size >= 4 && text[0] == 0 && text[1] == 0 && text[2] == 0xFE && text[3] == 0xFF )
		return true;

	size >>= 2; // to count

	if( size < 4 ) return false;

	ucs4_t ucs4, *p = ( ucs4_t * )text;

	for( int i = 0; i < size; ++i, ++p )
	{
		ucs4 = wxINT32_SWAP_ON_LE( *p );

		if( ucs4 <= 0 || ucs4 > 0x10FFFF ) return false;
	}

	return true;
}


bool IsTextUTF16LE( wxByte *text, int size )
{
	if( size >= 2 )
	{
		if( text[0] == 0xFF && text[1] == 0xFE ) // check BOM
		{
			if( size >= 4 && text[2] == 0 && text[3] == 0 ) // utf32le BOM
			{
				return false;
			}

			return true;
		}

		if( text[1] == 0xFF && text[0] == 0xFE )    // big-endian
			return false;

		bool ok = false;
		bool highsurrogate = false;
		size = size & 0x1FFFFFFE;   // to even

		while( size > 0 )
		{
			if( text[1] == 0 )
			{
				if( text[0] == 0 )
					return false;

				ok = true;
			}
			else
				if( text[1] >= 0xD8 && text[1] <= 0xDB )
				{
					if( highsurrogate )
						return false;

					highsurrogate = true;
				}
				else
					if( text[1] >= 0xDC && text[1] <= 0xDF )
					{
						if( highsurrogate == false )
							return false;

						highsurrogate = false;
					}

			size -= 2;
			text += 2;
		}

		return ok;
	}

	return false;
}

bool IsTextUTF16BE( wxByte *text, int size )
{
	if( size >= 2 && text[0] == 0xFE && text[1] == 0xFF )
		return true;

	bool ok = false;
	bool highsurrogate = false;
	size = size & 0x1FFFFFFE;     // to even

	while( size > 0 )
	{
		if( text[0] == 0 )
		{
			if( text[1] == 0 )
				return false;

			ok = true;
		}
		else
			if( text[0] >= 0xD8 && text[0] <= 0xDB )
			{
				if( highsurrogate )
					return false;

				highsurrogate = true;
			}
			else
				if( text[0] >= 0xDC && text[0] <= 0xDF )
				{
					if( highsurrogate == false )
						return false;

					highsurrogate = false;
				}

		size -= 2;
		text += 2;
	}

	return ok;
}

inline bool IsUTF8Tail( wxByte b )
{
	return ( b & 0xC0 ) == 0x80;
}
bool IsTextUTF8( wxByte *text, int size )
{
	//check BOM
	if( size >= 3 && text[0] == 0xEF && text[1] == 0xBB && text[2] == 0xBF )
		return true;

	int i = 0;
	int ok_count = 0;

	while( i < size )
	{
		if( text[i] < 0x80 )        // 1 byte
		{
			if( text[i] == 0 )
				return false;

			++i;
		}
		else
			if( text[i] <= 0xDF )
			{
				if( ++i < size && IsUTF8Tail( text[i] ) ) // 2 bytes
				{
					++i;
					++ok_count;
				}
				else
					if( size != i )
					{
						return false;
					}
			}
			else
				if( text[i] <= 0xEF )
				{
					if( ( ++i < size && IsUTF8Tail( text[i] ) ) // 3 bytes
							&& ( ++i < size && IsUTF8Tail( text[i] ) ) )
					{
						++i;
						++ok_count;
					}
					else
						if( size != i )
						{
							return false;
						}
				}
				else
					if( text[i] <= 0xF4 )
					{
						if( ( ++i < size && IsUTF8Tail( text[i] ) ) // 4 bytes
								&& ( ++i < size && IsUTF8Tail( text[i] ) ) //
								&& ( ++i < size && IsUTF8Tail( text[i] ) ) )
						{
							++i;
							++ok_count;
						}
						else
							if( size != i )
							{
								return false;
							}
					}
					//Extra check from http://codeblocks.sourcearchive.com/documentation/8.02-0ubuntu4/encodingdetector_8cpp-source.html
					else
						if( text[i] <= 0xFB ) // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
						{
							if( ( ++i < size && IsUTF8Tail( text[i] ) )
									&& ( ++i < size && IsUTF8Tail( text[i] ) )
									&& ( ++i < size && IsUTF8Tail( text[i] ) )
									&& ( ++i < size && IsUTF8Tail( text[i] ) ) )
							{
								return true;
							}
							else
								if( size != i )
								{
									return false;
								}
						}
						else
						{
							return false;
						}
	}

	return ok_count > 0;
}

bool IsBinaryData( wxByte *data, int size )
{
	int i = 0;
	wxByte b;

	while( i < size )
	{
		b = data[i];

		if( b == 0 )
			return true;

		++i;
	}

	return false;
}

void DetectChineseEncoding( const wxByte *text, int count, int &enc )
{
	// detect by punctuation marks
	int i = 0;
	int b0 = 0, b1 = 0, c = 0;
	int cp950 = 0;
	int cp936 = 0;

	while( i++ < count )
	{
		b0 = b1;
		b1 = c;
		c = *text++;

		if( c < 0x40 )
		{
			if( b0 == 0xa1 )
			{
				switch( b1 )
				{
				case 0x43:
				case 0x48:
				case 0x49:
				case 0x4b:
				case 0x58:
				case 0x5e:
				case 0x60:
				case 0x62:
				case 0x64:
				case 0x6a:
				case 0x6c:
				case 0x6e:
				case 0x70:
				case 0x72:
				case 0x76:
				case 0x7a:
				case 0xa2:
				case 0xa4:
				case 0xa6:
				case 0xa8:
				case 0xaa:
				case 0xac:
					++cp950;
					break;

				case 0xa3:
				case 0xad:
				case 0xaf:
				case 0xb1:
				case 0xb5:
				case 0xb7:
				case 0xb9:
				case 0xbb:
				case 0xbf:
				case 0xe4:
					++cp936;
					break;
				}
			}
			else
				if( b0 == 0xa3 )
				{
					if( b1 >= 0x74 && b1 <= 0x7E )
					{
						++cp950;
					}
					else
					{
						switch( b1 )
						{
						case 0xa8:
						case 0xaa:
						case 0xab:
						case 0xad:
						case 0xaf:
						case 0xbd:
						case 0xbe:
							++cp950;
							break;

						case 0xa1:
						case 0xa9:
						case 0xac:
						case 0xfd:
							++cp936;
							break;
						}
					}
				}
				else
					if( b1 <= 0xA0 && b1 >= 0x80 )
					{
						if( b0 >= 0x81 && b0 <= 0xFE )
						{
#ifdef __MAD_ENCODING_EXTENDED__
							if(EncodingsSet.find(wxFONTENCODING_CP936) != EncodingsSet.end())
#endif
								enc = wxFONTENCODING_CP936; // [0x81~0xFE][0x80~0xA0] are invalid in big5
#ifdef __MAD_ENCODING_EXTENDED__
							else
								enc = MAD_FONTENCODING_GB18030; // [0x81~0xFE][0x30~0x39] are invalid in big5 and GBK
#endif
							return;
						}
					}
					else if((b1 <= 0x39) && (b1 >= 0x30) && (b0 >= 0x81) && (b0 <= 0xFE))
						{
#ifdef __MAD_ENCODING_EXTENDED__
							enc = MAD_FONTENCODING_GB18030; // [0x81~0xFE][0x30~0x39] are invalid in big5 and GBK
#else
							enc = wxFONTENCODING_CP936; // [0x81~0xFE][0x80~0xA0] are invalid in big5
#endif
							return;
						}
					else
					{
						int w = ( b0 << 8 ) + b1;

						switch( w )
						{
						case 0xa6e1:
						case 0xa6e7:
						case 0xa6ef:
						case 0xa6f1:
						case 0xa895:
						case 0xa979:
						case 0xa97b:
						case 0xa9b4:
						case 0xa9bc:
							++cp936;
							break;

						case 0xa27b:
						case 0xa27d:
							++cp950;
							break;
						}
					}
		}
	}

	if( cp950 > cp936 ) enc = wxFONTENCODING_CP950;
	else
		if( cp936 > cp950 ) enc = wxFONTENCODING_CP936;
}

void DetectJapaneseEncoding( const wxByte *text, int count, int &enc )
{
	wxByte c;
	int i = 0;
	wxFontEncoding xenc = wxFontEncoding( 0 );

	while( xenc == 0 && i++ < count )
	{
		c = *text++;

		if( c >= 0x81 && c <= 0x9F )
		{
			if( c == 0x8E ) /* SS2 */
			{
				if( i++ >= count ) break;

				c = *text++;

				if( ( c >= 0x40 && c <= 0xA0 ) || ( c >= 0xE0 && c <= 0xFC ) )
					xenc = wxFONTENCODING_CP932;
			}
			else
				if( c == 0x8F ) /* SS3 */
				{
					if( i++ >= count ) break;

					c = *text++;

					if( c >= 0x40 && c <= 0xA0 )
						xenc = wxFONTENCODING_CP932;
					else
						if( c >= 0xFD )
							break;
				}
				else
					xenc = wxFONTENCODING_CP932;
		}
		else
			if( c >= 0xA1 && c <= 0xDF )
			{
				if( i++ >= count ) break;

				c = *text++;

				if( c <= 0x9F )
					xenc = wxFONTENCODING_CP932;
				else
					if( c >= 0xFD )
						break;
			}
			else
				if( c >= 0xE0 && c <= 0xEF )
				{
					if( i++ >= count ) break;

					c = *text++;

					if( c >= 0x40 && c <= 0xA0 )
						xenc = wxFONTENCODING_CP932;
					else
						if( c >= 0xFD )
							break;
				}
				else
					if( c >= 0xF0 )
						break;
	}

	if( xenc != 0 )
		enc = xenc;
}

void DetectEncoding( const wxByte *text, int count, int &enc )
{
	uchardet_t det = nullptr;
	det = uchardet_new();
	uchardet_handle_data( det, ( const char* )text, count );
	uchardet_data_end( det );
	wxString name( uchardet_get_charset( det ), wxConvLocal ), rest;
	uchardet_delete( det );
	name.MakeUpper();
	long value;

	if( name.StartsWith( wxT( "ISO-8859-" ), &rest ) )
	{
		if( rest.ToLong( &value ) )
		{
			enc = wxFontEncoding( long( wxFONTENCODING_ISO8859_1 ) - 1 + value );
		}
	}
	else
		if( name.StartsWith( wxT( "WINDOWS-125" ), &rest ) )
		{
			if( rest.ToLong( &value ) )
			{
				if( value == 2 ) //1252 or ?
				{
					int def = wxFONTENCODING_DEFAULT;

					if( enc == wxFONTENCODING_CP950 || enc == wxFONTENCODING_CP936 )
					{
						DetectChineseEncoding( text, count, def );

						if( def != wxFONTENCODING_DEFAULT )
						{
							value = -1;
							enc = def;
						}
					}
				}

				if( value >= 0 )
				{
					enc = wxFontEncoding( long( wxFONTENCODING_CP1250 ) + value );
				}
			}
		}
		else
			if( name.StartsWith( wxT( "UTF-" ), &rest ) )
			{
				if( rest[0] == wxT( '8' ) )
				{
					enc = wxFONTENCODING_UTF8;
				}
				else
					if( rest[0] == wxT( '1' ) ) // 16BE/LE
					{
						if( rest[2] == wxT( 'B' ) ) enc = wxFONTENCODING_UTF16BE;
						else                    enc = wxFONTENCODING_UTF16LE;
					}
					else // 32BE/LE
					{
						if( rest[2] == wxT( 'B' ) ) enc = wxFONTENCODING_UTF32BE;
						else                    enc = wxFONTENCODING_UTF32LE;
					}
			}
			else
				if( name.IsSameAs( wxT( "BIG5" ) ) )
				{
					enc = wxFONTENCODING_CP950;
				}
				else
					if( name.IsSameAs( wxT( "GB2312" ) )
							|| name.IsSameAs( wxT( "HZ-GB-2312" ) ) )
					{
						enc = wxFONTENCODING_CP936;
#ifdef __MAD_ENCODING_EXTENDED__
						if(EncodingsSet.find(enc) == EncodingsSet.end())
						{
							enc = MAD_FONTENCODING_GB18030;
						}
#endif
					}
					else
#ifdef __MAD_ENCODING_EXTENDED__
						if(name.IsSameAs( wxT( "GB18030" )))
							enc = MAD_FONTENCODING_GB18030;
						else
#endif
							if( name.IsSameAs( wxT( "SHIFT_JIS" ) ) )
							{
								enc = wxFONTENCODING_CP932;
							}
							else
								if( name.IsSameAs( wxT( "EUC-JP" ) ) )
								{
									enc = wxFONTENCODING_EUC_JP;
								}
								else
									if( name.IsSameAs( wxT( "EUC-KR" ) ) )
									{
										enc = wxFONTENCODING_CP949;
									}
									else
										if( name.IsSameAs( wxT( "KOI8-R" ) ) )
										{
											enc = wxFONTENCODING_KOI8;
										}
										else
											if( name.IsSameAs( wxT( "IBM866" ) ) )
											{
												enc = wxFONTENCODING_CP866;
											}
}
