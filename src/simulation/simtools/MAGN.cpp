#include "simulation/ToolCommon.h"

static int perform(SimTool *tool, Simulation *sim, Particle *cpart, int x, int y, int brushX, int brushY, float strength);

void SimTool::Tool_MAGN()
{
  Identifier = "DEFAULT_TOOL_MAGN";
  Name = "MAG-";
  Colour = 0x00DDFF_rgb;
  Description = "negative magnetism";
  Perform = &perform;
}

static int perform(SimTool *tool, Simulation *sim, Particle *cpart, int x, int y, int brushX, int brushY, float strength)
{
  const int cx = x / CELL;
  const int cy = y / CELL;
  if (cx < 0 || cx >= XCELLS || cy < 0 || cy >= YCELLS)
    return 0;

  sim->charge[cy][cx] = std::clamp(sim->charge[cy][cx] - strength * 0.01f, -5.0f, 5.0f);
  return 1;
}
