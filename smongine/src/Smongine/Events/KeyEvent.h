#pragma once

#include "Event.h"

namespace Smong {

	class SMONGINE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return keycode; }
		
		EVENT_CLASS_CATEGORY(KeyboardEventCategory | InputEventCategory)
	protected:
		KeyEvent(int keycode) : keycode(keycode) {}

		int keycode;
	};

	class SMONGINE_API KeyPressEvent : public KeyEvent
	{
	private:
		int repeatCount;
	public:
		KeyPressEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << keycode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyPress)
	};

	class SMONGINE_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyRelease)
	};
}