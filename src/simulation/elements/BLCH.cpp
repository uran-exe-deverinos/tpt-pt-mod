#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_BLCH()
{
	Identifier = "DEFAULT_PT_BLCH";
	Name = "BLCH";
	Colour = 0xE8EBC5_rgb;
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

	Weight = 36;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 23;
	Description = "Distilled water, does not conduct electricity.";

	Properties = TYPE_LIQUID | PROP_NEUTPASS | PROP_PHOTPASS |PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 266.15f;
	LowTemperatureTransition = PT_ICEI; //@ BLCH -> ICEI(BLCH)
	HighTemperature = 373.0f;
	HighTemperatureTransition = PT_CAUS; //@ BLCH -> CAUS

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS){
  for(auto rx=-2;rx<=2;rx++){
		for(auto ry=-2;ry<=2;ry++){
			if(rx||ry){
				auto r = pmap[y+ry][x+rx];
				if(!r) continue;
				if(TYP(r)==PT_SALT){
					
				}else if(TYP(r)==PT_VINE || TYP(r)==PT_PLNT){
          if(sim->rng.chance(1,5)){
            sim->delete_part(x+rx,y+ry);
          }else{
            parts[i].temp += 10;
          }
          if(sim->rng.chance(1,40)){
            if(sim->rng.chance(1,4)){
              sim->part_change_type(i, x, y, PT_CLOR);
            }else{
              sim->delete_part(x,y);
            }
            return 1;
          }
          parts[ID(pmap[y][x])].temp += 10;
        }else if(TYP(r)==PT_DSTW){
					if(sim->rng.chance(1,700)){
						sim->part_change_type(ID(r), x+rx, y+ry, PT_WATR);
						sim->part_change_type(ID(r), x, y, PT_CLWT);
						return 1;
					}
				}else if(TYP(r)==PT_LIFE){
					parts[i].temp=293.15f;
					if(sim->rng.chance(1,5)){
						sim->part_change_type(ID(r),rx+x,ry+y,PT_BLCH);
						parts[ID(r)].temp=293.15f;
					}else{
						parts[ID(r)].tmp2=50;
					}
				}
			}
		}
	}
  return 0;
}