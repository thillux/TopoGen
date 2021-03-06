/*
 * Copyright (c) 2013-2015, Michael Grey and Markus Theil
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SubmarineCable.hpp"

#include <boost/log/trivial.hpp>
#include <iostream>

SubmarineCable::SubmarineCable(std::string dbPath) {
    int retval = sqlite3_open(dbPath.c_str(), &_sqliteDB);

    // If connection failed, handle returns NULL
    if (retval) {
        BOOST_LOG_TRIVIAL(error) << "Database connection failed in SubmarineCable!";
    }

    std::string queryString = "SELECT lat1, lon1, lat2, lon2, link_id FROM submarinecable_edges";
    sqlite3_prepare_v2(_sqliteDB, queryString.c_str(), queryString.length(), &_stmt, NULL);

    retval = sqlite3_step(_stmt);
    if (retval == SQLITE_ROW)
        _rowAvailable = true;
    else
        _rowAvailable = false;
}

SubmarineCableEdge SubmarineCable::getNext() {
    double lat1 = sqlite3_column_double(_stmt, 0);
    double lon1 = sqlite3_column_double(_stmt, 1);
    double lat2 = sqlite3_column_double(_stmt, 2);
    double lon2 = sqlite3_column_double(_stmt, 3);
    int linkID = sqlite3_column_int(_stmt, 4);

    int retval = sqlite3_step(_stmt);
    if (retval == SQLITE_ROW)
        _rowAvailable = true;
    else
        _rowAvailable = false;

    SubmarineCableEdge edge(std::make_pair(lat1, lon1), std::make_pair(lat2, lon2), linkID);

    return edge;
}
