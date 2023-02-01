#pragma once

#include "Event.h"

namespace Smong {
	class WindowResizeEvent : public Event
	{
	private:
		int width, height;
	public:
		WindowResizeEvent(int width, int height) : width(width), height(height) {}

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		std::string ToString() const override
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

		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_CATEGORY(AppEventCategory)
		EVENT_CLASS_TYPE(EventType::WindowClose)
	};
}