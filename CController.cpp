/*
* Copyright (C) 2014 Daniel Stecker
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 3 of the
* License, or any later version.
*
* This program is distributed in the hope that it will be useful, but
* is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
* NON-INFRINGEMENT. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*
*/
#include "CController.h"

CController::CController() {
}

CController::CController(const CController& orig) {
}

CController::~CController() {
}

void CController::createController(string controller) {

    auto f_exist = [] (string name) {
        ifstream _f(name);
        return _f ? false : true;
    };

    if (f_exist(string(string("application/controllers/").append(controller).append("Controller.h")).c_str())) {
        ofstream f_controller_h(string(string("application/controllers/").append(controller).append("Controller.h")).c_str());
        if (f_controller_h.is_open()) {
            cout << "create application/controllers/" << controller << "Controller.h" << endl;
            f_controller_h << ""
                    "#ifndef _CONTROLLER_" << boost::to_upper_copy(controller) << "_H\n"
                    "#define _CONTROLLER_" << boost::to_upper_copy(controller) << "_H\n\n"
                    "#include <tnt/component.h>\n"
                    "#include <tnt/componentfactory.h>\n"
                    "#include <tnt/httprequest.h>\n"
                    "#include <tnt/httpreply.h>\n"
                    "#include <cxxtools/log.h>\n\n"
                    "// include the model\n"
                    "// #include \"model/" << controller << "Model.h\"\n"
                    "// define a log category\n"
                    "log_define(\"controller." << controller << "\");\n\n"
                    "// define a component which is callable from within tntnet.xml\n"
                    "class " << controller << "Controller : public tnt::Component\n"
                    "{\n"
                    "public:\n"
                    "\tunsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);\n"
                    "};\n"
                    "#endif  /* _CONTROLLER_" << boost::to_upper_copy(controller) << "_H */\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/controllers/" << controller << "Controller.h (exist ?)" << endl;
    }

    if (f_exist(string(string("application/controllers/").append(controller).append("Controller.cpp")).c_str())) {
        ofstream f_controller_cpp(string(string("application/controllers/").append(controller).append("Controller.cpp")).c_str());
        if (f_controller_cpp.is_open()) {
            cout << "create application/controllers/" << controller << "Controller.cpp" << endl;
            f_controller_cpp << ""
                    "#include \"" << controller << "Controller.h\"\n\n"
                    "// A static factory is used to instantiate the component.\n"
                    "// This also defines the name of the component, which is used\n"
                    "// in the mapping.\n"
                    "static tnt::ComponentFactoryImpl<" << controller << "Controller> factory(\"controllers/" << controller << "\");\n"
                    "// The operator() is the main method of the component. It is the\n"
                    "// starting point of our component.\n"
                    "unsigned " << controller << "Controller::operator() (tnt::HttpRequest& request,\n"
                    "					tnt::HttpReply& reply,\n"
                    "					tnt::QueryParams& qparam)\n"
                    "{\n"
                    "\t// This definition imports the _" << controller << " variable from the\n"
                    "\t// session.\n"
                    "\tTNT_SESSION_GLOBAL_VAR(" << controller << ", _" << controller << ", ());\n"
                    "\tlog_debug(\"" << controller << " controller called with qparam=\" << qparam.getUrl());\n"
                    "\t// Check if the user has pressed the increment button.\n"
                    "\t// The `qparam` variable has all the query parameters. Submitting a form sets\n"
                    "\t// the query parameter to the value of the button.\n"
                    "\t// Using `arg<bool>` checks whether the value is not empty.\n"
                    "\tif (qparam.arg<bool>(\"increment\"))\n"
                    "\t{\n"
                    "\t\t" << controller << "Controller.increment();\n"
                    "\t}\n"
                    "\t// Same as above with the decrement button.\n"
                    "\tif (qparam.arg<bool>(\"decrement\"))\n"
                    "\t{\n"
                    "\t\t" << controller << "Controller.decrement();\n"
                    "\t}\n"
                    "\treturn DECLINED;\n"
                    "}\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/controllers/" << controller << "Controller.cpp (exist ?)" << endl;
    }
}

