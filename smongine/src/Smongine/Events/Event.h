#pragma once

#include "smpch.h"

#include "Smongine/Core.h"

namespace Smong {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		KeyPress, KeyRelease,
		MouseBtnPress, MouseBtnRelease, MouseMove, MouseScroll
	};

	enum EventCategory
	{
		NoneEventCategory = 0,
		AppEventCategory = 1,
		InputEventCategory = 2,
		KeyboardEventCategory = 4,
		MouseEventCategory = 8,
		MouseBtnEventCategory = 16
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class SMONGINE_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int  GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

	protected:
		bool handled = false;
	};

	class EventDispatcher 
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{ 
				event.handled = func(*(T*)&event);
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}


