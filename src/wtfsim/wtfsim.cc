#include <vector>
#include <iostream>
#include <numeric>
#include <OpenGL/gl.h>

#include "util/geometry.h"


#include <SDL2/sdl.h>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometry.hpp>
#include <dispatch/dispatch.h>
#include <CoreFoundation/CoreFoundation.h>

#include <device/sdldevice.h>

namespace bpl = boost::geometry;


using namespace wtflights::util;
using namespace wtflights::device;

int main(int argc, char** argv) {
	SDLDevice device;
	device.Init();

	MultiPoint3D points;
	MultiPoint3D points2;

	AddIcosahedronPoints(&points);
    
    std::vector<PolygonIndex3> faces {
		{0,1,2},
		{0,2,3},
		{0,3,4},
		{0,4,5},
		{0,5,1},
		{11,6,7},
		{11,7,8},
		{11,8,9},
		{11,9,10},
		{11,10,6},
		{1,2,6},
		{2,3,7},
		{3,4,8},
		{4,5,9},
		{5,1,10},
		{6,7,2},
		{7,8,3},
		{8,9,4},
		{9,10,5},
		{10,6,1},
	};
    
    
    
    auto interpolate = [&points2](const Point3D a, const Point3D b, int positions){
        for (int i = 1; i < positions; i++) {
            Point3D diff(b);
            bpl::subtract_point(diff, a);
            bpl::multiply_value(diff, (float(i) / positions));
            bpl::add_point(diff, a);
            points2.emplace_back(diff);
        }
    };
    
    std::set<std::pair<int, int>> seenEdges;
    
    const int interpolationSteps = 18;
    
    auto doInterpolate = [&seenEdges, &points, &interpolate](int indexA, int indexB) {
        int minIdx = std::min(indexA, indexB);
        int maxIdx = std::max(indexA, indexB);
        
        if (seenEdges.emplace(minIdx, maxIdx).second) {
            interpolate(points[minIdx], points[maxIdx], interpolationSteps);
        }
    };
    
    for (const auto &f : faces) {
        int a = std::get<0>(f);
        int b = std::get<1>(f);
        int c = std::get<2>(f);
        
        doInterpolate(a, b);
        doInterpolate(b, c);
        doInterpolate(a, c);
    }

	device.SetPixelPoints(points2);
    
    device.SetRenderTask([&device, &points2](Timestamp time, Interval elapsed) {
        float phase = time % 500000 / 500000.0;
        
        device.SetRotation(phase  * 360 , 0, 1, 0);

        int i = 0;
        for (const auto &p : points2) {
            int lightphase = (int(2 * 360.0f * (float(i) / points2.size())) + int(phase * 30 * 360)) % 360;
            int lightphase2 = (int(360.0f * (float(i) / points2.size())) + int(phase * 30 * 360)) % 360;
            device.SetPixel(i, (lightphase2 + i) % 360 < 20 ? ColorRGB {0,0,0} : colorFromHSV(lightphase , 1.0, 1.0));
            i++;
        }
        
        device.FinishFrame();
    });
    
    device.Run();
}

