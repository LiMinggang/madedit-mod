///////////////////////////////////////////////////////////////////////////////
// Name:		EmbeddedPython.hpp
// Description:
// Author:		minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef __EMBEDDEDPYTHON__
#define __EMBEDDEDPYTHON__

#include <Python.h>

#define BOOST_PYTHON_STATIC_LIB
#include "../python/include/boost/python.hpp"
#include <boost/function.hpp>

#include <iostream>
#include <string>
#include <memory>

namespace py = ::boost::python;

extern void PrintString(const std::string& str);
namespace embedded_python {

	enum direction_type
	{
		STDIN,
		STDOUT,
		STDERR
	};

	template<direction_type>
	class py_redirector
	{
	public:
		py_redirector()
		{ }

		explicit py_redirector( boost::function<void ( const std::string& )> f )
			: m_write_fn( f )
		{ }

		void write( const std::string& text ) {
			if( m_write_fn )
			{ m_write_fn( text ); }
			else
			{ std::wcout << text; }
		}

	public:
		boost::function<void ( const std::string& )> m_write_fn;
	};

	typedef py_redirector<STDOUT>   stdout_redirector;
	typedef py_redirector<STDERR>   stderr_redirector;


	static std::auto_ptr<stdout_redirector> make_stdout_redirector()
	{
		std::auto_ptr<stdout_redirector> ptr( new stdout_redirector(PrintString) );
		return ptr;
	}

	static std::auto_ptr<stderr_redirector> make_stderr_redirector()
	{
		std::auto_ptr<stderr_redirector> ptr( new stderr_redirector(PrintString) );
		return ptr;
	}
}

class EmbeddedPython
{
public:
	EmbeddedPython();

	~EmbeddedPython() {
		// Boost.Python doesn't support Py_Finalize yet, so don't call it!
		//Py_Finalize();
	}

	void exec( const std::string& command ) {
		try {
			py::object result = py::exec( command.c_str(),
			                              m_main_global, m_main_global );
		}
		catch( const py::error_already_set& ) {
			if( PyErr_Occurred() ) {
				PyErr_Print();
				PyErr_Clear();
			}
			else {
				std::cerr << "A C++ exception was thrown for which "
				          << "there was no exception handler registered.\n";
			}
		}
	}

	void write_stdout( const std::string& text ) {
		std::cout << ">>> " << text;
	}

	void write_stderr( const std::string& text ) {
		std::cout << "xxx " << text;
	}

private:
	py::object m_main_module;
	py::dict   m_main_global;
};

extern EmbeddedPython * g_EmbeddedPython;
#endif //__EMBEDDEDPYTHON__

