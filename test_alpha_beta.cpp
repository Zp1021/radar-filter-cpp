#include "alpha_beta.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

static int failures = 0;

// Tiny check helper: prints PASS or FAIL for each test.
void check(bool ok, const char* name) {
    std::cout << (ok ? "PASS " : "FAIL ") << name << '\n';
    if (!ok) ++failures;
}

int main() {
    // 1. A target standing still with perfect readings stays put.
    {
        AlphaBetaFilter f(0.5, 0.1, 10.0);
        for (int i = 0; i < 20; i++) f.update(10.0, 1.0);
        check(std::abs(f.position() - 10.0) < 1e-9 && std::abs(f.velocity()) < 1e-9, "stationary target stays at 10 with zero velocity");
    }
    
    // 2. A target moving at 3 m/s: the filter learns the velocity.
    {
        AlphaBetaFilter f(0.5, 0.1, 0.0);
        for (int i = 1; i <= 100; i++) f.update(3.0 * i, 1.0);
        check(std::abs(f.velocity() - 3.0) < 0.01, "learns constant velocity of 3 m/s");
        check(std::abs(f.position() - 300.0) < 0.1, "tracks position of moving target");
    }

    // 3. Noise is reduced: the estimate stays closer to the truth than a bad reading.
    {
        AlphaBetaFilter f(0.5, 0.1, 0.0);
        for (int i = 0; i < 10; i++) f.update(0.0, 1.0);
        double est = f.update(10.0, 1.0);   // one wild reading of +10
        check(est < 10.0 && est > 0.0, "a single noisy spike is only partly accepted");
    }
    
    // 4. Invalid settings are rejected.
   {
        bool threw = false;
        try { AlphaBetaFilter bad(1.5, 0.1, 0.0); } catch (const std::invalid_argument&) { threw = true; }
        check(threw, "rejects alpha above 1");
   }
   {
        bool threw = false;
        AlphaBetaFilter f(0.5, 0.1, 0.0);
        try{ f.update(1.0, 0.0); } catch (const std::invalid_argument&) { threw = true; }
        check(threw, "rejects dt of zero");
   }

   std::cout << (failures == 0 ? "All tests passed\n" : "Some tests FAILED\n");
   return failures == 0 ? 0 : 1;
}