#pragma once

#include <functional>
#include <unordered_map>

#include "Event.h"

namespace otterbot
{


class EventDispatcher {
public:
    using EventHandlerID = int;
    using EventHandler = std::function<void(const Event&)>;

    // Register an event handler and return its unique identifier
    EventHandlerID register_handler(EventType event_type, EventHandler handler);

    // Unregister an event handler using its unique identifier
    void unregister_handler(EventType event_type, EventHandlerID handler_id);

    // Dispatch an event to all registered handlers for the event's type
    void dispatch(const Event& event);

private:
    int next_handler_id = 0;
    std::unordered_map<EventType, std::vector<std::pair<EventHandlerID, EventHandler>>> event_handlers;
};

} // namespace otterbot


