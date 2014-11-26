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
#include "CDatabase.h"

CDatabase::CDatabase() {
}

CDatabase::~CDatabase() {
}

bool CDatabase::createTable(tntdb::Connection conn, string tableName) {
    string sql = ""
            "CREATE TABLE `tablename` ("
            "`id` int(11) NOT NULL,"
            "`date_create` datetime NOT NULL,"
            "`date_update` datetime NOT NULL,"
            "`date_login` datetime NOT NULL,"
            "PRIMARY KEY (`id`),"
            "UNIQUE KEY `id` (`id`)"
            ")";
    boost::replace_all(sql, "tablename", tableName);
    
    try {
        conn.prepare(sql).execute();
        conn = tntdb::Connection();
    } catch (const std::exception& e) {

        conn = tntdb::Connection();
        return false;
    }


    return true;
}