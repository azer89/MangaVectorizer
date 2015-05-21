
#include "stdafx.h"
#include "DeformationManager.h"

#include "MeshDeformer.h"
#include "ScreentoneRenderer.h"
#include "UtilityFunctions.h"
#include "PointInTrianglesTest.h"
#include "LightingEffect.h"

DeformationManager::DeformationManager() :
	_partsMeshDeformer(0),
	_baseMeshDeformer(0),
	_state(0)
{
}

DeformationManager::~DeformationManager()
{
	if(_partsMeshDeformer) {delete _partsMeshDeformer;}
	if(_baseMeshDeformer) {delete _baseMeshDeformer;}
}

void DeformationManager::mousePressEvent(int x, int y)
{
	using namespace CVSystem;
	_state = 0;
	if(_partsMeshDeformer)
	{
		std::vector<MyPoint>		   vList = _partsMeshDeformer->GetVertexList();
		std::vector<MyIndexedTriangle> iTriangles = _partsMeshDeformer->GetIndexedTriangles();
		std::vector<MyTriangle> triangles = UtilityFunctions::ConvertIndexedTriangles(iTriangles, vList);
		PointInTrianglesTest pit;
		pit.PushTriangles(triangles);

		if(pit.IsInside(MyPoint(x, y)))
			{ _state = 1; }

	}

	if(_state == 0)
	{
		_baseMeshDeformer->mousePressEvent(x, y);
	}
	else
	{
		_partsMeshDeformer->mousePressEvent(x, y);
	}
}

void DeformationManager::mouseMoveEvent(int x, int y)
{
	if(_state == 0)
	{
		_baseMeshDeformer->mouseMoveEvent(x, y);
	}
	else
	{
		_partsMeshDeformer->mouseMoveEvent(x, y);
	}
}

void DeformationManager::mouseReleaseEvent(int x, int y)
{
	if(_state == 0)
	{
		_baseMeshDeformer->mouseReleaseEvent(x, y);
	}
	else
	{
		_partsMeshDeformer->mouseReleaseEvent(x, y);
	}
}

void DeformationManager::SetShiftButton(bool val)
{
	if(_state == 0)
	{
		_baseMeshDeformer->SetShiftButton(val);
	}
	else
	{
		_partsMeshDeformer->SetShiftButton(val);
	}
}

void DeformationManager::paintGL()
{	
	_baseMeshDeformer->paintGL();
	if(_partsMeshDeformer)
	{
		_partsMeshDeformer->paintGLWithoutGPU();
	}
}

void DeformationManager::SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, double _zoom_factor)
{
	if(_partsMeshDeformer)
		{_partsMeshDeformer->SetVariables(_x_offset, _y_offset, _frame_width, _frame_height, _zoom_factor); }

	if(_baseMeshDeformer)
		{ _baseMeshDeformer->SetVariables(_x_offset, _y_offset, _frame_width, _frame_height, _zoom_factor); }
}


bool DeformationManager::BindReady()
{
	if(_partsMeshDeformer)
	{
		return (_partsMeshDeformer->BindReady() && _baseMeshDeformer->BindReady());
	}

	return _baseMeshDeformer->BindReady();
}

void DeformationManager::BindHandles()
{
	if(_partsMeshDeformer)
	{
		_partsMeshDeformer->BindHandles();
	}
	_baseMeshDeformer->BindHandles();
}

void DeformationManager::DeleteHandles()
{
	if(_state == 0)
	{
		_baseMeshDeformer->DeleteHandles();
	}
	else
	{
		_partsMeshDeformer->DeleteHandles();
	}
}

//void DeformationManager::DeformMesh(double x, double y);
//void DeformPseudoEdge(double x, double y);
//void DeformPseudoEdge2(double x, double y);
//void DeformTranslation(double x, double y);
//void AddPseudoEdge(double x, double y);

//void DeformationManager::SetScreentoneSystem(std::vector<double> params, int img_width, int img_height)
//{
//}

void DeformationManager::SetScreentoneSystem(std::vector<double> params, int img_width, int img_height)
{
	_baseMeshDeformer->SetScreentoneSystem(params, img_width, img_height);

	// Parts don't support screentone
}

void DeformationManager::SetScreentoneOrientation(double degreeAngle)
{
	_baseMeshDeformer->GetScreentoneRenderer()->SetOrientation(degreeAngle);

	// Parts don't support screentone
	//if(_partsMeshDeformer)
	//	{ _partsMeshDeformer->GetScreentoneRenderer()->SetOrientation(degreeAngle); }

}

void DeformationManager::SetTriangleData(std::vector<int>	offsets,
									  std::vector<CVSystem::MyPoint>		   vertexList,
									  std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
									  std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
									  std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
									  std::vector<CVSystem::MyIndexedTriangle> scTriangles,
									  std::vector<CVSystem::MyIndexedTriangle> wTriangles,
									  std::vector<CVSystem::MyIndexedTriangle> bTriangles)
{
	using namespace CVSystem;

	this->_offsets = offsets;

	if(_offsets[0] == 0 &&
	   _offsets[1] == 0 &&
	   _offsets[2] == 0 &&
	   _offsets[3] == 0 &&
	   _offsets[4] == 0 &&
	   _offsets[5] == 0 &&
	   _offsets[6] == 0 &&
	   _offsets[7] == 0)
	{
		SetTrianglesBaseOnly(vertexList,
							indexedTriangles,
							borderSCTriangles,
							borderWTriangles,
							scTriangles,
							wTriangles,
							bTriangles);
	}
	else
	{
		SetTrianglesWithParts(vertexList,
							  indexedTriangles,
							  borderSCTriangles,
							  borderWTriangles,
							  scTriangles,
							  wTriangles,
							  bTriangles);
	}
}


void DeformationManager::SetTrianglesBaseOnly(std::vector<CVSystem::MyPoint>		   vertexList,
											  std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
											  std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
											  std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
											  std::vector<CVSystem::MyIndexedTriangle> scTriangles,
											  std::vector<CVSystem::MyIndexedTriangle> wTriangles,
											  std::vector<CVSystem::MyIndexedTriangle> bTriangles)
{
	_baseMeshDeformer = new MeshDeformer();
	_baseMeshDeformer->SetTriangles( vertexList, 
									indexedTriangles, 
									borderSCTriangles, 
									borderWTriangles, 
									scTriangles, 
									wTriangles, 
									bTriangles);
}

void DeformationManager::SetTrianglesWithParts(std::vector<CVSystem::MyPoint>		   vertexList,
											   std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
											   std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
											   std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
											   std::vector<CVSystem::MyIndexedTriangle> scTriangles,
											   std::vector<CVSystem::MyIndexedTriangle> wTriangles,
											   std::vector<CVSystem::MyIndexedTriangle> bTriangles)
{
	using namespace CVSystem;

	// 000 - Vertex list	
	std::vector<MyPoint> pVertexList;
	std::vector<MyPoint> bVertexList;
	for(int a = 0; a < vertexList.size(); a++)
	{
		if(a < _offsets[0]) { pVertexList.push_back(vertexList[a]); }
		else { bVertexList.push_back(vertexList[a]); }
	}

	// 111 - all triangles	
	std::vector<MyIndexedTriangle> pIndexedTriangles;
	std::vector<MyIndexedTriangle> bIndexedTriangles;
	for(int a = 0; a < indexedTriangles.size(); a++)
	{
		if(a < _offsets[1]) { pIndexedTriangles.push_back(indexedTriangles[a]); }
		else { bIndexedTriangles.push_back(indexedTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bIndexedTriangles, -_offsets[0]);

	// 222 - Black triangles	
	std::vector<MyIndexedTriangle> pBTriangles;
	std::vector<MyIndexedTriangle> bBTriangles;
	for(int a = 0; a < bTriangles.size(); a++)
	{
		if(a < _offsets[2]) { pBTriangles.push_back(bTriangles[a]); }
		else { bBTriangles.push_back(bTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bBTriangles, -_offsets[0]);

	// 333 - white triangles	
	std::vector<MyIndexedTriangle> pWTriangles;
	std::vector<MyIndexedTriangle> bWTriangles;
	for(int a = 0; a < wTriangles.size(); a++)
	{
		if(a < _offsets[3]) { pWTriangles.push_back(wTriangles[a]); }
		else { bWTriangles.push_back(wTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bWTriangles, -_offsets[0]);

	// 444 - Screentone triangles	
	std::vector<MyIndexedTriangle> pSCTriangles;
	std::vector<MyIndexedTriangle> bSCTriangles;
	for(int a = 0; a < scTriangles.size(); a++)
	{
		if(a < _offsets[4]) { pSCTriangles.push_back(scTriangles[a]); }
		else { bSCTriangles.push_back(scTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bSCTriangles, -_offsets[0]);

	// 555 - border screentone triangles	
	std::vector<MyIndexedTriangle> pBorderSCTriangles;
	std::vector<MyIndexedTriangle> bBorderSCTriangles;
	for(int a = 0; a < borderSCTriangles.size(); a++)
	{
		if(a < _offsets[5]) { pBorderSCTriangles.push_back(borderSCTriangles[a]); }
		else { bBorderSCTriangles.push_back(borderSCTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bBorderSCTriangles, -_offsets[0]);

	// 666 - border white triangles	
	std::vector<MyIndexedTriangle> pBorderWTriangles;
	std::vector<MyIndexedTriangle> bBorderWTriangles;
	for(int a = 0; a < borderWTriangles.size(); a++)
	{
		if(a < _offsets[6]) { pBorderWTriangles.push_back(borderWTriangles[a]); }
		else { bBorderWTriangles.push_back(borderWTriangles[a]); }
	}
	UtilityFunctions::ShiftIndices(bBorderWTriangles, -_offsets[0]);

	_partsMeshDeformer = new MeshDeformer();
	_partsMeshDeformer->SetTriangles( pVertexList, 
									  pIndexedTriangles, 
									  pBorderSCTriangles, 
									  pBorderWTriangles, 
									  pSCTriangles, 
									  pWTriangles, 
									  pBTriangles);

	_baseMeshDeformer = new MeshDeformer();
	_baseMeshDeformer->SetTriangles( bVertexList, 
									 bIndexedTriangles, 
									 bBorderSCTriangles, 
									 bBorderWTriangles, 
									 bSCTriangles, 
									 bWTriangles, 
									 bBTriangles);
}

void DeformationManager::SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves)
{
	using namespace CVSystem;

	// 777 - bezier curve
	if(_offsets[7] == 0)
	{
		_baseMeshDeformer->SetIndexedBezierCurves(indexedBezierCurves);
	}
	else
	{
		std::vector<MyIndexedBezierCurves> pIndexedBezierCurves;
		std::vector<MyIndexedBezierCurves> bIndexedBezierCurves;
		for(int a = 0; a < indexedBezierCurves.size(); a++)
		{
			if(a < _offsets[7]) { pIndexedBezierCurves.push_back(indexedBezierCurves[a]); }
			else { bIndexedBezierCurves.push_back(indexedBezierCurves[a]); }
		}
		UtilityFunctions::ShiftIndices(bIndexedBezierCurves, -_offsets[0]);

		_partsMeshDeformer->SetIndexedBezierCurves(pIndexedBezierCurves);
		_baseMeshDeformer->SetIndexedBezierCurves(bIndexedBezierCurves);
	}

}

void DeformationManager::IncreaseThickness() 
{ 
	//_thicknessFactor += 0.05;
	if(_partsMeshDeformer)
	{
		_partsMeshDeformer->GetLightingEffect()->IncreaseThickness();
	}
	if(_baseMeshDeformer)
	{
		_baseMeshDeformer->GetLightingEffect()->IncreaseThickness();
	}
}
void DeformationManager::DecreaseThickness() 
{ 
	//_thicknessFactor -= 0.05;
	if(_partsMeshDeformer)
	{
		_partsMeshDeformer->GetLightingEffect()->DecreaseThickness();
	}
	if(_baseMeshDeformer)
	{
		_baseMeshDeformer->GetLightingEffect()->DecreaseThickness();
	}
}

