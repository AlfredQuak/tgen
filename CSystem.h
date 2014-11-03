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
    void createMakeIncludeEcpp();
private:

};

#endif	/* CSYSTEM_H */

