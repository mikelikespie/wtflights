#include "geometry.h"

#include <iostream>
#include <cmath>
#include <boost/math/constants/constants.hpp>

namespace bmc = boost::math::constants;

// Algorithm from http://www.csee.umbc.edu/~squire/reference/polyhedra.shtml
void wtflights::util::AddIcosahedronPoints(MultiPoint3D *vertices) { 
	const float phi = bmc::phi<float>();
	vertices->insert(vertices->begin(), {
			Point3D(0, -1, -phi),
			Point3D(0, -1,  phi),
			Point3D(0,  1, -phi),
			Point3D(0,  1,  phi),
			Point3D(-1, -phi, 0),
			Point3D(-1,  phi, 0),
			Point3D(1,  -phi, 0),
			Point3D(1,   phi, 0),
			Point3D(-phi, 0, -1),
			Point3D(-phi, 0,  1),
			Point3D(phi,  0, -1),
			Point3D(phi,  0,  1),
    });
}

std::ostream& wtflights::util::operator<<(std::ostream& os, const wtflights::util::Point3D& p) {
    os << '(' << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << ')';
    return os;
}

std::ostream& wtflights::util::operator<<(std::ostream& os, const wtflights::util::MultiPoint3D& mp) {
    os << '[' << std::endl;
    for (const auto &p : mp) {
        os << "  " << p << std::endl;
    }
    os << ']';
	return os;
}


