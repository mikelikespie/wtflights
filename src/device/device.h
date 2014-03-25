#ifndef DEVICE_DEVICE_H_INCLUDED
#define DEVICE_DEVICE_H_INCLUDED

#include <limits>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <chrono>

namespace wtflights {
namespace device {
	// millis since epoch
	typedef std::chrono::system_clock::time_point Timestamp;

	// millis
	typedef std::chrono::system_clock::duration Interval;

	typedef std::int32_t PixelIndex;

	inline Interval InvalidInterval() {
		return std::numeric_limits<Interval>::min();
	}

	inline Timestamp InvalidTime() {
		return std::numeric_limits<Timestamp>::min();
	}

	typedef std::function<void(Timestamp time, Interval elapsed)> Task;


	class ColorRGB8 {
    public:
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
    
	class ColorRGB {
		public:
		float r;
		float g;
		float b;
        
        ColorRGB8 rgb8() const {
            return ColorRGB8 {
                static_cast<uint8_t>(std::min<int32_t>(r * 255, 255)),
                static_cast<uint8_t>(std::min<int32_t>(g * 255, 255)),
                static_cast<uint8_t>(std::min<int32_t>(b * 255, 255))
            };
        }
	};
    
    
    
    ColorRGB colorFromHSV(float h, float s, float v);

	class TaskConfig {
		private:
			Interval repeatInterval_;
			Timestamp startTime_;

			TaskConfig(Timestamp startTime, Interval repeatInterval) {
				repeatInterval_ = repeatInterval;
				startTime_ = startTime;
			}

		public:
			static TaskConfig RepeatedTaskConfig(Timestamp startTime, Interval repeatInterval);
			static TaskConfig OneTimeTaskConfig(Timestamp startTime);

			bool repeats() const {
				return repeatInterval_ != InvalidInterval();
			}

			Interval repeatInterval() const {
				return repeatInterval_;
			}

			Timestamp startTime() const {
				return startTime_;
			}
	};

	// This is a virtual class that a device must implement
	class Device {
		public:
		virtual void Init() = 0;

		// Starts the main runloop inifinitely
		virtual void Run() = 0;

		// Tells us to end the app
		virtual void Finish() = 0;

		virtual void SetPixel(PixelIndex pixel, ColorRGB color) = 0;
		virtual void FinishFrame() = 0;

		
		// Special task that is called when we are ready for a new frame.  This is driver dependent
		// This will copy the task
		virtual void SetRenderTask(const Task &t) = 0;
	};
}
}

#endif
