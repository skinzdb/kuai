#pragma once

#include "Event.h"

namespace Smong {
	class SMONGINE_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : mouseX(x), mouseY(y) {}

		inline float GetX() const { return mouseX; }
		inline float GetY() const { return mouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << mouseX << ", " << mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::MouseMove);
		EVENT_CLASS_CATEGORY(MouseEventCategory | InputEventCategory);

	private:
		float mouseX, mouseY;
	};

	class SMONGINE_API MouseScrollEvent : public Event
	{
	private:
		float xOffset, yOffset;
	public:
		MouseScrollEvent(float xOff, float yOff) : xOffset(xOff), yOffset(yOff) {}

		inline float GetXOffset() const { return xOffset; }
		inline float GetYOffset() const { return yOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << xOffset << ", " << yOffset;
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