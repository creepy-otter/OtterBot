#include <event/EventDispatcher.h>

#include <algorithm>

namespace otterbot {

EventDispatcher::EventHandlerID EventDispatcher::register_handler(
    EventType event_type, EventHandler handler) {
  EventHandlerID handler_id = next_handler_id++;
  event_handlers[event_type].emplace_back(handler_id, handler);
  return handler_id;
}

void EventDispatcher::unregister_handler(EventType event_type,
                                         EventHandlerID handler_id) {
  auto it = event_handlers.find(event_type);
  if (it != event_handlers.end()) {
    auto& handlers = it->second;
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                                  [handler_id](const auto& pair) {
                                    return pair.first == handler_id;
                                  }),
                   handlers.end());
  }
}

void EventDispatcher::dispatch(const Event& event) {
  EventType event_type = event.getType();
  auto it = event_handlers.find(event_type);
  if (it != event_handlers.end()) {
    for (const auto& pair : it->second) {
      pair.second(event);
    }
  }
}

}  // namespace otterbot