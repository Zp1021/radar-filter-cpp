#include "alpha_beta.hpp"

#include <stdexcept>

AlphaBetaFilter::AlphaBetaFilter(double alpha, double beta, double initial_position)
    : alpha_(alpha), beta_(beta), position_(initial_position), velocity_(0.0) {
    if (alpha <= 0.0 || alpha > 1.0 || beta <= 0.0 || beta > 2.0) {
        throw std::invalid_argument("alpha must be in (0, 1] and beta in (0.2]");
    }
}

double AlphaBetaFilter::update(double measurement, double dt) {
    if (dt <= 0.0) {
        throw std::invalid_argument("dt must be positive");
    }

    // 1. Predict: move the estimate forward at the current velocity.
    double predicted = position_ + velocity_ * dt;

    // 2. Measure the surprise: how far the reading is from the prediction.
    double residual = measurement - predicted;

    // 3. Correct: pull position and velocity toward the reading.
    position_ = predicted + alpha_ * residual;
    velocity_ = velocity_ + (beta_ / dt) * residual;
    return position_;
}