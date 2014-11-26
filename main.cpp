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
#include <cxxtools/regex.h>
#include <cxxtools/arg.h>

#include <cstdlib>
#include <iostream>
using namespace std;

#include "CModel.h"
#include "CView.h"
#include "CSystem.h"
#include "CController.h"
#include "CDatabase.h"

int main(int argc, char* argv[]) {
    CModel model;
    CView view;
    CSystem my_system;
    CController controller;
    CDatabase database;

    try {
        //cxxtools::Arg<std::string> string_scaffold(argc, argv, '-scaffold');
        cxxtools::Arg<std::string> string_controller(argc, argv, "controller");
        cxxtools::Arg<std::string> string_model(argc, argv, "model");
        cxxtools::Arg<std::string> string_view(argc, argv, "view");
        cxxtools::Arg<std::string> string_newProject(argc, argv, "new");
        cxxtools::Arg<std::string> string_crudModel(argc, argv, "crud");

        if (string_model.isSet()) {
            model.createModel(CSystem::getTntDBConn(), string_model);
        }
        if (string_crudModel.isSet()) {
            model.createModelCrud(CSystem::getTntDBConn(), string_crudModel);
        }
        if (string_view.isSet()) {
            view.createView(string_view);
        }
        if (string_newProject.isSet()) {
            my_system.newProject(string_newProject);
        }
        if (string_controller.isSet()) {
            controller.createController(string_controller);
        }

        // schalter
        if (argc > 1) {
            if (string("--parse-html").compare(argv[1]) == 0) {
                view.parseHtmlView();
            }
            if (string("--create-make").compare(argv[1]) == 0) {
                my_system.createMakeIncludeEcpp();
            }
            if (string("--dump-mysql").compare(argv[1]) == 0) {
                my_system.dumpMysql();
            }
            if (string("db").compare(argv[1]) == 0) {
                if(string("table").compare(argv[2]) == 0){
                    database.createTable(CSystem::getTntDBConn(),argv[3]);
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    return 0;
}

