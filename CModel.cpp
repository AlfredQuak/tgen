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

static cxxtools::Regex s_varchar("^varchar\\(([0-9]+)\\)$");
static cxxtools::Regex s_tinyint("^tinyint\\(([0-9]+)\\)$");
static cxxtools::Regex s_u_int("^int\\(([0-9]+)\\) unsigned$");

static cxxtools::Regex s_text("^text$");
static cxxtools::Regex s_int("^int\\(([0-9]+)\\)$");
static cxxtools::Regex s_decimal("^decimal\\(([0-9]+),([0-9]+)\\)$");
static cxxtools::Regex s_datetime("^datetime$");
static cxxtools::Regex s_date("^date$");

CModel::CModel() {
}

CModel::~CModel() {
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
        // datetime
        if (s_datetime.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(string _datetime);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(string _datetime){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _datetime;\n}\n\n");

            getFunction += string("\tstring get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("string ");
            member += string("\tstring ").append(r[0].getString()).append(";\n");
        }
        // date
        if (s_date.match(r[1].getString(), m)) {
            setFunction += string("\tvoid set_").append(r[0].getString()).append("(string _date);\n");
            setFunction1 += string("void gen_model_").append(table).append("::set_").append(r[0].getString()).append("(string _date){\n");
            setFunction1 += "\tthis->isDirty = true;\n";
            setFunction1 += string("\tthis->").append(r[0].getString()).append(" = _date;\n}\n\n");

            getFunction += string("\tstring get_").append(r[0].getString()).append("();\n");
            getFunction1 += string("string ");
            member += string("\tstring ").append(r[0].getString()).append(";\n");
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
                "#include <iostream>\n\n"
                "using namespace std;\n\n"
                "#include \"helper/CHelper.h\"\n\n"
                "class gen_model_" << table << " {\n"
                "public:\n"
                "\tgen_model_" << table << "();\n"
                "\tvirtual ~gen_model_" << table << "();\n\n"
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
                "};\n";
        gen_f_model_h << "#endif  /* _GEN_MODEL_" << boost::to_upper_copy(table) << "_H */\n";
    } else {
        cout << "Unable to write file.. " << endl;
    }

    ofstream gen_f_model_cpp(string(string("application/model/generate/gen_model_").append(table).append(".cpp")).c_str());
    if (gen_f_model_cpp.is_open()) {
        cout << "create/update application/model/generate/gen_model_" << table << ".cpp" << endl;
        gen_f_model_cpp << "#include \"model/generate/gen_model_" << table << ".h\"\n\n";
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
                "\t\t\tstd::cerr << e.what() << std::endl;\n"
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
                "\t\t\ttntdb::Statement st = conn.prepare(\"UPDATE " << table
                << " SET "
                << boost::algorithm::join(update_sql, ", ") <<
                " WHERE id=:id)\");\n"
                "\t\t\tst\n\t"
                << boost::algorithm::join(set_sql, "\n\t")
                << "\n\t\t\t.execute();\n"
                "\t\t\tconn = tntdb::Connection();\n"
                "\t\t\treturn true;\n"
                "\t\t}"
                "catch (const std::exception& e) {\n"
                "\t\t\tstd::cerr << e.what() << std::endl;\n"
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
                "\t\tstd::cerr << e.what() << std::endl;\n"
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
                "\t\tstd::cerr << e.what() << std::endl;\n"
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
                "\t\tstd::cerr << e.what() << std::endl;\n"
                "\t\tconn = tntdb::Connection();\n"
                "\t\treturn false;\n"
                "\t}\n"
                "\treturn false;\n"
                "}\n\n"
                ;
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
                    "class " << table << "Model : public gen_model_" << table << "  {\n"
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
                    "model_" << table << "::model_" << table << "(){\n"
                    "\t;\n"
                    "}\n"
                    "model_" << table << "::~model_" << table << "(){\n"
                    "\t;\n"
                    "}\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/model/" << table << "Model.h (exist ?)" << endl;
    }
}
