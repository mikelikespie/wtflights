#ifndef DEVICE_LEDSCAPEEVICE_H_INCLUDED
#define DEVICE_LEDSCAPEEVICE_H_INCLUDED

#include <memory>
#include <vector>

#include <util/geometry.h>
#include <device/device.h>
#include <chrono>

struct ledscape;
struct ledscape_frame;

namespace wtflights {
    namespace device {
        class LEDScapeDevice : public Device {
		private:
            struct ledscape* ledscape_;
            struct ledscape_frame* currentFrameHandle_;
            
            uint64_t frameNumber_;
            Task renderTask_;
            uint16_t numPixels_;
            bool finished_ = false;
            
            std::chrono::system_clock::time_point lastRender_;

		public:
            explicit LEDScapeDevice(uint16_t numPixels);
            
            void Init();
            
            // Starts the main runloop inifinitely
            void Run();
            
            // Tells us to end the app
            void Finish();
            
            void SetPixel(PixelIndex pixel, ColorRGB color);
            void FinishFrame();
            
            // Special task that is called when we are ready for a new frame.  This is driver dependent
            // This will copy the task
            void SetRenderTask(const Task &t);            
        };
    }
}

#endif
