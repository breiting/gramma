#pragma once

#include <chrono>

namespace gr {

/** Returns the time in seconds
 */
inline double Now() {
    using clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(clock::now().time_since_epoch()).count();
}

}  // namespace gr
