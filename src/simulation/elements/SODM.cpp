#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_SODM()
{
	Identifier = "DEFAULT_PT_SODM";
	Name = "SODM";
	Colour = 0x96948F_rgb;
	MenuVisible = 1;
	MenuSection = SC_REACTIVE;
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
	Meltable = 50;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 240;
	Description = "Sodium. Explosive, especially on contact with water. Low melting point.";

	Properties = TYPE_SOLID|PROP_CONDUCTS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 307.87f;
	HighTemperatureTransition = PT_LSDM; //@ SODM -> LSDM

  Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	bool burning = parts[i].temp > 673.15f;
	for (auto rx = -1; rx <= 1; rx++)
		for (auto ry = -1; ry <= 1; ry++)
			if (rx || ry) {
				auto r = pmap[y+ry][x+rx];
				if (!r) continue;
				auto rt = TYP(r);
				if (rt == PT_FIRE || rt == PT_PLSM)
					burning = true;
			}

	for (auto rx = -2; rx <= 2; rx++) {
		for (auto ry = -2; ry <= 2; ry++) {
			if (rx || ry) {
				auto r = pmap[y+ry][x+rx];

				if (!r) {
					// exposed to open air -> slowly oxidise into SDMO
					if (sim->rng.chance(1, 1000)) {
						sim->part_change_type(i, x, y, PT_SDMO);
						return 1;
					}
					continue;
				}

				auto rt = TYP(r);

				// burning + CLOR -> both become SALT (Na + Cl -> NaCl)
				if (rt == PT_CLOR && burning) {
					sim->part_change_type(ID(r), x+rx, y+ry, PT_SALT);
					sim->part_change_type(i, x, y, PT_SALT);
					return 1;
				}

				// WATR -> violent: water ignites, sodium consumed
				if (rt == PT_WATR) {
					sim->part_change_type(ID(r), x+rx, y+ry, PT_FIRE);
					sim->pv[y/CELL][x/CELL] += 8.0f;
					sim->delete_part(x, y);
					return 1;
				}
			}
		}
	}
	return 0;
}
