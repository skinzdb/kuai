#pragma once

#include "kpch.h"

#include "rekuai/Core/Core.h"

namespace kuai {

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

#define EVENT_CLASS_TYPE(type) static EventType get_static_type() { return type; }\
								virtual EventType get_event_type() const override { return get_static_type(); }\
								virtual const char* get_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override { return category; }

	/**
	* Base class for events.
	*/
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType get_event_type() const = 0;
		virtual const char* get_name() const = 0;
		virtual int get_category_flags() const = 0;
		virtual std::string to_string() const { return get_name(); }

		inline bool is_in_category(EventCategory category) { return get_category_flags() & category; }

		bool handled = false;
	};

	class EventDispatcher 
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : event(event) {}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (event.get_event_type() == T::get_static_type()) // Check if passed event type matches event type of template argument
			{ 
				event.handled |= func(static_cast<T&>(event)); // Call the function with that event as its argument
				return true; // Indicate the event has been handled
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.to_string();
	}
}


