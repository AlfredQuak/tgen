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
#ifndef CVIEW_H
#define	CVIEW_H

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <sys/stat.h>

class CView {
public:
    CView();
    virtual ~CView();
    
    void createView(string view);
    void parseHtmlView();
private:

};

#endif	/* CVIEW_H */

