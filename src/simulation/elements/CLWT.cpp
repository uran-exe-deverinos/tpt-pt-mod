#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CLWT()
{
	Identifier = "DEFAULT_PT_CLWT";
	Name = "CLWT";
	Colour = 0x29CC96_rgb;
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 15;

	Weight = 35;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 23;
	Description = "Distilled water, does not conduct electricity.";

	Properties = TYPE_LIQUID | PROP_NEUTPASS | PROP_PHOTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 266.15f;
	LowTemperatureTransition = PT_ICEI; //@ CLWT -> ICEI(CLWT)
	HighTemperature = 373.0f;
	HighTemperatureTransition = PT_WTRV; //@ CLWT -> WTRV

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS){
  for(auto rx=-2;rx<=2;rx++){
		for(auto ry=-2;ry<=2;ry++){
			if(rx||ry){
				auto r = pmap[y+ry][x+rx];
				auto thisr = pmap[y][x];
				if(!r) continue;
				if(TYP(r)==PT_SALT){
					if(sim->rng.chance(1,20)){
						sim->delete_part(x+rx,y+ry);
						sim->part_change_type(ID(thisr), x, y, PT_BLCH);
						return 1;
					}
				}else if(TYP(r)==PT_DSTW){
					if(sim->rng.chance(1,350)){
						sim->part_change_type(ID(r), x+rx, y+ry, PT_WATR);
						sim->part_change_type(ID(thisr), x, y, PT_WATR);
						return 1;
					}
				}else if(TYP(r)==PT_WATR){
					if(sim->rng.chance(1,700)){
						sim->part_change_type(ID(thisr), x, y, PT_WATR);
						return 1;
					}
				}else if(TYP(r)==PT_SLTW){
					if(sim->rng.chance(1,1400)){
						sim->part_change_type(ID(r), x+rx, y+ry, PT_SLTW);
						sim->part_change_type(ID(thisr), x, y, PT_WATR);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}