///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadSyntax.cpp
// Description: syntax parsing and syntax file management
// Author:      madedit@gmail.com
// Maintainer:  minggang.li@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadSyntax.h"
#include "MadEdit.h"
#include "MadEncoding.h"

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/filename.h>

#include "PersonalDictionary.h"

#ifdef _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern wxString g_MadEditHomeDir;

const wxChar *SystemAttributesName[] =
{
	wxT( "Text" ), wxT( "Delimiter" ), wxT( "Space" ), wxT( "Number" ), wxT( "String" ), wxT( "Comment" ),
	wxT( "Directive" ), wxT( "SpecialWord" ), wxT( "LineNumber" ), wxT( "ActiveLine" ), wxT( "Bookmark" ),
	wxT("HighlightWord"),
};

const wxChar *SystemAttributesColor[] =
{
	wxT( "Black" ), wxT( "SaddleBrown" ), wxT( "Aqua" ), wxT( "Blue" ), wxT( "Red" ), wxT( "Teal" ), wxT( "Green" ), wxT( "Maroon" ), wxT( "White" ),       wxT( "#\x46\x46\x46" ), wxT("DeepSkyBlue"), wxT("PaleGreen")
};
const wxChar *SystemAttributesBgColor[] =
{
	wxT( "White" ), wxT( "" ),           wxT( "" ),     wxT( "" ),     wxT( "" ),    wxT( "" ),     wxT( "" ),      wxT( "" ),      wxT( "#\xA0\xA0\xA0" ), wxT( "" ),        wxT("MediumBlue"), wxT("AntiqueWhite")
};

const wxString MadPlainTextTitle( wxT( "Plain Text" ) );

vector<wxString> g_SynPaths;

typedef deque< pair<wxString, wxString> > StringPairTable;
typedef deque< pair<wxString, wxString> >::iterator StringPairIterator;

// synfile of g_TitleSynfileTable is full path
StringPairTable g_TitleSynfileTable;

// syntax scheme files table (filename[*], fullpath)
StringPairTable g_NameSchfileTable;

// syntax attributs in syntax files
std::map< wxString, shared_ptr< MadSyntaxAttributes > > g_TitleSynAttrMap;

WX_DECLARE_HASH_MAP( wxString, wxString, wxStringHash, wxStringEqual, StringMap );

// synfile of below is filename only
StringMap g_ExtSynfileMap;
StringMap g_FirstlineSynfileMap;
StringMap g_FilenameSynfileMap;


void LoadListFile( const wxString &listfile, StringMap &map, bool noCase )
{
	wxTextFile list;

	if( wxFileExists( listfile ) && list.Open( listfile ) && !list.Eof() )
	{
		wxString str = list.GetFirstLine(), tok1, tok2;

		for( ;; )
		{
			str.Trim( false );

			if( !str.IsEmpty() && str[0] != '#' ) // not a comment line
			{
				wxStringTokenizer tkz( str );
				tok1 = tkz.GetNextToken();
				tok2 = tkz.GetNextToken();

				if( !tok1.IsEmpty() && !tok2.IsEmpty() )
				{
					if( noCase )
					{
						tok1.MakeUpper();
					}

					StringMap::iterator it = map.find( tok1 );

					if( it == map.end() )
					{
						map.insert( StringMap::value_type( tok1, tok2 ) );
					}
				}
			}

			if( list.Eof() )
				break;

			str = list.GetNextLine();
		}
	}
}

wxString MadSyntax::s_AttributeFilePath;
bool MadSyntax::s_Loaded = false;

void MadSyntax::LoadSyntaxFiles()
{
	for( size_t i = 0; i < g_SynPaths.size(); ++i )
	{
		wxDir dir( g_SynPaths[i] );
		wxString filename;
		bool cont = dir.GetFirst( &filename, wxT( "syntax_*.syn" ), wxDIR_FILES );

		while( cont )
		{
			//wxLogMessage(filename);
			filename = g_SynPaths[i] + filename;
			wxFileConfig syn( wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS );
			wxString entry, title;
			long idx = 0;
			bool econt = syn.GetNextEntry( entry, idx );

			while( econt )
			{
				syn.Read( entry, &title );
				entry.MakeLower();
				title.Trim( true );
				title.Trim( false );

				if( entry == wxT( "title" ) )
				{
					if( title == MadPlainTextTitle )
					{
						// "Plain Text" synfile must be first item of g_TitleSynfileTable
						title = wxGetTranslation(title);
						if( g_TitleSynfileTable.empty() || g_TitleSynfileTable[0].first != title)
						{
							g_TitleSynfileTable.insert( g_TitleSynfileTable.begin(), pair<wxString, wxString>(title, filename ) );
						}
					}
					else
					{
						StringPairIterator it = g_TitleSynfileTable.begin();
						StringPairIterator itend = g_TitleSynfileTable.end();
						bool insert = true;

						if( it != itend )
						{
							if( g_TitleSynfileTable[0].first == wxGetTranslation(MadPlainTextTitle) )
							{
								++it;
							}

							while( it != itend )
							{
								int cmp = it->first.CmpNoCase( title );

								if( cmp == 0 )
								{
									insert = false;
									break;
								}

								if( cmp > 0 )
								{
									break;
								}

								++it;
							}
						}

						if( insert )
						{
							g_TitleSynfileTable.insert( it, pair<wxString, wxString>( title, filename ) );
						}
					}

					break;
				}

				econt = syn.GetNextEntry( entry, idx );
			}

			cont = dir.GetNext( &filename );
		}

		// load syntax scheme files
		cont = dir.GetFirst( &filename, wxT( "*.sch" ), wxDIR_FILES );
		static int front = 0, back = 0;

		while( cont )
		{
			filename = g_SynPaths[i] + filename;
			wxFileName fn( filename );
			wxString name = fn.GetName();
			StringPairIterator it = g_NameSchfileTable.begin() + front;
#ifdef __WXMSW__

			if( filename.Upper().Find( s_AttributeFilePath.Upper().c_str() ) < 0 )
#else
			if( filename.Find( s_AttributeFilePath.c_str() ) < 0 )
#endif
			{
				name += wxT( '*' );
				++front;
			}
			else
			{
				it += back;
				++back;
			}

			g_NameSchfileTable.insert( it, pair<wxString, wxString>( name, filename ) );
			cont = dir.GetNext( &filename );
		}

		LoadListFile( g_SynPaths[i] + wxT( "extension.list" ), g_ExtSynfileMap, true );
		LoadListFile( g_SynPaths[i] + wxT( "firstline.list" ), g_FirstlineSynfileMap, false );
		LoadListFile( g_SynPaths[i] + wxT( "filename.list" ),  g_FilenameSynfileMap, true );
	}

	// make sure "Plain Text" was added, otherwise add it
	if( g_TitleSynfileTable.empty() || g_TitleSynfileTable[0].first != wxGetTranslation(MadPlainTextTitle) )
	{
		g_TitleSynfileTable.insert( g_TitleSynfileTable.begin(), pair<wxString, wxString>(wxGetTranslation(MadPlainTextTitle), wxEmptyString ) );
	}

	// add "Default*" to the g_NameSchfileTable
	g_NameSchfileTable.push_front( pair<wxString, wxString>( wxT( "Default*" ), wxEmptyString ) );
	s_Loaded = true;
}

void MadSyntax::AddSyntaxFilesPath( const wxString &path )
{
	if( wxDirExists( path ) ) g_SynPaths.push_back( path );
}

size_t MadSyntax::GetSyntaxCount()
{
	if( !s_Loaded ) LoadSyntaxFiles();

	return g_TitleSynfileTable.size();
}

wxString MadSyntax::GetSyntaxTitle( size_t index )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	if( index < g_TitleSynfileTable.size() )
	{
		return g_TitleSynfileTable[index].first;
	}

	return wxEmptyString;
}

wxString MadSyntax::GetSyntaxFile( size_t index )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	if( index < g_TitleSynfileTable.size() )
	{
		return g_TitleSynfileTable[index].second;
	}

	return wxEmptyString;
}

wxString MadSyntax::GetSyntaxFileByTitle( const wxString &title )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	for( size_t i = 0; i < g_TitleSynfileTable.size(); ++i )
	{
		if( g_TitleSynfileTable[i].first == title )
		{
			return g_TitleSynfileTable[i].second;
		}
	}

	return wxEmptyString;
}

wxString MadSyntax::GetAttributeFileByTitle( const wxString &title )
{
	wxString file = GetSyntaxFileByTitle( title );

	if( file.IsEmpty() )
	{
		file = s_AttributeFilePath + wxT( "temp.att" );
	}
	else
	{
		wxFileName fn( file );
		file = s_AttributeFilePath + fn.GetName() + wxT( ".att" );
	}

	return file;
}

MadSyntax* MadSyntax::GetSyntaxByTitle( const wxString &title )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	wxString synfile = GetSyntaxFileByTitle( title );

	if( wxFileExists( synfile ) )
	{
		return new MadSyntax( synfile );
	}

	return new MadSyntax(); // return a default syntax
}

MadSyntax* MadSyntax::GetSyntaxByExt( const wxString &ext )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	wxString synfile;
	StringMap::iterator it = g_ExtSynfileMap.find( ext.Upper() );

	if( it != g_ExtSynfileMap.end() )
	{
		synfile = it->second;
	}

	if( !synfile.IsEmpty() )
	{
		for( size_t idx = 0; idx < g_SynPaths.size(); ++idx )
		{
			if( wxFileExists( g_SynPaths[idx] + synfile ) )
			{
				return new MadSyntax( g_SynPaths[idx] + synfile );
			}
		}
	}

	return nullptr;
}

MadSyntax* MadSyntax::GetSyntaxByFirstLine( wxByte *data, int size )
{
	wxString line;
	wxChar ch(0);

	// get first non-empty line
	do
	{
		while( size > 0 && ( ch = wxChar( *data ) ) != 0x0D && ch != 0x0A && ch != 0 )
		{
			line << ch;
			++data;
			--size;
		}

		++data;
		--size;
		line.Trim( false );
	}
	while( line.IsEmpty() && size > 0 && ch != 0 );

	wxString synfile;

	if( !line.IsEmpty() && g_FirstlineSynfileMap.size() != 0 )
	{
		StringMap::iterator it = g_FirstlineSynfileMap.begin();

		do
		{
			if( line.Find( it->first.c_str() ) >= 0 )
			{
				synfile = it->second;
				break;
			}
		}
		while( ++it != g_FirstlineSynfileMap.end() );
	}

	if( !synfile.IsEmpty() )
	{
		for( size_t idx = 0; idx < g_SynPaths.size(); ++idx )
		{
			if( wxFileExists( g_SynPaths[idx] + synfile ) )
			{
				return new MadSyntax( g_SynPaths[idx] + synfile );
			}
		}
	}

	return nullptr;
}

MadSyntax* MadSyntax::GetSyntaxByFileName( const wxString &filename )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	wxString synfile;
	StringMap::iterator it = g_FilenameSynfileMap.find( filename.Upper() );

	if( it != g_FilenameSynfileMap.end() )
	{
		synfile = it->second;
	}

	if( !synfile.IsEmpty() )
	{
		for( size_t idx = 0; idx < g_SynPaths.size(); ++idx )
		{
			if( wxFileExists( g_SynPaths[idx] + synfile ) )
			{
				return new MadSyntax( g_SynPaths[idx] + synfile );
			}
		}
	}

	return nullptr;
}

size_t MadSyntax::GetSchemeCount()
{
	if( !s_Loaded ) LoadSyntaxFiles();

	return g_NameSchfileTable.size();
}
wxString MadSyntax::GetSchemeName( size_t index )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	if( index < g_NameSchfileTable.size() )
	{
		return g_NameSchfileTable[index].first;
	}

	return wxEmptyString;
}

wxString MadSyntax::GetSchemeFileByName( const wxString &schname, MadSyntax *default_syn, bool &star )
{
	if( !s_Loaded ) LoadSyntaxFiles();

	wxString name = schname;

	if( name.Right( 1 ) == wxT( '*' ) ) name = schname.Left( schname.Len() - 1 );

	if( name == wxT( "Default" ) )
	{
		star = true;

		if( default_syn ) return GetSyntaxFileByTitle( default_syn->m_SynAttr->m_Title );

		return wxEmptyString;
	}

	for( size_t i = 0; i < g_NameSchfileTable.size(); ++i )
	{
		wxFileName fn( g_NameSchfileTable[i].second );
		wxString n = fn.GetName();

		if( n == name )
		{
			star = ( g_NameSchfileTable[i].first.Last() == wxT( '*' ) );
			return g_NameSchfileTable[i].second;
		}
	}

	star = false;
	return wxEmptyString;
}

bool MadSyntax::LoadScheme( const wxString &schname, MadSyntax *syn )
{
	bool star;
	wxString schfile = GetSchemeFileByName( schname, syn, star );

	if( schfile.IsEmpty() && syn->m_SynAttr->m_Title == MadPlainTextTitle )
	{
		if( schname == wxT( "Default*" ) || schname == wxT( "Default" ) )
		{
			MadSyntax *newsyn = new MadSyntax( false );
			syn->AssignAttributes( newsyn );
			delete newsyn;
			return true;
		}
	}

	if( schfile.IsEmpty() || !wxFileExists( schfile ) ) return false;

	MadSyntax *newsyn = new MadSyntax( schfile, false, true );
	syn->AssignAttributes( newsyn );
	delete newsyn;
	return true;
}

bool MadSyntax::SaveScheme( const wxString &schname, MadSyntax *syn )
{
	wxASSERT( syn != nullptr );
	wxString name = schname;

	if( name.Right( 1 ) == wxT( '*' ) ) name = schname.Left( schname.Len() - 1 );

	bool star;
	wxString schfile = GetSchemeFileByName( name, syn, star );

	if( star || name.IsEmpty() ) return false;

	if( schfile.IsEmpty() || !wxFileExists( schfile ) )
	{
		MadSyntax *newsyn = new MadSyntax( false );
		size_t i;
		MadSyntaxRange ra;
		ra.bgcolor = *wxWHITE;

		for( i = newsyn->m_SynAttr->m_CustomRange.size(); i < 5; ++i )
		{
			newsyn->m_SynAttr->m_CustomRange.push_back( ra );
		}

		MadSyntaxKeyword ke;
		ke.m_Attr.color = *wxBLACK;
		ke.m_Attr.bgcolor = wxNullColour;
		ke.m_Attr.style = fsNone;

		for( i = newsyn->m_SynAttr->m_CustomKeyword.size(); i < 10; ++i )
		{
			newsyn->m_SynAttr->m_CustomKeyword.push_back( ke );
		}

		newsyn->AssignAttributes( syn, true );

		if( schfile.IsEmpty() )
		{
			schfile = s_AttributeFilePath + name + wxT( ".sch" );
			g_NameSchfileTable.push_back( pair<wxString, wxString>( name, schfile ) );
		}

		newsyn->SaveAttributes( schfile );
		delete newsyn;
	}
	else
	{
		MadSyntax *newsyn = new MadSyntax( schfile, false );
		newsyn->AssignAttributes( syn, true );
		newsyn->SaveAttributes( schfile );
		delete newsyn;
	}

	if( !wxFileExists( schfile ) ) return false;

	return true;
}

bool MadSyntax::DeleteScheme( const wxString &schname )
{
	bool star;
	wxString schfile = GetSchemeFileByName( schname, nullptr, star );

	if( star || schfile.IsEmpty() ) return false;

	StringPairIterator it = g_NameSchfileTable.begin();

	do
	{
		if( it->second == schfile )
		{
			g_NameSchfileTable.erase( it );
			break;
		}
	}
	while( ++it != g_NameSchfileTable.end() );

	wxRemoveFile( schfile );
	return true;
}

//========================================================

MadSyntax::MadSyntax( const wxString &filename, bool loadAttr/* = true*/, bool reParse/* = false*/ )
{
	LoadFromFile( filename, reParse );
	wxFileName fn( filename );

	if(!m_SynAttr) m_SynAttr.reset(new MadSyntaxAttributes());
	if( loadAttr && fn.GetExt().CmpNoCase( wxT( "syn" ) ) == 0 )
	{
		LoadAttributes();
	}
}

MadSyntax::MadSyntax( bool loadAttr/* = true*/ )
{
	m_SynAttr.reset(new MadSyntaxAttributes());
	if( loadAttr ) LoadAttributes();
}

MadSyntax::~MadSyntax()
{
	//Reset();    // free memories
}

void MadSyntax::LoadFromFile( const wxString &filename, bool reParse/* = false*/ )
{
	//Reset();    // reset attributes
	ParseSyntax( filename, reParse );
	if(m_SynAttr->m_Title == MadPlainTextTitle) m_SynAttr->m_IsPlainText = true;
	else m_SynAttr->m_IsPlainText = false;
}

void MadSyntax::ParseSyntax( const wxString &filename, bool reParse/* = false*/ )
{
	if(!reParse && g_TitleSynAttrMap.find(filename) != g_TitleSynAttrMap.end())
	{
		m_SynAttr = g_TitleSynAttrMap[filename];
		return;
	}
	wxFileConfig syn( wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS );
	wxString entry, value;
	
	m_SynAttr.reset(new MadSyntaxAttributes());
	m_SynAttr->m_Title = wxT( "No Title" );
	m_SynAttr->m_IsPlainText = false;
	long idx = 0;
	bool cont = syn.GetNextEntry( entry, idx );
	wxString s;

	while( cont )
	{
		//wxLogMessage(entry);
		syn.Read( entry, &value );
		entry.MakeLower();
		value.Trim( true );
		value.Trim( false );

		if( entry == wxT( "title" ) )
		{
			m_SynAttr->m_Title = value;
		}
		else
			if( entry == wxT( "encoding" ) )
			{
				m_SynAttr->m_Encoding = value;
			}
			else
				if( entry == wxT( "casesensitive" ) )
				{
					if( value.Lower() == wxT( "yes" ) )
						m_SynAttr->m_CaseSensitive = true;
				}
				else
					if( entry == wxT( "delimiter" ) )
					{
						//m_Delimiter = value;
						//m_Delimiter.clear();
						memset(m_SynAttr->m_Delimiter, 0, sizeof(m_SynAttr->m_Delimiter));
						for( size_t i = 0; i < value.Len(); ++i )
						{
							//m_Delimiter.insert((ucs4_t)value[i]);
							if(value[i] < 0x100)
								m_SynAttr->m_Delimiter[(ucs4_t)value[i]] = 1;
						}
					}
					else
						if( entry == wxT( "escapechar" ) )
						{
							m_SynAttr->m_EscapeChar = value;

							if( !m_SynAttr->m_EscapeChar.IsEmpty() )
								m_SynAttr->nw_EscapeChar = m_SynAttr->m_EscapeChar[0];
						}
						else
							if( entry == wxT( "stringchar" ) )
							{
								wxStringTokenizer tkz( value );
								s = tkz.GetNextToken();

								while( !s.IsEmpty() )
								{
									m_SynAttr->m_StringChar += s;
									s = tkz.GetNextToken();
								}
							}
							else
								if( entry == wxT( "directiveleading" ) )
								{
									m_SynAttr->m_DirectiveLeading = value;
								}
								else
									if( entry == wxT( "directiveleadingatbol" ) )
									{
										if( value.Lower() == wxT( "yes" ) )
											m_SynAttr->m_DirectiveLeadingAtBOL = true;
									}
									else
										if( entry == wxT( "keywordprefix" ) )
										{
											m_SynAttr->m_KeywordPrefix = value;
										}
										else
											if( entry == wxT( "specialwordprefix" ) )
											{
												m_SynAttr->m_SpecialWordPrefix = value;
											}
											else
												if( entry == wxT( "indentchar" ) )
												{
													wxStringTokenizer tkz( value );
													s = tkz.GetNextToken();

													while( !s.IsEmpty() )
													{
														m_SynAttr->m_IndentChar += s;
														s = tkz.GetNextToken();
													}
												}
												else
													if( entry == wxT( "unindentchar" ) )
													{
														wxStringTokenizer tkz( value );
														s = tkz.GetNextToken();

														while( !s.IsEmpty() )
														{
															m_SynAttr->m_UnindentChar += s;
															s = tkz.GetNextToken();
														}
													}
													else
														if( entry == wxT( "bracepair" ) )
														{
															wxStringTokenizer tkz( value );
															wxString pl = tkz.GetNextToken();
															wxString pr = tkz.GetNextToken();

															while( !pl.IsEmpty() && !pr.IsEmpty() )
															{
																m_SynAttr->m_LeftBrace.push_back( pl );
																m_SynAttr->m_RightBrace.push_back( pr );
																pl = tkz.GetNextToken();
																pr = tkz.GetNextToken();
															}
														}
														else
															if( entry == wxT( "autocompletepair" ) )
															{
																wxStringTokenizer tkz( value );
																wxString p = tkz.GetNextToken();

																while( !p.IsEmpty() )
																{
																	if( p.Len() == 2 )
																	{
																		m_SynAttr->m_AutoCompleteLeftChar += p[0];
																		m_SynAttr->m_AutoCompleteRightChar += p[1];
																	}

																	p = tkz.GetNextToken();
																}
															}
															else
																if( entry == wxT( "customrange" ) )
																{
																	wxStringTokenizer tkz( value );
																	MadSyntaxRange ra;

																	while( true )
																	{
																		s = tkz.GetNextToken();

																		if( !s.ToULong( &ra.id ) ) break;

																		if( ( ra.begin = tkz.GetNextToken() ).IsEmpty() ) break;

																		m_SynAttr->m_RangeBeginString.push_back( ra.begin );

																		if( ( ra.end = tkz.GetNextToken() ).IsEmpty() ) break;

																		if( ( s = tkz.GetNextToken() ).IsEmpty() ) break;

																		SetColor( s, ra.bgcolor );
																		m_SynAttr->m_CustomRange.push_back( ra );
																	}
																}
																else
																	if( entry == wxT( "stringinrange" ) )
																	{
																		SetInRange( value, m_SynAttr->m_StringInRange );
																	}
																	else
																		if( entry == wxT( "linecomment" ) )
																		{
																			wxStringTokenizer tkz( value );
																			s = tkz.GetNextToken();

																			while( !s.IsEmpty() )
																			{
																				m_SynAttr->m_LineComment.push_back( s );
																				s = tkz.GetNextToken();
																			}
																		}
																		else
																			if( entry == wxT( "linecommentinrange" ) )
																			{
																				SetInRange( value, m_SynAttr->m_LineCommentInRange );
																			}
																			else
																				if( entry == wxT( "linecommentatbol" ) )
																				{
																					if( value.Lower() == wxT( "yes" ) )
																						m_SynAttr->m_LineCommentAtBOL = true;
																				}
																				else
																					if( entry == wxT( "blockcomment" ) )
																					{
																						wxStringTokenizer tkz( value );
																						wxString on = tkz.GetNextToken();
																						wxString off = tkz.GetNextToken();

																						while( !on.IsEmpty() && !off.IsEmpty() )
																						{
																							m_SynAttr->m_BlockCommentOn.push_back( on );
																							m_SynAttr->m_BlockCommentOff.push_back( off );
																							on = tkz.GetNextToken();
																							off = tkz.GetNextToken();
																						}
																					}
																					else
																						if( entry == wxT( "blockcommentinrange" ) )
																						{
																							wxStringTokenizer tkz( value, wxT( "," ), wxTOKEN_RET_EMPTY_ALL );
																							size_t inx = 0;

																							while( tkz.HasMoreTokens() )
																							{
																								s = tkz.GetNextToken();
																								m_SynAttr->m_BlockCommentInRange.push_back( vector<int>() );
																								SetInRange( s, m_SynAttr->m_BlockCommentInRange[inx++] );
																							}
																						}
																						else
																						{
																							int attr = 0, alen = 5;

																							if( entry.Right( 7 ) == wxT( "bgcolor" ) )
																							{
																								attr = 1;
																								alen = 7;
																							}
																							else
																								if( ( s = entry.Right( 5 ) ) == wxT( "color" ) )
																								{
																									attr = 2;
																								}
																								else
																									if( s == wxT( "style" ) )
																									{
																										attr = 3;
																									}

																							int elen = ( int )entry.size();

																							if( attr != 0 && ( elen -= alen ) > 0 )
																							{
																								MadAttributes *at = GetAttributes( entry.Left( elen ) );

																								if( at )
																								{
																									switch( attr )
																									{
																									case 1:
																										SetColor( value, at->bgcolor );
																										break;

																									case 2:
																										SetColor( value, at->color );
																										break;

																									case 3:
																										SetStyle( value, at->style );
																										break;
																									}
																								}
																							}
																						}

		cont = syn.GetNextEntry( entry, idx );
	}

	size_t kwlen;
	m_SynAttr->nw_MaxKeywordLen = 0;
	// load custom keywords
	wxString group;
	long gidx = 0;
	bool gcont = syn.GetNextGroup( group, gidx );

	wxConfigBase  * cfg = wxConfigBase::Get(false);
	wxString oldpath = cfg->GetPath();
	cfg->SetPath(_T("/SpellChecker"));
	bool bb = false;
	cfg->Read(_T("AddKeywords"), &bb, true);
	m_SynAttr->m_SyntaxKeywordDict->DisableSort();
	while( gcont )
	{
		//wxLogMessage(entry);
		MadSyntaxKeyword *ck = GetCustomKeyword( group );
		ck->m_CaseSensitive = m_SynAttr->m_CaseSensitive;
		syn.SetPath( wxString( wxT( '/' ) ) + group );
		idx = 0;
		cont = syn.GetNextEntry( entry, idx );

		while( cont )
		{
			syn.Read( entry, &value );
			entry.MakeLower();
			value.Trim( true );
			value.Trim( false );

			if( entry == wxT( "color" ) )
			{
				SetColor( value, ck->m_Attr.color );
			}
			else
				if( entry == wxT( "style" ) )
				{
					SetStyle( value, ck->m_Attr.style );
				}
				else
					if( entry == wxT( "inrange" ) )
					{
						SetInRange( value, ck->m_InRange );
					}
					else
						if( entry == wxT( "casesensitive" ) )
						{
							value.MakeLower();

							if( value == wxT( "yes" ) ) ck->m_CaseSensitive = true;
							else
								if( value == wxT( "no" ) ) ck->m_CaseSensitive = false;
						}
						else
							if( entry == wxT( "keyword" ) )
							{
								wxStringTokenizer tkz( value );
								wxString kw = tkz.GetNextToken();

								while( !kw.IsEmpty() )
								{
									if( ( kwlen = kw.size() ) > m_SynAttr->nw_MaxKeywordLen )
									{
										m_SynAttr->nw_MaxKeywordLen = kwlen;
									}

									if( !ck->m_CaseSensitive )
										kw.MakeLower();

									ck->m_Keywords.insert( kw );
									if(bb)
									{
										if(m_SynAttr->m_SyntaxKeywordDict->IsWordInDictionary(kw) == false)
											m_SynAttr->m_SyntaxKeywordDict->AddWord(kw);
									}
									kw = tkz.GetNextToken();
								}
							}

			cont = syn.GetNextEntry( entry, idx );
		}

		syn.SetPath( wxString( wxT( '/' ) ) );
		gcont = syn.GetNextGroup( group, gidx );
	}

	if(bb)
		m_SynAttr->m_SyntaxKeywordDict->SortDict();
	// syn loaded, begin post-processing
#ifdef __WXMSW__

	if( m_SynAttr->m_SystemAttributes[aeText].color == wxNullColour )
	{
		m_SynAttr->m_SystemAttributes[aeText].color = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
	}

#endif

	if( m_SynAttr->m_SystemAttributes[aeText].color == wxNullColour ||
			m_SynAttr->m_SystemAttributes[aeText].bgcolor == wxNullColour )
	{
		m_SynAttr->m_SystemAttributes[aeText].color = *wxBLACK;
		m_SynAttr->m_SystemAttributes[aeText].bgcolor = *wxWHITE;
	}

#ifdef __WXMSW__

	if( m_SynAttr->m_SystemAttributes[aeText].bgcolor == *wxWHITE )
	{
		m_SynAttr->m_SystemAttributes[aeText].bgcolor = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );
	}

#endif
	vector < MadSyntaxKeyword >::iterator kit = m_SynAttr->m_CustomKeyword.begin();
	vector < MadSyntaxKeyword >::iterator kend = m_SynAttr->m_CustomKeyword.end();

	while( kit != kend )
	{
		if( kit->m_InRange.empty() )
			kit->m_InRange.push_back( 0 ); // Keyword have effect in global range

		++kit;
	}

	if(m_SynAttr->m_StringInRange.empty() )
		m_SynAttr->m_StringInRange.push_back( 0 ); // String have effect in global range

	if( !m_SynAttr->m_LineComment.empty() && m_SynAttr->m_LineCommentInRange.empty() )
		m_SynAttr->m_LineCommentInRange.push_back( 0 );      // m_LineComment have effect in global range

	size_t si = m_SynAttr->m_BlockCommentOn.size();

	if( si > m_SynAttr->m_BlockCommentInRange.size() )
	{
		m_SynAttr->m_BlockCommentInRange.resize( si );

		for( size_t sz = 0; sz < si; ++sz )
		{
			if(m_SynAttr->m_BlockCommentInRange[sz].empty() )
				m_SynAttr->m_BlockCommentInRange[sz].push_back( 0 );   // BlockComment[i] have effect in global range
		}
	}

	vector < wxString >::iterator sit, send;

	if(m_SynAttr->m_CaseSensitive == false )
	{
		// change string data to lowercase
		sit = m_SynAttr->m_LineComment.begin();
		send = m_SynAttr->m_LineComment.end();

		while( sit != send )
		{
			sit->MakeLower();
			++sit;
		}

		sit = m_SynAttr->m_BlockCommentOn.begin();
		send = m_SynAttr->m_BlockCommentOn.end();

		while( sit != send )
		{
			sit->MakeLower();
			++sit;
		}

		sit = m_SynAttr->m_BlockCommentOff.begin();
		send = m_SynAttr->m_BlockCommentOff.end();

		while( sit != send )
		{
			sit->MakeLower();
			++sit;
		}

		sit = m_SynAttr->m_LeftBrace.begin();
		send = m_SynAttr->m_LeftBrace.end();

		while( sit != send )
		{
			sit->MakeLower();
			++sit;
		}

		sit = m_SynAttr->m_RightBrace.begin();
		send = m_SynAttr->m_RightBrace.end();

		while( sit != send )
		{
			sit->MakeLower();
			++sit;
		}
	}

	if( !m_SynAttr->m_LineComment.empty() || !m_SynAttr->m_BlockCommentOn.empty()  || !m_SynAttr->m_StringChar.IsEmpty()
			|| !m_SynAttr->m_DirectiveLeading.IsEmpty() || !m_SynAttr->m_CustomRange.empty() || !m_SynAttr->m_LeftBrace.empty() )
	{
		m_SynAttr->m_CheckState = true;
	}

	if(!reParse)
		g_TitleSynAttrMap[filename] = m_SynAttr;
	cfg->SetPath(oldpath);
}

//static ucs4_t DefDelimiter[] = {wxT('~'), wxT('`'), wxT('!'), wxT('@'), wxT('#'), wxT('$'), wxT('%'), wxT('^'), wxT('&'), wxT('*'), wxT('('), wxT(')'), wxT('-'), wxT('+'), wxT('='), wxT('|'), wxT('\\'), wxT('{'), wxT('}'), wxT('['), wxT(']'), wxT(':'), wxT(';'), wxT('"'), wxT('\''), wxT(','), wxT('.'), wxT('<'), wxT('>'), wxT('/'), wxT('?')};
static unsigned char DefDelimiter[] = {'~', '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '+', '=', '|', '\\', '{', '}', '[', ']', ':', ';', '"', '\'', ',', '.', '<', '>', '/', '?'};

MadSyntaxAttributes::MadSyntaxAttributes()
{
	m_Title = MadPlainTextTitle;
	m_CaseSensitive = false;
	//m_Delimiter = wxT( "~`!@#$%^&*()-+=|\\{}[]:;\"\',.<>/?" );
	//m_Delimiter.clear();
	memset(m_Delimiter, 0, sizeof(m_Delimiter));
	for( int i = 0; i < (sizeof(DefDelimiter)/sizeof(unsigned char)); ++i )
	{
		//m_Delimiter.insert((ucs4_t)DefDelimiter[i]);
		m_Delimiter[DefDelimiter[i]] = 1;
	}
	m_LineCommentAtBOL = false;
	m_DirectiveLeadingAtBOL = false;
	m_SyntaxKeywordDict.reset(new PersonalDictionary());
	nw_MaxKeywordLen = 0;
	
	MadAttributes *pat = m_SystemAttributes;
#ifdef __WXMSW__
	wxColour c = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
	wxColour c1 = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );
	wxString s = c.GetAsString();
	wxString s1 = c1.GetAsString();
	SystemAttributesColor[0] = s;
	SystemAttributesBgColor[0] = s1;
#endif

	for( size_t i = 0; i < aeNone; ++i )
	{
		if( SystemAttributesColor[i][0] == 0 )
			pat->color = wxNullColour;
		else
			if( SystemAttributesColor[i][0] == wxT( '#' ) )
				pat->color.Set( SystemAttributesColor[i][1], SystemAttributesColor[i][2], SystemAttributesColor[i][3] );
			else
				pat->color = wxColour( SystemAttributesColor[i] );

		if( SystemAttributesBgColor[i][0] == 0 )
			pat->bgcolor = wxNullColour;
		else
			if( SystemAttributesBgColor[i][0] == wxT( '#' ) )
				pat->bgcolor.Set( SystemAttributesBgColor[i][1], SystemAttributesBgColor[i][2], SystemAttributesBgColor[i][3] );
			else
				pat->bgcolor = wxColour( SystemAttributesBgColor[i] );

		pat->style = fsNone;
		++pat;
	}
}

MadSyntaxAttributes::MadSyntaxAttributes(MadSyntaxAttributes & synAttrs)
{
	memcpy(m_Delimiter, synAttrs.m_Delimiter, 256*(sizeof(wxByte)));
	m_Title = synAttrs.m_Title;
	m_LineComment           = synAttrs.m_LineComment;
	m_BlockCommentOn        = synAttrs.m_BlockCommentOn;
	m_BlockCommentOff       = synAttrs.m_BlockCommentOff;
	m_EscapeChar            = synAttrs.m_EscapeChar;
	m_StringChar            = synAttrs.m_StringChar;
	m_DirectiveLeading      = synAttrs.m_DirectiveLeading;
	m_KeywordPrefix         = synAttrs.m_KeywordPrefix;
	m_SpecialWordPrefix     = synAttrs.m_SpecialWordPrefix;
	m_IndentChar            = synAttrs.m_IndentChar;
	m_UnindentChar          = synAttrs.m_UnindentChar;
	m_LeftBrace             = synAttrs.m_LeftBrace;
	m_RightBrace            = synAttrs.m_RightBrace;
	m_AutoCompleteLeftChar  = synAttrs.m_AutoCompleteLeftChar;
	m_AutoCompleteRightChar = synAttrs.m_AutoCompleteRightChar;
	m_Encoding              = synAttrs.m_Encoding;
	m_StringInRange         = synAttrs.m_StringInRange;
	m_LineCommentInRange    = synAttrs.m_LineCommentInRange;
	m_BlockCommentInRange   = synAttrs.m_BlockCommentInRange;
	nw_MaxKeywordLen        = synAttrs.nw_MaxKeywordLen;
	nw_EscapeChar           = synAttrs.nw_EscapeChar;
	m_CaseSensitive         = synAttrs.m_CaseSensitive;
	m_LineCommentAtBOL      = synAttrs.m_LineCommentAtBOL; // for diff/patch syntax
	m_DirectiveLeadingAtBOL = synAttrs.m_DirectiveLeadingAtBOL;
	m_CheckState            = synAttrs.m_CheckState;
	m_IsPlainText           = synAttrs.m_IsPlainText;
	m_CustomRange           = synAttrs.m_CustomRange; // user defined ranges
	m_RangeBeginString      = synAttrs.m_RangeBeginString;   
	m_CustomKeyword         = synAttrs.m_CustomKeyword; // user defined keywords
	m_SyntaxKeywordDict     = synAttrs.m_SyntaxKeywordDict;
	for( size_t i = 0; i < aeNone; ++i )
	{
		m_SystemAttributes[i] = synAttrs.m_SystemAttributes[i];
	}
}

void MadSyntax::Reset()
{
}

MadAttributes *MadSyntax::GetAttributes( const wxString &name )
{
	for( int i = 0; i < aeNone; ++i )
	{
		if( name.CmpNoCase( SystemAttributesName[i] ) == 0 )
			return &m_SynAttr->m_SystemAttributes[i];
	}

	return nullptr;
}

MadSyntaxKeyword *MadSyntax::GetCustomKeyword( const wxString &name )
{
	vector < MadSyntaxKeyword >::iterator it = m_SynAttr->m_CustomKeyword.begin();

	while( it != m_SynAttr->m_CustomKeyword.end() )
	{
		if( it->m_Name == name )
		{
			return &( *it );
		}

		++it;
	}

	it = m_SynAttr->m_CustomKeyword.insert(m_SynAttr->m_CustomKeyword.end(), MadSyntaxKeyword() );
	it->m_Name = name;

	return &( *it );
}

void MadSyntax::SetColor( const wxString &value, wxColour &c )
{
	if( value.Upper() == wxT( "NONE" ) )
	{
		c = wxNullColour;
	}
	else
	{
		c = wxTheColourDatabase->Find( value );

		if( !c.Ok() && !value.IsEmpty() )
		{
			unsigned long ul;

			if( value.ToULong( &ul, 16 ) )
			{
				c.Set( ( ul >> 16 ) & 0xFF, ( ul >> 8 ) & 0xFF, ul & 0xFF );
			}
		}
	}
}

void MadSyntax::SetStyle( const wxString &value, MadFontStyles &fs )
{
	wxStringTokenizer tkz( value );
	wxString s = tkz.GetNextToken();

	while( !s.IsEmpty() )
	{
		s.MakeLower();

		if( s == wxT( "bold" ) )
		{
			fs |= fsBold;
		}
		else
			if( s == wxT( "italic" ) )
			{
				fs |= fsItalic;
			}
			else
				if( s == wxT( "underline" ) )
				{
					fs |= fsUnderline;
				}
				else
					if( s == wxT( "strikeout" ) )
					{
						fs |= fsStrikeOut;
					}

		s = tkz.GetNextToken();
	}
}

void MadSyntax::SetInRange( const wxString &value, vector < int > &ir )
{
	wxStringTokenizer tkz( value );
	wxString s = tkz.GetNextToken();
	unsigned long ul;

	while( !s.IsEmpty() )
	{
		if( s.ToULong( &ul ) )
		{
			ir.push_back( ul );
		}

		s = tkz.GetNextToken();
	}
}

bool MadSyntax::IsInRange( int range, vector < int >&InRangeVector )
{
	if( !InRangeVector.empty() )
	{
		vector < int >::iterator it = InRangeVector.begin();

		do
		{
			if( range == *it ) return true;

			++it;
		}
		while( it != InRangeVector.end() );
	}

	return false;
}

MadSyntaxRange *MadSyntax::GetSyntaxRange( int rangeid )
{
	if( !m_SynAttr->m_CustomRange.empty() )
	{
		vector < MadSyntaxRange >::iterator it = m_SynAttr->m_CustomRange.begin();

		do
		{
			if( ( int )it->id == rangeid ) return &( *it );

			++it;
		}
		while( it != m_SynAttr->m_CustomRange.end() );
	}

	return nullptr;
}

wxString MadSyntax::GetAttributeName( MadAttributeElement ae )
{
	return wxString( SystemAttributesName[ae] );
}

void MadSyntax::SetAttributes( MadAttributes *attr )
{
	if( attr == m_SynAttr->m_SystemAttributes + aeText )
	{
		nw_BgColor = nw_CurrentBgColor;
	}
	else
	{
		nw_BgColor = attr->bgcolor;
	}

	if( nw_BgColor == wxNullColour )
	{
		nw_BgColor = nw_CurrentBgColor;
	}

	if( ( nw_Color = attr->color ) == wxNullColour )
	{
		nw_Color = m_SynAttr->m_SystemAttributes[aeText].color;
	}

	//if((attr->style&fsStrikeOut)!=0)

	if( attr != &m_SynAttr->m_SystemAttributes[aeSpace] &&
			attr != &m_SynAttr->m_SystemAttributes[aeActiveLine] &&
			attr != &m_SynAttr->m_SystemAttributes[aeBookmark] &&
			attr != &m_SynAttr->m_SystemAttributes[aeHighlightWord] )
	{
		nw_Font = wxTheFontList->FindOrCreateFont( nw_FontSize, (wxFontFamily)nw_FontFamily,
				  ( attr->style & fsItalic ) == 0 ? wxFONTSTYLE_NORMAL : wxFONTSTYLE_ITALIC,
				  ( attr->style & fsBold ) == 0 ? wxFONTWEIGHT_NORMAL : wxFONTWEIGHT_BOLD,
				  ( attr->style & fsUnderline ) != 0,
				  nw_FontName );
	}
}


void MadSyntax::InitNextWord1( MadLines *madlines, ucs4_t *word, int *widths, const wxString &fontname, int fontsize, int fontfamily )
{
	nw_MadLines = madlines;
	nw_MadEdit = madlines->m_MadEdit;
	nw_Encoding = nw_MadEdit->m_Encoding;
	nw_Word = word;
	nw_Widths = widths;
	nw_FontName = fontname;
	nw_FontSize = fontsize;
	nw_FontFamily = fontfamily;
	nw_Font = wxTheFontList->FindOrCreateFont( nw_FontSize, (wxFontFamily)nw_FontFamily,
			  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, nw_FontName );

	if(m_SynAttr->m_CaseSensitive )
		FindString = &MadSyntax::FindStringCase;
	else
		FindString = &MadSyntax::FindStringNoCase;
}

void MadSyntax::InitNextWord2( MadLineIterator &lit, size_t row )
{
	nw_LineIter = lit;
	nw_State.Reset();
	nw_CommentUntilEOL = false;
	nw_BeginOfLine = ( row == 0 );
	nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor; // set bgcolor for empty lines
	nw_RowIndexIter = lit->m_RowIndices.begin();
	std::advance( nw_RowIndexIter, row );
	nw_MadLines->InitNextUChar( lit, nw_RowIndexIter->m_Start );
	nw_NotSpaceCount = 0;
	nw_LineWidth = 0;
	nw_FirstIndex = 0;
	nw_RestCount = 0;
	if(lit->m_RowIndices[row + 1].m_Start >= nw_RowIndexIter->m_Start)
		nw_MaxLength = lit->m_RowIndices[row + 1].m_Start - nw_RowIndexIter->m_Start;
	else
	{
		wxASSERT(0);
		nw_MaxLength = 0;
	}

	if( m_SynAttr->m_CheckState )
	{
		MadLineState &state = lit->m_State;
		nw_SynRange = 0;

		if( ( nw_State.rangeid = state.RangeId ) != 0 )
		{
			nw_SynRange = GetSyntaxRange( nw_State.rangeid );
			if(nw_SynRange)
				nw_CurrentBgColor = nw_SynRange->bgcolor; // set bgcolor for empty lines

			if( nw_CurrentBgColor == wxNullColour )
			{
				nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
			}
		}

		if( ( nw_State.blkcmtid = state.CommentId ) != 0 )
		{
			if( m_SynAttr->m_SystemAttributes[aeComment].bgcolor != wxNullColour )
			{
				nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeComment].bgcolor; // set bgcolor for empty lines
			}
		}

		nw_ContainCommentOff = ( state.CommentOff != 0 );

		if( ( nw_State.stringid = state.StringId ) != 0 )
		{
			nw_StringChar = m_SynAttr->m_StringChar[nw_State.stringid - 1];

			if( m_SynAttr->m_SystemAttributes[aeString].bgcolor != wxNullColour )
			{
				nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeString].bgcolor; // set bgcolor for empty lines
			}
		}

		if( ( nw_State.linecmt = state.LineComment ) != 0 )
		{
			nw_CommentUntilEOL = true;
		}

		nw_State.directive = state.Directive;
	}

	nw_NextState = nw_State;
	nw_EndOfLine = false;

	if( nw_RowIndexIter->m_Width == 0 )
	{
		nw_EndOfLine = true;
	}

	if( !nw_ucqueue.empty() )
	{
		nw_ucqueue.clear();
	}
}

void MadSyntax::SetEncoding( MadEncoding *encoding )
{
	nw_Encoding = encoding;
}

// 0: none, 1: first, 2:...
int MadSyntax::FindStringCase( MadUCQueue & ucqueue, size_t first,
							   MadStringIterator begin, const MadStringIterator & end,
							   size_t & len )
{
	int idx = 1;
	ucs4_t firstuc = ucqueue[first].first;
	size_t ucsize = ucqueue.size() - first;
	wxASSERT( ucsize > 0 );
	bool noNewLine = true;
	MadUCQueueIterator it;

	if( ucsize > 1 )
	{
		__REGISTER ucs4_t uc = ucqueue.back().first;

		if( uc == 0x0D || uc == 0x0A )
		{
			noNewLine = false;
			--ucsize; // exclude NewLine char
		}
	}

	MadLines::NextUCharFuncPtr NextUChar = nw_MadLines->NextUChar;

	do
	{
		const wchar_t *cstr = begin->c_str();
		len = begin->size();

		if( firstuc == ( ucs4_t )cstr[0] )
		{
			if( len == 1 ) return idx;

			if( ucsize < len && noNewLine )
			{
				while( ( nw_MadLines->*NextUChar )( ucqueue ) )
				{
					__REGISTER ucs4_t uc = ucqueue.back().first;

					if( uc == 0x0D || uc == 0x0A )
					{
						noNewLine = false;
						break;
					}

					if( ( ++ucsize ) >= len ) break;
				}
			}

			if( ucsize >= len )
			{
				it = ucqueue.begin();
				std::advance( it, first + 1 );

				while( *( ++cstr ) != 0 )
				{
					if( ( ucs4_t )*cstr != it->first ) break;

					++it;
				}

				if( *cstr == 0 ) return idx;
			}
		}

		++idx;
	}
	while( ++begin != end );

	return 0;
}

int MadSyntax::FindStringNoCase( MadUCQueue & ucqueue, size_t first,
								 MadStringIterator begin, const MadStringIterator & end,
								 size_t & len )
{
	int idx = 1;
	ucs4_t firstuc = ucqueue[first].first;

	if( firstuc >= 'A' && firstuc <= 'Z' )
	{
		firstuc |= 0x20; // to lower case
	}

	size_t ucsize = ucqueue.size() - first;
	wxASSERT( ucsize > 0 );
	bool noNewLine = true;

	if( ucsize > 1 )
	{
		__REGISTER ucs4_t uc = ucqueue.back().first;

		if( uc == 0x0D || uc == 0x0A )
		{
			noNewLine = false;
			--ucsize; // exclude NewLine char
		}
	}

	MadLines::NextUCharFuncPtr NextUChar = nw_MadLines->NextUChar;
	MadUCQueueIterator it;

	do
	{
		const wchar_t *cstr = begin->c_str();
		len = begin->size();

		if( firstuc == ( ucs4_t )cstr[0] )
		{
			if( len == 1 ) return idx;

			if( ucsize < len && noNewLine )
			{
				while( ( nw_MadLines->*NextUChar )( ucqueue ) )
				{
					__REGISTER ucs4_t uc = ucqueue.back().first;

					if( uc == 0x0D || uc == 0x0A )
					{
						noNewLine = false;
						break;
					}

					if( ( ++ucsize ) >= len ) break;
				}
			}

			if( ucsize >= len )
			{
				it = ucqueue.begin();
				std::advance( it, first + 1 );

				while( *( ++cstr ) != 0 )
				{
					__REGISTER ucs4_t uc = it->first;

					if( uc >= 'A' && uc <= 'Z' )
					{
						uc |= 0x20; // to lower case
					}

					if( ( ucs4_t )*cstr != uc ) break;

					++it;
				}

				if( *cstr == 0 ) return idx;
			}
		}

		++idx;
	}
	while( ++begin != end );

	return 0;
}

// return wordlength
int MadSyntax::NextWord( int &wordwidth )
{
	wordwidth = 0;

	if( nw_RestCount == 0 )
	{
		if( nw_EndOfLine && nw_ucqueue.empty() )
			return 0;

		MadLines::NextUCharFuncPtr NextUChar = nw_MadLines->NextUChar;
		MadStringIterator sit, sitend;
		size_t strlen = 0;
		int idx = 0;
		nw_RestCount += nw_FirstIndex;

		do
		{
			if( nw_EndOfLine == false )
			{
				( nw_MadLines->*NextUChar )( nw_ucqueue );
			}

			size_t ucsize = nw_ucqueue.size();

			if( ucsize )
			{
				ucs4_t lastuc = nw_ucqueue.back().first;

				if( lastuc == 0x0D || lastuc == 0x0A )
				{
					nw_ucqueue.pop_back();
					--ucsize;
				}
			}

			if( nw_FirstIndex >= ucsize )
			{
				//nw_EndOfLine = true;
				if( nw_RestCount == 0 )
					return 0;

				break;
			}

			if( !nw_CommentUntilEOL )
			{
				ucs4_t firstuc = nw_ucqueue[nw_FirstIndex].first;

				if( /*firstuc < 0x100 && */ !IsSpace( firstuc ) )
				{
					++nw_NotSpaceCount;

					if( m_SynAttr->m_CheckState && firstuc < 0x100 )
					{
						// check EscapeChar
						if( firstuc == m_SynAttr->nw_EscapeChar )
						{
							( nw_MadLines->*NextUChar )( nw_ucqueue );
							idx = ( int )nw_FirstIndex + 1;

							if( nw_ucqueue.size() > ( size_t ) idx )
							{
								ucs4_t c = nw_ucqueue[idx].first;

								if( c != 0x0D && c != 0x0A )
								{
									++nw_FirstIndex;
									++nw_RestCount;
								}
							}

							goto _NEXTUCHAR_;
						}

						// check String Off
						if( nw_State.stringid != 0 )
						{
							if( firstuc == nw_StringChar )
							{
								nw_NextState.stringid = 0;
								++nw_FirstIndex;
								++nw_RestCount;
								break;
							}

							goto _NEXTUCHAR_;
						}

						// check Block Comment Off
						if( nw_State.blkcmtid != 0 )
						{
							if( !nw_ContainCommentOff )
							{
								nw_CommentUntilEOL = true;
								goto _NEXTUCHAR_;
							}

							sit = m_SynAttr->m_BlockCommentOff.begin();
							std::advance( sit, nw_State.blkcmtid - 1 );
							sitend = sit;
							++sitend;

							if( ( this->*FindString )( nw_ucqueue, nw_FirstIndex, sit, sitend, strlen ) != 0 )
							{
								nw_NextState.blkcmtid = 0;
								nw_FirstIndex += strlen;
								nw_RestCount = nw_FirstIndex;
								break;
							}

							goto _NEXTUCHAR_;
						}

						// check Range Off
						if( nw_State.rangeid != 0 )
						{
							if( nw_SynRange )
							{
								vector < wxString > strvec;
								strvec.push_back( nw_SynRange->end );

								if( ( this->*FindString )( nw_ucqueue, nw_FirstIndex,
														   strvec.begin(), strvec.end(), strlen ) != 0 )
								{
									nw_NextState.rangeid = 0;
									nw_FirstIndex += strlen;
									nw_RestCount = nw_FirstIndex;
									break;
								}
							}
						}

						// check Directive
						if( !m_SynAttr->m_DirectiveLeadingAtBOL || nw_BeginOfLine )
						{
							if( nw_NotSpaceCount == 1 && !m_SynAttr->m_DirectiveLeading.IsEmpty() )
							{
								if(m_SynAttr->m_DirectiveLeading.Find( wxChar( firstuc ) ) >= 0 )
								{
									nw_NextState.directive = 1;
									nw_FirstIndex += 1;

									if( nw_RestCount == 0 )
									{
										nw_State = nw_NextState;
										nw_RestCount = nw_FirstIndex;
										continue;
									}

									break;
								}
							}
						}

						// check Block Comment On
						if( !m_SynAttr->m_BlockCommentOn.empty() )
						{
							idx = ( this->*FindString )( nw_ucqueue, nw_FirstIndex,
								m_SynAttr->m_BlockCommentOn.begin(), m_SynAttr->m_BlockCommentOn.end(), strlen );

							if( idx != 0 )
							{
								// check InRange
								if( IsInRange( nw_State.rangeid, m_SynAttr->m_BlockCommentInRange[idx - 1] ) )
								{
									nw_NextState.blkcmtid = idx;
									nw_FirstIndex += strlen;

									if( nw_RestCount == 0 )
									{
										nw_State = nw_NextState;
										nw_RestCount = nw_FirstIndex;
										continue;
									}

									break;
								}
							}
						}

						// check Line Comment On
						if( !m_SynAttr->m_LineCommentAtBOL || nw_BeginOfLine )
						{
							if( !m_SynAttr->m_LineComment.empty() )
							{
								idx = ( this->*FindString )( nw_ucqueue, nw_FirstIndex,
									m_SynAttr->m_LineComment.begin(), m_SynAttr->m_LineComment.end(), strlen );

								if( idx != 0 )
								{
									if( IsInRange( nw_State.rangeid, m_SynAttr->m_LineCommentInRange ) )
									{
										// range-off mark may be beyond linecomment (ex: javascript)
										if( nw_State.rangeid == 0 )
										{
											nw_CommentUntilEOL = true;
										}

										nw_NextState.linecmt = idx;
										nw_FirstIndex += strlen;

										if( nw_RestCount == 0 )
										{
											nw_State = nw_NextState;
											nw_RestCount = nw_FirstIndex;
											continue;
										}

										break;
									}
								}
							}
						}

						// check String On
						if( !m_SynAttr->m_StringChar.IsEmpty()
								&& IsInRange( nw_State.rangeid, m_SynAttr->m_StringInRange ) )
						{
							if( ( idx = m_SynAttr->m_StringChar.Find( wxChar( firstuc ) ) + 1 ) > 0 )
							{
								nw_NextState.stringid = idx;
								nw_StringChar = firstuc;
								++nw_FirstIndex;

								if( nw_RestCount == 0 )
								{
									nw_State = nw_NextState;
									nw_RestCount = nw_FirstIndex;
									continue;
								}

								break;
							}
						}

						// check Range On
						if( nw_State.rangeid == 0 && !m_SynAttr->m_RangeBeginString.empty() )
						{
							idx = ( this->*FindString )( nw_ucqueue, nw_FirstIndex,
														 m_SynAttr->m_RangeBeginString.begin(), m_SynAttr->m_RangeBeginString.end(), strlen );

							if( idx != 0 )
							{
								nw_NextState.rangeid = m_SynAttr->m_CustomRange[idx - 1].id;
								nw_SynRange = GetSyntaxRange( nw_NextState.rangeid );
								nw_FirstIndex += strlen;

								if( nw_RestCount == 0 )
								{
									nw_State = nw_NextState;
									nw_RestCount = nw_FirstIndex;
									continue;
								}

								break;
							}
						}
					}
				}
			}

_NEXTUCHAR_:
			++nw_FirstIndex;
			++nw_RestCount;
			nw_BeginOfLine = false;
		}
		while( nw_RestCount < nw_MaxLength );
	}

	wxASSERT( nw_RestCount != 0 );
	wxASSERT( nw_RowIndexIter->m_Width != 0 );
	nw_BeginOfLine = false;
	// set current bgcolor by state
	bool setbgcolor = false;

	if( nw_State.blkcmtid != 0 || nw_State.linecmt != 0 )
	{
		nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeComment].bgcolor;

		if( nw_CurrentBgColor != wxNullColour ) setbgcolor = true;
	}

	if( setbgcolor == false )
	{
		if( nw_State.directive )
		{
			nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeDirective].bgcolor;

			if( nw_CurrentBgColor != wxNullColour ) setbgcolor = true;
		}
		else
			if( nw_State.stringid != 0 )
			{
				nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeString].bgcolor;

				if( nw_CurrentBgColor != wxNullColour ) setbgcolor = true;
			}
	}

	if( setbgcolor == false )
	{
		if( nw_State.rangeid != 0 )
		{
			MadSyntaxRange * r = GetSyntaxRange( nw_State.rangeid );
			nw_CurrentBgColor = r->bgcolor;

			if( nw_CurrentBgColor == wxNullColour )
			{
				nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
			}
		}
		else
		{
			nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
		}
	}

	// now the nw_ucqueue is the wanted Word
	// calc the width/length/type and return the word
	int idx = 0;
	int old_line_width = nw_LineWidth;
	int width;
	ucs4_t uc = nw_ucqueue.front().first;

	if( IsSpace( uc ) )
	{
		SetAttributes( aeSpace );
		int tabwidth = nw_MadEdit->m_TabColumns * nw_MadEdit->GetUCharWidth( 0x20 );

		do
		{
			nw_Word[idx] = uc;
			nw_ucqueue.pop_front();
			width = nw_MadEdit->GetUCharWidth( uc );

			if( uc == 0x09 )
			{
				int tbwidth = tabwidth;
				width = nw_RowIndexIter->m_Width - nw_LineWidth;
				tbwidth -= ( nw_LineWidth % tbwidth );

				if( tbwidth < width )
					width = tbwidth;
			}

			nw_Widths[idx] = width;
			nw_LineWidth += width;
			++idx;
			--nw_FirstIndex;
		}
		while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
				&& IsSpace( uc = nw_ucqueue.front().first ) );

		nw_Word[idx] = 0;
	}
	else
		if( nw_State.blkcmtid != 0 || nw_State.linecmt != 0 )
		{
			SetAttributes( aeComment );

			do
			{
				nw_Word[idx] = uc;
				nw_ucqueue.pop_front();
				width = nw_MadEdit->GetUCharWidth( uc );
				nw_Widths[idx] = width;
				nw_LineWidth += width;
				++idx;
				--nw_FirstIndex;
			}
			while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
					&& !IsSpace( uc = nw_ucqueue.front().first ) );

			nw_Word[idx] = 0;
		}
		else
			if( nw_State.directive )
			{
				SetAttributes( aeDirective );

				do
				{
					nw_Word[idx] = uc;
					nw_ucqueue.pop_front();
					width = nw_MadEdit->GetUCharWidth( uc );
					nw_Widths[idx] = width;
					nw_LineWidth += width;
					++idx;
					--nw_FirstIndex;
				}
				while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
						&& !IsSpace( uc = nw_ucqueue.front().first ) );

				nw_Word[idx] = 0;
			}
			else
				if( nw_State.stringid != 0 )
				{
					SetAttributes( aeString );

					do
					{
						nw_Word[idx] = uc;
						nw_ucqueue.pop_front();
						width = nw_MadEdit->GetUCharWidth( uc );
						nw_Widths[idx] = width;
						nw_LineWidth += width;
						++idx;
						--nw_FirstIndex;
					}
					while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
							&& !IsSpace( uc = nw_ucqueue.front().first ) );

					nw_Word[idx] = 0;
				}
				else
					if( uc < 0x100 )
					{
						if( uc >= '0' && uc <= '9' )
						{
							SetAttributes( aeNumber );

							do
							{
								if(m_SynAttr->m_IsPlainText && ( uc < '0' || uc > '9' )) break;

								nw_Word[idx] = uc;
								nw_ucqueue.pop_front();
								width = nw_MadEdit->GetUCharWidth( uc );
								nw_Widths[idx] = width;
								nw_LineWidth += width;
								++idx;
								--nw_FirstIndex;
							}
							while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
									&& IsNotDelimiter( uc = nw_ucqueue.front().first ));

							nw_Word[idx] = 0;
						}
						else
							if(m_SynAttr->m_SpecialWordPrefix.Find( wxChar( uc ) ) >= 0 )
							{
								do
								{
									if(m_SynAttr->m_IsPlainText && ( uc >= '0' && uc <= '9' )) break;
									nw_Word[idx] = uc;
									nw_ucqueue.pop_front();
									width = nw_MadEdit->GetUCharWidth( uc );
									nw_Widths[idx] = width;
									nw_LineWidth += width;
									++idx;
									--nw_FirstIndex;
								}
								while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
										&& IsNotDelimiter( uc = nw_ucqueue.front().first ) );

								nw_Word[idx] = 0;

								// check if is Keyword
								vector < MadSyntaxKeyword >::iterator kit = m_SynAttr->m_CustomKeyword.begin();
								vector < MadSyntaxKeyword >::iterator kend = m_SynAttr->m_CustomKeyword.end();
								bool bIsKeyword = false;

								if( m_SynAttr->nw_MaxKeywordLen != 0 && idx <= ( int )m_SynAttr->nw_MaxKeywordLen && kit != kend )
								{
									wxString strorg( wxT( ' ' ), idx ), strlower( wxT( ' ' ), idx );
									ucs4_t *puc = nw_Word;

									for( int i = 0; i < idx; ++i )
									{
										uc = *puc++;
#ifdef __WXMSW__

										if( uc < 0x10000 )
#endif
										{
											strorg[i] = wxChar( uc );

											if( uc <= ( ucs4_t )wxT( 'Z' ) && uc >= ( ucs4_t )wxT( 'A' ) )
												strlower[i] = wxChar( uc | 0x20 );
											else
												strlower[i] = wxChar( uc );
										}

#ifdef __WXMSW__
										else
										{
											//to surrogates????
											strorg[i] = wxChar( 0xFFFF );
											strlower[i] = wxChar( 0xFFFF );
										}

#endif
									}

									MadKeywordSet::iterator it;

									do
									{
										if( IsInRange( nw_State.rangeid, kit->m_InRange ) )
										{
											if( kit->m_CaseSensitive ) it = kit->m_Keywords.find( strorg );
											else                     it = kit->m_Keywords.find( strlower );

											if( it != kit->m_Keywords.end() )
											{
												bIsKeyword = true;
												break;
											}
										}
									}
									while( ++kit != kend );
								}

								if( bIsKeyword )
									SetAttributes( &( kit->m_Attr ) );
								else
									SetAttributes( aeSpecialWord );
							}
							else
								if(m_SynAttr->m_KeywordPrefix.Find( wxChar( uc ) ) >= 0 )
								{
									size_t old_firstindex = nw_FirstIndex;
									size_t old_rest_count = nw_RestCount;
									old_line_width = nw_LineWidth;

									// get full word
									do
									{
										nw_Word[idx] = uc;
										//nw_ucqueue.pop_front(); // cannot pop here
										width = nw_MadEdit->GetUCharWidth( uc );
										nw_Widths[idx] = width;
										nw_LineWidth += width;
										++idx;
										--nw_FirstIndex;
									}
									while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
											&& ( uc = nw_ucqueue[idx].first ) < 0x100 && IsNotDelimiter( uc ) );

									nw_Word[idx] = 0;
									// check if is Keyword
									vector < MadSyntaxKeyword >::iterator kit = m_SynAttr->m_CustomKeyword.begin();
									vector < MadSyntaxKeyword >::iterator kend = m_SynAttr->m_CustomKeyword.end();
									bool bIsKeyword = false;

									if( m_SynAttr->nw_MaxKeywordLen != 0 && idx <= ( int )m_SynAttr->nw_MaxKeywordLen && kit != kend )
									{
										wxString strorg( wxT( ' ' ), idx ), strlower( wxT( ' ' ), idx );
										ucs4_t *puc = nw_Word;

										for( int i = 0; i < idx; ++i )
										{
											uc = *puc++;
#ifdef __WXMSW__

											if( uc < 0x10000 )
#endif
											{
												strorg[i] = wxChar( uc );

												if( uc <= ( ucs4_t )wxT( 'Z' ) && uc >= ( ucs4_t )wxT( 'A' ) )
													strlower[i] = wxChar( uc | 0x20 );
												else
													strlower[i] = wxChar( uc );
											}

#ifdef __WXMSW__
											else
											{
												//to surrogates????
												strorg[i] = wxChar( 0xFFFF );
												strlower[i] = wxChar( 0xFFFF );
											}

#endif
										}

										MadKeywordSet::iterator it;

										do
										{
											if( IsInRange( nw_State.rangeid, kit->m_InRange ) )
											{
												if( kit->m_CaseSensitive ) it = kit->m_Keywords.find( strorg );
												else                     it = kit->m_Keywords.find( strlower );

												if( it != kit->m_Keywords.end() )
												{
													bIsKeyword = true;
													break;
												}
											}
										}
										while( ++kit != kend );
									}

									if( bIsKeyword )
									{
										SetAttributes( &( kit->m_Attr ) );
										int i = 0;

										while( i < idx )
										{
											nw_ucqueue.pop_front();
											++i;
										}
									}
									else
									{
										SetAttributes( aeDelimiter );
										idx = 1;
										nw_Word[1] = 0;
										nw_ucqueue.pop_front();
										nw_FirstIndex = old_firstindex - 1;
										nw_RestCount = old_rest_count - 1;
										nw_LineWidth = old_line_width + nw_MadEdit->GetUCharWidth( nw_Word[0] );
									}
								}
								else
									if( IsDelimiter( uc ) )
									{
										SetAttributes( aeDelimiter );

										do
										{
											nw_Word[idx] = uc;
											nw_ucqueue.pop_front();
											width = nw_MadEdit->GetUCharWidth( uc );
											nw_Widths[idx] = width;
											nw_LineWidth += width;
											++idx;
											--nw_FirstIndex;
										}
										while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
												&& IsDelimiter( uc = nw_ucqueue.front().first )
												&& m_SynAttr->m_KeywordPrefix.Find( wxChar( uc ) ) < 0
												&& m_SynAttr->m_SpecialWordPrefix.Find( wxChar( uc ) ) < 0 );

										nw_Word[idx] = 0;
									}
									else // for Keywords
									{
										// get full word
										do
										{
											if(m_SynAttr->m_IsPlainText && ( uc >= '0' && uc <= '9' )) break;
											nw_Word[idx] = uc;
											nw_ucqueue.pop_front();
											width = nw_MadEdit->GetUCharWidth( uc );
											nw_Widths[idx] = width;
											nw_LineWidth += width;
											++idx;
											--nw_FirstIndex;
										}
										while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
												&& IsNotDelimiter( uc = nw_ucqueue.front().first ) );

										nw_Word[idx] = 0;
										// check if is Keyword
										vector < MadSyntaxKeyword >::iterator kit = m_SynAttr->m_CustomKeyword.begin();
										vector < MadSyntaxKeyword >::iterator kend = m_SynAttr->m_CustomKeyword.end();
										bool bIsKeyword = false;

										if( m_SynAttr->nw_MaxKeywordLen != 0 && idx <= ( int )m_SynAttr->nw_MaxKeywordLen && kit != kend )
										{
											wxString strorg( wxT( ' ' ), idx ), strlower( wxT( ' ' ), idx );
											ucs4_t *puc = nw_Word;

											for( int i = 0; i < idx; ++i )
											{
												uc = *puc++;
#ifdef __WXMSW__
												if( uc < 0x10000 )
#endif
												{
													strorg[i] = wxChar( uc );

													if( uc <= ( ucs4_t )wxT( 'Z' ) && uc >= ( ucs4_t )wxT( 'A' ) )
														strlower[i] = wxChar( uc | 0x20 );
													else
														strlower[i] = wxChar( uc );
												}

#ifdef __WXMSW__
												else
												{
													//to surrogates????
													strorg[i] = wxChar( 0xFFFF );
													strlower[i] = wxChar( 0xFFFF );
												}

#endif
											}

											MadKeywordSet::iterator it;

											do
											{
												if( IsInRange( nw_State.rangeid, kit->m_InRange ) )
												{
													if( kit->m_CaseSensitive ) it = kit->m_Keywords.find( strorg );
													else                     it = kit->m_Keywords.find( strlower );

													if( it != kit->m_Keywords.end() )
													{
														bIsKeyword = true;
														break;
													}
												}
											}
											while( ++kit != kend );
										}

										if( bIsKeyword )
										{
											SetAttributes( &( kit->m_Attr ) );
										}
										else
										{
											SetAttributes( aeText );
										}
									}
					}
					else                                                    // Text
					{
						SetAttributes( aeText );

						do
						{
							nw_Word[idx] = uc;
							nw_ucqueue.pop_front();
							width = nw_MadEdit->GetUCharWidth( uc );
							nw_Widths[idx] = width;
							nw_LineWidth += width;
							++idx;
							--nw_FirstIndex;
						}
						while( --nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
								&& ( uc = nw_ucqueue.front().first ) >= 0x100 );

						nw_Word[idx] = 0;
					}

	//wxASSERT(nw_RowIndexIter->m_Width >= nw_LineWidth);
	//if(nw_RowIndexIter->m_Width < nw_LineWidth)
	//    nw_LineWidth += 0;
	wordwidth = nw_LineWidth - old_line_width;

	if( nw_RestCount == 0 )
	{
		// reset bgcolor
		if( nw_NextState.rangeid != nw_State.rangeid )
		{
			nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
		}
		else
			if( nw_NextState.blkcmtid != nw_State.blkcmtid ||
					nw_NextState.stringid != nw_State.stringid )
			{
				if( nw_NextState.directive != 0 )
				{
					nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeDirective].bgcolor;

					if( nw_CurrentBgColor == wxNullColour )
					{
						nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
					}
				}
				else
				{
					if( nw_NextState.rangeid != 0 )
					{
						MadSyntaxRange * r = GetSyntaxRange( nw_NextState.rangeid );
						nw_CurrentBgColor = r->bgcolor;

						if( nw_CurrentBgColor == wxNullColour )
						{
							nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
						}
					}
					else
					{
						nw_CurrentBgColor = m_SynAttr->m_SystemAttributes[aeText].bgcolor;
					}
				}
			}

		nw_State = nw_NextState;
	}

	if( nw_LineWidth >= nw_RowIndexIter->m_Width )
	{
		nw_LineWidth = 0;

		if( ( ++nw_RowIndexIter )->m_Width == 0 )
		{
			nw_EndOfLine = true;
		}
		else
		{
			MadRowIndexIterator nextit = nw_RowIndexIter;
			++nextit;
			if(nextit->m_Start >= nw_RowIndexIter->m_Start)
				nw_MaxLength = nextit->m_Start - nw_RowIndexIter->m_Start;
			else
			{
				wxASSERT(0);
				nw_MaxLength = 0;
			}
		}
	}

	return idx;
}

// for Printing
void MadSyntax::BeginPrint( bool printSyntax )
{
	m_PrintSyntax = printSyntax;

	if( m_PrintSyntax ) return;

	// store colors
	MadAttributes *pat = m_SynAttr->m_SystemAttributes;
	MadAttributes *old = m_old_SystemAttributes;

	for( int i = 0; i < aeNone; ++i )
	{
		old->color = pat->color;
		old->bgcolor = pat->bgcolor;
		pat->color = *wxBLACK;
		pat->bgcolor = *wxWHITE;
		++pat;
		++old;
	}

	size_t count = m_SynAttr->m_CustomRange.size();
	m_CustomRangeBgColor.resize( count );

	for( size_t i = 0; i < count; ++i )
	{
		m_CustomRangeBgColor[i] = m_SynAttr->m_CustomRange[i].bgcolor;
		m_SynAttr->m_CustomRange[i].bgcolor = *wxWHITE;
	}

	count = m_SynAttr->m_CustomKeyword.size();
	m_CustomKeywordColor.resize( count );

	for( size_t i = 0; i < count; ++i )
	{
		m_CustomKeywordColor[i].color = m_SynAttr->m_CustomKeyword[i].m_Attr.color;
		m_CustomKeywordColor[i].bgcolor = m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor;
		m_SynAttr->m_CustomKeyword[i].m_Attr.color = *wxBLACK;
		m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor = *wxWHITE;
	}
}

void MadSyntax::EndPrint()
{
	if( m_PrintSyntax ) return;

	// store colors
	MadAttributes *pat = m_SynAttr->m_SystemAttributes;
	MadAttributes *old = m_old_SystemAttributes;

	for( int i = 0; i < aeNone; ++i )
	{
		pat->color = old->color;
		pat->bgcolor = old->bgcolor;
		++pat;
		++old;
	}

	size_t count = m_SynAttr->m_CustomRange.size();

	for( size_t i = 0; i < count; ++i )
	{
		m_SynAttr->m_CustomRange[i].bgcolor = m_CustomRangeBgColor[i];
	}

	count = m_SynAttr->m_CustomKeyword.size();

	for( size_t i = 0; i < count; ++i )
	{
		m_SynAttr->m_CustomKeyword[i].m_Attr.color = m_CustomKeywordColor[i].color;
		m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor = m_CustomKeywordColor[i].bgcolor;
	}
}

void MadSyntax::LoadAttributes( const wxString &file )
{
	wxString attfile = file;

	if( attfile.IsEmpty() )
	{
		attfile = GetAttributeFileByTitle( m_SynAttr->m_Title );
	}

	if( wxFileExists( attfile ) )
	{
		MadSyntax *att = new MadSyntax( attfile );
		AssignAttributes( att );
		delete att;
	}
}

wxString GetColorName( wxColour &c )
{
	if( !c.Ok() || c == wxNullColour ) return wxString( wxT( "None" ) );

	wxString name = wxTheColourDatabase->FindName( c );

	if( name.IsEmpty() )
	{
		name.Printf( wxT( "%02X%02X%02X" ), c.Red(), c.Green(), c.Blue() );
	}

	return name;
}
wxString GetStyleString( MadFontStyles s )
{
	wxString str;

	if( ( s & fsBold ) != 0 ) str += wxT( " Bold" );

	if( ( s & fsItalic ) != 0 ) str += wxT( " Italic" );

	if( ( s & fsUnderline ) != 0 ) str += wxT( " Underline" );

	return str;
}

void MadSyntax::SaveAttributes( const wxString &file )
{
	wxString attfile = file;

	if( attfile.IsEmpty() )
	{
		attfile = GetAttributeFileByTitle( m_SynAttr->m_Title );
	}

	wxFileName fn( attfile );
	wxString dir = fn.GetPath( wxPATH_GET_VOLUME );

	if( !wxDirExists( dir ) )
	{
		wxMkdir( dir );
	}

	wxFileConfig syn( wxEmptyString, wxEmptyString, attfile, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS );
	// write custom ranges
	wxString str, value;
	size_t i;

	for( i = 0; i < m_SynAttr->m_CustomRange.size(); ++i )
	{
		wxString s, cname;
		cname = GetColorName(m_SynAttr->m_CustomRange[i].bgcolor );
		s.Printf( wxT( " %d b%d e%d %s" ), (unsigned int)(i + 1), (unsigned int)(i + 1), (unsigned int)(i + 1), cname.c_str() );
		str += s;
	}

	syn.Write( wxT( "/CustomRange" ), str );

	// write system attributes
	for( i = aeText; i < aeActiveLine; ++i )
	{
		str.Printf( wxT( "/%sColor" ), SystemAttributesName[i] );
		value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[i].color ).c_str() );
		syn.Write( str, value );
		str.Printf( wxT( "/%sBgColor" ), SystemAttributesName[i] );
		value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[i].bgcolor ).c_str() );
		syn.Write( str, value );
		str.Printf( wxT( "/%sStyle" ), SystemAttributesName[i] );
		value.Printf( wxT( "%s" ), GetStyleString( m_SynAttr->m_SystemAttributes[i].style ).c_str() );
		syn.Write( str, value );
	}

	str.Printf( wxT( "/%sColor" ), SystemAttributesName[aeActiveLine] );
	value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[aeActiveLine].color ).c_str() );
	syn.Write( str, value );
	str.Printf( wxT( "/%sColor" ), SystemAttributesName[aeBookmark] );
	value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[aeBookmark].color ).c_str() );
	syn.Write( str, value );
	str.Printf( wxT( "/%sBgColor" ), SystemAttributesName[aeBookmark] );
	value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[aeBookmark].bgcolor ).c_str() );
	syn.Write( str, value );
	str.Printf( wxT( "/%sColor" ), SystemAttributesName[aeHighlightWord] );
	value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[aeHighlightWord].color ).c_str() );
	syn.Write( str, value );
	str.Printf( wxT( "/%sBgColor" ), SystemAttributesName[aeHighlightWord] );
	value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_SystemAttributes[aeHighlightWord].bgcolor ).c_str() );
	syn.Write( str, value );

	// write custom keywords
	for( i = 0; i < m_SynAttr->m_CustomKeyword.size(); ++i )
	{
		str.Printf( wxT( "/%x.Keyword/Color" ), (unsigned int)(i + 1) );
		value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_CustomKeyword[i].m_Attr.color ).c_str() );
		syn.Write( str, value );
		str.Printf( wxT( "/%x.Keyword/BgColor" ), (unsigned int)(i + 1) );
		value.Printf( wxT( "%s" ), GetColorName( m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor ).c_str() );
		syn.Write( str, value );
		str.Printf( wxT( "/%x.Keyword/Style" ), (unsigned int)(i + 1) );
		value.Printf( wxT( "%s" ), GetStyleString(m_SynAttr->m_CustomKeyword[i].m_Attr.style ).c_str() );
		syn.Write( str, value );
	}
}

void MadSyntax::AssignAttributes( MadSyntax *syn, bool add )
{
	if( this == syn ) return;

	size_t i;

	for( i = 0; i < m_SynAttr->m_CustomRange.size() && i < syn->m_SynAttr->m_CustomRange.size(); ++i )
	{
		m_SynAttr->m_CustomRange[i].bgcolor = syn->m_SynAttr->m_CustomRange[i].bgcolor;
	}

	if( add )
	{
		for( ; i < syn->m_SynAttr->m_CustomRange.size(); ++i )
		{
			MadSyntaxRange ra;
			ra.bgcolor = syn->m_SynAttr->m_CustomRange[i].bgcolor;
			m_SynAttr->m_CustomRange.push_back( ra );
		}
	}

	for( i = aeText; i < aeNone; ++i )
	{
		m_SynAttr->m_SystemAttributes[i].color = syn->m_SynAttr->m_SystemAttributes[i].color;
		m_SynAttr->m_SystemAttributes[i].bgcolor = syn->m_SynAttr->m_SystemAttributes[i].bgcolor;
		m_SynAttr->m_SystemAttributes[i].style = syn->m_SynAttr->m_SystemAttributes[i].style;
	}

	for( i = 0; i < m_SynAttr->m_CustomKeyword.size() && i < syn->m_SynAttr->m_CustomKeyword.size(); ++i )
	{
		m_SynAttr->m_CustomKeyword[i].m_Attr.color = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.color;
		m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor;
		m_SynAttr->m_CustomKeyword[i].m_Attr.style = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.style;
	}

	if( add )
	{
		for( ; i < syn->m_SynAttr->m_CustomKeyword.size(); ++i )
		{
			MadSyntaxKeyword ke;
			ke.m_Attr.color  = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.color;
			ke.m_Attr.bgcolor = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.bgcolor;
			ke.m_Attr.style = syn->m_SynAttr->m_CustomKeyword[i].m_Attr.style;
			m_SynAttr->m_CustomKeyword.push_back( ke );
		}
	}
}

void MadSyntax::DuplicateAttributes()
{
	MadSyntaxAttributes * pCurSynAttr = m_SynAttr.get();
	m_SynAttr.reset(new MadSyntaxAttributes(*pCurSynAttr));
}


