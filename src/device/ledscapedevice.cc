

#include <device/ledscapedevice.h>

extern "C" {
#include <ledscape.h>
}

namespace wtflights {
namespace device {
    LEDScapeDevice::LEDScapeDevice(uint16_t numPixels) : numPixels_(numPixels) {

    }
    
    void LEDScapeDevice::Init() {
        ledscape_ = ledscape_init(numPixels_);
        
        lastRender_ = std::chrono::system_clock::now();
    }
    
    // Starts the main runloop inifinitely
    void LEDScapeDevice::Run() {
        while (!finished_) {
            
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            auto delta = now - lastRender_;
            lastRender_ = now;
            renderTask_(now.time_since_epoch() / std::chrono::milliseconds(), delta / std::chrono::milliseconds());
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
        for (int i = 0; i < LEDSCAPE_NUM_STRIPS; i++) {
            ledscape_set_color(currentFrameHandle_, i, pixel, color.r * 255, color.g * 255, color.b * 255);
        }
    }
    
    void LEDScapeDevice::FinishFrame() {
		const uint32_t response = ledscape_wait(ledscape_);
		ledscape_draw(ledscape_, frameNumber_ % 2);
        frameNumber_++;
    }
    
    // Special task that is called when we are ready for a new frame.  This is driver dependent
    // This will copy the task
    void LEDScapeDevice::SetRenderTask(const Task &t) {
            renderTask_ = t;
    }
}
}
