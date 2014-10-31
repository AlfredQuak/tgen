#include <cxxtools/regex.h>
#include <cxxtools/arg.h>

#include <cstdlib>
#include <iostream>
using namespace std;

#include "CModel.h"
#include "CView.h"
#include "CSystem.h"
#include "CController.h"


int main(int argc, char* argv[]) {
    CModel model;
    CView view;
    CSystem my_system;
    CController controller;

    try {
        //cxxtools::Arg<std::string> string_scaffold(argc, argv, '-scaffold');
        cxxtools::Arg<std::string> string_controller(argc, argv, "controller");
        cxxtools::Arg<std::string> string_model(argc, argv, "model");
        cxxtools::Arg<std::string> string_view(argc, argv, "view");
        cxxtools::Arg<std::string> string_newProject(argc, argv, "new");

        if (string_model.isSet()) {
            model.createModel(CSystem::getTntDBConn(), string_model);
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
        if (argc > 2) {
            if (string("--parse-html").compare(argv[1]) == 0) {
                view.parseHtmlView();
            }
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    return 0;
}

