
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "MeshDeformer.h"

#include "RenderUtility.h"
#include "ScreentoneRenderer.h"
#include "CurveRenderer2.h"
#include "SystemParams.h"
#include "UtilityFunctions.h"
#include "BBWCalculator.h"
#include "PointInTrianglesTest.h"
#include "LightingEffect.h"

#include <boost/chrono.hpp>


MeshDeformer::MeshDeformer() : 
	
	_renderUtility(new RenderUtility()),
	_pseudoEdges(new PseudoEdges()),
	_scRenderer(0),
	_cRendererV2(0),
	_ptInTriangleTest(0),
	 _lightingEffect(0),
	
	_mouseDown(false),
	_shiftDown(false),
	_isDeforming(false),

	_whatDo(DO_ADD_HANDLE),
	_vertexKDTree(0),

	_maxHandleDist(100)	// 20
{
}

MeshDeformer::~MeshDeformer()
{
	if(_vertexKDTree)	  { delete _vertexKDTree; }
	if(_renderUtility)	  { delete _renderUtility; }
	if(_cRendererV2)	  { delete _cRendererV2; }
	if(_scRenderer)		  { delete _scRenderer; }
	if(_ptInTriangleTest) { delete _ptInTriangleTest; }
	if(_lightingEffect)	  { delete _lightingEffect; }
}

void MeshDeformer::paintGLWithoutGPU()
{
	DrawMeshWithoutGPU();
	if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 0, 1); }
	if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 0, 1); }
	if(SystemParams::gr_show_bbw_pseudo_edge) { DrawPseudoEdges(); }
	if(SystemParams::gr_show_bbw_handles) { DrawHandles(); }
}

void MeshDeformer::paintGL()
{
	if(SystemParams::gr_gpu) { DrawMeshWithGPU(); }
	else { DrawMeshWithoutGPU(); }

	if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 0, 1); }
	if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 0, 1); }	
	if(SystemParams::gr_show_bbw_pseudo_edge) { DrawPseudoEdges(); }
	if(SystemParams::gr_show_bbw_handles) { DrawHandles(); }

	if(_lightingEffect)
	{
		_lightingEffect->SetVariables(_x_offset, _y_offset, _frame_width, _frame_height, /*this->_img_width, this->_img_height,*/ _zoom_factor);
		_lightingEffect->paintGL();
	}

	DrawStrokes();
}

void MeshDeformer::DrawStrokes()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	if(_stroke.size() > 1) // Original users' stroke
	{
		QColor col(0, 0, 255);
		glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

		for(size_t a = 0; a < _stroke.size() -1; a++)
		{ glVertex3f(_stroke[a].x, _stroke[a].y, 0); glVertex3f(_stroke[a+1].x, _stroke[a+1].y, 0); }
	}	
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void MeshDeformer::DrawMeshWithGPU()
{	
	QGLFramebufferObject* scFBO = 0;
	QGLFramebufferObject* crFBO = 0;

	
	/*if(_scRenderer && _lightingEffect)
	{ scFBO = _scRenderer->RenderToFBOWithLightingAA(_lightingEffect->_imgID, _lightingEffect->GetThickness(), _x_offset, _y_offset, _frame_width, _frame_height, _zoom_factor); }
	else if(_scRenderer)
	{
		scFBO = _scRenderer->RenderToFBOAA(_x_offset,_y_offset, _frame_width, _frame_height, _zoom_factor);
	}*/
	
	if(_scRenderer)
		{ scFBO = _scRenderer->RenderToFBOAA(_x_offset,_y_offset, _frame_width, _frame_height, _zoom_factor); }

	if(_cRendererV2 && !this->_isDeforming)
		{ crFBO = _cRendererV2->RenderCurvesToFBO(_x_offset,_y_offset, _frame_width, _frame_height, _zoom_factor); }

	_renderUtility->DrawFullscreen(crFBO, 
								   scFBO,
								   _vertexList,
								   _borderWTriangles,
								   _wTriangles,
								   _bTriangles,
								   _x_offset, 
								   _y_offset, 
								   _frame_width, 
								   _frame_height, 
								   _zoom_factor);

	if(scFBO) { delete scFBO; }
	if(crFBO) { delete crFBO; }
}

void MeshDeformer::DrawMeshWithoutGPU()
{
	_renderUtility->DrawTriangles(_vertexList, _wTriangles, 1.0, 1.0, 1.0);
	_renderUtility->DrawTriangles(_vertexList, _borderWTriangles, 1.0, 1.0, 1.0);
	_renderUtility->DrawTriangles(_vertexList, _scTriangles, 0.75, 0.75, 0.75);
	double blackness = SystemParams::cr_blackness;
	_renderUtility->DrawTriangles(_vertexList, _bTriangles, blackness, blackness, blackness);
}

void MeshDeformer::DrawHandles()
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(this->_pseudoEdges->_handleIndices.size() > 0)
	{
		for(int a = 0; a < this->_pseudoEdges->_handleIndices.size(); a++)
		{
			CVSystem::MyPoint pt = _vertexList[this->_pseudoEdges->_handleIndices[a]];

			glColor4f(0.058, 0.435, 0.776, 1.0);
			_renderUtility->DrawCircle(pt.x, pt.y, 10 / _zoom_factor);

			if(_pseudoEdges->_handleIndex2 == a) { glColor4f(1.0, 1.0, 0.0, 1.0); }
			else				   { glColor4f(0.043, 0.815, 0.851, 1.0); }
			
			_renderUtility->DrawCircle(pt.x, pt.y, 6 / _zoom_factor);
		}
	}

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void MeshDeformer::DrawPseudoEdges()
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// making an edge...
	if(this->_pseudoEdges->GetCandidateIndex1() != -1 && this->_mouseDown && this->_shiftDown)
	{
		int pIdx = this->_pseudoEdges->GetCandidateIndex1();
		CVSystem::MyPoint pt1 = this->_vertexList[pIdx];
		CVSystem::MyPoint pt2 = this->_pseudoEdges->_peCurrentPoint;

		glLineStipple(5, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(5.0);
		glColor4f(0, 0.69, 0.313, 1.0);
		glBegin(GL_LINES);

		glVertex3f(pt1.x, pt1.y, 0);
		glVertex3f(pt2.x, pt2.y, 0);

		glEnd();				
		glDisable(GL_LINE_STIPPLE);
	}

	// Draw existing edges
	if(this->_pseudoEdges->_peList.size() > 0)
	{
		glLineStipple(5, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(5.0);
		glColor4f(0, 0.69, 0.313, 1.0);
		glBegin(GL_LINES);
		for(size_t a = 0; a < this->_pseudoEdges->_peList.size(); a++)
		{
			std::vector<int> pEdge = this->_pseudoEdges->_peList[a];
			for(size_t b = 0; b < pEdge.size() - 1; b++)
			{
				CVSystem::MyPoint pt1 = _vertexList[pEdge[b]];
				CVSystem::MyPoint pt2 = _vertexList[pEdge[b+1]];

				glVertex3f(pt1.x, pt1.y, 0);
				glVertex3f(pt2.x, pt2.y, 0);
			}
		}
		glEnd();				
		glDisable(GL_LINE_STIPPLE);
	}

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void MeshDeformer::BindHandles()
{
	this->_whatDo = DO_DEFORMATION;
	std::cout << "Bind Handles\n";

	// Timing calculation
	using namespace boost::chrono ;
	auto startTime = steady_clock::now();



	std::vector<CVSystem::MyPoint> handlePoints;
	for(size_t a = 0; a < _pseudoEdges->_handleIndices.size(); a++)
		handlePoints.push_back(_vertexList[ _pseudoEdges->_handleIndices[a] ]);

	this->_weights = BBWCalculator::GetWeights(_vertexList, _indexedTriangles, handlePoints);

	auto dur = steady_clock::now() - startTime ;
	std::cout << "Binding time: " << duration_cast<milliseconds>(dur).count() << " milliseconds\n" ;
	std::cout << "handle size: " << _pseudoEdges->_handleIndices.size() << "\n";

	std::cout << "Done\n";
}

void MeshDeformer::BuildVertexKDTree()
{
	this->_vertexPointCloud.pts.clear();
	this->_vertexPointCloud.pts.resize(_vertexList.size());
	for(int a = 0; a < _vertexList.size(); a++)
	{
		_vertexPointCloud.pts[a].x = _vertexList[a].x;
		_vertexPointCloud.pts[a].y = _vertexList[a].y;
	}

	_vertexKDTree = new PointKDTree(2 /*dim*/, _vertexPointCloud, KDTreeSingleIndexAdaptorParams(64 /* max leaf */) );
	_vertexKDTree->buildIndex();
}

void MeshDeformer::mousePressEvent(int x, int y)
{
	/*if(this->_whatDo == DO_ADD_HANDLE) { AddHandle(x, y); }
	else if(this->_whatDo == DO_DEFORMATION && _shiftDown)
	{
		int index1; 	// index referencing _vertexList
		int index2;	    // index referencing _handleIndices
		GetIndices(x, y, index1, index2);
		this->_pseudoEdges->SetCandidateIndices(index1, index2);
	}
	else 
	{
		GetIndices(x, y, _pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2);
	}
		
	this->_pseudoEdges->_peCurrentPoint = CVSystem::MyPoint(x, y);

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_handleIndex2 == -1)
		{ _lightingEffect->mousePressEvent(x, y); }*/

	if(this->_whatDo == DO_DEFORMATION)
	{
		GetIndices(x, y, _pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2);
	}

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_handleIndex2 == -1)
	{ _lightingEffect->mousePressEvent(x, y); }

	this->_mouseDown = true;
}

void MeshDeformer::mouseMoveEvent(int x, int y)
{
	/*if(this->_mouseDown && !this->_shiftDown) { DeformMesh(x, y); }
	else { this->_pseudoEdges->_peCurrentPoint = CVSystem::MyPoint(x, y); }

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_handleIndex1 == -1)
		{ _lightingEffect->mouseMoveEvent(x, y); }*/

	if(this->_mouseDown && _whatDo == DO_ADD_HANDLE)
		{ _stroke.push_back(CVSystem::MyPoint(x, y)); }

	if(this->_mouseDown && _whatDo == DO_DEFORMATION)
		{ DeformMesh(x, y); }

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_candidateIndex1 == -1)
	{ _lightingEffect->mouseMoveEvent(x, y); }
}

void MeshDeformer::mouseReleaseEvent(int x, int y)
{
	using namespace CVSystem;

	/*if(this->_isDeforming) 
	{ 
		if(_cRendererV2) 
		{
			_cRendererV2->Recalculate(_vertexList);
			this->_isDeforming = false; 
		}
	}	

	if(this->_pseudoEdges->GetCandidateIndex1() != -1)
	{ this->_pseudoEdges->AddEdge(x, y, _pseudoEdges->_handleIndices, _vertexList); }

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_handleIndex1 == -1)
		{ _lightingEffect->mouseReleaseEvent(x, y); }*/


	if(this->_whatDo == DO_ADD_HANDLE)
	{
		
		if(_stroke.size() > 1)
		{
			MyPoint pt1 = _stroke[0];
			MyPoint pt2 = _stroke[_stroke.size() - 1];

			if(pt1.Distance(pt2) <= _maxHandleDist)
			{
				// only a point
				AddHandle(x, y);

			}
			else
			{
				int queryPtIdx1 = -1;
				MyPoint queryPt(-1, -1);
				QueryVertexPoint(pt1.x, pt1.y, queryPt, queryPtIdx1);	
				
				if(_pseudoEdges->DoesHandleExist(queryPtIdx1)) { /* connect */ }
				else { AddHandle(pt1.x, pt1.y); /* not connected */ }
				
				AddHandle(pt2.x, pt2.y);

				//// set first point
				int snappedPt1Idx1 = -1;
				int snappedPt1Idx2 = -1; 
				MyPoint snappedPt1(-1, -1);	
				QueryHandlePoint(pt1.x, pt1.y, snappedPt1, snappedPt1Idx1, snappedPt1Idx2);
				this->_pseudoEdges->SetCandidateIndices(snappedPt1Idx1, snappedPt1Idx2);

				//// set second point
				int snappedPt2Idx1 = -1; 
				int snappedPt2Idx2 = -1;	// not used
				MyPoint snappedPt2(-1, -1);
				QueryHandlePoint(pt2.x, pt2.y, snappedPt2, snappedPt2Idx1, snappedPt2Idx2);
				_pseudoEdges->AddEdge(snappedPt2.x, snappedPt2.y, _vertexList);
			}
		}
		else
		{
			// only point
			AddHandle(x, y);
		}

		
	}
	//else
	//{
	//	GetIndices(x, y, _pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2);
	//}

	if(this->_isDeforming) 
	{ 
		if(_cRendererV2) 
		{
			_cRendererV2->Recalculate(_vertexList);
			this->_isDeforming = false; 
		}
	}	

	//if(this->_pseudoEdges->GetCandidateIndex1() != -1)
	//{ this->_pseudoEdges->AddEdge(x, y, _pseudoEdges->_handleIndices, _vertexList); }

	if(_lightingEffect && !_shiftDown && this->_whatDo == DO_DEFORMATION && _pseudoEdges->_handleIndex1 == -1)
	{ _lightingEffect->mouseReleaseEvent(x, y); }


	this->_mouseDown = false;
	_stroke.clear();

	//// Don't uncomment these or the program will be really slow
	////if(_cRendererV2) { _cRendererV2->Recalculate(_vertexList); }
}

void MeshDeformer::QueryVertexPoint(float x, float y, CVSystem::MyPoint& pt, int& index1)
{
	float query_pt[2] = { x, y};
	std::vector<size_t> ret_index(1);
	std::vector<float> out_dist_sqr(1);
	_vertexKDTree->knnSearch(&query_pt[0], 1, &ret_index[0], &out_dist_sqr[0]);

	index1 = ret_index[0];
	pt = CVSystem::MyPoint(_vertexPointCloud.pts[index1].x, _vertexPointCloud.pts[index1].y);
}

void MeshDeformer::QueryHandlePoint(float x, float y, CVSystem::MyPoint& pt, int& index1, int& index2)
{
	using namespace CVSystem;
	double dist = DBL_MAX;
	for(size_t a = 0; a < _pseudoEdges->_handleIndices.size(); a++)
	{
		CVSystem::MyPoint tempPt = _vertexList[_pseudoEdges->_handleIndices[a]];
		double d = tempPt.Distance(CVSystem::MyPoint(x, y));
		if(d < dist)
		{ 
			index1 = _pseudoEdges->_handleIndices[a];
			index2 = a;
			pt = tempPt;
			dist = d;
		}
	}
}

CVSystem::MyPoint MeshDeformer::AddHandle(float x, float y)
{
	using namespace CVSystem;
	MyPoint retPt(-1, -1);

	if(!_vertexKDTree) { return retPt; }

	int retIdx = -1;
	
	QueryVertexPoint(x, y, retPt, retIdx);

	if(!_pseudoEdges->DoesHandleExist(retIdx))
	{
		this->_pseudoEdges->_handleIndices.push_back(retIdx);
		std::cout << "Handle added on (" << retPt.x << ", " << retPt.y << ")\n";

		// Pseudo Edge
		this->_pseudoEdges->InvalidateCandidateIndices();
	}

	_pseudoEdges->UpdatePairIndex();

	return retPt;
}

void MeshDeformer::AddPseudoEdge(double x, double y)
{
	if(!_vertexKDTree) { return; }

	std::cout << "add pseudo edge\n";

	int retIdx = -1;
	for(size_t a = 0; a < _pseudoEdges->_handleIndices.size(); a++)
	{
		CVSystem::MyPoint pt = _vertexList[_pseudoEdges->_handleIndices[a]];
		if(pt.Distance(CVSystem::MyPoint(x, y)) < M_EPS)
		{ retIdx = _pseudoEdges->_handleIndices[a]; break; }
	}

	if(retIdx == -1) {return;}
}

// FAILED !
void MeshDeformer::DeformPseudoEdge2(double x, double y)
{
	using namespace CVSystem;

	// previous position of current handle		prevOfCurPt
	// previous position of parent handle		prevOfParentPt
	// previous position next handle			prevOfNextPt
	MyPoint prevOfCurPt, prevOfParentPt, prevOfNextPt;
	_pseudoEdges->GetNeighborPoints(_vertexList, _pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2, prevOfCurPt, prevOfParentPt, prevOfNextPt);

	// index of parent handle	parentIndex
	// index of next handle		nextIndex
	int parentIndex1 = -1; int nextIndex1 = -1;
	_pseudoEdges->GetNeighborIndices(_pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2, parentIndex1, nextIndex1);

	// If first handle of an pseudo edge == current handle, it's simple translation
	if(parentIndex1 == -1)  { DeformTranslation(x, y);  return; }

	// Current handle
	CVSystem::MyPoint curPoint = MyPoint(x, y);

	// Get angle from parent
	MyPoint prevDirFromParent = prevOfParentPt.DirectionTo(prevOfCurPt);
	MyPoint curDirFromParent = prevOfParentPt.DirectionTo(curPoint);

	// should be inversed ?
	double angleRad = UtilityFunctions::GetRotation(prevDirFromParent, curDirFromParent);
	double angleDeg = UtilityFunctions::RadianToDegree(angleRad);	

	Eigen::Matrix2f rotMatrix;
	rotMatrix << cos(angleDeg), -sin(angleDeg), sin(angleDeg), cos(angleDeg);

	// Build translation back
	Eigen::Matrix2f transBackMatrix;
	transBackMatrix << prevOfParentPt.x, 0, 0,  prevOfParentPt.y;

	// Build Inverse translation
	Eigen::Matrix2f transInvMatrix;
	transBackMatrix << -prevOfParentPt.x, 0, 0,  -prevOfParentPt.y;

	// From the cur direction set the difference from norm, it's going to be the translation
	double dist1 = prevDirFromParent.Length();	// Get prev distance from parent
	double dist2 = curDirFromParent.Length();	// Get cur distance from parent
	double distDiff = dist2 - dist1;
	MyPoint translationVector = curDirFromParent.Norm() * distDiff;
	Eigen::Matrix2f transMatrix;
	transMatrix << translationVector.x, 0, 0,  translationVector.y;

	// in / out
	Eigen::Vector2f inputPos;
	Eigen::Vector2f outPos;

	for(int a = 0; a < _vertexList.size(); a++)
	{
		double w = _weights(a, _pseudoEdges->_handleIndex2);
		if(w < 0.001){continue;}

		MyPoint* curVertex = &_vertexList[a];

		inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
		outPos = transInvMatrix *  (rotMatrix * w) * transBackMatrix * inputPos;
		curVertex->x = outPos(0); curVertex->y = outPos(1); 
	}

	/*// rotate parent
	int parentIndex2 = GetSecondHandleIndex(parentIndex1);
	for(int a = 0; a < _vertexList.size(); a++)
	{
		double w = _weights(a, parentIndex2);
		//if(w > -M_EPS && w < M_EPS) {continue;}
		if(w < 0.001){continue;}
		w *= 0.5;

		MyPoint* curVertex = &_vertexList[a];
		//Eigen::Rotation2D<float> rotTransform(angleRad * w);
		//Eigen::Translation<float,2> transTransform(translationVector.x * w, translationVector.y * w);

		inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
		outPos =  (transMatrix * w) * transBackMatrix * (rotMatrix * w) * transInvMatrix * inputPos;
		curVertex->x = outPos(0); 
		curVertex->y = outPos(1); 

		if(a == parentIndex1)
		{ _pseudoEdges->UpdateHandlePosition(parentIndex1, parentIndex2, outPos(0), outPos(1)); }
	}


	// rotate next
	if(nextIndex1 != -1)
	{
		int nextIndex2 = GetSecondHandleIndex(nextIndex1);
		for(int a = 0; a < _vertexList.size(); a++)
		{
			double w = _weights(a, nextIndex2);
			//if(w > -M_EPS && w < M_EPS) {continue;}
			if(w < 0.001){continue;}
			w *= 0.5;

			MyPoint* curVertex = &_vertexList[a];
			//Eigen::Rotation2D<float> rotTransform(angleRad * w);
			//Eigen::Translation<float,2> transTransform(translationVector.x * w, translationVector.y * w);

			inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
			outPos =  (transMatrix * w) * transBackMatrix * (rotMatrix * w) * transInvMatrix * inputPos;
			curVertex->x = outPos(0);  curVertex->y = outPos(1); 

			if(a == nextIndex1)
			{ _pseudoEdges->UpdateHandlePosition(nextIndex1, nextIndex2, outPos(0), outPos(1)); }
		}
	}*/
}

void MeshDeformer::DeformPseudoEdge(double x, double y)
{
	using namespace CVSystem;
	//using namespace Eigen;
	
	// previous position of current handle		prevOfCurPt
	// previous position of parent handle		prevOfParentPt
	// previous position next handle			prevOfNextPt
	MyPoint prevOfCurPt, prevOfParentPt, prevOfNextPt;
	_pseudoEdges->GetNeighborPoints(_vertexList, _pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2, prevOfCurPt, prevOfParentPt, prevOfNextPt);

	// index of parent handle	parentIndex
	// index of next handle		nextIndex
	int parentIndex1 = -1; int nextIndex1 = -1;
	_pseudoEdges->GetNeighborIndices(_pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2, parentIndex1, nextIndex1);

	// If first handle of an pseudo edge == current handle, it's simple translation
	if(parentIndex1 == -1)  { DeformTranslation(x, y);  return; }

	// Current handle
	CVSystem::MyPoint curPoint = MyPoint(x, y);

	// Get angle from parent
	MyPoint prevDirFromParent = prevOfParentPt.DirectionTo(prevOfCurPt);
	MyPoint curDirFromParent = prevOfParentPt.DirectionTo(curPoint);

	// should be inversed ?
	double angleRad = UtilityFunctions::GetRotation(prevDirFromParent, curDirFromParent);
	double angleDeg = UtilityFunctions::RadianToDegree(angleRad);	
	
	// Build translation back
	Eigen::Translation<float,2> transBack(prevOfParentPt.x, prevOfParentPt.y);

	// Build Inverse translation
	Eigen::Translation<float,2> transInv(-prevOfParentPt.x, -prevOfParentPt.y);
	
	// From the cur direction set the difference from norm, it's going to be the translation
	double dist1 = prevDirFromParent.Length();	// Get prev distance from parent
	double dist2 = curDirFromParent.Length();	// Get cur distance from parent
	double distDiff = dist2 - dist1;
	MyPoint translationVector = curDirFromParent.Norm() * distDiff;

	// in / out
	Eigen::Vector2f inputPos;
	Eigen::Vector2f outPos;

	for(int a = 0; a < _vertexList.size(); a++)
	{
		double w = _weights(a, this->_pseudoEdges->_handleIndex2);
		if(w < 0.001){continue;}

		MyPoint* curVertex = &_vertexList[a];
		Eigen::Rotation2D<float> rotTransform(angleRad * w);
		Eigen::Translation<float,2> transTransform(translationVector.x * w, translationVector.y * w);

		inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
		outPos =  transTransform * transBack * rotTransform * transInv * inputPos;
		curVertex->x = outPos(0); curVertex->y = outPos(1); 
	}
		
	// rotate parent
	int parentIndex2 = _pseudoEdges->GetSecondHandleIndex(parentIndex1);
	for(int a = 0; a < _vertexList.size(); a++)
	{
		double w = _weights(a, parentIndex2);
		if(w < 0.001){continue;}
		w *= 0.5;

		MyPoint* curVertex = &_vertexList[a];
		Eigen::Rotation2D<float> rotTransform(angleRad * w);
		Eigen::Translation<float,2> transTransform(translationVector.x * w, translationVector.y * w);

		inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
		outPos =  transTransform * transBack * rotTransform * transInv * inputPos;
		curVertex->x = outPos(0); 
		curVertex->y = outPos(1); 
	}

	
	// rotate next
	if(nextIndex1 != -1)
	{
		int nextIndex2 = _pseudoEdges->GetSecondHandleIndex(nextIndex1);
		for(int a = 0; a < _vertexList.size(); a++)
		{
			double w = _weights(a, nextIndex2);
			if(w < 0.001){continue;}
			w *= 0.5;

			MyPoint* curVertex = &_vertexList[a];
			Eigen::Rotation2D<float> rotTransform(angleRad * w);
			Eigen::Translation<float,2> transTransform(translationVector.x * w, translationVector.y * w);

			inputPos(0) = curVertex->x; inputPos(1) = curVertex->y;
			outPos =  transTransform * transBack * rotTransform * transInv * inputPos;
			curVertex->x = outPos(0);  
			curVertex->y = outPos(1); 
		}
	}
}

void MeshDeformer::DeformTranslation(double x, double y)
{
	CVSystem::MyPoint curHandle = this->_vertexList[this->_pseudoEdges->_handleIndex1];
	CVSystem::MyPoint transValue(x - curHandle.x, y - curHandle.y);
	for(int a = 0; a < _vertexList.size(); a++)
	{
		double w = _weights(a, this->_pseudoEdges->_handleIndex2);
		_vertexList[a].x = _vertexList[a].x + (transValue.x * w); 
		_vertexList[a].y = _vertexList[a].y + (transValue.y * w); 
	}
}

void MeshDeformer::DeformMesh(double x, double y)
{
	if(this->_pseudoEdges->_handleIndex1 < 0 || this->_whatDo != DO_DEFORMATION) { return; }

	this->_isDeforming = true;
	CVSystem::MyPoint curHandle = this->_vertexList[this->_pseudoEdges->_handleIndex1];
	double dist = curHandle.Distance(x, y);
	if(dist > -M_EPS && dist < M_EPS ) { return; }

	bool isPartOfPE = _pseudoEdges->IsPartOfPseudoEdge(_pseudoEdges->_handleIndex1, _pseudoEdges->_handleIndex2);
	if(isPartOfPE) { DeformPseudoEdge(x, y); }
	else { DeformTranslation(x, y); }
	
	if(_scTriangles.size() > 0)
		{ _scRenderer->UpdateVertices(_vertexList); }
}

/*
 * index1	index referencing _vertexList
 * index2	index referencing _handleIndices
 */
void MeshDeformer::GetIndices(float x, float y, int& index1, int& index2)
{
	double dist = DBL_MAX;
	index1 = -1;
	index2 = -1;
	for(int a = 0; a < this->_pseudoEdges->_handleIndices.size(); a++)
	{
		double d = _vertexList[_pseudoEdges->_handleIndices[a]].Distance(x, y);
		if(d < dist && d < _maxHandleDist)
		{
			index1 = _pseudoEdges->_handleIndices[a];
			index2 = a;
			dist = d;
		}
	}
}

void MeshDeformer::DeleteHandles()
{
	if(this->_whatDo == DO_ADD_HANDLE)
	{	
		_pseudoEdges->DeleteHandles();
	}
}

bool MeshDeformer::BindReady()
{
	if(_vertexList.size() > 0 && _indexedTriangles.size() > 0 && _pseudoEdges->_handleIndices.size() > 0)
	{ return true; }
	return false;
}

void  MeshDeformer::SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, double _zoom_factor)
{
	this->_x_offset     = _x_offset;
	this->_y_offset     = _y_offset;
	this->_frame_width  = _frame_width;
	this->_frame_height = _frame_height;
	this->_zoom_factor  = _zoom_factor;
}

void MeshDeformer::SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves)
{
	this->_indexedBezierCurves = indexedBezierCurves;

	if(_cRendererV2) delete _cRendererV2;
	_cRendererV2 = new CurveRenderer2();
	_cRendererV2->Init();
	_cRendererV2->SetBezierCurves(_indexedBezierCurves, _vertexList);
}

void MeshDeformer::SetTriangles( std::vector<CVSystem::MyPoint>			vertexList,
							      std::vector<CVSystem::MyIndexedTriangle>	indexedTriangles,
							      std::vector<CVSystem::MyIndexedTriangle>	borderSCTriangles,
							      std::vector<CVSystem::MyIndexedTriangle>	borderWTriangles,
							      std::vector<CVSystem::MyIndexedTriangle>	scTriangles,
							      std::vector<CVSystem::MyIndexedTriangle>	wTriangles,
							      std::vector<CVSystem::MyIndexedTriangle>	bTriangles)
{
	using namespace CVSystem;

	this->_vertexList		 = vertexList;
	this->_indexedTriangles  = indexedTriangles;
	this->_borderSCTriangles = borderSCTriangles;
	this->_borderWTriangles  = borderWTriangles;
	this->_scTriangles       = scTriangles;
	this->_wTriangles        = wTriangles;
	this->_bTriangles        = bTriangles;

	BuildVertexKDTree();
}

void MeshDeformer::SetScreentoneSystem(std::vector<double> params, int img_width, int img_height)
{
	if(_scRenderer) delete _scRenderer;
	_scRenderer = new ScreentoneRenderer();
	_scRenderer->SetTriangles(_vertexList, _scTriangles, _borderSCTriangles);
	_scRenderer->SetParams(params);
	_scRenderer->SetVariables(img_width, img_height);

	if(_lightingEffect) {delete _lightingEffect;}
	_lightingEffect = new LightingEffect();
	_lightingEffect->SetUISystem(img_width, img_height);
}
