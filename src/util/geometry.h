#ifndef _UTIL_GEOMETRY_H_INCLUDED
#define _UTIL_GEOMETRY_H_INCLUDED

#include <vector>
#include <tuple>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>


namespace wtflights {
namespace util {

    typedef boost::geometry::model::point<float, 3, boost::geometry::cs::cartesian> Point3D;
    typedef boost::geometry::model::polygon<Point3D> Poly3D;
    //    typedef boost::geometry::<Point3D> MultiPoint3D;
    typedef boost::geometry::model::multi_point<Point3D> MultiPoint3D;
    typedef boost::geometry::model::multi_polygon<Poly3D> MultiPolygon3D;
    
    typedef std::tuple<int, int, int> PolygonIndex3;
    
    class Entity3D {
    private:
        MultiPoint3D vertices_;
        MultiPolygon3D faces_;
    public:
        explicit Entity3D(const MultiPoint3D &vertices, const std::vector<PolygonIndex3> & polygonIndexes) : vertices_(vertices) {
            
        }
    };


	// Add icosahedron points to vertices
	void AddIcosahedronPoints(MultiPoint3D *vertices);

    std::ostream& operator<<(std::ostream& os, const wtflights::util::Point3D& p);
    std::ostream& operator<<(std::ostream& os, const wtflights::util::MultiPoint3D& p);
}
}



#endif
