#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_TEST()
{
	Identifier = "DEFAULT_PT_TEST";
	Name = "TEST";
	Colour = 0xA0F0A0_rgb;
	MenuVisible = 1;
	MenuSection = SC_POWDERS;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 2;
	Hardness = 1;

	Weight = 10;

	HeatConduct = 255;
	Description = "Test element. very light, absorbs salt";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 983.0f;
	HighTemperatureTransition = PT_LAVA; //@ TEST -> LAVA

  Update = &update;
}
static int update(UPDATE_FUNC_ARGS)
{
  for(auto rx=-1;rx<=1;rx++){
    for(auto ry=-1;ry<=1;ry++){
      if(rx||ry){
        auto r = pmap[y+ry][x+rx];
        if(!r) continue;
        if(TYP(r)==PT_SALT && sim->rng.chance(1,20)){
          sim->part_change_type(ID(r), x+rx, y+ry, PT_CLOR);
          return 1;
        }
      }
    }
  }
  return 0;
}