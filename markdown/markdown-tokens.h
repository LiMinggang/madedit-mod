
/*
	Copyright (c) 2009 by Chad Nelson
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef MARKDOWN_TOKENS_H_INCLUDED
#define MARKDOWN_TOKENS_H_INCLUDED

#include "markdown.h"

#include <vector>

namespace markdown {

typedef TokenGroup::iterator TokenGroupIter;
typedef TokenGroup::const_iterator CTokenGroupIter;

class LinkIds {
	public:
	struct Target {
		std::wstring url;
		std::wstring title;

		Target(const std::wstring& url_, const std::wstring& title_):
			url(url_), title(title_) { }
	};

	optional<Target> find(const std::wstring& id) const;
	void add(const std::wstring& id, const std::wstring& url, const
		std::wstring& title);

	private:
	typedef boost::unordered_map<std::wstring, Target> Table;

	static std::wstring _scrubKey(std::wstring str);

	Table mTable;
};

class Token {
	public:
	Token() { }
	virtual ~Token() { }

	virtual void writeAsHtml(std::wostream&) const=0;
	virtual void writeAsOriginal(std::wostream& out) const { writeAsHtml(out); }
	virtual void writeToken(std::wostream& out) const=0;
	virtual void writeToken(size_t indent, std::wostream& out) const {
		out << std::wstring(indent*2, ' ');
		writeToken(out);
	}

	virtual optional<TokenGroup> processSpanElements(const LinkIds& idTable)
		{ return none; }

	virtual optional<const std::wstring&> text() const { return none; }

	virtual bool canContainMarkup() const { return false; }
	virtual bool isBlankLine() const { return false; }
	virtual bool isContainer() const { return false; }
	virtual bool isUnmatchedOpenMarker() const { return false; }
	virtual bool isUnmatchedCloseMarker() const { return false; }
	virtual bool isMatchedOpenMarker() const { return false; }
	virtual bool isMatchedCloseMarker() const { return false; }
	virtual bool inhibitParagraphs() const { return false; }

	protected:
	virtual void preWrite(std::wostream& out) const { }
	virtual void postWrite(std::wostream& out) const { }
};

namespace token {

size_t isValidTag(const std::wstring& tag, bool nonBlockFirst=false);

enum EncodingFlags { cAmps=0x01, cDoubleAmps=0x02, cAngles=0x04, cQuotes=0x08 };

class TextHolder: public Token {
	public:
	TextHolder(const std::wstring& text, bool canContainMarkup, unsigned int
		encodingFlags): mText(text), mCanContainMarkup(canContainMarkup),
		mEncodingFlags(encodingFlags) { }

	virtual void writeAsHtml(std::wostream& out) const;

	virtual void writeToken(std::wostream& out) const { out << L"TextHolder: " << mText << (wchar_t)'\n'; }

	virtual optional<const std::wstring&> text() const { return mText; }

	virtual bool canContainMarkup() const { return mCanContainMarkup; }

	private:
	const std::wstring mText;
	const bool mCanContainMarkup;
	const int mEncodingFlags;
};

class RawText: public TextHolder {
	public:
	RawText(const std::wstring& text, bool canContainMarkup=true):
		TextHolder(text, canContainMarkup, cAmps|cAngles|cQuotes) { }

	virtual void writeToken(std::wostream& out) const { out << L"RawText: " << *text() << (wchar_t)'\n'; }

	virtual optional<TokenGroup> processSpanElements(const LinkIds& idTable);

	private:
	typedef std::vector<TokenPtr> ReplacementTable;

	static std::wstring _processHtmlTagAttributes(std::wstring src, ReplacementTable& replacements);
	static std::wstring _processCodeSpans(std::wstring src, ReplacementTable& replacements);
	static std::wstring _processEscapedCharacters(const std::wstring& src);
	static std::wstring _processLinksImagesAndTags(const std::wstring& src, ReplacementTable& replacements, const LinkIds& idTable);
	static std::wstring _processSpaceBracketedGroupings(const std::wstring& src, ReplacementTable& replacements);
	static TokenGroup _processBoldAndItalicSpans(const std::wstring& src, ReplacementTable& replacements);

	static TokenGroup _encodeProcessedItems(const std::wstring& src, ReplacementTable& replacements);
	static std::wstring _restoreProcessedItems(const std::wstring &src, ReplacementTable& replacements);
};

class HtmlTag: public TextHolder {
	public:
	HtmlTag(const std::wstring& contents): TextHolder(contents, false, cAmps|cAngles) { }

	virtual void writeToken(std::wostream& out) const { out << L"HtmlTag: " << *text() << (wchar_t)'\n'; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << (wchar_t)'<'; }
	virtual void postWrite(std::wostream& out) const { out << (wchar_t)'>'; }
};

class HtmlAnchorTag: public TextHolder {
	public:
	HtmlAnchorTag(const std::wstring& url, const std::wstring& title=std::wstring());

	virtual void writeToken(std::wostream& out) const { out << L"HtmlAnchorTag: " << *text() << (wchar_t)'\n'; }
};

class InlineHtmlContents: public TextHolder {
	public:
	InlineHtmlContents(const std::wstring& contents): TextHolder(contents, false,
		cAmps|cAngles) { }

	virtual void writeToken(std::wostream& out) const { out << L"InlineHtmlContents: " << *text() << (wchar_t)'\n'; }
};

class InlineHtmlComment: public TextHolder {
	public:
	InlineHtmlComment(const std::wstring& contents): TextHolder(contents, false,
		0) { }

	virtual void writeToken(std::wostream& out) const { out << L"InlineHtmlComment: " << *text() << (wchar_t)'\n'; }
};

class CodeBlock: public TextHolder {
	public:
	CodeBlock(const std::wstring& actualContents): TextHolder(actualContents,
		false, cDoubleAmps|cAngles|cQuotes) { }

	virtual void writeAsHtml(std::wostream& out) const;

	virtual void writeToken(std::wostream& out) const { out << L"CodeBlock: " << *text() << (wchar_t)'\n'; }
};

class CodeSpan: public TextHolder {
	public:
	CodeSpan(const std::wstring& actualContents): TextHolder(actualContents,
		false, cDoubleAmps|cAngles|cQuotes) { }

	virtual void writeAsHtml(std::wostream& out) const;
	virtual void writeAsOriginal(std::wostream& out) const;
	virtual void writeToken(std::wostream& out) const { out << L"CodeSpan: " << *text() << (wchar_t)'\n'; }
};

class Header: public TextHolder {
	public:
	Header(size_t level, const std::wstring& text): TextHolder(text, true,
		cAmps|cAngles|cQuotes), mLevel(level) { }

	virtual void writeToken(std::wostream& out) const { out << L"Header " <<
		mLevel << ": " << *text() << (wchar_t)'\n'; }

	virtual bool inhibitParagraphs() const { return true; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"<h" << mLevel << L">"; }
	virtual void postWrite(std::wostream& out) const { out << L"</h" << mLevel << L">\n"; }

	private:
	size_t mLevel;
};

class BlankLine: public TextHolder {
	public:
	BlankLine(const std::wstring& actualContents=std::wstring()):
		TextHolder(actualContents, false, 0) { }

	virtual void writeToken(std::wostream& out) const { out << L"BlankLine: " << *text() << (wchar_t)'\n'; }

	virtual bool isBlankLine() const { return true; }
};



class EscapedCharacter: public Token {
	public:
	EscapedCharacter(wchar_t c): mChar(c) { }

	virtual void writeAsHtml(std::wostream& out) const { out << mChar; }
	virtual void writeAsOriginal(std::wostream& out) const { out << (wchar_t)'\\' << mChar; }
	virtual void writeToken(std::wostream& out) const { out << L"EscapedCharacter: " << mChar << (wchar_t)'\n'; }

	private:
	const wchar_t mChar;
};



class Container: public Token {
	public:
	Container(const TokenGroup& contents=TokenGroup()): mSubTokens(contents),
		mParagraphMode(false) { }

	const TokenGroup& subTokens() const { return mSubTokens; }
	void appendSubtokens(TokenGroup& tokens) { mSubTokens.splice(mSubTokens.end(), tokens); }
	void swapSubtokens(TokenGroup& tokens) { mSubTokens.swap(tokens); }

	virtual bool isContainer() const { return true; }

	virtual void writeAsHtml(std::wostream& out) const;

	virtual void writeToken(std::wostream& out) const { out << L"Container: error!" << (wchar_t)'\n'; }
	virtual void writeToken(size_t indent, std::wostream& out) const;

	virtual optional<TokenGroup> processSpanElements(const LinkIds& idTable);

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new Container(newContents)); }
	virtual std::wstring containerName() const { return L"Container"; }

	protected:
	TokenGroup mSubTokens;
	bool mParagraphMode;
};

class InlineHtmlBlock: public Container {
	public:
	InlineHtmlBlock(const TokenGroup& contents, bool isBlockTag=false):
		Container(contents), mIsBlockTag(isBlockTag) { }
	InlineHtmlBlock(const std::wstring& contents): mIsBlockTag(false) {
		mSubTokens.push_back(TokenPtr(new InlineHtmlContents(contents)));
	}

	virtual bool inhibitParagraphs() const { return !mIsBlockTag; }

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new InlineHtmlBlock(newContents)); }
	virtual std::wstring containerName() const { return L"InlineHtmlBlock"; }

	// Inline HTML blocks always end with a blank line, so report it as one for
	// parsing purposes.
	virtual bool isBlankLine() const { return true; }

	private:
	bool mIsBlockTag;
};

class ListItem: public Container {
	public:
	ListItem(const TokenGroup& contents): Container(contents),
		mInhibitParagraphs(true) { }

	void inhibitParagraphs(bool set) { mInhibitParagraphs=set; }

	virtual bool inhibitParagraphs() const { return mInhibitParagraphs; }

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new ListItem(newContents)); }
	virtual std::wstring containerName() const { return L"ListItem"; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"<li>"; }
	virtual void postWrite(std::wostream& out) const { out << L"</li>\n"; }

	private:
	bool mInhibitParagraphs;
};

class UnorderedList: public Container {
	public:
	UnorderedList(const TokenGroup& contents, bool paragraphMode=false);

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new UnorderedList(newContents)); }
	virtual std::wstring containerName() const { return L"UnorderedList"; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"\n<ul>\n"; }
	virtual void postWrite(std::wostream& out) const { out << L"</ul>\n\n"; }
};

class OrderedList: public UnorderedList {
	public:
	OrderedList(const TokenGroup& contents, bool paragraphMode=false):
		UnorderedList(contents, paragraphMode) { }

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new OrderedList(newContents)); }
	virtual std::wstring containerName() const { return L"OrderedList"; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"<ol>\n"; }
	virtual void postWrite(std::wostream& out) const { out << L"</ol>\n\n"; }
};

class BlockQuote: public Container {
	public:
	BlockQuote(const TokenGroup& contents): Container(contents) { }

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new BlockQuote(newContents)); }
	virtual std::wstring containerName() const { return L"BlockQuote"; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"<blockquote>\n"; }
	virtual void postWrite(std::wostream& out) const { out << L"\n</blockquote>\n"; }
};

class Paragraph: public Container {
	public:
	Paragraph() { }
	Paragraph(const TokenGroup& contents): Container(contents) { }

	virtual TokenPtr clone(const TokenGroup& newContents) const { return TokenPtr(new Paragraph(newContents)); }
	virtual std::wstring containerName() const { return L"Paragraph"; }

	protected:
	virtual void preWrite(std::wostream& out) const { out << L"<p>"; }
	virtual void postWrite(std::wostream& out) const { out << L"</p>\n\n"; }
};



class BoldOrItalicMarker: public Token {
	public:
	BoldOrItalicMarker(bool open, wchar_t c, size_t size): mOpenMarker(open),
		mTokenCharacter(c), mSize(size), mMatch(0), mCannotMatch(false),
		mDisabled(false), mId(-1) { }

	virtual bool isUnmatchedOpenMarker() const { return (mOpenMarker && mMatch==0 && !mCannotMatch); }
	virtual bool isUnmatchedCloseMarker() const { return (!mOpenMarker && mMatch==0 && !mCannotMatch); }
	virtual bool isMatchedOpenMarker() const { return (mOpenMarker && mMatch!=0); }
	virtual bool isMatchedCloseMarker() const { return (!mOpenMarker && mMatch!=0); }
	virtual void writeAsHtml(std::wostream& out) const;
	virtual void writeToken(std::wostream& out) const;

	bool isOpenMarker() const { return mOpenMarker; }
	wchar_t tokenCharacter() const { return mTokenCharacter; }
	size_t size() const { return mSize; }
	bool matched() const { return (mMatch!=0); }
	BoldOrItalicMarker* matchedTo() const { return mMatch; }
	int id() const { return mId; }

	void matched(BoldOrItalicMarker *match, int id=-1) { mMatch=match; mId=id; }
	void cannotMatch(bool set) { mCannotMatch=set; }
	void disable() { mCannotMatch=mDisabled=true; }

	private:
	bool mOpenMarker; // Otherwise it's a close-marker
	wchar_t mTokenCharacter; // Underscore or asterisk
	size_t mSize; // 1=italics, 2=bold, 3=both
	BoldOrItalicMarker* mMatch;
	bool mCannotMatch;
	bool mDisabled;
	int mId;
};

class Image: public Token {
	public:
	Image(const std::wstring& altText, const std::wstring& url, const std::wstring&
		title): mAltText(altText), mUrl(url), mTitle(title) { }

	virtual void writeAsHtml(std::wostream& out) const;

	virtual void writeToken(std::wostream& out) const { out << L"Image: " << mUrl << (wchar_t)'\n'; }

	private:
	const std::wstring mAltText, mUrl, mTitle;
};

} // namespace token
} // namespace markdown

#endif // MARKDOWN_TOKENS_H_INCLUDED
