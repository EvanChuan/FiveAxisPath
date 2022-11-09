#pragma once
#include "CustomisedPolyhedron.h"
#include "FillHole.h"

class PlaneCutter
{
public:
	PlaneCutter() = default;
	~PlaneCutter() = default;

	// Approach 1: this would modify original polyhedron
	bool cut(Polyhedron& poly_left, Polyhedron& poly_right, const Plane3& pl);

	bool cut_and_fill(Polyhedron& poly_left, Polyhedron& poly_right, const Plane3& pl);

	// Approach 2: this won't affect original polyhedron
	std::pair<Polyhedron, Polyhedron> cut(const Polyhedron& poly, const Plane3& pl);

	std::pair<Polyhedron, Polyhedron> cut_and_fill(const Polyhedron& poly, const Plane3& pl);

};
