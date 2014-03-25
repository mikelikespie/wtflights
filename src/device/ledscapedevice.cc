
#include <device/ledscapedevice.h>
#include <iostream>

extern "C" {
#include <ledscape.h>
}

namespace wtflights {
namespace device {
    const int LEDScapeDevice::frameLogMultiple_;
    
    LEDScapeDevice::LEDScapeDevice(uint16_t numPixels) : numPixels_(numPixels) {
        lastLogFrame_ = std::chrono::system_clock::now();
    }
    
    void LEDScapeDevice::Init() {
        ledscape_ = ledscape_init(numPixels_);
        lastRender_ = std::chrono::system_clock::now();
        currentFrameHandle_ = ledscape_frame(ledscape_, frameNumber_ % 2);
    }
    
    // Starts the main runloop inifinitely
    void LEDScapeDevice::Run() {
        while (!finished_) {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::chrono::system_clock::duration delta = now - lastRender_;
            lastRender_ = now;
            renderTask_(now, delta);
            currentFrameHandle_ = ledscape_frame(ledscape_, frameNumber_ % 2);
        }
    }
    
    // Tells us to end the app
    void LEDScapeDevice::Finish() {
        ledscape_close(ledscape_);
        finished_ = true;
    }
    
    void LEDScapeDevice::SetPixel(PixelIndex pixel, ColorRGB color) {
        // Set all strips for now.  This is suboptimal
        ColorRGB8 c = color.rgb8();
        for (int i = 0; i < LEDSCAPE_NUM_STRIPS; i++) {
            ledscape_set_color(currentFrameHandle_, i, pixel, c.r, c.g, c.b);
        }
    }
    
    void LEDScapeDevice::FinishFrame() {
		const uint32_t response = ledscape_wait(ledscape_);
		ledscape_draw(ledscape_, frameNumber_ % 2);
        frameNumber_++;
        
        if (frameNumber_ % frameLogMultiple_ == 0 ) {
            auto now = std::chrono::system_clock::now();
            float fps = 1.0/(((now - lastLogFrame_) / frameLogMultiple_) / std::chrono::nanoseconds(1) / 1000000000.0);
            lastLogFrame_ = now;
            std::cout << "fps: " << fps << std::endl;
        }
    }
    
    // Special task that is called when we are ready for a new frame.  This is driver dependent
    // This will copy the task
    void LEDScapeDevice::SetRenderTask(const Task &t) {
        renderTask_ = t;
    }
}
}
