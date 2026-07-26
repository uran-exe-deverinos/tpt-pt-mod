 #include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_ZINC()
{
	Identifier = "DEFAULT_PT_ZINC";
	Name = "ZINC";
	Colour = 0x8DA8A2_rgb;
	MenuVisible = 1;
	MenuSection = SC_METALS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.75f;
	Loss = 0.70f;
	Collision = 0.0f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 12;
	PhotonReflectWavelengths = 0x3FFAAA00;

	Weight = 100;

	HeatConduct = 75;
	Description = "Zinc, deforms under pressure when hot, but brittle when cold.";

	Properties = TYPE_SOLID|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 420.0f+273.15f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
	Graphics = &graphics;
}

constexpr float ADVECTION = 1.0f;

static int update(UPDATE_FUNC_ARGS)
{
  if(parts[i].temp > 373.15f){
    if (sim->pv[y/CELL][x/CELL]>5.0f)
      parts[i].life = sim->rng.between((5*sim->pv[y/CELL][x/CELL]),(7*sim->pv[y/CELL][x/CELL]));
  }else if (sim->pv[y/CELL][x/CELL]>(2.0f+(parts[i].temp-373.15f)/100.0f)){
    sim->part_change_type(i, x, y, PT_BRMT);
    parts[i].ctype = PT_ZINC;
    parts[i].tmp=142158153;
  }
	if (parts[i].life)
	{
    auto gravity = SimulationData::CRef().elements[PT_ZINC].Gravity;
		parts[i].vx += ADVECTION*sim->vx[y/CELL][x/CELL];
		parts[i].vy += ADVECTION*sim->vy[y/CELL][x/CELL]+gravity;
    parts[i].life = sim->rng.between(100,120);
	}

  for(int rx=-1;rx<=1;rx++){for(int ry=-1;ry<=1;ry++){if(rx||ry){
    auto r=pmap[y+ry][x+rx];
    if(!r) continue;
    if(TYP(r)==PT_BMTL)
    {
      parts[ID(r)].tmp=0;
      sim->part_change_type(ID(r),rx+x,ry+y,parts[ID(r)].ctype);
    }
  }}}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS){
	auto z=cpart->temp-273.15f;
	if (z>100){
	*colr += int((z-100)*1.5);
	*colg -= int((z-100)/4);
	*colb -= int((z-100)/2);
	}
	return 0;
}
