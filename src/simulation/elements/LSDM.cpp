#include "simulation/ElementCommon.h"

void Element::Element_LSDM()
{
	Identifier = "DEFAULT_PT_LSDM";
	Name = "LSDM";
	Colour = 0xAFA5A5_rgb;
	MenuVisible = 1;
	MenuSection = SC_REACTIVE;
	Enabled = 1;

	Advection = 0.3f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.15f;
	Diffusion = 0.00f;
	HotAir = 0.000001f* CFDS;
	Falldown = 2;

	Flammable = 1000;
	Explosive = 1;
	Meltable = 0;
	Hardness = 2;

	Weight = 40;

	DefaultProperties.temp = R_TEMP + 45.0f + 273.15f;
	HeatConduct = 170;
	Description = "Liquid Sodium.";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 307.87f;
	LowTemperatureTransition = PT_SODM; //@ LSDM -> SDMO
	HighTemperature = 541.0f;
	HighTemperatureTransition = PT_LAVA; //@ LSDM -> FIRE
}
