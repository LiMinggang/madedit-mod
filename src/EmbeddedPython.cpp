///////////////////////////////////////////////////////////////////////////////
// Name:		EmbeddedPython.cpp
// Description:
// Author:		minggang.li@gmail.com
// Licence: 	GPL
///////////////////////////////////////////////////////////////////////////////

#include "EmbeddedPython.hpp"
#include "MadPython.hpp"

BOOST_PYTHON_MODULE(redirector)
{
    using namespace embedded_python;
    using namespace py;

    class_<stdout_redirector>("stdout",
                              "This class redirects python's standard output "
                              "to the console.",
                              init<>("initialize the redirector."))
        .def("__init__", make_constructor(make_stdout_redirector), "initialize the redirector.")
        .def("write", &stdout_redirector::write, "write sys.stdout redirection.")
        ;

    class_<stderr_redirector>("stderr",
                              "This class redirects python's error output "
                              "to the console.",
                              init<>("initialize the redirector."))

        .def("__init__", make_constructor(make_stderr_redirector), "initialize the redirector.")
        .def("write", &stderr_redirector::write, "write sys.stderr redirection.");
}

EmbeddedPython::EmbeddedPython()
{
    // Register the module with the interpreter; must be called before Py_Initialize.
    /* ToDo: C style cast to avoid compiler warning about
    deprecated conversion from string constant to 'char*' */
    if (PyImport_AppendInittab((char*)"redirector", initredirector) == -1) {
        throw std::runtime_error("Failed to add embedded_python to python's "
            "interpreter builtin modules");
    }

    if (PyImport_AppendInittab((char*)"madpython", initmadpython) == -1) {
        throw std::runtime_error("Failed to add madpython to python's "
            "interpreter builtin modules");
    }

    Py_Initialize();

    // Retrieve the main module
    m_main_module = py::import("__main__");

    // Retrieve the main module's namespace
    m_main_global = py::dict(m_main_module.attr("__dict__"));

    try {

        static const char* const redirect_py =
            "import sys\n"
            "import redirector\n"
            "sys.stdout = redirector.stdout()\n"
            "sys.stderr = redirector.stderr()\n"
            "from madpython import MadEdit, MadEditCommand, "
            "MadCaretType, MadNewLineType, MadConvertEncodingFlag, MadNumberingStepType, "
            "MadNumberFormat, MadNumberAlign, MadSortLinesFlags, MsgBox, InputBox, MadDlgStyle, MadMsgBoxRet, Utf8ToLocal, LocalToUtf8";

#if 0
        /* FixMe: exception thrown, mmh - seems a bug in boost.python, see
        * http://www.nabble.com/Problems-with-Boost::Python-Embedding-Tutorials-td18799129.html */
        py::exec(redirect_py, m_main_global);
#else
        PyRun_String(redirect_py,
            Py_file_input,
            m_main_global.ptr(), m_main_global.ptr());
#endif

    }
    catch(const py::error_already_set&) {

        if (PyErr_Occurred()) {
            std::cerr << "*** Redirection failed ***\n";
            PyErr_Print();
            PyErr_Clear();
        }
        else {
            std::cerr << "A C++ exception was thrown for which "
                << "there was no exception handler registered.";
        }
    }
}

