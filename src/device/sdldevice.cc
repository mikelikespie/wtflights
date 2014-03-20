#include <device/sdldevice.h>

#include <chrono>
#include <iostream>
#include <algorithm>

#include <OpenGL/gl.h>
#include <assert.h>
#include <OpenGL/glu.h>

#include <CoreText/CoreText.h>
#include <CoreGraphics/CGBitmapContext.h>

#define USER_RENDER_CODE 1

namespace wtflights {
namespace device {
    
    void DrawText(const std::string text, const util::Point3D &point) {
        CFStringRef str = CFStringCreateWithCStringNoCopy(nullptr, text.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
        CFAttributedStringRef stringRef = CFAttributedStringCreate(nullptr, str, nullptr);
        CTLineRef line = CTLineCreateWithAttributedString(stringRef);

        CGRect bounds = CTLineGetImageBounds(line, nullptr);
        // TODO: make this not recreate stuff
        size_t width = std::ceil(bounds.size.width);
        size_t height = std::ceil(bounds.size.height);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

        CGBitmapInfo bitmapInfo;

        CGContextRef bitmapContext = CGBitmapContextCreate(nullptr, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
        
        void *const data = CGBitmapContextGetData(bitmapContext);
        CTLineDraw(line, bitmapContext);
        
        CFRelease(line);
        CFRelease(stringRef);
        CFRelease(bitmapContext);
        CFRelease(colorSpace);
        CFRelease(str);
    }
    
	Uint32 SDLDevice::RenderCallback(Uint32 interval, void *param) {
		SDLDevice *device = static_cast<SDLDevice *>(param);
        if (!device->rendered_) {
            return;
        }
        device->rendered_ = false;
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
				800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);

		SDL_AddTimer(renderInterval_, SDLDevice::RenderCallback, this);
        ctx_ = SDL_GL_CreateContext(window_);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( 75, 4.0f / 3.0f, .00001, 100);
        glTranslated(0, 0, -.8);
        glRotated(10, 1, 0, 0);
        glMatrixMode(GL_MODELVIEW);

    }
    
    void SDLDevice::Run() {
		SDL_Event event;
        while(SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
            if (event.type == SDL_USEREVENT && event.user.code == USER_RENDER_CODE) {
                Timestamp ts = std::chrono::system_clock::now().time_since_epoch() /
                std::chrono::milliseconds(1);
                renderTask_(ts, renderInterval_);
                rendered_ = true;
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    hideOthers_ = std::max(-1, hideOthers_ - 1);
                } else if (event.key.keysym.sym == SDLK_UP) {
                    hideOthers_ = std::min(static_cast<PixelIndex>(colors_.size() - 1), hideOthers_ + 1);
                }
                
                if (hideOthers_ < 0) {
                    std::cout << "all visible" << std::endl;
                } else {
                    std::cout << hideOthers_<< " visible" << std::endl;
                }
            }
        }
    }

	void SDLDevice::SetRotation(float angle, float x, float y, float z) {
		rotation_ = {angle, x, y, z};
	}
    
    void SDLDevice::FinishFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        glPushMatrix();
		glRotatef(rotation_.angle, rotation_.x, rotation_.y, rotation_.z);

        glPointSize(5.0);
		glBegin( GL_POINTS );

		for (int i = 0; i < colors_.size(); ++i) {
			const ColorRGB &color = colors_[i];
			const util::Point3D &point = points_[i];
            
            bool visible = hideOthers_ < 0 || hideOthers_ == i;
            
            glColor4f(color.r, color.g, color.b, visible ? 1.0 : 0.3);
            glVertex3f(point.get<0>()/2.0, point.get<1>()/2.0, point.get<2>()/2.0);
		}
        
		glEnd();
        
		glPopMatrix();
		glFinish();
        SDL_GL_SwapWindow(window_);
    }

    // Tells us to end the app
    void SDLDevice::Finish() {
        SDL_GL_DeleteContext(ctx_);
		SDL_DestroyWindow(window_);
    }
    
    void SDLDevice::SetPixel(PixelIndex pixel, ColorRGB color) {
		assert(pixel < colors_.size());
		colors_[pixel] = color;
    }

	void SDLDevice::SetPixelPoints(const util::MultiPoint3D &points) {
		points_ = points;
		colors_.resize(points_.size());
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