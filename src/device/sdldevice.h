#ifndef DEVICE_SDLDEVICE_H_INCLUDED
#define DEVICE_SDLDEVICE_H_INCLUDED

#include <device/device.h>
#include <SDL2/SDL.h>

namespace wtflights {
namespace device {
	// usecs since epoch
	class SDLDevice : public Device {
		private:
			SDL_Window *window_;
			SDL_GLContext ctx_;
			Task renderTask_;
			Uint32 renderInterval_;

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

		
		// Special task that is called when we are ready for a new frame.  This is driver dependent
		// This will copy the task
		void SetRenderTask(const Task &t);

		// This will copy the task
		void EnqueueTask(const Task &t, TaskConfig interval);
	};
}
}

#endif
