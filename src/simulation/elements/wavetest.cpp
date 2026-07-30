#include "simulation/ElementCommon.h"
#include "simulation/Air.h"
#include <iostream>
#include <algorithm>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_wavetest()
{
	Identifier = "DEFAULT_PT_WAVE";
	Name = "test2";
	Colour = 0xFFFFFF_rgb;
	MenuVisible = 0;//change to 1 later
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;//1 for powder 2 for liquid

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 100;

	HeatConduct = 0;
	Description = "wave test";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	// can't melt by normal heat conduction, this is used by other elements for special melting behavior
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
	Create = &create;
}
//tmp is angle
//tmp2 is magnetude
//tmp3 is frequency
//tmp4 is the state
static int update(UPDATE_FUNC_ARGS){
	parts[i].tmp4++;
	
  return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS){
	*pixel_mode = PSPEC_WAVE;
  return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS){
	sim->parts[i].tmp = sim->rng.between(0,359);
	sim->parts[i].tmp2 = sim->rng.between(1,5);
	sim->parts[i].tmp3 = sim->rng.between(1,10);
}