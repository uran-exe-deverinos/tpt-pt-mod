#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CLOR()
{
	Identifier = "DEFAULT_PT_CLOR";
	Name = "CLOR";
	Colour = 0xCACC60_rgb;
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 1;

	DefaultProperties.temp = R_TEMP + 2.0f + 273.15f;
	HeatConduct = 42;
	Description = "Chlorine gas.";

	Properties = TYPE_GAS | PROP_NEUTPASS | PROP_PHOTPASS | PROP_DEADLY;

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
	
	if(parts[i].temp<-34.6f+273.15f+4*pow(1.04f,sim->pv[y/CELL][x/CELL]/3)+(sim->pv[y/CELL][x/CELL]/1.5f)){
		sim->part_change_type(i,x,y,PT_LCLR);
		return 1;
	}
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
