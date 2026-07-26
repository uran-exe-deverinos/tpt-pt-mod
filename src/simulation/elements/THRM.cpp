#include "simulation/ElementCommon.h"

static void create(ELEMENT_CREATE_FUNC_ARGS);
static int update(UPDATE_FUNC_ARGS);

void Element::Element_THRM()
{
	Identifier = "DEFAULT_PT_THRM";
	Name = "THRM";
	Colour = 0xA08090_rgb;
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
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

	HeatConduct = 87;
	Description = "Thermite. Burns into extremely hot molten metal.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	Create=&create;
	Update=&update;
}

static void create(ELEMENT_CREATE_FUNC_ARGS){
	if(!sim->parts[i].tmp || !sim->parts[i].tmp2){
		sim->parts[i].tmp2 = 600;
		sim->parts[i].tmp3 = 2;
	}
}
static int update(UPDATE_FUNC_ARGS){
	
	if(parts[i].temp >= 1.3f*parts[i].tmp2){
	if(sim->rng.chance(1,std::max(parts[i].tmp3,1))){
		sim->part_change_type(i,x,y,PT_LAVA);
		parts[i].life = 400;
		parts[i].ctype = PT_THRM;
		// molten temperature reached is proportional to the thermite's burn temp
		parts[i].temp = restrict_flt((parts[i].tmp2+273)*2.0f, MIN_TEMP, MAX_TEMP);
		parts[i].tmp = 20;
	}}
	return 0;
}
