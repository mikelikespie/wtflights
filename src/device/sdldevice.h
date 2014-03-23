#ifndef DEVICE_SDLDEVICE_H_INCLUDED
#define DEVICE_SDLDEVICE_H_INCLUDED

#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include <util/geometry.h>
#include <device/device.h>

namespace wtflights {
namespace device {
//    class Sprite {
//    private:
//        std::
//    };
	// usecs since epoch
	class SDLDevice : public Device {
		private:
        SDL_Window *window_;
        SDL_GLContext ctx_;
        Task renderTask_;
        Uint32 renderInterval_;
        PixelIndex hideOthers_ = -1;
        bool rendered_ = true;

			std::vector<ColorRGB> colors_;
			util::MultiPoint3D points_;

			struct {
				float angle;
				float x;
				float y;
				float z;
			} rotation_;

			static Uint32 RenderCallback(Uint32 interval, void *param);

		public:
        explicit SDLDevice();
        
		void Init();

		// Starts the main runloop inifinitely
		void Run();

		// Tells us to end the app
		void Finish();

		void SetPixel(PixelIndex pixel, ColorRGB color);
		void FinishFrame();

		void SetRotation(float angle, float x, float y, float z);

		
		// Special task that is called when we are ready for a new frame.  This is driver dependent
		// This will copy the task
		void SetRenderTask(const Task &t);

		void SetPixelPoints(const util::MultiPoint3D &points);
	};
}
}

#endif
