#include <device/ledscapedevice.h>

using namespace wtflights::util;
using namespace wtflights::device;

int main(int argc, char** argv) {
    int edgeCount = 16;
    int pixelsPerEdge = 16;
    int numLeds = edgeCount * pixelsPerEdge;
    
	LEDScapeDevice device(numLeds);
    
	device.Init();
    
    device.SetRenderTask([&](Timestamp time, Interval elapsed) {
        float phase = time % 500000 / 500000.0;
        
        for (int i = 0; i < numLeds; i++) {
            int lightphase = (int(2 * 360.0f * (float(i) / numLeds)) + int(phase * 30 * 360)) % 360;
            int lightphase2 = (int(360.0f * (float(i) / numLeds)) + int(phase * 30 * 360)) % 360;
            device.SetPixel(i, (lightphase2 + i) % 360 < 20 ? ColorRGB {0,0,0} : colorFromHSV(lightphase , 1.0, 1.0));

        }
        
        device.FinishFrame();
    });
    
    device.Run();
}

