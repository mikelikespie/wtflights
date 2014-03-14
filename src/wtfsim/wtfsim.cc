#include <vector>
#include <iostream>
#include <numeric>
#include <OpenGL/gl.h>

#include "util/geometry.h"

#include <device/sdldevice.h>

#include <SDL2/sdl.h>
#include <boost/geometry/geometries/geometries.hpp>

#include <dispatch/dispatch.h>
#include <CoreFoundation/CoreFoundation.h>

namespace bpl = boost::geometry;

using namespace wtflights::util;
using namespace wtflights::device;

int main(int argc, char** argv) {
	SDLDevice device;
	device.Init();
    
    device.SetRenderTask([&device](Timestamp time, Interval elapsed) {
        float phase = time % 1000 / 1000.0;
        device.SetPixel(0, ColorRGB {phase, phase, phase});
        device.FinishFrame();
    });
    
    device.Run();
}

