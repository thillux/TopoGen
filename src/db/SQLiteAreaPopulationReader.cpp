/*
 * Copyright (c) 2013-2014, Michael Grey and Markus Theil
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

#include "SQLiteAreaPopulationReader.hpp"
#include <sstream>
#include <cassert>
#include <iostream>

SQLiteAreaPopulationReader::SQLiteAreaPopulationReader(std::string dbPath, double lat, double lon, double length)
    : _lat(lat), _lon(lon), _length(length) {
    int retval = sqlite3_open(dbPath.c_str(), &_ppDB);

    // If connection failed, handle returns NULL
    if (retval) {
        std::cerr << "Database connection failed" << std::endl;
    }

    // std::cout << "Checking box: " << _lat - (length / 2) << " " << _lat + (length / 2) << " " << lon - (length / 2)
    // << " " << _lon + (length / 2) << std::endl;
    std::string queryString(
        " SELECT geo.Latitude AS Latitude,"
        "        geo.Longitude AS Longitude,"
        "        geo.population AS Population,"
        "        ci.country AS Country"
        " FROM geoname as geo, countryinfo as ci"
        " WHERE geo.Latitude >= ?1"
        "   AND geo.Latitude <= ?2"
        "   AND geo.Longitude >= ?3"
        "   AND geo.Longitude <= ?4"
        "   AND geo.country_code = ci.iso"
        " ORDER BY Population DESC");

    sqlite3_prepare_v2(_ppDB, queryString.c_str(), queryString.length(), &_stmt, NULL);
    sqlite3_bind_double(_stmt, 1, _lat - (length / 2));
    sqlite3_bind_double(_stmt, 2, _lat + (length / 2));
    sqlite3_bind_double(_stmt, 3, _lon - (length / 2));
    sqlite3_bind_double(_stmt, 4, _lon + (length / 2));
    retval = sqlite3_step(_stmt);

    if (retval == SQLITE_ROW) {
        _rowAvailable = true;
    } else {
        _rowAvailable = false;
    }
}

bool SQLiteAreaPopulationReader::hasNext() {
    return _rowAvailable;
}

PopulatedPosition SQLiteAreaPopulationReader::getNext() {
    assert(_rowAvailable);

    double latitude(sqlite3_column_double(_stmt, 0));
    double longitude(sqlite3_column_double(_stmt, 1));
    double population(sqlite3_column_double(_stmt, 2));
    std::string country(reinterpret_cast<const char*>(sqlite3_column_text(_stmt, 3)));

    PopulatedPosition pp(population, latitude, longitude, country);

    if (sqlite3_step(_stmt) == SQLITE_ROW)
        _rowAvailable = true;
    else {
        _rowAvailable = false;
    }
    return pp;
}

SQLiteAreaPopulationReader::~SQLiteAreaPopulationReader() {
    sqlite3_finalize(_stmt);
    sqlite3_close(_ppDB);
}