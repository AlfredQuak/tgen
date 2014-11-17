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
#include "CView.h"

CView::CView() {
}

CView::~CView() {
}

void CView::createView(string view) {
    auto f_exist = [] (string name) {
        ifstream _f(name);
        return _f ? false : true;
    };

    vector<string> path_view;
    boost::split(path_view, view, boost::is_any_of("/"));

    if (path_view.size() >= 2) {
        string create_path_html = "application/view/html/";
        string create_path_ecpp = "application/view/ecpp/";

        for (size_t i = 0; i < path_view.size() - 1; i++) {
            create_path_html += path_view[i].append("/");
            create_path_ecpp += path_view[i].append("/");
        }

        boost::filesystem::create_directories(create_path_html);
        boost::filesystem::create_directories(create_path_ecpp);
    }

    if (f_exist(string(string("application/view/ecpp/").append(view).append(".ecpp")).c_str())) {
        ofstream ecpp_file(string(string("application/view/ecpp/").append(view).append(".ecpp")).c_str());

        string include_string = string("application/view/html/").append(view).append(".html");
        boost::replace_all(include_string, "//", "/");

        if (ecpp_file.is_open()) {
            cout << "create application/view/ecpp/" << view << ".ecpp" << endl;
            ecpp_file << ""
                    "<%pre>\n"
                    "\n"
                    "</%pre>\n"
                    "<%args>\n\n</%args>\n"
                    "<%session>\n\n</%session>\n"
                    "<%request\n"
                    "    scope=\"global\">\n"
                    "</%request>\n"
                    "<%cpp>\n\n</%cpp>\n\n"
                    "<%include>" << include_string << "</%include>\n\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/view/ecpp/" << view << ".ecpp (exist ?)" << endl;
    }

    if (f_exist(string(string("application/view/html/").append(view).append(".html")).c_str())) {
        ofstream html_file(string(string("application/view/html/").append(view).append(".html")).c_str());
        if (html_file.is_open()) {
            cout << "create application/view/html/" << view << ".html" << endl;
            html_file << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
                    "<html>\n"
                    "  <head>\n"
                    "    <title>" << view << "</title>\n"
                    "    <!-- " << view << ".html -->\n"
                    "  </head>\n"
                    "  <body>\n"
                    "    <p>your " << view << ".html content</p>\n"
                    "  </body>\n"
                    "</html>\n\n";
        } else {
            cout << "Unable to write file.. " << endl;
        }
    } else {
        cout << "Unable to write file application/view/html/" << view << ".html (exist ?)" << endl;
    }
}

void CView::parseHtmlView() {
    cout << "start parsing ... " << endl;

    boost::filesystem::path path = "application/view/html";
    boost::filesystem::recursive_directory_iterator itr(path);

    while (itr != boost::filesystem::recursive_directory_iterator()) {
        if (itr->path().extension().compare(".html") == 0) {
            string _t = itr->path().string();

            boost::replace_all(_t, "/html/", "/ecpp/");
            boost::erase_all(_t, ".html");
            boost::erase_all(_t, "application/view/html");
            boost::erase_all(_t, "application/view/ecpp");

            boost::replace_all(_t, "//", "/");
            this->createView(_t);
        }

        ++itr;
    }
    cout << "parsing ready ..." << endl;
}
