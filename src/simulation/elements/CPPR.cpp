#include "simulation/ElementCommon.h"
#include "simulation/Air.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_CPPR()
{
	Identifier = "DEFAULT_PT_CPPR";
	Name = "CPPR";
	Colour = 0xB87333_rgb;
	MenuVisible = 1;
	MenuSection = SC_METALS;
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
	PhotonReflectWavelengths = 0x3FF00000;

	Weight = 100;

	HeatConduct = 255;
	Description = "Copper, excelent heat conductor and pressure resistant";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_HOT_GLOW|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1357.77f;
	HighTemperatureTransition = PT_LAVA; //@ GOLD -> LAVA(GOLD)

	Update = &update;
	Graphics = &graphics;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS){
  int cppr = 0;//fr fr copied the ttan code

	auto oxidize=[&](){
		
		for(auto rx=-1;rx<=1;rx++){for(auto ry=-1;ry<=1;ry++){if(rx||ry){
			auto r=pmap[y+ry][x+rx];
			switch (TYP(r))
			{
				case PT_O2:
				if(sim->rng.chance(1,2*int(6000-parts[i].temp+273.15f))){
					return true;
				}
				break;
				case PT_WATR:
				if(sim->rng.chance(1,int(4000-parts[i].temp+273.15f))){
					return true;
				}
				break;
				case PT_SLTW:
				if(sim->rng.chance(1,5*int(10000-parts[i].temp+273.15f))){
					return true;
				}
				break;
			}
		}}}
		return false;
	};

	parts[i].tmp3=sim->rng.between(0,10);
	if (nt <= 2)
		cppr = 2;
	else if (parts[i].tmp3)
		cppr = 2;
	else if (nt <= 6)
	{
		for (int rx = -1; rx <= 1; rx++)
		{
			for (int ry = -1; ry <= 1; ry++)
			{
				if (!rx != !ry)
				{
					auto r = pmap[y+ry][x+rx];
					if (TYP(r) == PT_CPPR)
					{
						cppr++;
						parts[i].tmp3 += parts[ID(r)].tmp3;
					}
				}
			}
		}
	}
	parts[i].tmp3 /= cppr+1;

	if (cppr >= 2)
	{
		sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
		sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	}

	if (parts[i].tmp2>0&&parts[i].temp>1300.0f){
		parts[i].tmp2=0;
	}else if(parts[i].temp>400.0f+273.15f && parts[i].tmp2>0){
		if(sim->rng.chance(1,5)){
			parts[i].tmp2-=1;
			parts[i].temp-=1.5f;
		}
	}

	if (oxidize()&&parts[i].tmp2<=90){
		parts[i].tmp2+=sim->rng.between(1,20);
	}else if(parts[i].tmp2>=90){
		sim->part_change_type(i,x,y,PT_BRMT);
		parts[i].ctype = PT_CPPR;
		parts[i].tmp=80173146;
	}
  return 0;
}
static int graphics(GRAPHICS_FUNC_ARGS){
	int z=cpart->tmp3;
	int oxi=cpart->tmp2;
	*colr += int(z*3-(oxi*0.8f));
	*colg += (z*2)+(oxi);
	*colb += (z*2)+(oxi);
  return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	sim->parts[i].tmp2=0;
	sim->parts[i].tmp3=sim->rng.between(0,10);
}