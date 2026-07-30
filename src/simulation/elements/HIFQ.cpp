#include "simulation/ElementCommon.h"
#include "simulation/Air.h"
#include <iostream>
#include <algorithm>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_HIFQ()
{
	Identifier = "DEFAULT_PT_HIFQ";
	Name = "HIFQ";
	Colour = 0xFFE3FA_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -.99f;
	Gravity = 0.0f;
	Diffusion = 0.0f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 0;

	HeatConduct = 10;
	Description = "Protons. Transfer heat to materials, and remove sparks.";

	Properties = TYPE_ENERGY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 75;

	Update = &update;
	Graphics = &graphics;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS){
	parts[i].tmp4++;
	parts[i].tmp2 = (float)(std::sqrt(std::pow(parts[i].vx,2)+std::pow(parts[i].vy,2)))*2;
  parts[i].tmp = (atan2f(parts[i].vy,parts[i].vx)/(2*3.14159f))*360.0f;
  return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS){
	*pixel_mode = PSPEC_WAVE;
  return 0;
}
static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	float a = sim->rng.between(0, 35) * 0.17453f;
	sim->parts[i].tmp4 = sim->rng.between(0,100);
	sim->parts[i].life = 680;
	sim->parts[i].vx = 2.0f * cosf(a);
	sim->parts[i].vy = 2.0f * sinf(a);
}