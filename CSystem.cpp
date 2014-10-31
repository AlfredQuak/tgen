#include "CSystem.h"

CSystem::CSystem() {
}

CSystem::~CSystem() {
}

tntdb::Connection CSystem::getTntDBConn() {
    
    cxxtools::IniFile ini("config/database.ini");
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
    boost::filesystem::create_directories("application/view/html");
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
    string database_f = "config/database.ini";


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
                    "    <p>your content</p>\n"
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
                    "int main(int argc, char* argv[]) {\n"
                    "    try {\n"
                    "        std::set<std::string> filesProcessed;\n"
                    "        tnt::Tntnet app;\n"
                    "        app.mapUrl(\"^/$\", \"index\");\n\n"
                    "        _mappingUrl(app);\n\n"
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

    CView myView;
    myView.parseHtmlView();
    this->mapping();
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