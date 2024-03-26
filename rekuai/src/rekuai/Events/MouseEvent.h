#pragma once

#include "Event.h"

namespace kuai {
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : xpos(x), ypos(y) {}

		inline float get_x() const { return xpos; }
		inline float get_y() const { return ypos; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << xpos << ", " << ypos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseMove);
		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);

	private:
		float xpos, ypos;
	};

	class MouseScrollEvent : public Event
	{
	private:
		float xoff, yoff;
	public:
		MouseScrollEvent(float xoff, float yoff) : xoff(xoff), yoff(yoff) {}

		inline float get_xoff() const { return xoff; }
		inline float get_yoff() const { return yoff; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << xoff << ", " << yoff;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseScroll);
		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);
	};


	class MouseBtnEvent : public Event
	{
	public:
		inline int get_btn() const { return button; }

		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);
	protected:
		MouseBtnEvent(int button) : button(button) {}

		int button;
	};


	class MouseBtnPressEvent : public MouseBtnEvent
	{
	public:
		MouseBtnPressEvent(int button) : MouseBtnEvent(button) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseBtnPressEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseBtnPress);
	};

	class MouseBtnReleaseEvent : public MouseBtnEvent
	{
	public:
		MouseBtnReleaseEvent(int button) : MouseBtnEvent(button) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseBtnReleaseEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseBtnRelease);
	};
}