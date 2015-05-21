
/**
 * is used for more complex deformation involving many contexts
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Deformation_Manager__
#define __Deformation_Manager__

#include <vector>
#include "MyPoint.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedBezierCurves.h"

class MeshDeformer;

class DeformationManager
{
public:
	// Constructor
	DeformationManager();

	// Destructor
	~DeformationManager();

	// Delete handles (press backspace)
	void Deletehandles();

	// set data
	void SetTriangleData(std::vector<int>						  offsets,
						 std::vector<CVSystem::MyPoint>		      vertexList,
						 std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
						 std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
						 std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
						 std::vector<CVSystem::MyIndexedTriangle> scTriangles,
						 std::vector<CVSystem::MyIndexedTriangle> wTriangles,
						 std::vector<CVSystem::MyIndexedTriangle> bTriangles);

	// set bezier curve data
	void SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves);

	// set screentone renderer
	void SetScreentoneSystem(std::vector<double> params, int img_width, int img_height);

	// if users want to change screentone orientation
	void SetScreentoneOrientation(double degreeAngle);

	// mouse input
	void mousePressEvent(int x, int y);
	void mouseMoveEvent(int x, int y);
	void mouseReleaseEvent(int x, int y);

	// not used
	void SetShiftButton(bool val);

	// draw
	void paintGL();	

	// set variables used to render things
	void SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, double _zoom_factor);	
	
	// should we enable bind button
	bool BindReady();	
	// do handle binding and calculate BBW
	void BindHandles();
	// delete handle
	void DeleteHandles();

	void IncreaseThickness();
	void DecreaseThickness();

private:
	// context mesh
	MeshDeformer* _partsMeshDeformer;
	// base mesh
	MeshDeformer* _baseMeshDeformer;

	std::vector<int> _offsets;
	
	// 0 base
	// 1 context
	int _state;
	

private:
	
	// mesh without context
	void SetTrianglesBaseOnly(std::vector<CVSystem::MyPoint>		     vertexList,
								std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
								std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
								std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
								std::vector<CVSystem::MyIndexedTriangle> scTriangles,
								std::vector<CVSystem::MyIndexedTriangle> wTriangles,
								std::vector<CVSystem::MyIndexedTriangle> bTriangles);

	// mesh with triangle
	void SetTrianglesWithParts(std::vector<CVSystem::MyPoint>		     vertexList,
								std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
								std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
								std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
								std::vector<CVSystem::MyIndexedTriangle> scTriangles,
								std::vector<CVSystem::MyIndexedTriangle> wTriangles,
								std::vector<CVSystem::MyIndexedTriangle> bTriangles);
};

#endif