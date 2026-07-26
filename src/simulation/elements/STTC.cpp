#include "simulation/ElementCommon.h"

static int graphics(GRAPHICS_FUNC_ARGS);
static int update(UPDATE_FUNC_ARGS);

void Element::Element_STTC()
{
	Identifier = "DEFAULT_PT_STTC";
	Name = "STTC";
	Colour = 0x000000_rgb;
	MenuVisible = 1;
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
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 0;

	Weight = 100;

	HeatConduct = 251;
	Description = "colortest";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}
static int update(UPDATE_FUNC_ARGS)
{
  parts[i].tmp3=sim->rng.between(0, 255);
	auto t=1;
	for(auto rx=-(parts[i].tmp);rx<=(parts[i].tmp);rx++){
		for(auto ry=-(parts[i].tmp2);ry<=(parts[i].tmp2);ry++){
			if(rx||ry){
				auto r = pmap[y+ry][x+rx];
				if(!r) continue;
				parts[i].tmp3 += parts[ID(r)].tmp3;
				t++;
			}
		}
	}
	parts[i].tmp3 /= t;
  return 0;
}
static int graphics(GRAPHICS_FUNC_ARGS)
{
	int z = cpart->tmp3;//speckles!                 but more
	if(cpart->temp > 273.15f+200.0f){
		z += int((cpart->temp-273.15f-200.0f)*0.1f);
	}
	*colr += z+cpart->temp-273.15f-22.0f;
	*colg += z;
	*colb += z;
	return 0;
}