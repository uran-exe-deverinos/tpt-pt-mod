#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_LCLR()
{
	Identifier = "DEFAULT_PT_LCLR";
	Name = "LCLR";
	Colour = 0xCAD372_rgb;
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = .6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.0f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 31;

	DefaultProperties.temp = -40.0f + 273.15f;
	HeatConduct = 42;
	Description = "Chlorine gas.";

	Properties = TYPE_LIQUID | PROP_NEUTPASS | PROP_PHOTPASS | PROP_DEADLY;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT; 
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT; 

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
  if(parts[i].temp>-34.6f+273.15f+4*pow(1.04f,sim->pv[y/CELL][x/CELL]/3)+(sim->pv[y/CELL][x/CELL]/1.5f)){
		sim->part_change_type(i,x,y,PT_CLOR);
		return 1;
	}//finish later
  for(auto rx=-1;rx<=1;rx++){
    for(auto ry=-1;ry<=1;ry++){
      if(rx||ry){
        auto r = pmap[y+ry][x+rx];
        if(!r) continue;
        if(TYP(r)==PT_WATR){
					if(sim->rng.chance(1,45)){
						sim->part_change_type(ID(r), x+rx, y+ry, PT_CLWT);
						sim->delete_part(x,y);
						return 1;
					}
        }else if(TYP(r)==PT_SLTW){
          if(sim->rng.chance(1,100)){
            sim->part_change_type(ID(r), x+rx, y+ry, PT_BLCH);
            sim->delete_part(x,y);
            return 1;
          }
        }else if(TYP(r)==PT_LIFE){
					parts[i].temp=293.15f;
					parts[ID(r)].tmp2=50;
					if(sim->rng.chance(1,20))
						sim->kill_part(ID(r));
				}
      }
    }
  }
  return 0;
}
