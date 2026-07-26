#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_E116()
{
	Identifier = "DEFAULT_PT_116";
	Name = "EQVE";
	Colour = 0xFFE0A0_rgb;
	MenuVisible = 0;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.0f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;

	Weight = 85;

	HeatConduct = 70;
	Description = "A failed shared velocity test.";

	Properties = TYPE_PART;

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
	float sumvx = 0.0f, sumvy = 0.0f;   // neighbours' velocities
	float cohx = 0.0f, cohy = 0.0f;     // direction toward neighbours
	int neigh = 0;

	for(auto rx=-3;rx<=3;rx++){
		for(auto ry=-3;ry<=3;ry++){
			if(rx||ry){
				if(x+rx<0 || y+ry<0 || x+rx>=XRES || y+ry>=YRES) continue; // stay in bounds
				auto r = pmap[y+ry][x+rx];
				if(!r) continue;
				if(TYP(r)!=PT_E116) continue; // only share velocity with our own kind
				sumvx += parts[ID(r)].vx;
				sumvy += parts[ID(r)].vy;
				cohx += rx;
				cohy += ry;
				neigh++;
			}
		}
	}

	if(neigh){//petah, the horse is here
		float avgvx = sumvx / neigh;
		float avgvy = sumvy / neigh;
		const float align = 0.5f;  // ease toward the group's velocity (don't overwrite)
		const float stick = 0.2f; // gentle pull toward the group's centre
		parts[i].vx += (avgvx - parts[i].vx) * align + (cohx / neigh) * stick;
		parts[i].vy += (avgvy - parts[i].vy) * align + (cohy / neigh) * stick;
	}

	// hard speed limit so nothing can ever run away
	const float maxspd = 4.0f;
	parts[i].vx = restrict_flt(parts[i].vx, -maxspd, maxspd);
	parts[i].vy = restrict_flt(parts[i].vy, -maxspd, maxspd);
	return 0;
}
//ok so heres the thing.
//this... just doesnt work.
//oh well