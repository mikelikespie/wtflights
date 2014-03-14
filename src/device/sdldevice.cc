#include <device/sdldevice.h>

#include <OpenGL/gl.h>

#define USER_RENDER_CODE 1

namespace wtflights {
namespace device {
	
	Uint32 SDLDevice::RenderCallback(Uint32 interval, void *param) {
		SDLDevice *device = static_cast<SDLDevice *>(param);
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = USER_RENDER_CODE;
        SDL_PushEvent(&event);
		return interval;
	}

    SDLDevice::SDLDevice() : renderInterval_(1000/60) {
    }
    
    void SDLDevice::Init() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        window_ = SDL_CreateWindow("Fun times",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

		SDL_AddTimer(renderInterval_, SDLDevice::RenderCallback, this);
        ctx_ = SDL_GL_CreateContext(window_);
    }
    
    void SDLDevice::Run() {
		SDL_Event event;
        while(SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
            if (event.type == SDL_USEREVENT && event.user.code == USER_RENDER_CODE) {
                Timestamp ts = SDL_GetTicks();
                renderTask_(ts, renderInterval_);
            }
        }
    }
    
    void SDLDevice::FinishFrame() {
        SDL_GL_SwapWindow(window_);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // Tells us to end the app
    void SDLDevice::Finish() {
        SDL_GL_DeleteContext(ctx_);
		SDL_DestroyWindow(window_);
    }
    
    void SDLDevice::SetPixel(PixelIndex pixel, ColorRGB color) {
        glClearColor(color.r, color.g, color.b, 1.0);
    }
    
    // Special task that is called when we are ready for a new frame.  This is driver dependent
    // This will copy the task
    void SDLDevice::SetRenderTask(const Task &t) {
        renderTask_ = t;
    }
    
    // This will copy the task
    void SDLDevice::EnqueueTask(const Task &t, TaskConfig interval) {
        
    }

}
}
//void wtflights::device::SDLDevice
//void wtflligInit();
//
//// Starts the main runloop inifinitely
//void Run();
//
//// Tells us to end the app
//void Finish();
//
//void SetPixel(PixelIndex pixel, ColorRGB color);
//void FinishFrame();
//
//
//// Special task that is called when we are ready for a new frame.  This is driver dependent
//// This will copy the task
//void SetRenderTask(const Task &t);
//
//// This will copy the task
//void EnqueueTask(const Task &t, TaskConfig interval);
