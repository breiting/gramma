#pragma once
namespace gr {

/** Simple timer that computes dt. */
class Time {
    double now = 0.0;
    double dt = 0.0;
    void Tick();
};

}  // namespace gr
