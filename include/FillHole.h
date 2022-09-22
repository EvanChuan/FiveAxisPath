#pragma once
#include "CustomisedPolyhedron.h"

class FillHole
{
public:
	void fill_hole(Polyhedron& poly, Vector3& nr, const double density = 0.4);
};
