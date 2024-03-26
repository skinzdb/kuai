#pragma once

#include "Event.h"

namespace kuai {

	class KeyEvent : public Event
	{
	public:
		inline int get_keycode() const { return keycode; }
		
		EVENT_CLASS_CATEGORY(KeyboardEventCategory | InputEventCategory)
	protected:
		KeyEvent(int keycode) : keycode(keycode) {}

		int keycode;
	};

	class KeyPressEvent : public KeyEvent
	{
	private:
		int repeatCount;
	public:
		KeyPressEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeatCount(repeatCount) {}

		inline int get_repeat_count() const { return repeatCount; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << keycode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyPress)
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyRelease)
	};
}