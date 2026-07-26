#include "simulation/ToolCommon.h"
#include "simulation/Air.h"

static int perform(SimTool *tool, Simulation * sim, Particle * cpart, int x, int y, int brushX, int brushY, float strength);

void SimTool::Tool_NOPE()
{
	Identifier = "DEFAULT_TOOL_NOPE";
	Name = "NOPE";
	Colour = 0x999999_rgb;
	Description = "No pressure, resets the pressure to zero";
	Perform = &perform;
}

static int perform(SimTool *tool, Simulation * sim, Particle * cpart, int x, int y, int brushX, int brushY, float strength)
{
	sim->pv[y/CELL][x/CELL] = 0;
  sim->vx[y/CELL][x/CELL] = 0.0f;
  sim->vy[y/CELL][x/CELL] = 0.0f;
  return 1;
}
