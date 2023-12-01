// ASLocalizer.h
// Copyright (c) 2023 The Artistic Style Authors.
// This code is licensed under the MIT License.
// License.md describes the conditions under which this software may be distributed.


#ifndef ASLOCALIZER_H
#define ASLOCALIZER_H

#include <string>
#include <vector>

// library builds do not need ASLocalizer
#ifdef ASTYLE_JNI
	#ifndef ASTYLE_LIB    // ASTYLE_LIB must be defined for ASTYLE_JNI
		#define ASTYLE_LIB
	#endif
#endif  //  ASTYLE_JNI

namespace astyle {

#ifndef ASTYLE_LIB

//-----------------------------------------------------------------------------
// ASLocalizer class for console build.
// This class encapsulates all language-dependent settings and is a
// generalization of the C locale concept.
//-----------------------------------------------------------------------------
class Translation;

class ASLocalizer
{
public:		// functions
	ASLocalizer();
	virtual ~ASLocalizer();
	std::string getLanguageID() const;
	const Translation* getTranslationClass() const;
#ifdef _WIN32
	void setLanguageFromLCID(size_t lcid);
#endif
	void setLanguageFromName(const char* langID);
	const char* settext(const char* textIn) const;

private:	// functions
	void setTranslationClass();

private:	// variables
	Translation* m_translationClass;// pointer to a polymorphic Translation class
	std::string m_langID;				// language identifier from the locale
	std::string m_subLangID;				// sub language identifier, if needed
#ifdef _WIN32
	size_t m_lcid;					// LCID of the user locale (Windows only)
	size_t m_codepage;				// active codepage, 65001 = utf-8
#endif
};

//----------------------------------------------------------------------------
// Translation base class.
//----------------------------------------------------------------------------

class Translation
// This base class is inherited by the language translation classes.
// Polymorphism is used to call the correct language translator.
// This class contains the translation std::vector and settext translation method.
// The language std::vector is built by the language sub classes.
// NOTE: This class must have virtual methods for typeid() to work.
//       typeid() is used by AStyleTestI18n_Localizer.cpp.
{
public:
	Translation();
	virtual ~Translation() = default;
	std::string convertToMultiByte(const std::wstring& wideStr) const;
	std::string getTranslationString(size_t i) const;
	size_t getTranslationVectorSize() const;
	bool getWideTranslation(const std::string& stringIn, std::wstring& wideOut) const;
	std::string& translate(const std::string& stringIn) const;

protected:
	void addPair(const std::string& english, const std::wstring& translated);
	// variables
	std::vector<std::pair<std::string, std::wstring> > m_translationVector;

private:
	// the number of translation pairs added a constructor
	static const size_t translationElements = 30;	// need static for vs2013
	// the translated std::string
	mutable std::string m_mbTranslation;
};

//----------------------------------------------------------------------------
// Translation classes
// One class for each language.
// These classes have only a constructor which builds the language std::vector.
//----------------------------------------------------------------------------

class Bulgarian : public Translation
{ public: Bulgarian(); };

class ChineseSimplified : public Translation
{ public: ChineseSimplified(); };

class ChineseTraditional : public Translation
{ public: ChineseTraditional(); };

class Dutch : public Translation
{ public: Dutch(); };

class English : public Translation
{ public: English(); };

class Estonian : public Translation
{ public: Estonian(); };

class Finnish : public Translation
{ public: Finnish(); };

class French : public Translation
{ public: French(); };

class German : public Translation
{ public: German(); };

class Greek : public Translation
{ public: Greek(); };

class Hindi : public Translation
{ public: Hindi(); };

class Hungarian : public Translation
{ public: Hungarian(); };

class Italian : public Translation
{ public: Italian(); };

class Japanese : public Translation
{ public: Japanese(); };

class Korean : public Translation
{ public: Korean(); };

class Norwegian : public Translation
{ public: Norwegian(); };

class Polish : public Translation
{ public: Polish(); };

class Portuguese : public Translation
{ public: Portuguese(); };

class Romanian : public Translation
{ public: Romanian(); };

class Russian : public Translation
{ public: Russian(); };

class Spanish : public Translation
{ public: Spanish(); };

class Swedish : public Translation
{ public: Swedish(); };

class Ukrainian : public Translation
{ public: Ukrainian(); };


#endif	//  ASTYLE_LIB

}	// namespace astyle

#endif	//  ASLOCALIZER_H
