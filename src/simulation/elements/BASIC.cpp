#include "simulation/ElementCommon.h"
#include "simulation/Air.h"
#include <iostream>
#include <algorithm>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_BASIC()
{
	Identifier = "DEFAULT_PT_BASIC";
	Name = "basic";
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
	Description = "basic";

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
}

static int update(UPDATE_FUNC_ARGS){
  return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS){
  return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS){

}