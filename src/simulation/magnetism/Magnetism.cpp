#include "Magnetism.h"
#include "../Simulation.h"
#include "../ElementClasses.h"
#include <cmath>
#include <algorithm>

Magnetism::Magnetism(Simulation &sim) : sim(sim)
{
}

void Magnetism::Update(int xz, int yz)
{
  float &magx = sim.mgx[yz][xz];
  float &magy = sim.mgy[yz][xz];
  float &chg = sim.charge[yz][xz];

  float strongx = 0.0f;
  float strongy = 0.0f;
  float weakx = 0.0f;
  float weaky = 0.0f;

  for (int x1 = -1; x1 <= 1; x1++) // damn dude the format option be carrying in vscode
  {
    for (int y1 = -1; y1 <= 1; y1++)
    {
      if (x1 + y1 != 1)
      {
        strongx += sim.mgx[yz + y1][xz + x1] * (chg + sim.charge[yz + y1][xz + x1]) / 2.0f;
        strongy += sim.mgy[yz + y1][xz + x1] * (chg + sim.charge[yz + y1][xz + x1]) / 2.0f;
        chg += sim.charge[yz + y1][xz + x1] / 4.0f;
      }
      else if (x1 || y1)
      {
        weakx += sim.mgx[yz + y1][xz + x1] * (chg + sim.charge[yz + y1][xz + x1]) / 4.0f;
        weaky += sim.mgy[yz + y1][xz + x1] * (chg + sim.charge[yz + y1][xz + x1]) / 4.0f;
        chg += sim.charge[yz + y1][xz + x1] / 8.0f;
      }
    }
  }

  chg = std::clamp(chg, -1.0f, 1.0f);

  magx = std::clamp(strongx + weakx, -MAX_MAGNETIC_VALUE, MAX_MAGNETIC_VALUE);
  magy = std::clamp(strongy + weaky, -MAX_MAGNETIC_VALUE, MAX_MAGNETIC_VALUE);
}

void Magnetism::UpdateAll()
{
  for (int y = 0; y < YCELLS; y++)
  {
    for (int x = 0; x < XCELLS; x++)
    {
      Update(x, y);
    }
  }
}