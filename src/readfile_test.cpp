#include <CGAL/Point_set_3.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "MCF_Skeleton.h"
#include "Function.h"

typedef CGAL::Point_3< CGAL::Simple_cartesian<double>> P;
typedef std::vector<std::array<double, 3>> skel_points;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "usage: ./main <input_file>" << std::endl;
        return 0;
    }
    std::ifstream input(argv[1]);
    Polyhedron tmesh;
    input >> tmesh;
    if (!CGAL::is_triangle_mesh(tmesh))
    {
        std::cout << "Input geometry is not triangulated." << std::endl;
        return EXIT_FAILURE;
    }
    
    // Skeletonize
    Skeleton skeleton;       // the result of skeleton is stored in this
    Skeletonization mcs(tmesh);
    MCF_Skeleton MS;         // declare the class as a object
    MS.MCF_Skeleton_Fun(skeleton, mcs);
    
    // Extract Skeleton Points
    Function FN;
    std::vector<Function::Skel_Points> SKP = FN.ExtractSkelPoints(skeleton);
    // Rearrange the skeleton points. But it needs general solution far.
    FN.Sorting(SKP);

    /*
    // Output all the edges of the skeleton.
    std::vector<struct Skel_Points> points;
    std::ofstream output("skel_points.txt");
    for(Skeleton_edge e : CGAL::make_range(edges(skeleton)))
    {
        const Point& s = skeleton[source(e, skeleton)].point;    // start point of edge  data type : CGAL::Point_3<CGAL::Simple_cartesian<double>>
        //std::cout << s[0] << "\n";
        //const Point& t = skeleton[target(e, skeleton)].point;    // end point of edge
        output << s << "\n";
        SP.X = s[0];
        SP.Y = s[1];
        SP.Z = s[2];
        points.push_back(SP);
    }
    output.close();
    std::sort(points.begin(), points.end(), compareZ);
    
    std::ofstream output2("order_skel_points.txt");
    for(std::vector<struct Skel_Points>::iterator it = points.begin(); it != points.end(); ++it)
    {
        output2 << "X" << it->X << " Y" << it->Y << " Z" << it->Z << "\n";
    }
    output2.close();
    */
    
    
    return 0;
}
