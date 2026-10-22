#pragma once

// One-dimensional alpha-beta filter.
// It keeps an estimate of position and velocity and blends each new
// noisy measurement into that estimate
class AlphaBetaFilter {
private:
    /* data */
    double alpha_;
    double beta_;
    double position_;
    double velocity_;

public:
    AlphaBetaFilter(double alpha, double beta, double initial_position);
    
    // Feed one measurement taken dt seconds after the previous one.
    // Returns the new position estimate.
    double update(double measurement, double dt);
    
    double position() const { return position_; }
    double velocity() const { return velocity_;}
};
