#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_MPMP()
{
	Identifier = "DEFAULT_PT_MPMP";
	Name = "MPMP";
	Colour = 0x264252_rgb;
	MenuVisible = 1;
	MenuSection = SC_POWERED;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 9;

	Weight = 100;

	DefaultProperties.temp = 273.15f+22.0f;
	HeatConduct = 0;
	Description = "Thermostat. Changes temprature to its tempemrature when activated. (use HEAT/COOL).";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 10;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].life != 10)
	{
		if (parts[i].life>0)
			parts[i].life--;
	}
	else
	{

		if (parts[i].tmp != 1)
    {
      auto r = pmap[y][x];
      sim->charge[y/CELL][x/CELL]=parts[ID(r)].temp-273.15f;
    }
		for (auto rx = -2; rx <= 2; rx++)
		{
			for (auto ry = -2; ry <= 2; ry++)
			{
				if (rx || ry)
				{
					auto r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if (TYP(r) == PT_TMMR)
					{
						if (parts[ID(r)].life < 10 && parts[ID(r)].life > 0)
							parts[i].life = 9;
						else if (parts[ID(r)].life == 0)
							parts[ID(r)].life = 10;
					}
				}
			}
		}
	}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	int lifemod = ((cpart->life>10?10:cpart->life)*15);
	*colb += lifemod;
	return 0;
}