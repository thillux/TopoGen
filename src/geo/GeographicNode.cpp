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

#include "GeographicNode.hpp"
#include "topo/base_topo/BaseTopology.hpp"

GeographicNode::GeographicNode() : _id(-1) {
}

GeographicNode::GeographicNode(int id, double lat, double lon) : GeographicPosition(lat, lon), _id(id) {
}

GeographicNode::GeographicNode(const GeographicNode& other)
    : GeographicPosition(other._latitude, other._longitude), _id(other._id) {
}

GeographicNode& GeographicNode::operator=(const GeographicNode& other) {
    this->_id = other._id;
    this->_latitude = other._latitude;
    this->_longitude = other._longitude;
    this->_valid = other._valid;
    return *this;
}

GeographicPositionTuple GeographicNode::coord() {
    return std::make_pair(_latitude, _longitude);
}

int GeographicNode::id() {
    return _id;
}

void GeographicNode::setId(int i) {
    _id = i;
}

// for usage as map key
bool GeographicNode::operator<(const GeographicNode& other) const {
    return this->_id < other._id;
}
