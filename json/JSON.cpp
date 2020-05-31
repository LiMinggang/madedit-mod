/* $Id: JSON.cpp 1375 2014-04-16 12:17:54Z dpd $
 *
 * Copyright   : (c) 2010 by Open Source Solutions Pty Ltd.  All Rights Reserved
 * Project     :
 * File        : JSON
 *
 * Author      : Denis Dowling
 * Created     : 9/12/2010
 *
 * Description : Methods to encode and later decode JavaScript Object Notation
 *               messages
 */
#include "JSON.h"
#include <assert.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

namespace JSON
{
	char indentChar = '\t';
	int indentUnit = 1;
	// **** Value ****
	std::string Value::getString() const
	{
	throw Exception("Not String Type");
	}

	// Return a JSON value as a string
	std::string Value::getAsString() const
	{
		if (isString())
			return getString();
		else
			return encode();
	}

	double Value::getNumber() const
	{
	throw Exception("Not Number Type");
	}

	double Value::getAsNumber() const
	{
		if (isNumber())
			return getNumber();
		else if (isString())
		{
			std::string s = getString();
			return atof(s.c_str());
		}
		else if (isBoolean())
			return getBoolean() ? 1.0 : 0.0;
		else
			return 0;
	}

	bool Value::getBoolean() const
	{
	throw Exception("Not Boolean Type");
	}

	bool Value::getAsBoolean() const
	{
		if (isBoolean())
			return getBoolean();
		else if (isString())
		{
			std::string s = getString();
			if (s.size() > 0 && s == "true")
				return true;
			else
				return false;
		}
		else if (isNumber())
			return getNumber() != 0.0;
		else
			return false;
	}

	std::string Value::encode() const
	{
	throw Exception("No Encoding");
	}

	std::string Value::encodePretty(int indent) const
	{
	return encode();
	}

	// **** String ****
	std::string String::getString() const
	{
	return val;
	}

	// Static method that can be used on other classes to do raw encoding
	// of strings
	std::string String::encode(const std::string &raw)
	{
	std::string result;
	// Reserve enough space to store the original string, the quotes and
	// a couple of escaped characters. If more is needed then the string
	// will automatically resize
	result.reserve(raw.size() + 4);

	// MS VC++ 6.0 complains about push_back :-(
	result += '"';
	for (size_t i = 0; i < raw.size(); i++)
	{
		int c = (unsigned char)raw[i];

		// Common case first
		if (c >= 0x20 && c != '"' && c != '\\' && c < 0x7f)
		result += c;
		else
		{
		result += '\\';

		if (c == '"' || c == '\\')
			result += c;
		else if (c == 0x08) // BS
			result += 'b';
		else if (c == 0x0c) // FF
			result += 'f';
		else if (c == 0x0a) // NL
			result += 'n';
		else if (c == 0x0d) // CR
			result += 'r';
		else if (c == 0x09) // HT
			result += 't';
		else
		{
			std::ostringstream ss;
			ss << "u"
			   << std::setfill('0') << std::hex << std::setw(4)
			   << c
			   << std::setfill(' ') << std::dec;
			result += ss.str();
		}
		}
	}

	result += '"';

	return result;
	}

	std::string String::encode() const
	{
	return encode(val);
	}

	// **** Number ****
	double Number::getNumber() const
	{
	return val;
	}

	std::string Number::encode(double d)
	{
#if 0
	std::ostringstream ss;
	ss << std::setprecision(15) << d;
	return ss.str();
#else
		// This is meant to automatically determine the precision required and
		// it should be faster than the ostringstream
		return boost::lexical_cast<std::string>(d);
#endif
	}

	std::string Number::encode() const
	{
	return encode(val);
	}

	// **** Boolean ****
	bool Boolean::getBoolean() const
	{
	return val;
	}

	std::string Boolean::encode() const
	{
	if (val)
		return "true";
	else
		return "false";
	}

	// **** Object ****
	void Object::setProperty(const std::string &name, ValuePtr value)
	{
	properties[name] = value;
	}

	void Object::setProperty(const std::string &name,
				 const std::string &value)
	{
	properties[name].reset(new String(value));
	}

	void Object::setProperty(const std::string &name,
				 const char *value)
	{
	properties[name].reset(new String(value));
	}

	void Object::setProperty(const std::string &name, double value)
	{
	properties[name].reset(new Number(value));
	}

	void Object::setProperty(const std::string &name, long value)
	{
	properties[name].reset(new Number(value));
	}

	void Object::setProperty(const std::string &name, unsigned long value)
	{
	properties[name].reset(new Number(value));
	}

	void Object::setProperty(const std::string &name, int value)
	{
	properties[name].reset(new Number(value));
	}

	void Object::setProperty(const std::string &name, bool value)
	{
	properties[name].reset(new Boolean(value));
	}

	void Object::setProperty(const std::string &name, ObjectPtr value)
	{
	properties[name] = value;
	}

	void Object::setProperty(const std::string &name, ArrayPtr value)
	{
	properties[name] = value;
	}

	bool Object::hasProperty(const std::string &name)
	{
	return properties.find(name) != properties.end();
	}

	ValuePtr Object::getProperty(const std::string &name) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return ValuePtr();

		return (*iter).second;
	}

	bool Object::getProperty(const std::string &name, std::string &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsString();
		return true;
	}

	bool Object::getProperty(const std::string &name, double &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsNumber();
		return true;
	}

	bool Object::getProperty(const std::string &name, long &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsNumber();
		return true;
	}

	bool Object::getProperty(const std::string &name, unsigned long &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsNumber();
		return true;
	}

	bool Object::getProperty(const std::string &name, int &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsNumber();
		return true;
	}

	bool Object::getProperty(const std::string &name, bool &value) const
	{
		Properties::const_iterator iter = properties.find(name);
		if (iter == properties.end())
			return false;

		value = (*iter).second->getAsBoolean();
		return true;
	}

	std::string Object::encode() const
	{
	std::string result;

	result = "{";

	Properties::const_iterator iter;
	for (iter = properties.begin(); iter != properties.end(); ++iter)
	{
		if (iter != properties.begin())
		result += ",";

		result += String::encode((*iter).first) +
		":" + (*iter).second->encode();
	}

	result += "}";

	return result;
	}

	std::string Object::encodePretty(int indent) const
	{
	std::string result;

	result = "{";

	indent += indentUnit;

	Properties::const_iterator iter;
	for (iter = properties.begin(); iter != properties.end(); ++iter)
	{
		if (iter != properties.begin())
		result += ",";

		result += "\n";
		result += std::string(indent, indentChar);

		result += String::encode((*iter).first) +
		" : " + (*iter).second->encodePretty(indent);
	}

	indent -= indentUnit;
	result += "\n";
	result += std::string(indent, indentChar);

	result += "}";

	return result;
	}

	// **** Array ****
	void Array::add(ValuePtr value)
	{
	elements.push_back(value);
	}

	void Array::add(const std::string &value)
	{
	ValuePtr p(new String(value));
	elements.push_back(p);
	}

	void Array::add(const char *value)
	{
	ValuePtr p(new String(value));
	elements.push_back(p);
	}

	void Array::add(double value)
	{
	ValuePtr p(new Number(value));
	elements.push_back(p);
	}

	void Array::add(long value)
	{
	ValuePtr p(new Number(value));
	elements.push_back(p);
	}

	void Array::add(int value)
	{
	ValuePtr p(new Number(value));
	elements.push_back(p);
	}

	void Array::add(bool value)
	{
	ValuePtr p(new Boolean(value));
	elements.push_back(p);
	}

	void Array::add(ObjectPtr value)
	{
	elements.push_back(value);
	}

	void Array::add(ArrayPtr value)
	{
	elements.push_back(value);
	}

	size_t Array::size() const
	{
		return elements.size();
	}

	std::string Array::encode() const
	{
	std::string result;

	result = "[";

	Elements::const_iterator iter;
	for (iter = elements.begin(); iter != elements.end(); ++iter)
	{
		if (iter != elements.begin())
		result += ",";

		result += (*iter)->encode();
	}

	result += "]";

	return result;
	}

	std::string Array::encodePretty(int indent) const
	{
	std::string result;

	result = "[";

	Elements::const_iterator iter;
	for (iter = elements.begin(); iter != elements.end(); ++iter)
	{
		if (iter != elements.begin())
		result += ", ";

		result += (*iter)->encodePretty(indent);
	}

	result += "]";

	return result;
	}

	// **** Null ****
	std::string Null::encode() const
	{
	return "null";
	}

	// **** Exception ****
	Exception::Exception(const std::string &str)
	: reason(str)
	{
	}

	Exception::~Exception() throw()
	{
	}

	const char * Exception::what() const throw()
	{
	return reason.c_str();
	}

	// **** ParserException *****
	ParserException::ParserException(const std::string &str, size_t line, size_t column)
	: Exception(str)
	{
	std::ostringstream ss;
	ss << " at line " << line << " and column " << column;
	reason += ss.str();
	}

	// **** Parser ****
	std::vector<Parser::TokenType> Parser::tokenType;
	std::vector<std::string> Parser::tokenName;

	Parser::Parser(int features_)
	: features(features_)
	{

	if (tokenType.empty())
	{
		// Initialise the token type array if it is empty
		tokenType.reserve(256);
		size_t i;
		for (i = 0; i < 256; i++)
		tokenType.push_back(TOK_ERROR);

		tokenType['{'] = TOK_OPEN_BRACE;
		tokenType['}'] = TOK_CLOSE_BRACE;
		tokenType[','] = TOK_COMMA;
		tokenType[':'] = TOK_COLON;
		tokenType['['] = TOK_OPEN_BRACKET;
		tokenType[']'] = TOK_CLOSE_BRACKET;
		tokenType['"'] = TOK_DOUBLE_QUOTE;
		// Only recognise the single quote token if this feature us enabled
		if (features & ALLOW_SINGLE_QUOTES)
		tokenType['\''] = TOK_SINGLE_QUOTE;
		tokenType['-'] = TOK_NUMBER;
		for(i = 0; i < 10; i++)
		tokenType['0' + i] = TOK_NUMBER;
		tokenType[' '] = TOK_WHITE_SPACE;
		tokenType['\t'] = TOK_WHITE_SPACE;
		tokenType['\n'] = TOK_WHITE_SPACE;
		tokenType['\r'] = TOK_WHITE_SPACE;

		char c;
		for(c = 'A'; c <= 'Z'; c++)
		tokenType[c] = TOK_IDENTIFIER;
		for(c = 'a'; c <= 'z'; c++)
		tokenType[c] = TOK_IDENTIFIER;
		tokenType['_'] = TOK_IDENTIFIER;

		if (features & ALLOW_COMMENTS)
		tokenType['/'] = TOK_COMMENT;
	}

	if (tokenName.empty())
	{
		tokenName.push_back("open brace '{'"); // TOK_OPEN_BRACE
		tokenName.push_back("close brace '}'"); // TOK_CLOSE_BRACE
		tokenName.push_back("comma ','"); // TOK_COMMA
		tokenName.push_back("colon ':'"); // TOK_COLON
		tokenName.push_back("open bracket '('"); // TOK_OPEN_BRACKET
		tokenName.push_back("close bracket ')'"); // TOK_CLOSE_BRACKET
		tokenName.push_back("double quote '\"'"); // TOK_DOUBLE_QUOTE
		tokenName.push_back("single quote '''"); // TOK_SINGLE_QUOTE
		tokenName.push_back("number"); // TOK_NUMBER
		tokenName.push_back("white space"); // TOK_WHITE_SPACE
		tokenName.push_back("forward slash '/'"); // TOK_COMMENT
		tokenName.push_back("identifier"); // TOK_IDENTIFIER
		tokenName.push_back("end of string"); // TOK_EOS
		tokenName.push_back("error"); // TOK_ERROR
	}

	}

	ValuePtr Parser::parse(const std::string &str_)
	{
	// Setup the tokenizer
	pos = 0;
	line = 1;
	column = 1;
	str = str_;

	ValuePtr v = parseValue();

	// Explicitly check that no more input as this can happen if an extra closing brace
	// causes the parser to bomb out early
	getToken(TOK_EOS);

	return v;
	}

	ValuePtr Parser::parseValue()
	{
	while(true)
	{
		char c = getChar();

		TokenType t = tokenType[c];
		switch(t)
		{
		case TOK_DOUBLE_QUOTE:
		case TOK_SINGLE_QUOTE:
		return parseStringValue(t);
		case TOK_NUMBER:
		ungetChar();
		return parseNumericValue();
		case TOK_OPEN_BRACE:
		return parseObject();
		case TOK_OPEN_BRACKET:
		return parseArray();
		case TOK_IDENTIFIER:
		{
		std::string ident = parseIdentifier();
		if (ident == "true")
			return ValuePtr(new Boolean(true));
		else if (ident == "false")
			return ValuePtr(new Boolean(false));
		else if (ident == "null")
			return ValuePtr(new Null);
		else
			throw ParserException("Illegal identifier", line, column);
		}
		case TOK_WHITE_SPACE:
		break; // continue with next character
		case TOK_COMMENT:
		parseComment();
		break;
		default:
			{
			std::string err = "Unexpected '";
			err += c;
			err += "'";
			throw ParserException(err, line, column);
			}
		}
	}
	}

	std::string Parser::parseString(TokenType start_token)
	{
	std::string res;

	while(true)
	{
		char c = getChar();

		if (c == '\\')
		{
		c = getChar();
		if (c == 'b')
			res += 0x08;
		else if (c == 'f')
			res += 0x0c;
		else if (c == 'n')
			res += 0x0a;
		else if (c == 'r')
			res += 0x0d;
		else if (c == 't')
			res += 0x09;
		else if (c == 'u')
		{
			int val = 0;
			for (int i = 0; i < 4; i++)
			{
			c = getChar();

			val <<= 4;
			if (c >= '0' && c <= '9')
				val += (c - '0');
			else if (c >= 'a' && c <= 'f')
				val += (c - 'a' + 0x0a);
			else if (c >= 'A' && c <= 'F')
				val += (c - 'A' + 0x0a);
			else
				throw ParserException("Illegal hex digit", line, column);

			res += val;
			}
		}
		else
			// MS VC++ 6.0 will not let us use push_back here
			res += c;
		}
		else if ((c == '"' && start_token == TOK_DOUBLE_QUOTE))
		break;
		else if ((c == '\'' && start_token == TOK_SINGLE_QUOTE))
		break;
		else
		res += c;
	}

	return res;
	}

	std::string Parser::parseIdentifier()
	{
	// Identifier needs to get the first TOK_IDENTIFIER character
	ungetChar();

	std::string res;

	while(true)
	{
		char c = getChar();
		TokenType token = tokenType[c]; // Used for whitespace detection
		if (token != TOK_IDENTIFIER && ! (c >= '0' && c <= '9'))
		{
		// When we get a non TOK_IDENTIFIER or digit character then
		// we know it is end of identifer but need to put the
		// char back
		ungetChar();
		break;
		}

		res += c;
	}

	return res;
	}

	ValuePtr Parser::parseStringValue(TokenType start_token)
	{
	return ValuePtr(new String(parseString(start_token)));
	}

	ValuePtr Parser::parseNumericValue()
	{
	std::string res;
	while(true)
	{
		if (pos >= str.size())
		break;

		char c =  str[pos++];
		if (isspace(c))
		continue;
		else if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' ||
		c == 'e' || c == 'E')
		{
		res += c;
		}
		else
		{
		// Numbers dont have an explicit end character
		ungetChar();
		break;
		}
	}

	double d = atof(res.c_str());

	return ValuePtr(new Number(d));
	}

	char Parser::getChar()
	{
	if (pos >= str.size())
		throw ParserException("Unexpected end of buffer", line, column);

	char c = str[pos++];
	if (c == '\n')
	{
		line++;
		column = 1;
	}

	return c;
	}

	void Parser::ungetChar()
	{
	if (pos <= 0)
		throw ParserException("Unexpected start of buffer", line, column);

	pos--;

	// Decrement line count as it will be incremented again on the next
	// getChar() call
	char c = str[pos];
	if (c == '\n')
		line--;
	}

	Parser::TokenType Parser::getNextToken()
	{
	while (pos < str.size())
	{
		char c = getChar();
		TokenType t = tokenType[c];
		if (t == TOK_WHITE_SPACE)
		continue;
		else if (t == TOK_COMMENT)
		{
		parseComment();
		continue;
		}
		else
		return t;
	}

	return TOK_EOS;
	}

	void Parser::getToken(TokenType match)
	{
	TokenType token = getNextToken();

	if (token == match)
		return;
	else
		throw ParserException("Expected " + tokenName[match], line, column);
	}

	ValuePtr Parser::parseObject()
	{
	Object *obj = new Object;
	ValuePtr ret(obj);

	while(true)
	{
		std::string name;

		TokenType token = getNextToken();
		if (token == TOK_DOUBLE_QUOTE || token == TOK_SINGLE_QUOTE)
		name = parseString(token);
		else if (token == TOK_IDENTIFIER &&
			 (features & ALLOW_UNQUOTED_FIELD_NAMES))
		name = parseIdentifier();
		else if (token == TOK_CLOSE_BRACE &&
			 (obj->properties.size() == 0 ||
			  (features & ALLOW_TRAILING_COMMAS)))
		return ret;
		else
		throw ParserException("Expected string or identifier", line, column);

		getToken(TOK_COLON);

		ValuePtr value = parseValue();

		if (obj->hasProperty(name))
		throw ParserException("Duplicate property '" + name +
					  "' on object", line, column);

		obj->setProperty(name, value);

		token = getNextToken();
		if (token == TOK_CLOSE_BRACE)
		return ret;
		else if (token != TOK_COMMA)
		throw ParserException("Expected " + tokenName[TOK_COMMA] +
					  " or " + tokenName[TOK_CLOSE_BRACE],
					  line, column);
	}
	}

	ValuePtr Parser::parseArray()
	{
	Array *arr = new Array;
	// Wrap the raw pointer in an shared_ptr so will be deleted on return
	ValuePtr ret(arr);

	while(true)
	{
		TokenType token = getNextToken();
		if (token == TOK_CLOSE_BRACKET)
		{
		if (arr->elements.size() != 0 &&
			!(features & ALLOW_TRAILING_COMMAS))
			throw ParserException("Expected value", line, column);

		return ret;
		}

		ungetChar();
		ValuePtr v = parseValue();

		arr->add(v);

		token = getNextToken();
		if (token == TOK_CLOSE_BRACKET)
		return ret;
		if (token != TOK_COMMA)
		throw ParserException("Expected " + tokenName[TOK_COMMA] +
					  " or " + tokenName[TOK_CLOSE_BRACKET],
					  line, column);
	}
	}

	void Parser::parseComment()
	{
	char c = getChar();

	if (c == '*')
	{
		// C style comment. Look for closing '*/'
		while(true)
		{
		c = getChar();

		// Use 'while' here instead of 'if' as we want to match
		// things like '****/'
		while (c == '*')
		{
			c = getChar();
			if (c == '/')
			return;
		}
		}
	}
	else if (c == '/')
	{
		// C++ style comment. Look for EOL
		while(true)
		{
		c = getChar();

		if (c == '\n')
			return;
		}
	}
	else
		throw ParserException("Expected start of comment", line, column);
	}
}
