#include<iostream>
#include "CustomisedPolyhedron.h"

using namespace std;

class RWfile
{
public:
    static void WritePolytoOFF(Polyhedron& poly,vector<Polyhedron> cut_res);

};