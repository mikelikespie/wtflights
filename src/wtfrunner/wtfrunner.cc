extern "C" {
#include <assert.h>
}
#include <map>
#include <iostream>
#include <cmath>

#include <device/ledscapedevice.h>

using namespace wtflights::device;

class StripMap {
private:
    
    // Represents the physical strip number and whether or not it is reversed;
    // This is stored by logical order
    std::vector<std::pair<int, bool>> mapping_;
    int pixelsPerEdge_;
    
public:
    // Mapping is in the physical orer.  Where our
    explicit StripMap(int pixelsPerEdge,
                      const std::vector<std::pair<int, bool>> &physicalMapping) : pixelsPerEdge_(pixelsPerEdge) {
        
        int maxEdge = 0;
        for (auto const &m : physicalMapping) {
            maxEdge = std::max(m.first, maxEdge);
        }
        
        mapping_ = physicalMapping;
    }
    
    PixelIndex toPhysical(PixelIndex logicalIndex) {
        if (logicalIndex >=  pixelsPerEdge_ * mapping_.size()) {
            return -1;
        }
        
        int edge = logicalIndex / pixelsPerEdge_;
        
        int posOnEdge = logicalIndex % pixelsPerEdge_;
        
        auto const &m = mapping_[edge];
        
        if (m.first == -1) {
            return -1;
        }
        
        // If we're reverse, reverse it
        if (m.second) {
            posOnEdge = pixelsPerEdge_ - (posOnEdge + 1);
        }
        
        return m.first * pixelsPerEdge_ + posOnEdge;
    }
};

int main(int argc, char** argv) {
    int edgeCount = 0x0C;
    int pixelsPerEdge = 16;
    int numLeds = edgeCount * pixelsPerEdge;
    
	LEDScapeDevice device(numLeds);
    
    std::vector<std::pair<int, bool>> physicalMapping {
        {0x01,false},
        {0x06,true},
        {0x07,false},
        {0x09,true},
        {0x0A,false},
        
        {0x02,true},
        {0x05,false},
        {0x08,true},
        {0x0B,false},
        {0x00,true},
        
        {-1,false},
        {0x03,false},
        {-1,false},
        {-1,false},
        {-1,false},
        
        {-1,false},
        {-1,false},
        {0x04,false},
        {-1,false},
        {-1,false},
    };
    
    StripMap stripMap(pixelsPerEdge,
                      physicalMapping);
    
    std::cout << stripMap.toPhysical(0) << std::endl;
    
	device.Init();
    
    

    device.SetRenderTask([&](Timestamp time, Interval elapsed) {
        
        for (int i = 0; i < numLeds; i++) {
            device.SetPixel(i, ColorRGB {0, 0, 0});
        }
        
        double timePast = (time.time_since_epoch() / std::chrono::microseconds(1)) / 1000000.0;
     
        double period = 1.0;
        
        double mult = 0.125;
        double phase = fmod(timePast * mult, period);
        float phaseDegrees = phase * 360.0;
        
        for (int edge = 0; edge < edgeCount; edge++) {
            for (int p = 0; p < pixelsPerEdge; p++) {
                
                if (edge < 5) {
                    int pos = edge * pixelsPerEdge + p;
                    int mappedPos = stripMap.toPhysical(pos);
                    device.SetPixel(mappedPos, colorFromHSV(phaseDegrees, 1.0, 0.125));
                } else {
                    
                }
            }
        }
        
        device.FinishFrame();
    });
    
    device.Run();
}

