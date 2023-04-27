#pragma once

#include "Event.h"

namespace otterbot {

class OrderFillEvent : public Event {
public:
    OrderFillEvent(int order_id, int quantity)
        : Event(EventType::OrderFill), order_id(order_id), quantity(quantity) {}
    int get_order_id() const { return order_id; }
    int get_quantity() const { return quantity; }

private:
    int order_id;
    int quantity;
};

} // namespace otterbot
