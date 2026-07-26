#include "simulation/ElementCommon.h"
#include <iostream>
#include <string>
using namespace std;

static int update(UPDATE_FUNC_ARGS);

void Element::Element_BMTL()
{
	Identifier = "DEFAULT_PT_BMTL";
	Name = "BMTL";
	Colour = 0x505070_rgb;
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
	Hardness = 1;

	Weight = 100;

	HeatConduct = 251;
	Description = "Breakable metal. Common conductive building material, can melt and break under pressure.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1273.0f;
	HighTemperatureTransition = NT; //@ BMTL -> LAVA(BMTL)

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	auto &sd = SimulationData::CRef();
	auto &elements = sd.elements;
	if(sim->pv[y/CELL][x/CELL]>=1.0f){
		sim->part_change_type(i,x,y,PT_BRMT);
		parts[i].tmp = 112080096;
	}
	// https://i.imgflip.com/6gp1di.jpg
	// I LOVE [reusing code] CHARLIE
	// I LOVE [reusing code] !1!!!11!
	// Melt at the original metal's melting point
	int metal = parts[i].ctype;
	if (metal <= 0 || metal >= PT_NUM || metal == PT_LAVA || !elements[metal].Enabled)
		metal = PT_BMTL;
	if (parts[i].temp >= elements[metal].HighTemperature+10)
	{
		//@ BRMT -> molten somthing(original metal)
		sim->part_change_type(i, x, y, PT_LAVA);
		parts[i].ctype = metal;
		parts[i].life = int(restrict_flt((parts[i].temp-700)/7, 0, 400));
		parts[i].tmp = 0;
		return 1;
	}
	if (parts[i].tmp>1)
	{
		parts[i].tmp--;
		for (auto rx = -1; rx <= 1; rx++)
		{
			for (auto ry = -1; ry <= 1; ry++)
			{
				if (rx || ry)
				{
					auto r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if ((TYP(r)==PT_METL || TYP(r)==PT_IRON) && sim->rng.chance(1, 100))
					{
						//@ BMTL + METL/IRON -> 2xBMTL
						parts[ID(r)].ctype=parts[ID(r)].type;
						sim->part_change_type(ID(r),x+rx,y+ry,PT_BMTL);
						parts[ID(r)].tmp = (parts[i].tmp<=7) ? parts[i].tmp=1 : parts[i].tmp - sim->rng.between(0, 4);
					}
				}
			}
		}
	}
	else if (parts[i].tmp==1 && sim->rng.chance(1, 1000))
	{
		//@ BMTL -> BRMT
		
		parts[i].tmp = 112080096;
		sim->part_change_type(i,x,y,PT_BRMT);
	}

	return 0;
}
