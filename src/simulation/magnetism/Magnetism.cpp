#include "Magnetism.h"
#include "../Simulation.h"
#include "../ElementClasses.h"
#include <cmath>
#include <algorithm>

Magnetism::Magnetism(Simulation &sim) : sim(sim)
{
}

std::array<float, 3> Magnetism::Update(int xz, int yz)
{
  const float chg = sim.charge[yz][xz];

  std::array<float, 3> returns;

  float strongx = 0.0f;
  float strongy = 0.0f;
  float weakx = 0.0f;
  float weaky = 0.0f;

  float chgtot = chg * 0.85f;
  const float decay = 0.08f;
  chgtot -= std::copysign(decay, chgtot) * (std::abs(chgtot) > 0.01f ? 1.0f : 0.0f);
  if (std::abs(chgtot) < 0.02f)
    chgtot = 0.0f;

  for (int x1 = -3; x1 <= 3; x1++)
  {
    for (int y1 = -3; y1 <= 3; y1++)
    {
      if (x1 == 0 && y1 == 0)
        continue;

      const int nx = xz + x1;
      const int ny = yz + y1;
      if (nx < 0 || nx >= XCELLS || ny < 0 || ny >= YCELLS)
        continue;

      const int manhattan = std::abs(x1) + std::abs(y1);
      const int radius = std::max(std::abs(x1), std::abs(y1));

      float influence = 0.0f;
      // mmmmm pressure wave
      if (manhattan == 1)
        influence = 0.08f;
      else if (manhattan == 2)
        influence = 0.05f;
      else if (manhattan == 3)
        influence = 0.03f;
      else if (manhattan == 4)
        influence = 0.015f;
      else if (manhattan == 5)
        influence = 0.008f;
      else if (manhattan == 6)
        influence = 0.004f;

      if (influence <= 0.0f)
        continue;

      const float nchg = sim.charge[ny][nx];
      const float nmgx = sim.mgx[ny][nx];
      const float nmgy = sim.mgy[ny][nx];

      const float interaction = chg * nchg;
      const float polarityFactor = (interaction < 0.0f) ? 1.0f : -1.0f;

      strongx += nmgx * std::abs(interaction) * influence * 0.5f * polarityFactor + interaction * x1 * 0.01f; // christ thats a tonna math
      strongy += nmgy * std::abs(interaction) * influence * 0.5f * polarityFactor + interaction * y1 * 0.01f;
      weakx += nmgx * std::abs(interaction) * influence * 0.2f * polarityFactor + interaction * x1 * 0.005f;
      weaky += nmgy * std::abs(interaction) * influence * 0.2f * polarityFactor + interaction * y1 * 0.005f;
    }
  }

  strongx *= 0.90f;
  strongy *= 0.90f;
  weakx *= 0.88f;
  weaky *= 0.88f;

  returns[0] = std::clamp(chgtot, -5.0f, 5.0f);
  returns[1] = std::clamp(strongx + weakx, -MAX_MAGNETIC_VALUE, MAX_MAGNETIC_VALUE) * 0.15f;
  returns[2] = std::clamp(strongy + weaky, -MAX_MAGNETIC_VALUE, MAX_MAGNETIC_VALUE) * 0.15f;
  return returns;
}

void Magnetism::UpdateAll()
{
  std::array<float, 3> tempall[YCELLS][XCELLS];
  for (int y = 0; y < YCELLS; y++)
  {
    for (int x = 0; x < XCELLS; x++)
    {
      tempall[y][x] = Update(x, y);
    }
  }
  for (int y = 0; y < YCELLS; y++)
  {
    for (int x = 0; x < XCELLS; x++)
    {
      sim.charge[y][x] = tempall[y][x][0];
      sim.mgx[y][x] = tempall[y][x][1];
      sim.mgy[y][x] = tempall[y][x][2];
    }
  }
}

void Magnetism::Clear()
{
  for (int y = 0; y < YCELLS; y++)
  {
    for (int x = 0; x < XCELLS; x++)
    {
      sim.charge[y][x] = 0.0f;
      sim.mgx[y][x] = 0.0f;
      sim.mgy[y][x] = 0.0f;
    }
  }
}

// 500 car batteries