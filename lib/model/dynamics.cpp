#include "dynamics.hpp"

#include <cmath>

#include "kernel.hpp"

namespace scarf::model {

auto compute_density(const std::vector<Vector<double, 2>>& positions, int i,
                     int j) -> double {
  return kernel(positions[i], positions[j], OUTER_R, 1.0);
}

auto compute_densities(std::function<std::vector<int>(int)> neighbor_map,
                       const std::vector<Vector<double, 2>>& positions)
    -> std::vector<double> {
  auto n = positions.size();
  std::vector<double> densities(n, 0.0);
  for (auto i = 0; i < n; i++) {
    densities[i] += PARTICLE_MASS * compute_density(positions, i, i);
    auto neighbors = neighbor_map(i);
    for (auto j : neighbors) {
      if (i < j) {
        auto v = PARTICLE_MASS * compute_density(positions, i, j);
        densities[i] += v;
        densities[j] += v;
      }
    }
  }
  return densities;
}

auto compute_pressures(double reference_density,
                       const std::vector<double>& densities)
    -> std::vector<double> {
  auto n = densities.size();
  std::vector<double> pressures(n, -std::pow(reference_density, 7.0));
  for (auto i = 0; i < n; i++) {
    pressures[i] += std::pow(densities[i], 7.0);
    pressures[i] *= 100000.0;
  }
  return pressures;
}

auto compute_acceleration(const std::vector<Vector<double, 2>>& positions,
                          const std::vector<double>& densities,
                          const std::vector<double>& pressures, int i, int j)
    -> Vector<double, 2> {
  auto grad = kernel_gradient(positions[i], positions[j], OUTER_R, 1.0);
  auto l = pressures[i] / (densities[i] * densities[i]);
  auto r = pressures[j] / (densities[j] * densities[j]);
  auto acc = PARTICLE_MASS * (l + r) * grad;
  return acc;
}

auto compute_repulsive_force(const Vector<double, 2>& position)
    -> Vector<double, 2> {
  double d0 = 0.5;   // Threshold distance for repulsive force
  double k = 100.0;  // Strength of the repulsive force

  // Check distance from the boundary (e.g., y = 0)
  if (position[1] >= d0) {
    return Vector<double, 2>(
        0.0, 0.0);  // No force if particle is far from the boundary
  }

  // Compute repulsive force in the upward direction (y-axis)
  double distance = position[1];
  double force_magnitude =
      -k * (1.0 / distance - 1.0 / d0) / (distance * distance);

  return Vector<double, 2>(0.0, force_magnitude);
}

auto compute_accelerations(std::function<std::vector<int>(int)> neighbor_map,
                           const std::vector<Vector<double, 2>>& positions,
                           const std::vector<double>& densities,
                           const std::vector<double>& pressures)
    -> std::vector<Vector<double, 2>> {
  auto n = positions.size();
  std::vector<Vector<double, 2>> accelerations(n, Vector<double, 2>(0.0, 10.0));
  for (auto i = 0; i < n; i++) {
    auto repulsive_force = compute_repulsive_force(positions[i]);
    accelerations[i] += repulsive_force;

    accelerations[i] +=
        compute_acceleration(positions, densities, pressures, i, i);
    auto neighbors = neighbor_map(i);
    for (auto j : neighbors) {
      if (i < j) {
        auto acc = compute_acceleration(positions, densities, pressures, i, j);
        accelerations[i] += acc;
        accelerations[j] += -1.0 * acc;
      }
    }
  }
  return accelerations;
}

}  // namespace scarf::model
