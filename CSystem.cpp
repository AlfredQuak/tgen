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
#include "CSystem.h"

CSystem::CSystem() {
}

CSystem::~CSystem() {
}

void CSystem::dumpMysql() {
    cxxtools::IniFile ini("config/settings.ini");
    string dumpMysql = string("mysqldump --add-drop-table --dump-date ")
            .append(ini.getValue(ini.getValue("settings", "connectiontype"), "DatabaseName"))
            .append(" --user='")
            .append(ini.getValue(ini.getValue("settings", "connectiontype"), "UserName"))
            .append("' --password='")
            .append(ini.getValue(ini.getValue("settings", "connectiontype"), "Password"))
            .append("'");
    if (!ini.getValue(ini.getValue("settings", "connectiontype"), "Host").empty()) {
        dumpMysql
                .append(" --host ")
                .append(ini.getValue(ini.getValue("settings", "connectiontype"), "Host"));
    }

    dumpMysql.append(" > config/mysqlCreate_dump.sql");
    std::system(dumpMysql.c_str());
}

void CSystem::createMakeIncludeEcpp() {
    cxxtools::IniFile ini("config/settings.ini");

    vector<string> includeString;
    vector<string> cleanString;
    vector<string> resources;

    auto f_exist = [] (string name) {
        ifstream _f(name);
        return _f ? false : true;
    };
    try {
        ofstream rescMakeInc("config/resourceMakeInclude.mk");
        if (rescMakeInc.is_open()) {
            rescMakeInc << ""
                    ".resource: \n"
                    "\tecppc -bb -z -I./application/view/html -p -o extern/resources -n resources \\\n";

            boost::filesystem::path path = "./application/view/html/img";
            boost::filesystem::recursive_directory_iterator itr(path);

            while (itr != boost::filesystem::recursive_directory_iterator()) {
                string _t = itr->path().string();
                boost::replace_all(_t, "./application/view/html", "");
                resources.push_back(string("\t").append(_t));
                ++itr;
            }
            
            path = "./application/view/html/scripts";
            boost::filesystem::recursive_directory_iterator itr_1(path);
            
            while (itr_1 != boost::filesystem::recursive_directory_iterator()) {
                string _t = itr_1->path().string();
                boost::replace_all(_t, "./application/view/html", "");
                resources.push_back(string("\t").append(_t));
                ++itr_1;
            }
            rescMakeInc << boost::algorithm::join(resources, " \\\n");
            rescMakeInc.close();
        }

    } catch (...) {

    }
    try {
        ofstream ecppMakeInc("config/ecppMakeInclude.mk");
        if (ecppMakeInc.is_open()) {
            ecppMakeInc << ""
                    "$(APP_NAME): \\\n";

            boost::filesystem::path path = ".";
            boost::filesystem::recursive_directory_iterator itr(path);

            while (itr != boost::filesystem::recursive_directory_iterator()) {
                if (itr->path().filename() == "crud") {
                    //std::cout << itr->path().filename() << std::endl;
                    itr.no_push(); // don't recurse into this directory.
                }
                if (itr->path().extension().compare(".ecpp") == 0) {
                    string _t = itr->path().string();
                    boost::replace_all(_t, ".ecpp", ".o");
                    includeString.push_back(string("\t").append(_t));
                    cleanString.push_back(string("\trm -rf ").append(_t));
                    //ecppMakeInc << "\t" << _t << " \\\n";
                }

                if (itr->path().extension().compare(".cpp") == 0) {
                    string _t = itr->path().string();
                    boost::replace_all(_t, ".cpp", ".o");
                    includeString.push_back(string("\t").append(_t));
                    cleanString.push_back(string("\trm -rf ").append(_t));
                    //ecppMakeInc << "\t" << _t << " \\\n";
                }
                ++itr;
            }

            path = "./config";

            while (itr != boost::filesystem::recursive_directory_iterator()) {
                if (itr->path().extension().compare(".h") == 0) {
                    string _t = itr->path().string();
                    boost::replace_all(_t, ".h", ".o");
                    includeString.push_back(string("\t").append(_t));
                    cleanString.push_back(string("\trm -rf ").append(_t));
                    //ecppMakeInc << "\t" << _t << " \\\n";
                }
                ++itr;
            }
            ecppMakeInc << boost::algorithm::join(includeString, " \\\n");
            ecppMakeInc << "\n\t${CXX} -o $@ $^ ${LDFLAGS}\n";
            ecppMakeInc << "\n\nclean: \n" << boost::algorithm::join(cleanString, " \n");
            ecppMakeInc << "\n\trm -rf $(APP_NAME)";
            ecppMakeInc.close();
        }

        if (f_exist("Makefile")) {
            ofstream ecppMake("Makefile");
            if (ecppMake.is_open()) {
                ecppMake << ""
                        "ECPPC=/usr/bin/ecppc\n"
                        "CXXFLAGS+=-I/usr/local/include -I./application/ -I./helper -I./config -I./extern -I./lib -I./plugin -I./ -O2 -std=c++11\n"
                        "LDFLAGS+=-L/usr/local/lib -ltntnet -ltntdb -lcxxtools\n"
                        "APP_NAME=" << ini.getValue("settings", "projectname") << "\n"
                        "\n"
                        "all: .prebuild $(APP_NAME)\n"
                        "\n"
                        ".prebuild:\n"
                        "\ttgen --create-make\n"
                        "\n"
                        "include config/ecppMakeInclude.mk\n"
                        "\n"
                        ".SUFFIXES: .ecpp\n"
                        "\n"
                        ".ecpp.cpp:\n"
                        "\t${ECPPC} ${ECPPFLAGS} ${ECPPFLAGS_CPP} -o $@ $<\n";
            }
        }
    } catch (...) {

    }
}

tntdb::Connection CSystem::getTntDBConn() {

    cxxtools::IniFile ini("config/settings.ini");
    string connectionstring;

    if (boost::to_upper_copy(
            ini.getValue(ini.getValue("settings", "connectiontype"), "DriverType")
            ).compare("MYSQL") == 0) {

        connectionstring
                .append("mysql:dbname=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "DatabaseName")).append(";")
                .append("user=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "UserName")).append(";")
                .append("passwd=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "Password")).append(";");

    } else
        if (boost::to_upper_copy(
            ini.getValue(ini.getValue("settings", "connectiontype"), "DriverType")
            ).compare("POSTGRESQL") == 0) {

        connectionstring
                .append("postgresql:dbname=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "DatabaseName")).append(" ")
                .append("user=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "UserName")).append(" ")
                .append("passwd=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "Password")).append(" ");

    } else
        if (boost::to_upper_copy(
            ini.getValue(ini.getValue("settings", "connectiontype"), "DriverType")
            ).compare("SQLITE") == 0) {

        connectionstring
                .append("sqlite:").append(ini.getValue(ini.getValue("settings", "connectiontype"), "DatabaseName")).append(";")
                /*.append("user=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "UserName")).append(";")
                .append("passwd=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "Password")).append(";")*/;

    } else
        if (boost::to_upper_copy(
            ini.getValue(ini.getValue("settings", "connectiontype"), "DriverType")
            ).compare("ORACLE") == 0) {

        connectionstring
                .append("mysql:dbname=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "DatabaseName")).append(";")
                .append("user=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "UserName")).append(";")
                .append("passwd=").append(ini.getValue(ini.getValue("settings", "connectiontype"), "Password")).append(";");

    }

    return tntdb::connect(connectionstring);
}

void CSystem::newProject(string projectName) {

    boost::filesystem::create_directories(projectName);

    boost::filesystem::path _p = string(boost::filesystem::current_path().string())
            .append(string("/"))
            .append(projectName);
    boost::filesystem::current_path(_p);

    boost::filesystem::create_directories("config");
    boost::filesystem::create_directories("plugin");
    boost::filesystem::create_directories("lib");
    boost::filesystem::create_directories("log");
    boost::filesystem::create_directories("helper");
    boost::filesystem::create_directories("application/view/html/img");
    boost::filesystem::create_directories("application/view/html/scripts");
    boost::filesystem::create_directories("application/view/ecpp");
    boost::filesystem::create_directories("application/model/generate");
    boost::filesystem::create_directories("application/controllers");
    boost::filesystem::create_directories("application/component");

    auto f_exist = [] (string name) {
        ifstream _f(name);
        return _f ? false : true;
    };

    string main_file = "main.cpp";
    string index_html_f = "application/view/html/index.html";
    string database_f = "config/settings.ini";
    string give_static_f = "application/view/ecpp/giveStaticFile.ecpp";
    string logging_f = "log.xml";

    if (f_exist(give_static_f)) {
        ofstream give_static(give_static_f);
        if (give_static.is_open()) {
            give_static << ""
                    "<%pre>\n"
                    "#include <iostream>\n"
                    "#include <fstream>\n"
                    "#include <tnt/mimedb.h>\n"
                    "</%pre>\n"
                    "<%cpp>\n"
                    "std::ifstream in((\"./application/view/html/\" + request.getPathInfo()).c_str());\n"
                    "if (!in){\n"
                    "  return DECLINED;\n"
                    "}else{\n"
                    "    tnt::MimeDb mimeDb(\"/etc/mime.types\");\n"
                    "    reply.setContentType(mimeDb.getMimetype(request.getPathInfo()));\n"
                    "    reply.out() << in.rdbuf();  // send the content of the ifstream to the page\n"
                    "}\n"
                    "</%cpp>\n";
            give_static.close();
        }
    }

    if (f_exist(index_html_f)) {
        ofstream index_html(index_html_f);
        if (index_html.is_open()) {
            index_html << ""
                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
                    "<html>\n"
                    "  <head>\n"
                    "    <title></title>\n"
                    "    <!-- index.html -->\n"
                    "  </head>\n"
                    "  <body>\n"
                    "    <p>your index.html content</p>\n"
                    "  </body>\n"
                    "</html>\n\n";
            index_html.close();
        }
    }

    if (f_exist(main_file)) {
        ofstream main_cpp(main_file);
        if (main_cpp.is_open()) {
            main_cpp << ""
                    "#include <tnt/process.h>\n"
                    "#include <tnt/tntnet.h>\n"
                    "#include <tnt/cmd.h>\n"
                    "#include <tnt/tntconfig.h>\n\n"
                    "#include <cxxtools/log.h>\n"
                    "#include <cxxtools/arg.h>\n\n"
                    "#include <iostream>\n"
                    "using namespace std;\n\n"
                    "#include \"config/url_mapping.h\"\n\n"
                    "log_define(\"" << projectName << "\")\n\n"
                    "int main(int argc, char* argv[]) {\n"
                    "        log_init();\n"
                    "        log_debug(\"starting app ... \");\n"
                    "    try {\n"
                    "        tnt::Tntnet app;\n"
                    "        app.listen(8000);\n"
                    "        app.mapUrl(\"^/$\", \"index\");\n\n"
                    "        _mappingUrl(app);\n\n"
                    "        // create routing for mvc\n"
                    "        // comment out if unlike\n"
                    "        // your url like: login \n"
                    "        // routing to loginController and loginView\n"
                    "        app.mapUrl(\"^/(.*)$\", \"$1Controller\" );\n"
                    "        app.mapUrl(\"^/(.*)$\", \"$1View\" );\n\n"
                    "        app.mapUrl(\"^/([^.]+)(\\\\..+)?\", \"$1\");\n"
                    "        app.mapUrl(\"^/img/(.*)\", \"giveStaticFile\");\n"
                    "        app.mapUrl(\"^/scripts/(.*)\", \"giveStaticFile\");\n"
                    "        std::cout << \"server will start ..\" << std::endl;\n"
                    "        app.run();\n"
                    "    } catch (const std::exception& e) {\n"
                    "        std::cerr << e.what() << std::endl;\n"
                    "    }\n\n"
                    "    return 0;\n"
                    "}\n";
            main_cpp.close();
        }
    }

    if (f_exist(database_f)) {
        ofstream database_h(database_f);
        if (database_h.is_open()) {
            database_h << ""
                    "#####################################\n"
                    "## global settings\n"
                    "#####################################\n"
                    "[settings]\n"
                    "connectiontype=dev\n"
                    "projectname=" << projectName << "\n"
                    "#####################################\n"
                    "## database settings\n"
                    "#####################################\n"
                    "[dev]\n"
                    "DriverType=MYSQL\n"
                    "DatabaseName=sploindyBI\n"
                    "#HostName=\n"
                    "#Port=\n"
                    "UserName=root\n"
                    "Password=pass\n"
                    "#ConnectOptions=\n"
                    "\n"
                    "[test]\n"
                    "DriverType=MYSQL\n"
                    "#DatabaseName=\n"
                    "#HostName=\n"
                    "#Port=\n"
                    "#UserName=\n"
                    "#Password=\n"
                    "#ConnectOptions=\n"
                    "\n"
                    "[product]\n"
                    "DriverType=MYSQL\n"
                    "#DatabaseName=\n"
                    "#HostName=\n"
                    "#Port=\n"
                    "#UserName=\n"
                    "#Password=\n"
                    "#ConnectOptions=\n";
            database_h.close();
        }
    }

    if (f_exist(logging_f)) {
        ofstream logging_xml(logging_f);
        if (logging_xml.is_open()) {
            logging_xml << ""
                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                    "<!--\n"
                    "  sample logging-properties for application sploindyBI\n"
                    "  put this in sploindyBI.properties and use:\n"
                    "    log_init(\"" << projectName << ".properties\");\n"
                    "  in your application to initialize logging\n"
                    "\n"
                    "  define categories with:\n"
                    "    log_define(\"some.category\")\n"
                    "  this defines a static function, so you must put it outside other functions.\n"
                    "  you can define a category per file or a category per namespace.\n"
                    "\n"
                    "  print logging-messages with:\n"
                    "    log_fatal(\"some fatal message\");\n"
                    "    log_error(\"some error message\");\n"
                    "    log_warn(\"some warn message\");\n"
                    "    log_info(\"some info message\");\n"
                    "    log_debug(\"some debug message\");\n"
                    "-->\n"
                    "<logging>\n"
                    "    <rootlogger>INFO</rootlogger>\n"
                    "    <loggers>\n"
                    "        <logger>\n"
                    "            <category>" << projectName << "</category>\n"
                    "            <level>INFO</level>\n"
                    "        </logger>\n"
                    "    </loggers>\n"
                    "    <!-- <file>log/" << projectName << ".log</file> -->\n"
                    "    <!-- <maxfilesize>1MB</maxfilesize> -->\n"
                    "    <!-- <maxbackupindex>2</maxbackupindex> -->\n"
                    "    <!-- <host>localhost:1234</host> --> <!--  # send log-messages with udp -->\n"
                    "</logging>\n";
            logging_xml.close();
        }
    }

    CView myView;
    myView.parseHtmlView();
    this->mapping();
    this->createMakeIncludeEcpp();
}

void CSystem::mapping() {
    string mapping_file = "config/url_mapping.h";
    ofstream mapping_h(mapping_file);
    if (mapping_h.is_open()) {
        mapping_h << ""
                "#include \"tnt/tntnet.h\"\n\n"
                "void _mappingUrl(tnt::Tntnet & app){\n";
        try {
            boost::filesystem::path path = "application/view/html";
            boost::filesystem::recursive_directory_iterator itr(path);

            while (itr != boost::filesystem::recursive_directory_iterator()) {
                if (itr->path().extension().compare(".html") == 0) {
                    string _t = itr->path().string();
                    boost::erase_all(_t, "application/view/html");
                    boost::replace_all(_t, "//", "/");

                    mapping_h << "\tapp.mapUrl(\"^" << _t << "$\", \"";

                    _t = itr->path().filename().string();
                    boost::erase_all(_t, ".html");
                    mapping_h << _t << "\");\n";
                }

                ++itr;
            }
        } catch (...) {

        }
        mapping_h << ""
                "}\n";
        mapping_h.close();
    }
}