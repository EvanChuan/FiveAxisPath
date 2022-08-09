#ifndef CUSTOMISED_POLYHEDRON_H
#define CUSTOMISED_POLYHEDRON_H

#include <fstream>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

template <typename Refs, typename Tag, typename Point, typename Patch_id, class Vector_>
class Polyhedron_demo_vertex :
	public CGAL::HalfedgeDS_vertex_base<Refs, Tag, Point>
{
public:
	typedef std::set<Patch_id> Set_of_indices;
	Vector_ vertexNormal;
private:
	typedef CGAL::HalfedgeDS_vertex_base<Refs, Tag, Point> Pdv_base;

	Set_of_indices indices;
	std::size_t mID;
	std::size_t time_stamp_;

public:
	int nb_of_feature_edges;

	bool is_corner() const
	{
		return nb_of_feature_edges > 2;
	}

	bool is_feature_vertex() const
	{
		return nb_of_feature_edges != 0;
	}

	void add_incident_patch(const Patch_id i)
	{
		indices.insert(i);
	}

	/// For the determinism of Compact_container iterators
	///@{
	typedef CGAL::Tag_true Has_timestamp;

	std::size_t time_stamp() const
	{
		return time_stamp_;
	}

	void set_time_stamp(const std::size_t& ts)
	{
		time_stamp_ = ts;
	}

	///}@

	const Set_of_indices&
		incident_patches_ids_set() const
	{
		return indices;
	}

	std::size_t& id() { return mID; }
	std::size_t id() const { return mID; }

	Polyhedron_demo_vertex() : Pdv_base(), mID(-1), nb_of_feature_edges(0)
	{
	}

	Polyhedron_demo_vertex(const Point& p) : Pdv_base(p), mID(-1), nb_of_feature_edges(0)
	{
	}
};

template <class Refs, class Tprev, class Tvertex, class Tface>
class Polyhedron_demo_halfedge :
	public CGAL::HalfedgeDS_halfedge_base<Refs, Tprev, Tvertex, Tface>
{
private:
	bool feature_edge;
	std::size_t time_stamp_;
	std::size_t mask_;

public:

	Polyhedron_demo_halfedge()
		: feature_edge(false), mask_(0)
	{
	};

	bool is_feature_edge() const
	{
		return feature_edge;
	}

	void set_feature_edge(const bool b)
	{
		feature_edge = b;
		this->opposite()->feature_edge = b;
	}

	std::size_t& mask() { return mask_; }
	std::size_t mask() const { return mask_; }

	void set_mask(std::size_t m) { mask_ = m; }

	/// For the determinism of Compact_container iterators
	///@{
	typedef CGAL::Tag_true Has_timestamp;

	std::size_t time_stamp() const
	{
		return time_stamp_;
	}

	void set_time_stamp(const std::size_t& ts)
	{
		time_stamp_ = ts;
	}

	///@}
};

// Defined facet base
// auxID is used in customised_mesh_slicer, to identify the id of cross-section's faces
template <class Refs, class T_, class Pln_, class Patch_id_, class Vector_>
class Polyhedron_demo_face :
	public CGAL::HalfedgeDS_face_base<Refs, T_, Pln_>
{
public:
	Patch_id_ patch_id_;

	int auxID;
	std::size_t time_stamp_;
	bool isVisited;
	Vector_ facetNormal;
	Vector_ facetCentroid;

	bool isSupported;
	double area_;
	//std::vector<int> vecIDs;
public:
	typedef Patch_id_ Patch_id;

	Polyhedron_demo_face()
		: patch_id_(1), isVisited(false), isSupported(true), auxID(-1),
		area_(0)
	{
		//vecIDs.reserve(1);
	}

	int patch_id() const
	{
		return patch_id_;
	}

	void set_patch_id(const int i)
	{
		patch_id_ = i;
	}

	void set_face_area(const double a)
	{
		area_ = a;
	}

	const double& area() { return area_; }
	const double area() const { return area_; }

	bool& visited() { return isVisited; }
	bool visited() const { return isVisited; }

	bool& supported() { return isSupported; }
	bool supported() const { return isSupported; }

	/// For the determinism of Compact_container iterators
	///@{
	typedef CGAL::Tag_true Has_timestamp;

	std::size_t time_stamp() const
	{
		return time_stamp_;
	}

	void set_time_stamp(const std::size_t& ts)
	{
		time_stamp_ = ts;
	}

	///@}
};

template <typename Patch_id>
class Polyhedron_demo_items : public CGAL::Polyhedron_items_3
{
public:
	// wrap vertex
	template <class Refs, class Traits>
	struct Vertex_wrapper
	{
		typedef typename Traits::Point_3 Point;
		typedef Polyhedron_demo_vertex<Refs,
			CGAL::Tag_true,
			Point,
			Patch_id,
			typename Traits::Vector_3> Vertex;
	};

	// wrap face
	template <class Refs, class Traits>
	struct Face_wrapper
	{
		typedef Polyhedron_demo_face<Refs,
			CGAL::Tag_true,
			typename Traits::Plane_3,
			Patch_id,
			typename Traits::Vector_3> Face;
	};

	// wrap halfedge
	template <class Refs, class Traits>
	struct Halfedge_wrapper
	{
		typedef Polyhedron_demo_halfedge<Refs,
			CGAL::Tag_true,
			CGAL::Tag_true,
			CGAL::Tag_true> Halfedge;
	};
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel    Kernel;
//typedef CGAL::Simple_cartesian<double>                        Kernel;
typedef Kernel::Point_3                                        Point3;
typedef Kernel::Plane_3										   Plane3;
typedef Kernel::Vector_3 									   Vector3;
typedef CGAL::Surface_mesh<Point3>                             Triangle_mesh;
typedef CGAL::Polyhedron_3<Kernel, Polyhedron_demo_items<int>> Polyhedron;
//typedef CGAL::Polyhedron_3<Kernel>                            Polyhedron;
typedef Polyhedron::Halfedge_handle                   		   Halfedge_handle;
typedef Polyhedron::Edge_iterator 							   Edge_iterator;
typedef Polyhedron::Facet_handle 							   Facet_handle;
typedef Polyhedron::Facet_iterator         					   Facet_iterator;

typedef boost::graph_traits<Polyhedron>::vertex_descriptor     vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron>  Skeletonization;
typedef Skeletonization::Skeleton                              Skeleton;
typedef Skeleton::vertex_descriptor                            Skeleton_vertex;
typedef Skeleton::edge_descriptor                              Skeleton_edge;

#endif