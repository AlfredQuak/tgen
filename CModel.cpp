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
#include "CModel.h"
#include "CController.h"

static cxxtools::Regex s_varchar("^varchar\\(([0-9]+)\\)$");
static cxxtools::Regex s_tinyint("^tinyint\\(([0-9]+)\\)$");
static cxxtools::Regex s_u_int("^int\\(([0-9]+)\\) unsigned$");

static cxxtools::Regex s_text("^text$");
static cxxtools::Regex s_int("^int\\(([0-9]+)\\)$");
static cxxtools::Regex s_decimal("^decimal\\(([0-9]+),([0-9]+)\\)$");
static cxxtools::Regex s_datetime("^datetime$");
static cxxtools::Regex s_timestamp("^timestamp$");
static cxxtools::Regex s_date("^date$");
static cxxtools::Regex s_double("^double$");

CModel::CModel() {
}

CModel::~CModel() {
}

void CModel::createModelViewCrud(tntdb::Connection conn, string table) {
    string sql = string("describe ").append(table);
    //string setFunction = "";
    //string retType = "";

    string f_createView = string("component/crud/").append(table).append("/").append(table).append("CreateView");
    string f_updateView = string("component/crud/").append(table).append("/").append(table).append("UpdateView");
    string f_readView = string("component/crud/").append(table).append("/").append(table).append("ReadView");

    CView *view = new CView();
    view->createView(f_createView);
    view->createView(f_updateView);
    view->createView(f_readView);
    delete(view);

    string create_string = string("<h1>Create</h1>\n\t<form method=\"post\" action=\"")
            .append(table)
            .append("/")
            .append(table)
            .append("Create\">\n")
            .append("\t<ui>\n");
    string update_string = string("<h1>Update</h1>\n\t<form method=\"post\" action=\"")
            .append(table)
            .append("/")
            .append(table)
            .append("Update\">\n")
            .append("\t<ui>\n");


    ofstream ecpp_file(string(string("application/view/ecpp/component/crud/").append(table).append("/").append(table).append("ReadView.ecpp")).c_str());
    string include_string = string("application/view/html/component/crud/").append(table).append("/").append(table).append("ReadView").append(".html");
    boost::replace_all(include_string, "//", "/");

    if (ecpp_file.is_open()) {
        ecpp_file << ""
                "<%pre>\n"
                "    #include \"model/" << table << "Model.h\"\n"
                "</%pre>\n"
                "<%args>\n\n</%args>\n"
                "<%session\n"
                "    scope=\"global\">\n"
                "</%session>\n"
                "<%request\n"
                "    scope=\"global\">\n"
                "    " << table << "Model crud_" << table << "Model;\n"
                "</%request>\n"
                "<%cpp>\n\n</%cpp>\n\n"
                "<%include>" << include_string << "</%include>\n\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    ofstream u_ecpp_file(string(string("application/view/ecpp/component/crud/").append(table).append("/").append(table).append("UpdateView.ecpp")).c_str());
    string u_include_string = string("application/view/html/component/crud/").append(table).append("/").append(table).append("UpdateView").append(".html");
    boost::replace_all(u_include_string, "//", "/");

    if (u_ecpp_file.is_open()) {
        u_ecpp_file << ""
                "<%pre>\n"
                "    #include \"model/" << table << "Model.h\"\n"
                "</%pre>\n"
                "<%args>\n\n</%args>\n"
                "<%session\n"
                "    scope=\"global\">\n"
                "</%session>\n"
                "<%request\n"
                "    scope=\"global\">\n"
                "    " << table << "Model crud_" << table << "Model;\n"
                "</%request>\n"
                "<%cpp>\n\n</%cpp>\n\n"
                "<%include>" << u_include_string << "</%include>\n\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }


    tntdb::Result result = conn.select(sql);
    for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it) {
        tntdb::Row r = *it;

        create_string += string("\t\t<li>")
                .append(r[0].getString())
                .append("<input type=\"text\" name=\"")
                .append(r[0].getString())
                .append("\" id=\"")
                .append(r[0].getString())
                .append("\" value=\"")
                //.append(name@example.com)
                .append("\"></li>\n");

        update_string += string("\t\t<li>")
                .append(r[0].getString())
                .append("<input type=\"text\" name=\"")
                .append(r[0].getString())
                .append("\" id=\"")
                .append(r[0].getString())
                .append("\" value=\"<$ crud_")
                .append(table).append("Model.get_").append(r[0].getString()).append("()");

        if (r[1].getString().compare("datetime") == 0 || r[1].getString().compare("date") == 0) {
            update_string.append(".getIso()");
        }

        update_string.append(" $>\"></li>\n");
    }
    create_string += ""
            "\t\t<li><button type=\"submit\" >save</button></li>\n"
            "\t</ui>\n"
            "\t</form>\n";
    update_string += ""
            "\t\t<li><button type=\"submit\" >save</button></li>\n"
            "\t</ui>\n"
            "\t</form>\n";

    ofstream create_f(string("application/view/html/").append(f_createView).append(".html"));
    if (create_f.is_open()) {
        create_f << ""
                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
                "<html>\n"
                "  <head>\n"
                "    <title>" << f_createView << "</title>\n"
                "    <!-- " << f_createView << " -->\n"
                "  </head>\n"
                "  <body>\n"
                << create_string <<
                "  </body>\n"
                "</html>\n\n";
    }

    ofstream update_f(string("application/view/html/").append(f_updateView).append(".html"));
    if (update_f.is_open()) {
        update_f << ""
                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
                "<html>\n"
                "  <head>\n"
                "    <title>" << f_updateView << "</title>\n"
                "    <!-- " << f_updateView << " -->\n"
                "  </head>\n"
                "  <body>\n"
                << update_string <<
                "  </body>\n"
                "</html>\n\n";
    }
}

void CModel::createModelCrud(tntdb::Connection conn, string table) {

    this->createModel(conn, table);
    this->createModelViewCrud(conn, table);

    cxxtools::RegexSMatch m;
    string sql = string("describe ").append(table);
    string setFunction = "";
    string retType = "";

    tntdb::Result result = conn.select(sql);
    for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it) {
        tntdb::Row r = *it;
        retType = r[1].getString();

        if (s_varchar.match(r[1].getString(), m)) retType = "string"; // varchar
        if (s_int.match(r[1].getString(), m)) retType = "int"; // int
        if (s_u_int.match(r[1].getString(), m)) retType = "unsigned int"; // unsigned int
        if (s_decimal.match(r[1].getString(), m)) retType = "double"; // decimal
        if (s_datetime.match(r[1].getString(), m)) retType = "tntdb::Datetime"; // datetime
        if (s_timestamp.match(r[1].getString(), m)) retType = "string"; // timestamp
        if (s_date.match(r[1].getString(), m)) retType = "tntdb::Date"; // date
        if (s_text.match(r[1].getString(), m) || r[1].getString() == "longtext") retType = "string"; // text
        if (s_tinyint.match(r[1].getString(), m)) retType = "bool"; // tinyint

        cout << retType << "=" << r[1].getString() << endl;

        if (r[0].getString().compare("date_create") != 0
                && r[0].getString().compare("date_update") != 0
                ) {

            setFunction += "\t\tif (qparam.arg<bool>(\"";
            setFunction += r[0].getString();
            setFunction += "\")) ";
            setFunction += table;

            if (retType.compare("tntdb::Datetime") == 0) {
                setFunction += string("->set_")
                        .append(r[0].getString())
                        .append("(tntdb::Datetime::fromIso(qparam.param")
                        .append("(\"")
                        .append(r[0].getString())
                        .append("\")));\n");
            } else if (retType.compare("tntdb::Date") == 0) {
                setFunction += string("->set_")
                        .append(r[0].getString())
                        .append("(tntdb::Datetime::fromIso(qparam.param")
                        .append("(\"")
                        .append(r[0].getString())
                        .append("\").getDate()));\n");
            } else {
                setFunction += string("->set_")
                        .append(r[0].getString())
                        .append("(qparam.arg<")
                        .append(retType)
                        .append(">(\"")
                        .append(r[0].getString())
                        .append("\"));\n");
            }
        }
    }

    boost::filesystem::create_directories(string("application/controllers/crud/").append(table));
    string create_h = string(string("application/controllers/crud/").append(table).append("/").append(table).append("CreateController.h"));
    string create_cpp = string(string("application/controllers/crud/").append(table).append("/").append(table).append("CreateController.cpp"));

    string update_h = string(string("application/controllers/crud/").append(table).append("/").append(table).append("UpdateController.h"));
    string update_cpp = string(string("application/controllers/crud/").append(table).append("/").append(table).append("UpdateController.cpp"));

    string read_h = string(string("application/controllers/crud/").append(table).append("/").append(table).append("ReadController.h"));
    string read_cpp = string(string("application/controllers/crud/").append(table).append("/").append(table).append("ReadController.cpp"));

    ofstream f_controller_h(create_h.c_str());
    if (f_controller_h.is_open()) {
        cout << "create " << create_h << endl;
        f_controller_h << ""
                "#ifndef _CONTROLLER_CREATE_" << boost::to_upper_copy(table) << "_H\n"
                "#define _CONTROLLER_CREATE_" << boost::to_upper_copy(table) << "_H\n\n"
                "// define a log category\n"
                "log_define(\"controller.CrudCreate" << table << "\");\n\n"
                "#include <tnt/component.h>\n"
                "#include <tnt/componentfactory.h>\n"
                "#include <tnt/httprequest.h>\n"
                "#include <tnt/httpreply.h>\n"
                "#include <tntdb/datetime.h>\n"
                "#include <cxxtools/conversionerror.h>\n"
                "#include <cxxtools/log.h>\n\n"
                "// include the model\n"
                "#include \"model/" << table << "Model.h\"\n"
                "// define a component which is callable from within tntnet.xml\n"
                "class " << table << "CreateController : public tnt::Component\n"
                "{\n"
                "public:\n"
                "\t" << table << "CreateController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c);\n"
                "\tvirtual unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);\n"
                "};\n"
                "#endif  /* _CONTROLLER_CREATE_" << boost::to_upper_copy(table) << "_H */\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }


    ofstream f_controller_cpp(create_cpp.c_str());
    if (f_controller_cpp.is_open()) {
        cout << "create " << create_cpp << endl;
        f_controller_cpp << ""
                "#include \"" << table << "CreateController.h\"\n\n"
                "" << table << "CreateController::" << table << "CreateController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c){\n"
                "    ;\n"
                "}\n\n"
                "// A static factory is used to instantiate the component.\n"
                "// This also defines the name of the component, which is used\n"
                "// in the mapping.\n"
                "static tnt::ComponentFactoryImpl<" << table << "CreateController> factory(\"controllers/crud/" << table << "CreateController\");\n"
                "// The operator() is the main method of the component. It is the\n"
                "// starting point of our component.\n"
                "unsigned " << table << "CreateController::operator() (tnt::HttpRequest& request,\n"
                "					tnt::HttpReply& reply,\n"
                "					tnt::QueryParams& qparam)\n"
                "{\n"
                "\t// This definition imports the _" << table << " variable from the\n"
                "\t// session.\n"
                "\t//TNT_SESSION_GLOBAL_VAR(" << table << ", _" << table << ", ());\n"
                "\tlog_debug(\"" << table << " controller called with qparam=\" << qparam.getUrl());\n"
                "\t" << table << "Model *" << table << " = new " << table << "Model();\n"
                "\ttry{\n"
                << setFunction <<
                "\t\tif (!" << table << "->_create()) log_error( \"error create " << table << "\");\n"
                "\t}catch(cxxtools::ConversionError &e){\n"
                "\t\tlog_error(e.what());\n"
                "\t}\n"
                "\tdelete(" << table << ");\n"
                "\treturn DECLINED;\n"
                "}\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    // update

    ofstream fu_controller_h(update_h.c_str());
    if (fu_controller_h.is_open()) {
        cout << "create " << update_h << endl;
        fu_controller_h << ""
                "#ifndef _CONTROLLER_Update_" << boost::to_upper_copy(table) << "_H\n"
                "#define _CONTROLLER_Update_" << boost::to_upper_copy(table) << "_H\n\n"
                "#include <tnt/component.h>\n"
                "#include <tnt/componentfactory.h>\n"
                "#include <tnt/httprequest.h>\n"
                "#include <tnt/httpreply.h>\n"
                "#include <cxxtools/conversionerror.h>\n"
                "#include <cxxtools/log.h>\n\n"
                "// include the model\n"
                "#include \"model/" << table << "Model.h\"\n"
                "// define a log category\n"
                "log_define(\"controller.CrudUpdate" << table << "\");\n\n"
                "// define a component which is callable from within tntnet.xml\n"
                "class " << table << "UpdateController : public tnt::Component\n"
                "{\n"
                "public:\n"
                "\t" << table << "UpdateController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c);\n"
                "\tvirtual unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);\n"
                "};\n"
                "#endif  /* _CONTROLLER_UPDATE_" << boost::to_upper_copy(table) << "_H */\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }


    ofstream fu_controller_cpp(update_cpp.c_str());
    if (fu_controller_cpp.is_open()) {
        cout << "update " << update_cpp << endl;
        fu_controller_cpp << ""
                "#include \"" << table << "UpdateController.h\"\n\n"
                "" << table << "UpdateController::" << table << "UpdateController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c){\n"
                "    ;\n"
                "}\n\n"
                "// A static factory is used to instantiate the component.\n"
                "// This also defines the name of the component, which is used\n"
                "// in the mapping.\n"
                "static tnt::ComponentFactoryImpl<" << table << "UpdateController> factory(\"controllers/crud/" << table << "UpdateController\");\n"
                "// The operator() is the main method of the component. It is the\n"
                "// starting point of our component.\n"
                "unsigned " << table << "UpdateController::operator() (tnt::HttpRequest& request,\n"
                "					tnt::HttpReply& reply,\n"
                "					tnt::QueryParams& qparam)\n"
                "{\n"
                "\t// model shared var\n"
                "\t//TNT_REQUEST_SHARED_VAR( " << table << "Model, crud_" << table << "Model, ());\n\n"
                "\t// This definition imports the _" << table << " variable from the\n"
                "\t// session.\n"
                "\t//TNT_SESSION_GLOBAL_VAR(" << table << ", _" << table << ", ());\n"
                "\tlog_debug(\"" << table << " controller called with qparam=\" << qparam.getUrl());\n"
                "\t" << table << "Model *" << table << " = new " << table << "Model();\n"
                "\ttry{\n"
                << setFunction <<
                "\t\tif (!" << table << "->_update()) std::cout << \"error update " << table << "\";\n"
                "\t}catch(cxxtools::ConversionError &e){\n"
                "\t\tcout << e.what() << endl;\n"
                "\t}\n"
                "\tdelete(" << table << ");\n"
                "\treturn DECLINED;\n"
                "}\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    // update

    ofstream fr_controller_h(read_h.c_str());
    if (fr_controller_h.is_open()) {
        cout << "create " << read_h << endl;
        fr_controller_h << ""
                "#ifndef _CONTROLLER_READ_" << boost::to_upper_copy(table) << "_H\n"
                "#define _CONTROLLER_READ_" << boost::to_upper_copy(table) << "_H\n\n"
                "#include <tnt/component.h>\n"
                "#include <tnt/componentfactory.h>\n"
                "#include <tnt/httprequest.h>\n"
                "#include <tnt/httpreply.h>\n"
                "#include <cxxtools/conversionerror.h>\n"
                "#include <cxxtools/log.h>\n\n"
                "// include the model\n"
                "#include \"model/" << table << "Model.h\"\n"
                "// define a log category\n"
                "log_define(\"controller.CrudRead" << table << "\");\n\n"
                "// define a component which is callable from within tntnet.xml\n"
                "class " << table << "ReadController : public tnt::Component\n"
                "{\n"
                "public:\n"
                "\t" << table << "ReadController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c);\n"
                "\tvirtual unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);\n"
                "};\n"
                "#endif  /* _CONTROLLER_READ_" << boost::to_upper_copy(table) << "_H */\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }


    ofstream fr_controller_cpp(read_cpp.c_str());
    if (fr_controller_cpp.is_open()) {
        cout << "update " << read_cpp << endl;
        fr_controller_cpp << ""
                "#include \"" << table << "ReadController.h\"\n\n"
                "" << table << "ReadController::" << table << "ReadController(const tnt::Compident& a, const tnt::Urlmapper& b, tnt::Comploader& c){\n"
                "    ;\n"
                "}\n\n"
                "// A static factory is used to instantiate the component.\n"
                "// This also defines the name of the component, which is used\n"
                "// in the mapping.\n"
                "static tnt::ComponentFactoryImpl<" << table << "ReadController> factory(\"controllers/crud/" << table << "ReadController\");\n"
                "// The operator() is the main method of the component. It is the\n"
                "// starting point of our component.\n"
                "unsigned " << table << "ReadController::operator() (tnt::HttpRequest& request,\n"
                "					tnt::HttpReply& reply,\n"
                "					tnt::QueryParams& qparam)\n"
                "{\n"
                "\t// model shared var\n"
                "\t//TNT_REQUEST_SHARED_VAR( " << table << "Model, crud_" << table << "Model, ());\n\n"
                "\t// This definition imports the _" << table << " variable from the\n"
                "\t// session.\n"
                "\t//TNT_SESSION_GLOBAL_VAR(" << table << ", _" << table << ", ());\n"
                "\tlog_debug(\"" << table << " controller called with qparam=\" << qparam.getUrl());\n"
                "\t" << table << "Model *" << table << " = new " << table << "Model();\n"
                "\ttry{\n"
                << setFunction <<
                "\t\tif (!" << table << "->_update()) std::cout << \"error update " << table << "\";\n"
                "\t}catch(cxxtools::ConversionError &e){\n"
                "\t\tcout << e.what() << endl;\n"
                "\t}\n"
                "\tdelete(" << table << ");\n"
                "\treturn DECLINED;\n"
                "}\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }
}

/**
 * 
 * @param conn
 * @param table
 */
void CModel::createModel(tntdb::Connection conn, string table) {
    cxxtools::RegexSMatch m;
    string sql = string("describe ").append(table);
    string setFunction = "";
    string getFunction = "";
    string setFunction1 = "";
    string getFunction1 = "";
    string member = "";
    vector<string> create_sql;
    vector<string> update_sql;
    vector<string> set_sql;
    vector<string> res_get;
    tntdb::Result result = conn.select(sql);

    for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it) {
        tntdb::Row r = *it;
        // varchar
        if (s_varchar.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(string _s);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(string _s){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _s;\n}\n\n");

            getFunction += string("\tstring get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("string ");

            member += string("\tstring ").append(r[0].getString()).append(";\n");
        }
        // int
        if (s_int.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(int _i);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(int _i){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _i;\n}\n\n");

            getFunction += string("\tint get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("int ");
            member += string("\tint ").append(r[0].getString()).append(";\n");
        }
        // unsigned int
        if (s_u_int.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(unsigned int _i);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(unsigned int _i){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _i;\n}\n\n");

            getFunction += string("\tint get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("unsigned int ");
            member += string("\tunsigned int ").append(r[0].getString()).append(";\n");
        }
        // decimal
        if (s_decimal.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(double _d);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(double _d){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _d;\n}\n\n");

            getFunction += string("\tdouble get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("double ");
            member += string("\tdouble ").append(r[0].getString()).append(";\n");
        }
        // double
        if (s_double.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(double _d);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(double _d){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _d;\n}\n\n");

            getFunction += string("\tdouble get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("double ");
            member += string("\tdouble ").append(r[0].getString()).append(";\n");
        }
        // datetime
        if (s_datetime.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(tntdb::Datetime _datetime);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(tntdb::Datetime _datetime){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _datetime;\n}\n\n");

            getFunction += string("\ttntdb::Datetime get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("tntdb::Datetime ");
            member += string("\ttntdb::Datetime ").append(r[0].getString()).append(";\n");
        }
        // timestamp
        if (s_timestamp.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(string _timestamp);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(string _timestamp){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _timestamp;\n}\n\n");

            getFunction += string("\tstring get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("string ");
            member += string("\tstring ").append(r[0].getString()).append(";\n");
        }
        // date
        if (s_date.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(tntdb::Date _date);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(tntdb::Date _date){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _date;\n}\n\n");

            getFunction += string("\ttntdb::Date get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("tntdb::Date ");
            member += string("\ttntdb::Date ").append(r[0].getString()).append(";\n");
        }
        // text
        if (s_text.match(r[1].getString(), m) || r[1].getString() == "longtext") {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(string _t);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(string _t){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _t;\n}\n\n");

            getFunction += string("\tstring get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("string ");
            member += string("\tstring ").append(r[0].getString()).append(";\n");
        }
        // tinyint
        if (s_tinyint.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(bool _b);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(bool _b){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _b;\n}\n\n");

            getFunction += string("\tbool get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("bool ");
            member += string("\tbool ").append(r[0].getString()).append(";\n");
        }

        getFunction1 += string("gen_model_").append(table).append("::get_").append(r[0].getString())
                .append("(){\n"
                "\treturn this->")
                .append(r[0].getString()).append(";\n}\n\n");

        if (r[0].getString() != "id") {
            create_sql.push_back(string(r[0].getString()));
            update_sql.push_back(string(r[0].getString()).append("=:").append(r[0].getString()));
        }
        res_get.push_back(string("row[\"") + r[0].getString().append("\"].get(this->") + r[0].getString().append(");"));
        set_sql.push_back(string("\t\t.set(\"").append(r[0].getString()).append("\", this->get_").append(r[0].getString()).append("())"));
    }

    ofstream gen_f_model_h(string(string("application/model/generate/gen_model_").append(table).append(".h")).c_str());
    if (gen_f_model_h.is_open()) {
        cout << "create/update application/model/generate/gen_model_" << table << ".h" << endl;
        gen_f_model_h << "#ifndef _GEN_MODEL_" << boost::to_upper_copy(table) << "_H\n"
                "#define _GEN_MODEL_" << boost::to_upper_copy(table) << "_H\n\n"
                "#include <tntdb/connection.h>\n"
                "#include <tntdb/connect.h>\n"
                "#include <tntdb/statement.h>\n"
                "#include <tntdb/result.h>\n"
                "#include <tntdb/row.h>\n"
                "#include <tntdb/value.h>\n"
                "#include <cxxtools/log.h>\n\n"
                "#include <tntdb/connect.h>\n\n"
                "#include <string>\n"
                "#include <iostream>\n"
                "using namespace std;\n\n"
                "#include \"helper/CHelper.h\"\n\n"
                "namespace TGEN_MODEL_" << boost::to_upper_copy(table) << " {\n"
                "log_define(\"model.gen_model_" << table << "\");\n\n"
                "class gen_model_" << table << " {\n"
                "public:\n"
                "\tgen_model_" << table << "();\n"
                "\tvirtual ~gen_model_" << table << "();\n"
                "\tgen_model_" << table << " fillData(tntdb::Row &row);\n\n"
                "\tbool _create();\n"
                "\tbool _update();\n"
                "\tbool _delete();\n"
                "\tbool _read();\n"
                "\tbool _where(string where);\n\n"
                << setFunction <<
                "\n"
                << getFunction <<
                "private:\n"
                << member <<
                "\tbool isDirty;\n"
                "};\n}\n";
        gen_f_model_h << "#endif  /* _GEN_MODEL_" << boost::to_upper_copy(table) << "_H */";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    ofstream gen_f_model_cpp(string(string("application/model/generate/gen_model_").append(table).append(".cpp")).c_str());
    if (gen_f_model_cpp.is_open()) {
        cout << "create/update application/model/generate/gen_model_" << table << ".cpp" << endl;
        gen_f_model_cpp << "#include \"model/generate/gen_model_" << table << ".h\"\n\n"
                "namespace TGEN_MODEL_" << boost::to_upper_copy(table) << " {\n";
        gen_f_model_cpp << setFunction1;
        gen_f_model_cpp << getFunction1;

        gen_f_model_cpp <<
                "gen_model_" << table << "::gen_model_" << table << "(){\n"
                "\t;\n"
                "}\n"
                "gen_model_" << table << "::~gen_model_" << table << "(){\n"
                "\t;\n"
                "}\n"
                "bool gen_model_"
                << table << "::_create(){\n"
                "\ttntdb::Connection conn = CHelper::getTntDBConn();\n"
                "\tif(this->isDirty){\n"
                "\t\ttry{\n"
                "\t\tthis->set_date_create(tntdb::Datetime::localtime());\n"
                "\t\t\ttntdb::Statement st = conn.prepare(\"INSERT INTO "
                << table << " (" << boost::algorithm::join(create_sql, ", ")
                << ") VALUES (:"
                << boost::algorithm::join(create_sql, " ,:") <<
                ")\");\n"
                "\t\t\tst\n\t"
                << boost::algorithm::join(set_sql, "\n\t") <<
                "\n\t\t\t.execute();\n"
                "\t\t\tconn = tntdb::Connection();\n"
                "\t\t\treturn true;\n"
                "\t\t}"
                "catch (const std::exception& e) {\n"
                "\t\t\tlog_error(e.what());\n"
                "\t\t\tconn = tntdb::Connection();\n"
                "\t\t\treturn false;\n"
                "\t\t}\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                "bool gen_model_" << table << "::_update(){\n"
                "\tif(this->isDirty){\n"
                "\ttntdb::Connection conn = CHelper::getTntDBConn();\n"
                "\t\ttry{\n"
                "\t\tthis->set_date_update(tntdb::Datetime::localtime());\n"
                "\t\t\ttntdb::Statement st = conn.prepare(\"UPDATE " << table
                << " SET "
                << boost::algorithm::join(update_sql, ", ") <<
                " WHERE id=:id\");\n"
                "\t\t\tst\n\t"
                << boost::algorithm::join(set_sql, "\n\t")
                << "\n\t\t\t.execute();\n"
                "\t\t\tconn = tntdb::Connection();\n"
                "\t\t\treturn true;\n"
                "\t\t}"
                "catch (const std::exception& e) {\n"
                "\t\t\tlog_error(e.what());\n"
                "\t\t\tconn = tntdb::Connection();\n"
                "\t\t\treturn false;\n"
                "\t\t}\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                "bool gen_model_" << table << "::_delete(){\n"
                "\ttntdb::Connection conn = CHelper::getTntDBConn();\n"
                "\ttry{\n"
                "\t\ttntdb::Statement st = conn.prepare(\"DELETE * FROM " << table << " WHERE id=:id\");\n"
                "\t\tst.set(\"id\",this->get_id()).execute();\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn true;\n"
                "\t}catch (const std::exception& e) {\n"
                "\t\tlog_error(e.what());\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn false;\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                "bool gen_model_" << table << "::_read(){\n"
                "\ttntdb::Connection conn = CHelper::getTntDBConn();\n"
                "\ttry{\n"
                "\t\ttntdb::Row row =conn.prepare(\"SELECT * FROM " << table << " WHERE id=:id\")\n"
                "\t\t\t.set(\"id\",this->get_id())\n\t\t\t.selectRow();\n\t\t"
                << boost::algorithm::join(res_get, "\n\t\t") <<
                "\n\t\tthis->isDirty = false;\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn true;\n"
                "\t}catch (const std::exception& e) {\n"
                "\t\tlog_error(e.what());\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn false;\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                "bool gen_model_" << table << "::_where(string where){\n"
                "\ttntdb::Connection conn = CHelper::getTntDBConn();\n"
                "\ttry{\n"
                "\t\tstring sql = \"SELECT * FROM " << table << " WHERE \";\n"
                "\t\tsql.append(where);\n"
                "\t\ttntdb::Row row =conn.prepare(sql)\n"
                << boost::algorithm::join(set_sql, "\n") <<
                "\n\t\t\t.selectRow();\n\t\t"
                << boost::algorithm::join(res_get, "\n\t\t") <<
                "\n\t\tthis->isDirty = false;\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn true;\n"
                "\t}catch (const std::exception& e) {\n"
                "\t\tlog_error(e.what());\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn false;\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                "gen_model_" << table << " gen_model_" << table << "::fillData(tntdb::Row &row) {\n"
                "\tgen_model_" << table << " _t;\n\t";
        string s_t = boost::algorithm::join(res_get, "\n\t");
        boost::replace_all(s_t, "this->", "_t.");
        gen_f_model_cpp << s_t <<
                "\n"
                "\treturn _t;\n"
                "}\n\n}\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    auto f_exist = [] (string name) {
        ifstream _f(name);
        return _f ? false : true;
    };

    //  public classes
    if (f_exist(string(string("application/model/").append(table).append("Model.h")).c_str())) {
        ofstream f_model_h(string(string("application/model/").append(table).append("Model.h")).c_str());
        if (f_model_h.is_open()) {
            cout << "create application/model/" << table << "Model.h" << endl;
            f_model_h << "#ifndef _MODEL_" << boost::to_upper_copy(table) << "_H\n"
                    "#define _MODEL_" << boost::to_upper_copy(table) << "_H\n\n"
                    "#include <tntdb/connection.h>\n"
                    "#include <tntdb/connect.h>\n"
                    "#include <tntdb/statement.h>\n"
                    "#include <tntdb/result.h>\n"
                    "#include <tntdb/row.h>\n"
                    "#include <tntdb/value.h>\n"
                    "#include <cxxtools/log.h>\n\n"
                    "#include <tntdb/connect.h>\n\n"
                    "#include <string>\n"
                    "#include <iostream>\n\n"
                    "using namespace std;\n\n"
                    "#include \"model/generate/gen_model_" << table << ".h\"\n\n"
                    "class " << table << "Model : public TGEN_MODEL_" << boost::to_upper_copy(table) << "::gen_model_" << table << "  {\n"
                    "public:\n"
                    "\t" << table << "Model();\n"
                    "\tvirtual ~" << table << "Model();\n\n"
                    "private:\n"
                    "};\n"
                    "#endif  /* _MODEL_" << boost::to_upper_copy(table) << "_H */\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/model/" << table << "Model.h (exist ?)" << endl;
    }

    if (f_exist(string(string("application/model/").append(table).append("Model.cpp")).c_str())) {
        ofstream f_model_cpp(string(string("application/model/").append(table).append("Model.cpp")).c_str());
        if (f_model_cpp.is_open()) {
            cout << "create application/model/" << table << "Model.cpp" << endl;
            f_model_cpp << "#include \"model/" << table << "Model.h\"\n\n"
                    "" << table << "Model::" << table << "Model(){\n"
                    "\t;\n"
                    "}\n"
                    "" << table << "Model::~" << table << "Model(){\n"
                    "\t;\n"
                    "}\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/model/" << table << "Model.h (exist ?)" << endl;
    }
}
