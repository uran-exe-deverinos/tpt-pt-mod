#include "simulation/ElementCommon.h"
#include "simulation/Air.h"
#include <iostream>
#include <algorithm>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_LOFQ()
{
	Identifier = "DEFAULT_PT_LOFQ";
	Name = "LOFQ";
	Colour = 0x7D2C2C_rgb;
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
	Description = "Low frequency electromagnetic waves, goes through almost everything and sparks antenne";

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
	parts[i].tmp2 = 7;
  parts[i].tmp = (atan2f(parts[i].vy,parts[i].vx)/(2*3.14159f))*360.0f;
	for(auto nx= -1;nx<1;nx++){
		for(auto ny= -1;ny<1;ny++){
			// slow down/scatter in some elements
		}
	}
  return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS){
	*pixel_mode = PSPEC_WAVE;
  return 0;
}
static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	float a = sim->rng.between(0, 1) * 3.14159f;
	a += sim->rng.between(-20,20)/30.0f;
	sim->parts[i].life = 680;
	sim->parts[i].vx = 1.5f * cosf(a);
	sim->parts[i].vy = 1.5f * sinf(a);
}