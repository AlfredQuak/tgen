#ifndef CSYSTEM_H
#define	CSYSTEM_H

#include <tntdb/result.h>
#include <tntdb/row.h>
#include <tntdb/value.h>
#include <tntdb/statement.h>
#include <tntdb/connection.h>
#include <tntdb/connect.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <cxxtools/inifile.h>

#include "CView.h"

class CSystem {
public:
    CSystem();
    virtual ~CSystem();
    
    static tntdb::Connection getTntDBConn();
    void newProject(string projectName);
    void mapping();
private:

};

#endif	/* CSYSTEM_H */

