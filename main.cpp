#include "alpha_beta.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Measurement {
    double t, x, y;
};

// Read "t, x, y" rows from a csv file, skipping the header line.
std::vector<Measurement> read_csv(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("cannot open " + path);
    }
    std::vector<Measurement> rows;
    std::string line;
    std::getline(file, line);   // skip header
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Measurement m{};
        char comma;
        ss >> m.t >> comma >> m.x >> comma >> m.y;
        rows.push_back(m);
    }
    return rows;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: tracker <measurements.csv>\n";
        return 1;
    }
    try {
        std::vector<Measurement> rows = read_csv(argv[1]);
        if (rows.empty()) {
            std::cerr << "no measurements found\n";
            return 1;
        }
        // Track x and y with two independent filters.
        AlphaBetaFilter fx(0.5, 0.1, rows[0].x);
        AlphaBetaFilter fy(0.5, 0.1, rows[0].y);

        std::cout << "t,meas_x,meas_y,est_x,est_y,vel_x,vel_y\n";
        std::cout << std::fixed << std::setprecision(2);
        for (size_t i = 1; i < rows.size(); ++i) {
            double dt = rows[i].t - rows[i -1].t;
            fx.update(rows[i].x, dt);
            fy.update(rows[i].y, dt);
            std::cout << rows[i].t << ',' << rows[i].x << ',' << rows[i].y << ',' << fx.position() << ',' << fy.position() << ',' << fx.velocity() << ',' << fy.velocity() << '\n';
        }
        
    } catch (const std::exception& e){
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}