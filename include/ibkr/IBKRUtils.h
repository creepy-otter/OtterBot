#pragma once

#include <atomic>

#include "TwsApiDefs.h"

namespace otterbot {

static std::atomic<TickerId> nextTickerId(0);

inline TickerId getNextTickerId() { return ++nextTickerId; }

}  // namespace otterbot