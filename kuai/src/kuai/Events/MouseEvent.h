#pragma once

#include "Event.h"

namespace kuai {
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : xpos(x), ypos(y) {}

		inline float getX() const { return xpos; }
		inline float getY() const { return ypos; }

		std::string toString() const override
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

		inline float getXOffset() const { return xoff; }
		inline float getYOffset() const { return yoff; }

		std::string toString() const override
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
		inline int getButton() const { return button; }

		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);
	protected:
		MouseBtnEvent(int button) : button(button) {}

		int button;
	};


	class MouseBtnPressEvent : public MouseBtnEvent
	{
	public:
		MouseBtnPressEvent(int button) : MouseBtnEvent(button) {}

		std::string toString() const override
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

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseBtnReleaseEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseBtnRelease);
	};
}