#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_SDMO()
{
	Identifier = "DEFAULT_PT_SDMO";
	Name = "SDMO";
	Colour = 0x85827E_rgb;
	MenuVisible = 0;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 1;
	Explosive = 0;
	Meltable = 5;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 200;
	Description = "Sodium oxide, formed by the oxidation of sodium.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT; 
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 310.87f;
	HighTemperatureTransition = PT_LSDM; //@ SDMO -> LSDM

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
  for(auto rx=-1;rx<=1;rx++){
    for(auto ry=-1;ry<=1;ry++){
      if(rx||ry){
        auto r = pmap[y+ry][x+rx];
        if(!r) continue;
        if(TYP(r)==PT_WATR){
          if(sim->rng.chance(1,45)){
            sim->part_change_type(ID(r), x, y, PT_FIRE);
            sim->delete_part(x+rx,y+ry);
            return 1;
          }
        }
      }
    }
  }
  return 0;
}