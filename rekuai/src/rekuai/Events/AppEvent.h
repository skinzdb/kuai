#pragma once

#include "Event.h"

namespace kuai {
	class WindowResizeEvent : public Event
	{
	private:
		int width, height;
	public:
		WindowResizeEvent(int width, int height) : width(width), height(height) {}

		inline int get_width() const { return width; }
		inline int get_height() const { return height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(AppEventCategory)
		EVENT_CLASS_TYPE(EventType::WindowResize)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		std::string to_string() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_CATEGORY(AppEventCategory)
		EVENT_CLASS_TYPE(EventType::WindowClose)
	};
}