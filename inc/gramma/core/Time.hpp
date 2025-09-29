#pragma once
namespace gr {

/** Simple timer that computes dt. */
struct Time {
    double now = 0.0;
    double dt = 0.0;
    void Tick();
};

}  // namespace gr
