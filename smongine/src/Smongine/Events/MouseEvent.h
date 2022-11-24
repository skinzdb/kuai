#pragma once

#include "Event.h"

namespace Smong {
	class SMONGINE_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : xpos(x), ypos(y) {}

		inline float GetX() const { return xpos; }
		inline float GetY() const { return ypos; }

		std::string ToString() const override
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

	class SMONGINE_API MouseScrollEvent : public Event
	{
	private:
		float xoff, yoff;
	public:
		MouseScrollEvent(float xoff, float yoff) : xoff(xoff), yoff(yoff) {}

		inline float GetXOffset() const { return xoff; }
		inline float GetYOffset() const { return yoff; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << xoff << ", " << yoff;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseScroll);
		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);
	};


	class SMONGINE_API MouseBtnEvent : public Event
	{
	public:
		inline int GetButton() const { return button; }

		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);
	protected:
		MouseBtnEvent(int button) : button(button) {}

		int button;
	};


	class SMONGINE_API MouseBtnPressEvent : public MouseBtnEvent
	{
	public:
		MouseBtnPressEvent(int button) : MouseBtnEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseBtnPressEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseBtnPress);
	};

	class SMONGINE_API MouseBtnReleaseEvent : public MouseBtnEvent
	{
	public:
		MouseBtnReleaseEvent(int button) : MouseBtnEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseBtnReleaseEvent: " << button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseBtnRelease);
	};
}