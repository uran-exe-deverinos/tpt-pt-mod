#pragma once
#include "GravityData.h"
#include "GravityPtr.h"

class Gravity
{
protected:
	Gravity() = default;

public:
	// potentially clobbers gravIn
	// ---------------------------------------------------------
	// mod maker here... what the hell are you taking about?????
	// what does this mean ??????? whuh??????????
	// ---------------------------------------------------------

	void Exchange(GravityOutput &gravOut, GravityInput &gravIn, bool forceRecalc);

	static GravityPtr Create();
};
