#ifndef DEVICE_DEVICE_H_INCLUDED
#define DEVICE_DEVICE_H_INCLUDED

#include <limits>
#include <cstdint>
#include <functional>

namespace wtflights {
namespace device {
	// millis since epoch
	typedef std::int64_t Timestamp;

	// millis
	typedef std::int64_t Interval;

	typedef std::int32_t PixelIndex;

	inline Interval InvalidInterval() {
		return std::numeric_limits<Interval>::min();
	}

	inline Timestamp InvalidTime() {
		return std::numeric_limits<Timestamp>::min();
	}

	typedef std::function<void(Timestamp time, Interval elapsed)> Task;

	class ColorRGB {
		public:
		float r;
		float g;
		float b;
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
