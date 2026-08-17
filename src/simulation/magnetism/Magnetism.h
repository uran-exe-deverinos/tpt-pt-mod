#pragma once
#include "SimulationConfig.h"

class Simulation;
struct RenderableSimulation;

class Magnetism
{
public:
  explicit Magnetism(Simulation &sim);
  Simulation &sim;
  constexpr static float MAX_MAGNETIC_VALUE = 255.0f;
  std::array<float, 3> Update(int xz, int yz);
  void UpdateAll();
  void Clear();
};