#include "Magnetism.h"
#include "../Simulation.h"
#include "../Air.h"
#include "../ElementClasses.h"
#include <cmath>
#include <algorithm>

Magnetism::Magnetism(Simulation &sim) : sim(sim)
{
  std::fill(&bmap_blockmag[0][0], &bmap_blockmag[0][0] + YCELLS * XCELLS, false);
}

std::array<float, 4> Magnetism::Update(int xz, int yz)
{
  std::array<float, 4> returns;
  if (xz == 0 || xz == XCELLS - 1 || yz == 0 || yz == YCELLS - 1)
  {
    returns[0] = 0.0f;
    returns[1] = 0.0f;
    returns[2] = 0.0f;
    returns[3] = 0.0f;
    return returns;
  }

  if (bmap_blockmag[yz][xz])
  {
    returns[0] = 0.0f;
    returns[1] = 0.0f;
    returns[2] = 0.0f;
    returns[3] = 0.0f;
    return returns;
  }

  float charge = sim.charge[yz][xz];
  float vel = sim.vvelmg[yz][xz];
  float mgx = 0.0f;
  float mgy = 0.0f;
  float density = sim.mediumd[yz][xz] + 1;
  constexpr float springStrength = 0.08f;
  constexpr float damping = 0.99f;
  constexpr float velocityChargeInfluence = 0.25f;
  constexpr float maxCharge = 400.0f;
  constexpr float maxVelocity = 1.0f;

  for (int y = -1; y <= 1; y++)
  {
    for (int x = -1; x <= 1; x++)
    {
      if (x == 0 && y == 0)
        continue;
      int nx = xz + x;
      int ny = yz + y;
      if (nx < 0 || nx >= XCELLS || ny < 0 || ny >= YCELLS)
        continue;
      float dist = std::sqrtf(float(x * x + y * y));
      float ncharge = sim.charge[ny][nx];
      float displacement = ncharge - charge;

      vel += displacement * springStrength / density / dist;
      mgx += displacement * (-x) / dist;
      mgy += displacement * (-y) / dist;
    }
  }
  vel = std::clamp(vel * damping, -maxVelocity, maxVelocity);
  charge = std::clamp(charge + vel * velocityChargeInfluence, -maxCharge, maxCharge);
  if (std::abs(charge) < 0.01f)
    charge = 0.0f;
  returns[0] = charge;
  returns[1] = vel;
  returns[2] = mgx;
  returns[3] = mgy;
  return returns;
}

void Magnetism::UpdateAll()
{
  std::array<float, 4> tempall[YCELLS][XCELLS];
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
      sim.mgx[y][x] = tempall[y][x][2];
      sim.mgy[y][x] = tempall[y][x][3];
      sim.vvelmg[y][x] = tempall[y][x][1];
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
      sim.vvelmg[y][x] = 0.0f;
    }
  }
}

// 500 car batteries