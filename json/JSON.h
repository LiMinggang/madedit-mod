/* $Id: JSON.h 1375 2014-04-16 12:17:54Z dpd $
 *
 * Copyright   : (c) 2010 by Open Source Solutions Pty Ltd.  All Rights Reserved
 * Project     :
 * File        : JSON
 *
 * Author      : Denis Dowling
 * Created     : 9/12/2010
 *
 * Description : Methods to encode and later decode JavaScript Object Notation
 *               messages.
 *               Follows the syntax described at www.json.org
 */
#ifndef JSON_H
#define JSON_H

#include <string>
#include <map>
#include <vector>
#include <exception>
#include <boost/shared_ptr.hpp>

//using boost::shared_ptr;

namespace JSON
{
	// Need to forward declare these class as they all refer to each other
    class Object;
    typedef boost::shared_ptr<Object> ObjectPtr;

    class Array;
    typedef boost::shared_ptr<Array> ArrayPtr;

    class Value;
    typedef boost::shared_ptr<Value> ValuePtr;

    class Value
	{
    public:
	virtual ~Value() {}

	bool isString() const { return type == TypeString; }
	virtual std::string getString() const;
		// Return a JSON value as a string
	    std::string getAsString() const;

	bool isNumber() const { return type == TypeNumber; }
	virtual double getNumber() const;
		// Return a JSON value as a number if possible otherwise returns 0
	    double getAsNumber() const;

	bool isBoolean() const { return type == TypeBoolean; }
	virtual bool getBoolean() const;
		// Return a JSON value as a boolean if possible otherwise returns false
	    bool getAsBoolean() const;

	bool isObject() const { return type == TypeObject; }
		// To convert value to object use
		// boost::dynamic_pointer_cast<JSON::Object>(v)

	bool isArray() const { return type == TypeArray; }
		// To convert value to array use
		// boost::dynamic_pointer_cast<JSON::Array>(v)

	bool isNull() const { return type == TypeNull; }

	// Encode the Value into JSON format
	virtual std::string encode() const;
	virtual std::string encodePretty(int indent = 0) const;
    protected:
	enum Type { TypeString, TypeNumber, TypeBoolean,
		    TypeObject, TypeArray, TypeNull
	};
	Type type;

	// Cannot construct directly
    Value(Type type_) : type(type_) {}
    private:
	// Cannot copy this base class
	Value(const Value &) {}
	Value &operator=(const Value &) { return *this; }
	};

    class String
	: public Value
	{
    public:
		// Itentionally not explicit as want automatic type conversion
	    String(const std::string &str) : Value(TypeString), val(str) {}
	    String(const char *str) : Value(TypeString), val(str) {}
	virtual ~String() {}

	virtual std::string getString() const;

	static std::string encode(const std::string &raw);
	virtual std::string encode() const;
    protected:
	std::string val;
	};

    class Number
	: public Value
	{
    public:
		// Itentionally not explicit as want automatic type conversion
	    Number(double d) : Value(TypeNumber), val(d) {}
	    Number(long d) : Value(TypeNumber), val(d) {}
	    Number(unsigned long d) : Value(TypeNumber), val(d) {}
	    Number(int d) : Value(TypeNumber), val(d) {}

	virtual ~Number() {}

	virtual double getNumber() const;

	static std::string encode(double d);
	virtual std::string encode() const;
    protected:
	double val;
	};

    class Boolean
	: public Value
	{
    public:
		// Itentionally not explicit as want automatic type conversion
	    Boolean(bool b) : Value(TypeBoolean), val(b) {}
	virtual ~Boolean() {}

	virtual bool getBoolean() const;

	virtual std::string encode() const;
    protected:
	bool val;
	};

    class Object
	: public Value
	{
    public:
	    Object() : Value(TypeObject) {}
	virtual ~Object() {}

	void setProperty(const std::string &name, ValuePtr value);

	// It is a little messy to repeat all of these but
	// it works well
	void setProperty(const std::string &name, const std::string &value);
	void setProperty(const std::string &name, const char *value);
	void setProperty(const std::string &name, double value);
	void setProperty(const std::string &name, unsigned long value);
	void setProperty(const std::string &name, long value);
	void setProperty(const std::string &name, int value);
	void setProperty(const std::string &name, bool value);
	void setProperty(const std::string &name, ObjectPtr value);
	void setProperty(const std::string &name, ArrayPtr value);

	bool hasProperty(const std::string &name);

	    ValuePtr getProperty(const std::string &name) const;

	bool getProperty(const std::string &name, std::string &value) const;
	bool getProperty(const std::string &name, double &value) const;
	bool getProperty(const std::string &name, unsigned long &value) const;
	bool getProperty(const std::string &name, long &value) const;
	bool getProperty(const std::string &name, int &value) const;
	bool getProperty(const std::string &name, bool &value) const;

	typedef std::map<std::string, ValuePtr> Properties;
	Properties properties;

	virtual std::string encode() const;
	virtual std::string encodePretty(int indent = 0) const;
	};

    class Array
	: public Value
	{
    public:
	    Array() : Value(TypeArray) {}
	virtual ~Array() {}

	void add(ValuePtr value);

	// Helpers to do the type conversion
	void add(const std::string &value);
	void add(const char *value);
	void add(double value);
	void add(long value);
	void add(int value);
	void add(bool value);
	void add(ObjectPtr value);
	void add(ArrayPtr value);

	size_t size() const;

	typedef std::vector<ValuePtr> Elements;
	Elements elements;

	virtual std::string encode() const;
	virtual std::string encodePretty(int indent = 0) const;
	};

    class Null
	: public Value
	{
    public:
		Null() : Value(TypeNull)  {}
	virtual ~Null() {}

	virtual std::string encode() const;
	};

    class Exception : public std::exception
	{
    public:
	Exception(const std::string &str);
	virtual ~Exception() throw();

	virtual const char *what() const throw();
    protected:
	std::string reason;
	};

    class ParserException : public Exception
	{
    public:
	ParserException(const std::string &str, size_t line, size_t column);
	};

	// Support relaxing the strict JSON parsing syntax to allow some
	// common Javascript features. This idea is copied from the Jackson
	// JSON parser
    enum ParserFeature {
	ALLOW_COMMENTS = 1<<0,
	ALLOW_UNQUOTED_FIELD_NAMES = 1<<1,
	ALLOW_SINGLE_QUOTES = 1<<2,
	ALLOW_TRAILING_COMMAS = 1<<3,
	};

    class Parser
	{
    public:
	Parser(int features =
		   ALLOW_COMMENTS | ALLOW_UNQUOTED_FIELD_NAMES |
		   ALLOW_SINGLE_QUOTES | ALLOW_TRAILING_COMMAS);

	ValuePtr parse(const std::string &str);
    protected:
	int features;

	enum TokenType
	{
	    TOK_OPEN_BRACE, // '{'
	    TOK_CLOSE_BRACE, // '}'
	    TOK_COMMA, // ','
	    TOK_COLON, // ':'
	    TOK_OPEN_BRACKET, // '['
	    TOK_CLOSE_BRACKET, // ']'
	    TOK_DOUBLE_QUOTE, // '"'
	    TOK_SINGLE_QUOTE, // '\''
	    TOK_NUMBER, // Number starts with any of "-0123456789". Can also contain any of these '.eE+'
	    TOK_WHITE_SPACE, // space (0x20), HT (0x09), LF (0x0a), CR (0x0d)
	    TOK_COMMENT, // '/' for ALLOW_COMMENT
	    TOK_IDENTIFIER, // '[a-zA-Z_]' for ALLOW_UNQUOTED_FIELD_NAMES
	    TOK_EOS, // Matched the end of the string
	    TOK_ERROR, // Any other characters
	};

	// Vector will be initialised to 256 entries to map from an ASCII
	// character to a token type
	static std::vector<TokenType> tokenType;

	// Human readable strings for the token names
	static std::vector<std::string> tokenName;

	size_t pos;
	size_t line;
	size_t column;
	std::string str;

	char getChar();
	void ungetChar();
	TokenType getNextToken();
	void getToken(TokenType match);

	ValuePtr parseValue();
	std::string parseString(TokenType start_token);
	std::string parseIdentifier();
	ValuePtr parseStringValue(TokenType start_token);
	ValuePtr parseNumericValue();
	ValuePtr parseObject();
	ValuePtr parseArray();
	void parseComment();
	};

	extern char indentChar/* = '\t'*/;
	extern int indentUnit/* = 1*/;
}

#endif
