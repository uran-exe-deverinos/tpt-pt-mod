#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BRMT()
{
	Identifier = "DEFAULT_PT_BRMT";
	Name = "BRMT";
	Colour = 0x705060_rgb;
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
	Hardness = 2;

	Weight = 90;

	HeatConduct = 211;
	Description = "Broken metal. Created when iron rusts or when metals break from pressure.";

	Properties = TYPE_PART|PROP_LIFE_DEC|PROP_HOT_GLOW;
	CarriesTypeIn = 1U << FIELD_CTYPE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = MAX_TEMP;
	HighTemperatureTransition = ST; //@ BRMT -> LAVA(BMTL)

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	auto &sd = SimulationData::CRef();
	auto &elements = sd.elements;

	// Melt at the original metal's melting point + a lil extra
	// Honestly this stuff sucks
	int metal = parts[i].ctype;
	if (metal <= 0 || metal >= PT_NUM || metal == PT_LAVA || !elements[metal].Enabled)
		metal = PT_BMTL;
	if (parts[i].temp >= elements[metal].HighTemperature+30)
	{
		//@ BRMT -> molten somthing(original metal)
		sim->part_change_type(i, x, y, PT_LAVA);
		parts[i].ctype = metal;
		parts[i].life = int(restrict_flt((parts[i].temp-700)/7, 0, 400));
		parts[i].tmp = 0;//cause christ was that a high value
		return 1;
	}

	if (parts[i].temp > 523.15f)//250.0f+273.15f
	{
		auto tempFactor = int(1000 - ((523.15f-parts[i].temp)*2));
		if(tempFactor < 2)
			tempFactor = 2;
		for (auto rx = -1; rx <= 1; rx++)
		{
			for (auto ry = -1; ry <= 1; ry++)
			{
				if (rx || ry)
				{
					auto r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if (TYP(r)==PT_BREC && sim->rng.chance(1, tempFactor))
					{
						if (sim->rng.chance(1, 2))
						{
							//@ BRMT + BREC -> BRMT + THRM
							sim->create_part(ID(r), x+rx, y+ry, PT_THRM);
						}
						else //@ BRMT + BREC -> THRM + BREC
							sim->create_part(i, x, y, PT_THRM);
					}
				}
			}
		}
	}

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)//holy moly im so proud of this code
{
	
	if(cpart->ctype!=PT_NONE){
		auto h = cpart->tmp;
		*colr = (h/1000000)%1000;
		*colg = (h/1000)%1000;
		*colb = h%1000;}
	return 0;
}