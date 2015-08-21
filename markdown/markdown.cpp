
/*
	Copyright (c) 2009 by Chad Nelson
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include "markdown.h"
#include "markdown-tokens.h"

#include <sstream>
#include <cassert>

#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/xpressive/traits/null_regex_traits.hpp>
#include <boost/xpressive/traits/cpp_regex_traits.hpp>

using std::cerr;
using std::endl;

using boost::optional;
using boost::none;
namespace xpressive = boost::xpressive;
using markdown::TokenPtr;
using markdown::CTokenGroupIter;

namespace {

struct HtmlTagInfo {
	std::wstring tagName, extra;
	bool isClosingTag;
	size_t lengthOfToken; // In original string
};

const std::wstring cHtmlTokenSource(L"<((/?)([a-zA-Z0-9]+)(?:( +[a-zA-Z0-9]+?(?: ?= ?(\"|').*?\\5))*? */? *))>");
xpressive::wsregex cHtmlTokenExpression = xpressive::wsregex::compile(cHtmlTokenSource),
	cStartHtmlTokenExpression = xpressive::wsregex::compile(L"^"+cHtmlTokenSource),
	cOneHtmlTokenExpression = xpressive::wsregex::compile(L"^"+cHtmlTokenSource+L"$");

enum ParseHtmlTagFlags { cAlone, cStarts };

optional<HtmlTagInfo> parseHtmlTag(std::wstring::const_iterator begin,
	std::wstring::const_iterator end, ParseHtmlTagFlags flags)
{
	xpressive::wsmatch m;
	if (xpressive::regex_search(begin, end, m, (flags==cAlone ?
		cOneHtmlTokenExpression : cStartHtmlTokenExpression)))
	{
		HtmlTagInfo r;
		r.tagName=m[3];
		if (m[4].matched) r.extra=m[4];
		r.isClosingTag=(m[2].length()>0);
		r.lengthOfToken=m[0].length();
		return r;
	}
	return none;
}

markdown::TokenGroup parseInlineHtmlText(const std::wstring& src) {
	markdown::TokenGroup r;
	std::wstring::const_iterator prev=src.begin(), end=src.end();
	while (1) {
		xpressive::wsmatch m;
		if (xpressive::regex_search(prev, end, m, cHtmlTokenExpression)) {
			if (prev!=m[0].first) {
				//cerr << "  Non-tag (" << std::distance(prev, m[0].first) << "): " << std::wstring(prev, m[0].first) << endl;
				r.push_back(TokenPtr(new markdown::token::InlineHtmlContents(std::wstring(prev, m[0].first))));
			}
			//cerr << "  Tag: " << m[1] << endl;
			r.push_back(TokenPtr(new markdown::token::HtmlTag(m[1])));
			prev=m[0].second;
		} else {
			std::wstring eol;
			if (prev!=end) {
				eol=std::wstring(prev, end);
				//cerr << "  Non-tag: " << eol << endl;
			}
			eol+=(wchar_t)'\n';
			r.push_back(TokenPtr(new markdown::token::InlineHtmlContents(eol)));
			break;
		}
	}
	return r;
}

bool isHtmlCommentStart(std::wstring::const_iterator begin,
	std::wstring::const_iterator end)
{
	// It can't be a single-line comment, those will already have been parsed
	// by isBlankLine.
	static const xpressive::wsregex cExpression = xpressive::wsregex::compile(L"^<!--");
	return xpressive::regex_search(begin, end, cExpression);
}

bool isHtmlCommentEnd(std::wstring::const_iterator begin,
	std::wstring::const_iterator end)
{
	static const xpressive::wsregex cExpression = xpressive::wsregex::compile(L".*-- *>$");
	return xpressive::regex_match(begin, end, cExpression);
}

bool isBlankLine(const std::wstring& line) {
	static const xpressive::wsregex cExpression = xpressive::wsregex::compile(L" {0,3}(<--(.*)-- *> *)* *");
	return xpressive::regex_match(line, cExpression);
}

optional<TokenPtr> parseInlineHtml(CTokenGroupIter& i, CTokenGroupIter end) {
	// Preconditions: Previous line was blank, or this is the first line.
	if ((*i)->text()) {
		const std::wstring& line(*(*i)->text());

		bool tag=false, comment=false;
		optional<HtmlTagInfo> tagInfo=parseHtmlTag(line.begin(), line.end(), cStarts);
		if (tagInfo && markdown::token::isValidTag(tagInfo->tagName)>1) {
			tag=true;
		} else if (isHtmlCommentStart(line.begin(), line.end())) {
			comment=true;
		}

		if (tag) {
			// Block continues until an HTML tag (alone) on a line followed by a
			// blank line.
			markdown::TokenGroup contents;
			CTokenGroupIter firstLine=i, prevLine=i;
			size_t lines=0;

			bool done=false;
			do {
				// We encode HTML tags so that their contents gets properly
				// handled -- i.e. "<div style=">"/>" becomes <div style="&gt;"/>
				if ((*i)->text()) {
					markdown::TokenGroup t=parseInlineHtmlText(*(*i)->text());
					contents.splice(contents.end(), t);
				} else contents.push_back(*i);

				prevLine=i;
				++i;
				++lines;

				if (i!=end && (*i)->isBlankLine() && (*prevLine)->text()) {
					if (prevLine==firstLine) {
						done=true;
					} else {
						const std::wstring& text(*(*prevLine)->text());
						if (parseHtmlTag(text.begin(), text.end(), cAlone)) done=true;
					}
				}
			} while (i!=end && !done);

			if (lines>1 || markdown::token::isValidTag(tagInfo->tagName, true)>1) {
				i=prevLine;
				return TokenPtr(new markdown::token::InlineHtmlBlock(contents));
			} else {
				// Single-line HTML "blocks" whose initial tags are span-tags
				// don't qualify as inline HTML.
				i=firstLine;
				return none;
			}
		} else if (comment) {
			// Comment continues until a closing tag is found; at present, it
			// also has to be the last thing on the line, and has to be
			// immediately followed by a blank line too.
			markdown::TokenGroup contents;
			CTokenGroupIter firstLine=i, prevLine=i;

			bool done=false;
			do {
				if ((*i)->text()) contents.push_back(TokenPtr(new markdown::token::InlineHtmlComment(*(*i)->text()+(wchar_t)'\n')));
				else contents.push_back(*i);

				prevLine=i;
				++i;

				if (i!=end && (*i)->isBlankLine() && (*prevLine)->text()) {
					if (prevLine==firstLine) {
						done=true;
					} else {
						const std::wstring& text(*(*prevLine)->text());
						if (isHtmlCommentEnd(text.begin(), text.end())) done=true;
					}
				}
			} while (i!=end && !done);
			i=prevLine;
			return TokenPtr(new markdown::token::InlineHtmlBlock(contents));
		}
	}

	return none;
}

optional<std::wstring> isCodeBlockLine(CTokenGroupIter& i, CTokenGroupIter end) {
	if ((*i)->isBlankLine()) {
		// If we get here, we're already in a code block.
		++i;
		if (i!=end) {
			optional<std::wstring> r=isCodeBlockLine(i, end);
			if (r) return std::wstring(L"\n"+*r);
		}
		--i;
	} else if ((*i)->text() && (*i)->canContainMarkup()) {
		const std::wstring& line(*(*i)->text());
		if (line.length()>=4) {
			std::wstring::const_iterator si=line.begin(), sie=si+4;
			while (si!=sie && *si==(wchar_t)' ') ++si;
			if (si==sie) {
				++i;
				return std::wstring(si, line.end());
			}
		}
	}
	return none;
}

optional<TokenPtr> parseCodeBlock(CTokenGroupIter& i, CTokenGroupIter end) {
	if (!(*i)->isBlankLine()) {
		optional<std::wstring> contents=isCodeBlockLine(i, end);
		if (contents) {
			std::wostringstream out;
			out << *contents << (wchar_t)'\n';
			while (i!=end) {
				contents=isCodeBlockLine(i, end);
				if (contents) out << *contents << (wchar_t)'\n';
				else break;
			}
			return TokenPtr(new markdown::token::CodeBlock(out.str()));
		}
	}
	return none;
}



size_t countQuoteLevel(const std::wstring& prefixString) {
	size_t r=0;
	for (std::wstring::const_iterator qi=prefixString.begin(),
		qie=prefixString.end(); qi!=qie; ++qi)
			if (*qi==(wchar_t)'>') ++r;
	return r;
}

optional<TokenPtr> parseBlockQuote(CTokenGroupIter& i, CTokenGroupIter end) {
	static const xpressive::wsregex cBlockQuoteExpression = xpressive::wsregex::compile(L"^((?: {0,3}>)+) (.*)$");
	// Useful captures: 1=prefix, 2=content

	if (!(*i)->isBlankLine() && (*i)->text() && (*i)->canContainMarkup()) {
		const std::wstring& line(*(*i)->text());
		xpressive::wsmatch m;
		if (xpressive::regex_match(line, m, cBlockQuoteExpression)) {
			size_t quoteLevel=countQuoteLevel(m[1]);
			xpressive::wsregex continuationExpression=xpressive::wsregex::compile(L"^((?: {0,3}>){"+boost::lexical_cast<std::wstring>(quoteLevel)+L"}) ?(.*)$");

			markdown::TokenGroup subTokens;
			subTokens.push_back(TokenPtr(new markdown::token::RawText(m[2])));

			// The next line can be a continuation of this quote (with or
			// without the prefix string) or a blank line. Blank lines are
			// treated as part of this quote if the following line is a
			// properly-prefixed quote line too, otherwise they terminate the
			// quote.
			++i;
			while (i!=end) {
				if ((*i)->isBlankLine()) {
					CTokenGroupIter ii=i;
					++ii;
					if (ii==end) {
						i=ii;
						break;
					} else {
						const std::wstring& line(*(*ii)->text());
						if (xpressive::regex_match(line, m, continuationExpression)) {
							if (m[1].matched && m[1].length()>0) {
								i=++ii;
								subTokens.push_back(TokenPtr(new markdown::token::BlankLine));
								subTokens.push_back(TokenPtr(new markdown::token::RawText(m[2])));
							} else break;
						} else break;
					}
				} else {
					const std::wstring& line(*(*i)->text());
					if (xpressive::regex_match(line, m, continuationExpression)) {
						assert(m[2].matched);
						if (!isBlankLine(m[2])) subTokens.push_back(TokenPtr(new markdown::token::RawText(m[2])));
						else subTokens.push_back(TokenPtr(new markdown::token::BlankLine(m[2])));
						++i;
					} else break;
				}
			}

			return TokenPtr(new markdown::token::BlockQuote(subTokens));
		}
	}
	return none;
}

optional<TokenPtr> parseListBlock(CTokenGroupIter& i, CTokenGroupIter end, bool sub=false) {
	static const xpressive::wsregex cUnorderedListExpression = xpressive::wsregex::compile(L"^( *)([*+-]) +([^*-].*)$");
	static const xpressive::wsregex cOrderedListExpression = xpressive::wsregex::compile(L"^( *)([0-9]+)\\. +(.*)$");

	enum ListType { cNone, cUnordered, cOrdered };
	ListType type=cNone;
	if (!(*i)->isBlankLine() && (*i)->text() && (*i)->canContainMarkup()) {
		xpressive::wsregex nextItemExpression, startSublistExpression;
		size_t indent=0;

		const std::wstring& line(*(*i)->text());

		//cerr << "IsList? " << line << endl;

		markdown::TokenGroup subTokens, subItemTokens;

		xpressive::wsmatch m;
		if (xpressive::regex_match(line, m, cUnorderedListExpression)) {
			indent=m[1].length();
			if (sub || indent<4) {
				type=cUnordered;
				wchar_t startChar=*m[2].first;
				subItemTokens.push_back(TokenPtr(new markdown::token::RawText(m[3])));

				std::wostringstream next;
				next << L"^" << std::wstring(indent, (wchar_t)' ') << L"\\" << startChar << L" +([^*-].*)$";
				nextItemExpression=xpressive::wsregex::compile(next.str());
			}
		} else if (xpressive::regex_match(line, m, cOrderedListExpression)) {
			indent=m[1].length();
			if (sub || indent<4) {
				type=cOrdered;
				subItemTokens.push_back(TokenPtr(new markdown::token::RawText(m[3])));

				std::wostringstream next;
				next << L"^" << std::wstring(indent, (wchar_t)' ') << L"[0-9]+\\. +(.*)$";
				nextItemExpression=xpressive::wsregex::compile(next.str());
			}
		}

		if (type!=cNone) {
			CTokenGroupIter originalI=i;
			size_t itemCount=1;
			std::wostringstream sub;
			sub << L"^" << std::wstring(indent, (wchar_t)' ') << L" +(([*+-])|([0-9]+\\.)) +.*$";
			startSublistExpression=xpressive::wsregex::compile(sub.str());

			// There are several options for the next line. It's another item in
			// this list (in which case this one is done); it's a continuation
			// of this line (collect it and keep going); it's the first item in
			// a sub-list (call this function recursively to collect it), it's
			// the next item in the parent list (this one is ended); or it's
			// blank.
			//
			// A blank line requires looking ahead. If the next line is an item
			// for this list, then switch this list into paragraph-items mode
			// and continue processing. If it's indented by four or more spaces
			// (more than the list itself), then it's another continuation of
			// the current item. Otherwise it's either a new paragraph (and this
			// list is ended) or the beginning of a sub-list.
			static const xpressive::wsregex cContinuedItemExpression=xpressive::wsregex::compile(L"^ *([^ ].*)$");

			xpressive::wsregex continuedAfterBlankLineExpression=xpressive::wsregex::compile(L"^ {"+
				boost::lexical_cast<std::wstring>(indent+4)+L"}([^ ].*)$");
			xpressive::wsregex codeBlockAfterBlankLineExpression=xpressive::wsregex::compile(L"^ {"+
				boost::lexical_cast<std::wstring>(indent+8)+L"}(.*)$");

			enum NextItemType { cUnknown, cEndOfList, cAnotherItem };
			NextItemType nextItem=cUnknown;
			bool setParagraphMode=false;

			++i;
			while (i!=end) {
				if ((*i)->isBlankLine()) {
					CTokenGroupIter ii=i;
					++ii;
					if (ii==end) {
						i=ii;
						nextItem=cEndOfList;
					} else if ((*ii)->text()) {
						const std::wstring& line(*(*ii)->text());
						if (xpressive::regex_match(line, startSublistExpression)) {
							setParagraphMode=true;
							++itemCount;
							i=ii;
							optional<TokenPtr> p=parseListBlock(i, end, true);
							assert(p);
							subItemTokens.push_back(*p);
							continue;
						} else if (xpressive::regex_match(line, m, nextItemExpression)) {
							setParagraphMode=true;
							i=ii;
							nextItem=cAnotherItem;
						} else if (xpressive::regex_match(line, m, continuedAfterBlankLineExpression)) {
							assert(m[1].matched);
							subItemTokens.push_back(TokenPtr(new markdown::token::BlankLine()));
							subItemTokens.push_back(TokenPtr(new markdown::token::RawText(m[1])));
							i=++ii;
							continue;
						} else if (xpressive::regex_match(line, m, codeBlockAfterBlankLineExpression)) {
							setParagraphMode=true;
							++itemCount;
							assert(m[1].matched);
							subItemTokens.push_back(TokenPtr(new markdown::token::BlankLine()));

							std::wstring codeBlock=m[1]+(wchar_t)'\n';
							++ii;
							while (ii!=end) {
								if ((*ii)->isBlankLine()) {
									CTokenGroupIter iii=ii;
									++iii;
									const std::wstring& nextLine(*(*iii)->text());
									if (xpressive::regex_match(nextLine, m, codeBlockAfterBlankLineExpression)) {
										codeBlock+=(wchar_t)'\n'+m[1]+(wchar_t)'\n';
										ii=iii;
									} else break;
								} else if ((*ii)->text()) {
									const std::wstring& line(*(*ii)->text());
									if (xpressive::regex_match(line, m, codeBlockAfterBlankLineExpression)) {
										codeBlock+=m[1]+(wchar_t)'\n';
									} else break;
								} else break;
								++ii;
							}

							subItemTokens.push_back(TokenPtr(new markdown::token::CodeBlock(codeBlock)));
							i=ii;
							continue;
						} else {
							nextItem=cEndOfList;
						}
					} else break;
				} else if ((*i)->text()) {
					const std::wstring& line(*(*i)->text());
					if (xpressive::regex_match(line, startSublistExpression)) {
						++itemCount;
						optional<TokenPtr> p=parseListBlock(i, end, true);
						assert(p);
						subItemTokens.push_back(*p);
						continue;
					} else if (xpressive::regex_match(line, m, nextItemExpression)) {
						nextItem=cAnotherItem;
					} else {
						if (xpressive::regex_match(line, m, cUnorderedListExpression)
							|| xpressive::regex_match(line, m, cOrderedListExpression))
						{
							// Belongs to the parent list
							nextItem=cEndOfList;
						} else {
							xpressive::regex_match(line, m, cContinuedItemExpression);
							assert(m[1].matched);
							subItemTokens.push_back(TokenPtr(new markdown::token::RawText(m[1])));
							++i;
							continue;
						}
					}
				} else nextItem=cEndOfList;

				if (!subItemTokens.empty()) {
					subTokens.push_back(TokenPtr(new markdown::token::ListItem(subItemTokens)));
					subItemTokens.clear();
				}

				assert(nextItem!=cUnknown);
				if (nextItem==cAnotherItem) {
					subItemTokens.push_back(TokenPtr(new markdown::token::RawText(m[1])));
					++itemCount;
					++i;
				} else { // nextItem==cEndOfList
					break;
				}
			}

			// In case we hit the end with an unterminated item...
			if (!subItemTokens.empty()) {
				subTokens.push_back(TokenPtr(new markdown::token::ListItem(subItemTokens)));
				subItemTokens.clear();
			}

			if (itemCount>1 || indent!=0) {
				if (type==cUnordered) {
					return TokenPtr(new markdown::token::UnorderedList(subTokens, setParagraphMode));
				} else {
					return TokenPtr(new markdown::token::OrderedList(subTokens, setParagraphMode));
				}
			} else {
				// It looked like a list, but turned out to be a false alarm.
				i=originalI;
				return none;
			}
		}
	}
	return none;
}

bool parseReference(CTokenGroupIter& i, CTokenGroupIter end, markdown::LinkIds &idTable) {
	if ((*i)->text()) {
		static const xpressive::wsregex cReference=xpressive::wsregex::compile(L"^ {0,3}\\[(.+)\\]: +<?([^ >]+)>?(?: *(?:('|\")(.*)\\3)|(?:\\((.*)\\)))?$");
		// Useful captures: 1=id, 2=url, 4/5=title

		const std::wstring& line1(*(*i)->text());
		xpressive::wsmatch m;
		if (xpressive::regex_match(line1, m, cReference)) {
			std::wstring id(m[1]), url(m[2]), title;
			if (m[4].matched) title=m[4];
			else if (m[5].matched) title=m[5];
			else {
				CTokenGroupIter ii=i;
				++ii;
				if (ii!=end && (*ii)->text()) {
					// It could be on the next line
					static const xpressive::wsregex cSeparateTitle=xpressive::wsregex::compile(L"^ *(?:(?:('|\")(.*)\\1)|(?:\\((.*)\\))) *$");
					// Useful Captures: 2/3=title

					const std::wstring& line2(*(*ii)->text());
					if (xpressive::regex_match(line2, m, cSeparateTitle)) {
						++i;
						title=(m[2].matched ? m[2] : m[3]);
					}
				}
			}

			idTable.add(id, url, title);
			return true;
		}
	}
	return false;
}

void flushParagraph(std::wstring& paragraphText, markdown::TokenGroup&
	paragraphTokens, markdown::TokenGroup& finalTokens, bool noParagraphs)
{
	if (!paragraphText.empty()) {
		paragraphTokens.push_back(TokenPtr(new markdown::token::RawText(paragraphText)));
		paragraphText.clear();
	}

	if (!paragraphTokens.empty()) {
		if (noParagraphs) {
			if (paragraphTokens.size()>1) {
				finalTokens.push_back(TokenPtr(new markdown::token::Container(paragraphTokens)));
			} else finalTokens.push_back(*paragraphTokens.begin());
		} else finalTokens.push_back(TokenPtr(new markdown::token::Paragraph(paragraphTokens)));
		paragraphTokens.clear();
	}
}

optional<TokenPtr> parseHeader(CTokenGroupIter& i, CTokenGroupIter end) {
	if (!(*i)->isBlankLine() && (*i)->text() && (*i)->canContainMarkup()) {
		// Hash-mark type
		static const xpressive::wsregex cHashHeaders=xpressive::wsregex::compile(L"^(#{1,6}) +(.*?) *#*$");
		const std::wstring& line=*(*i)->text();
		xpressive::wsmatch m;
		if (xpressive::regex_match(line, m, cHashHeaders))
			return TokenPtr(new markdown::token::Header(m[1].length(), m[2]));

		// Underlined type
		CTokenGroupIter ii=i;
		++ii;
		if (ii!=end && !(*ii)->isBlankLine() && (*ii)->text() && (*ii)->canContainMarkup()) {
			static const xpressive::wsregex cUnderlinedHeaders=xpressive::wsregex::compile(L"^([-=])\\1*$");
			const std::wstring& line=*(*ii)->text();
			if (xpressive::regex_match(line, m, cUnderlinedHeaders)) {
				wchar_t typeChar=std::wstring(m[1])[0];
				TokenPtr p=TokenPtr(new markdown::token::Header((typeChar==(wchar_t)'='
					? 1 : 2), *(*i)->text()));
				i=ii;
				return p;
			}
		}
	}
	return none;
}

optional<TokenPtr> parseHorizontalRule(CTokenGroupIter& i, CTokenGroupIter end) {
	if (!(*i)->isBlankLine() && (*i)->text() && (*i)->canContainMarkup()) {
		static const xpressive::wsregex cHorizontalRules=xpressive::wsregex::compile(L"^ {0,3}((?:-|\\*|_) *){3,}$");
		const std::wstring& line=*(*i)->text();
		if (xpressive::regex_match(line, cHorizontalRules)) {
			return TokenPtr(new markdown::token::HtmlTag(L"hr/"));
		}
	}
	return none;
}

} // namespace



namespace markdown {

optional<LinkIds::Target> LinkIds::find(const std::wstring& id) const {
	Table::const_iterator i=mTable.find(_scrubKey(id));
	if (i!=mTable.end()) return i->second;
	else return none;
}

void LinkIds::add(const std::wstring& id, const std::wstring& url, const
	std::wstring& title)
{
	mTable.insert(std::make_pair(_scrubKey(id), Target(url, title)));
}

std::wstring LinkIds::_scrubKey(std::wstring str) {
	boost::algorithm::to_lower(str);
	return str;
}



const size_t Document::cSpacesPerInitialTab=4; // Required by Markdown format
const size_t Document::cDefaultSpacesPerTab=cSpacesPerInitialTab;

Document::Document(size_t spacesPerTab): cSpacesPerTab(spacesPerTab),
	mTokenContainer(new token::Container), mIdTable(new LinkIds),
	mProcessed(false)
{
	// This space deliberately blank ;-)
}

Document::Document(std::wistream& in, size_t spacesPerTab):
	cSpacesPerTab(spacesPerTab), mTokenContainer(new token::Container),
	mIdTable(new LinkIds), mProcessed(false)
{
	read(in);
}

Document::~Document() {
	delete mIdTable;
}

bool Document::read(const std::wstring& src) {
	std::wistringstream in(src);
	return read(in);
}

bool Document::_getline(std::wistream& in, std::wstring& line) {
	// Handles \n, \r, and \r\n (and even \n\r) on any system. Also does tab-
	// expansion, since this is the most efficient place for it.
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)        
    line = std::wstring(L"");
#else
	line.clear();
#endif

	bool initialWhitespace=true;
	wchar_t c;
	while (in.get(c)) {
		if (c==(wchar_t)'\r') {
			if ((in.get(c)) && c!=(wchar_t)'\n') in.unget();
			return true;
		} else if (c==(wchar_t)'\n') {
			if ((in.get(c)) && c!=(wchar_t)'\r') in.unget();
			return true;
		} else if (c==(wchar_t)'\t') {
			size_t convert=(initialWhitespace ? cSpacesPerInitialTab :
				cSpacesPerTab);
			line+=std::wstring(convert-(line.length()%convert), (wchar_t)' ');
		} else {
			line.push_back(c);
			if (c!=(wchar_t)' ') initialWhitespace=false;
		}
	}
	return !line.empty();
}

bool Document::read(std::wistream& in) {
	if (mProcessed) return false;

	token::Container *tokens=dynamic_cast<token::Container*>(mTokenContainer.get());
	assert(tokens!=0);

	std::wstring line;
	TokenGroup tgt;
	while (_getline(in, line)) {
		if (isBlankLine(line)) {
			tgt.push_back(TokenPtr(new token::BlankLine(line)));
		} else {
			tgt.push_back(TokenPtr(new token::RawText(line)));
		}
	}
	tokens->appendSubtokens(tgt);

	return true;
}

void Document::write(std::wostream& out) {
    try {
    	_process();
    	mTokenContainer->writeAsHtml(out);
    }
    catch(...)
    {
        out<< L"\n\n\nInvalid Markdown format!!";
    }
}

void Document::writeTokens(std::wostream& out) {
	_process();
	mTokenContainer->writeToken(0, out);
}

void Document::_process() {
	if (!mProcessed) {
		_mergeMultilineHtmlTags();
		_processInlineHtmlAndReferences();
		_processBlocksItems(mTokenContainer);
		_processParagraphLines(mTokenContainer);
		mTokenContainer->processSpanElements(*mIdTable);
		mProcessed=true;
	}
}

void Document::_mergeMultilineHtmlTags() {
	static const xpressive::wsregex cHtmlTokenStart=xpressive::wsregex::compile(L"<((/?)([a-zA-Z0-9]+)(?:( +[a-zA-Z0-9]+?(?: ?= ?(\"|').*?\\5))*? */? *))$");
	static const xpressive::wsregex cHtmlTokenEnd=xpressive::wsregex::compile(L"^ *((?:( +[a-zA-Z0-9]+?(?: ?= ?(\"|').*?\\3))*? */? *))>");

	TokenGroup processed;

	token::Container *tokens=dynamic_cast<token::Container*>(mTokenContainer.get());
	assert(tokens!=0);

	for (TokenGroup::const_iterator i=tokens->subTokens().begin(),
		ie=tokens->subTokens().end(); i!=ie; ++i)
	{
		if ((*i)->text() && xpressive::regex_match(*(*i)->text(), cHtmlTokenStart)) {
			TokenGroup::const_iterator i2=i;
			++i2;
			if (i2!=tokens->subTokens().end() && (*i2)->text() &&
				xpressive::regex_match(*(*i2)->text(), cHtmlTokenEnd))
			{
				processed.push_back(TokenPtr(new markdown::token::RawText(*(*i)->text()+(wchar_t)' '+*(*i2)->text())));
				++i;
				continue;
			}
		}
		processed.push_back(*i);
	}
	tokens->swapSubtokens(processed);
}

void Document::_processInlineHtmlAndReferences() {
	TokenGroup processed;

	token::Container *tokens=dynamic_cast<token::Container*>(mTokenContainer.get());
	assert(tokens!=0);

	for (TokenGroup::const_iterator ii=tokens->subTokens().begin(),
		iie=tokens->subTokens().end(); ii!=iie; ++ii)
	{
		if ((*ii)->text()) {
			if (processed.empty() || processed.back()->isBlankLine()) {
				optional<TokenPtr> inlineHtml=parseInlineHtml(ii, iie);
				if (inlineHtml) {
					processed.push_back(*inlineHtml);
					if (ii==iie) break;
					continue;
				}
			}

			if (parseReference(ii, iie, *mIdTable)) {
				if (ii==iie) break;
				continue;
			}

			// If it gets down here, just store it in its current (raw text)
			// form. We'll group the raw text lines into paragraphs in a
			// later pass, since we can't easily tell where paragraphs
			// end until then.
		}
		processed.push_back(*ii);
	}
	tokens->swapSubtokens(processed);
}

void Document::_processBlocksItems(TokenPtr inTokenContainer) {
	if (!inTokenContainer->isContainer()) return;

	token::Container *tokens=dynamic_cast<token::Container*>(inTokenContainer.get());
	assert(tokens!=0);

	TokenGroup processed;

	for (TokenGroup::const_iterator ii=tokens->subTokens().begin(),
		iie=tokens->subTokens().end(); ii!=iie; ++ii)
	{
		if ((*ii)->text()) {
			optional<TokenPtr> subitem;
			if (!subitem) subitem=parseHeader(ii, iie);
			if (!subitem) subitem=parseHorizontalRule(ii, iie);
			if (!subitem) subitem=parseListBlock(ii, iie);
			if (!subitem) subitem=parseBlockQuote(ii, iie);
			if (!subitem) subitem=parseCodeBlock(ii, iie);

			if (subitem) {
				_processBlocksItems(*subitem);
				processed.push_back(*subitem);
				if (ii==iie) break;
				continue;
			} else processed.push_back(*ii);
		} else if ((*ii)->isContainer()) {
			_processBlocksItems(*ii);
			processed.push_back(*ii);
		}
	}
	tokens->swapSubtokens(processed);
}

void Document::_processParagraphLines(TokenPtr inTokenContainer) {
	token::Container *tokens=dynamic_cast<token::Container*>(inTokenContainer.get());
	assert(tokens!=0);

	bool noPara=tokens->inhibitParagraphs();
	for (TokenGroup::const_iterator ii=tokens->subTokens().begin(),
		iie=tokens->subTokens().end(); ii!=iie; ++ii)
			if ((*ii)->isContainer()) _processParagraphLines(*ii);

	TokenGroup processed;
	std::wstring paragraphText;
	TokenGroup paragraphTokens;
	for (TokenGroup::const_iterator ii=tokens->subTokens().begin(),
		iie=tokens->subTokens().end(); ii!=iie; ++ii)
	{
		if ((*ii)->text() && (*ii)->canContainMarkup() && !(*ii)->inhibitParagraphs()) {
			static const xpressive::wsregex cExpression=xpressive::wsregex::compile(L"^(.*)  $");
			if (!paragraphText.empty()) paragraphText+=L" ";

			xpressive::wsmatch m;
			if (xpressive::regex_match(*(*ii)->text(), m, cExpression)) {
				paragraphText += m[1];
				flushParagraph(paragraphText, paragraphTokens, processed, noPara);
				processed.push_back(TokenPtr(new markdown::token::HtmlTag(L"br/")));
			} else paragraphText += *(*ii)->text();
		} else {
			flushParagraph(paragraphText, paragraphTokens, processed, noPara);
			processed.push_back(*ii);
		}
	}

	// Make sure the last paragraph is properly flushed too.
	flushParagraph(paragraphText, paragraphTokens, processed, noPara);

	tokens->swapSubtokens(processed);
}

} // namespace markdown
