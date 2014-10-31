#ifndef CMODEL_H
#define	CMODEL_H

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include <tntdb/result.h>
#include <tntdb/row.h>
#include <tntdb/value.h>
#include <tntdb/statement.h>
#include <tntdb/connection.h>
#include <tntdb/connect.h>

#include <boost/algorithm/string.hpp>

#include <cxxtools/regex.h>

class CModel {
public:
    CModel();
    virtual ~CModel();
    
    void createModel(tntdb::Connection conn, string table);
private:

};

#endif	/* CMODEL_H */

