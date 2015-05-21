
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PseudoEdges.h"

PseudoEdges::PseudoEdges() :
	_candidateIndex1(-1),
	_candidateIndex2(-1)
{
}

PseudoEdges::~PseudoEdges()
{
}

void PseudoEdges::SetCandidateIndices(int index1, int index2)
{
	// We don't like branching
	for(size_t a = 0; a < _peList.size(); a++)
	{
		std::vector<int> pEdgeTemp = _peList[a];
		for(size_t b = 0; b < pEdgeTemp.size() - 1; b++)
			{ if(index1 == pEdgeTemp[b]) { return; } }
	}
	_candidateIndex1 = index1;
	_candidateIndex2 = index2;
}

bool PseudoEdges::IsPartOfPseudoEdge(int index1, int index2)
{
	if(_pePairIndexList.size() == 0) {return false;}

	int vectorIdx1   = _pePairIndexList[index2].first;
	//int vectorIdx2 = _peIndices[index2].second;
	if(vectorIdx1 == -1) {return false;}
	return true;
}

//void PseudoEdges1::UpdateHandlePosition(std::vector<CVSystem::MyPoint> vertexList, int index1, int index2, double x, double y)
//{
	/*using namespace CVSystem;

	if(_pePairIndexList.size() == 0) {return;}

	int vectorIdx1 = _pePairIndexList[index2].first;
	int vectorIdx2 = _pePairIndexList[index2].second;

	if(vectorIdx1 == -1) {return;}

	_pePointList[vectorIdx1][vectorIdx2] = MyPoint(x, y);*/
//}

void PseudoEdges::GetNeighborIndices(int idx1, int idx2, int& prevIdx, int& nextIdx)
{
	int vectorIdx1 = _pePairIndexList[idx2].first;
	int vectorIdx2 = _pePairIndexList[idx2].second;
	prevIdx = -1; nextIdx = -1;

	if(vectorIdx1 == -1) {return;}

	std::vector<int> pEdgeTemp = _peList[vectorIdx1];
	if(vectorIdx2 > 0) { prevIdx = pEdgeTemp[vectorIdx2 - 1]; }
	if(vectorIdx2 < pEdgeTemp.size() - 1) { nextIdx = pEdgeTemp[vectorIdx2 + 1]; }
}

void PseudoEdges::GetNeighborPoints(std::vector<CVSystem::MyPoint> vertexList, int idx1, int idx2, CVSystem::MyPoint& curPt, CVSystem::MyPoint& prevPt, CVSystem::MyPoint& nextPt)
{
	using namespace CVSystem;

	if(_pePairIndexList.size() == 0) {return;}

	int vectorIdx1 = _pePairIndexList[idx2].first;
	int vectorIdx2 = _pePairIndexList[idx2].second;

	if(vectorIdx1 == -1) {return;}

	std::vector<int> pe = _peList[vectorIdx1];
	curPt = vertexList[ pe[vectorIdx2] ];
	if(vectorIdx2 > 0) { prevPt = vertexList[ pe[vectorIdx2 - 1] ]; }
	if(vectorIdx2 < pe.size() - 1) { nextPt = vertexList[ pe[vectorIdx2 + 1] ]; }

	//std::vector<MyPoint> pePointsTemp = _pePointList[vectorIdx1];
	//curPt = pePointsTemp[vectorIdx2];
	//if(vectorIdx2 > 0) { prevPt = pePointsTemp[vectorIdx2 - 1]; }
	//if(vectorIdx2 < pePointsTemp.size() - 1) { nextPt = pePointsTemp[vectorIdx2 + 1]; }
}

void PseudoEdges::UpdatePairIndex()
{
	//// update from empty
	if(_pePairIndexList.size() == 0)
	{
		for(size_t a = 0; a < _handleIndices.size(); a++)
			{ _pePairIndexList.push_back(PE_Pair_Index(-1, -1)); }
	}

	//// update from existing
	if(_pePairIndexList.size() != _handleIndices.size())
	{
		//// Add
		int deltaSize1 = _handleIndices.size() - _pePairIndexList.size();
		for(size_t a = 0; a < deltaSize1; a++)
			{ _pePairIndexList.push_back(PE_Pair_Index(-1, -1)); }
	}
}

void PseudoEdges::AddEdge(double x, double y, std::vector<CVSystem::MyPoint> vertexList)
{
	//// 1. Get index of tail edge
	int retIdx1 = -1;
	int retIdx2 = -1;
	double dist = 0;
	for(size_t a = 0; a < _handleIndices.size(); a++)
	{
		CVSystem::MyPoint pt = vertexList[_handleIndices[a]];
		dist = pt.Distance(CVSystem::MyPoint(x, y));
		if(dist < 50)
		{ 
			retIdx1 = _handleIndices[a];
			retIdx2 = a;
			break; 
		}
	}

	//// 2. Cannot find any handle
	if(retIdx1 == -1) 
	{
		//std::cout << dist << "\n";
		return;
	}

	////// 3. initialization from empty
	//if(_pePairIndexList.size() == 0)
	//{
	//	for(size_t a = 0; a < _handleIndices.size(); a++)
	//		{ _pePairIndexList.push_back(PE_Pair_Index(-1, -1)); }
	//}

	////// 3. initialization from existing
	//if(_pePairIndexList.size() != _handleIndices.size())
	//{
	//	//// Add
	//	int deltaSize1 = _handleIndices.size() - _pePairIndexList.size();
	//	for(size_t a = 0; a < deltaSize1; a++)
	//		{ _pePairIndexList.push_back(PE_Pair_Index(-1, -1)); }
	//}
		
	//// 4. Where I am ?
	int vectorIdx = _pePairIndexList[_candidateIndex2].first;

	//// 5. No cycle !
	if(vectorIdx != -1)
	{
		std::vector<int> pEdgeTemp = _peList[vectorIdx];
		for(size_t a = 0; a < pEdgeTemp.size(); a++)
			{ if(retIdx1 == pEdgeTemp[a]) { return; } }
	}

	//// 6. Don't connect two pseudo edges
	if(_pePairIndexList[_candidateIndex2].first != _pePairIndexList[retIdx2].first &&  
	   _pePairIndexList[_candidateIndex2].first != -1 &&
	   _pePairIndexList[retIdx2].first != -1)
		{ return; }

	//// 7. Create new edge
	if(vectorIdx == -1)
	{
		std::cout << "add pseudo edge 1\n";

		int pEdgesSize = _peList.size();
		std::vector<int> pSkeletonTemp;
		std::vector<CVSystem::MyPoint> pePointsTemp;
		
		pSkeletonTemp.push_back(_candidateIndex1);	// parent
		pSkeletonTemp.push_back(retIdx1);		// child

		pePointsTemp.push_back(vertexList[_candidateIndex1]);
		pePointsTemp.push_back(vertexList[retIdx1]);

		_peList.push_back(pSkeletonTemp);
		//_pePointList.push_back(pePointsTemp);

		_pePairIndexList[_candidateIndex2].first = pEdgesSize;
		_pePairIndexList[_candidateIndex2].second = 0;

		_pePairIndexList[retIdx2].first = pEdgesSize;
		_pePairIndexList[retIdx2].second = 1;
	}

	//// 8. Add point to an existing edge
	else
	{
		std::cout << "add pseudo edge 2\n";

		std::vector<int>* pEdgeTemp = &_peList[vectorIdx];
		//std::vector<CVSystem::MyPoint>* pePointsTemp = &_pePointList[vectorIdx];

		int pEdgeTempSize = pEdgeTemp->size();

		pEdgeTemp->push_back(retIdx1);
		//pePointsTemp->push_back(vertexList[retIdx1]);

		_pePairIndexList[retIdx2].first = vectorIdx;
		_pePairIndexList[retIdx2].second = pEdgeTempSize;		
	}

	//// Invalidate
	_candidateIndex1 = -1;
	_candidateIndex2 = -1;
}

void PseudoEdges::DeleteHandles()
{
	//std::vector<int>				_handleIndices;		// ~~~ index list referencing _vertexList ~~~	
	//std::vector<std::vector<int>>	_peList;			// ~~~ index list referencing _vertexList ~~~	
	//std::vector<PE_Pair_Index>	_pePairIndexList;	// ~~~ index list referencing _peList ~~~ 

	int deletedIndex = _handleIndices[_handleIndices.size() - 1];

	if(_handleIndices.size() > 0)
	{
		_handleIndices.pop_back(); 
		_pePairIndexList.pop_back();
		//this->_whatDo = DO_ADD_HANDLE;
	}

	if(_peList.size() > 0)
	{
		std::vector<std::vector<int>>::iterator peListIter = _peList.begin();
		for(int a = _peList.size() - 1; a >= 0; a--)
		{
			int eIndex = _peList[a][_peList[a].size() - 1];
			if(eIndex == deletedIndex)
			{
				_peList[a].pop_back();
				if(_peList[a].size() < 2)
				{
					_peList.erase(peListIter + a);
				}
				break;
			}
		}

	}

	for(size_t a = 0; a < _pePairIndexList.size(); a++)
	{
		if(_pePairIndexList[a].first >= _peList.size())
		{
			_pePairIndexList[a].first = -1;
			_pePairIndexList[a].second = -1;
		}
	}

	std::cout << "_handleIndices size: " << _handleIndices.size() << "\n";
	std::cout << "_peList size: " << _peList.size() << "\n";
	std::cout << "_pePairIndexList size: " << _pePairIndexList.size() << "\n";
	std::cout << "\n";
}

int PseudoEdges::GetSecondHandleIndex(int index1)
{
	for(int a = 0; a < _handleIndices.size(); a++)
	{
		if(index1 == _handleIndices[a]) 
		{ return a; }
	}
	return -1;
}

bool PseudoEdges::DoesHandleExist(int index)
{
	for(int a = 0; a < _handleIndices.size(); a++)
		{ if(_handleIndices[a] == index) {return true;} }
	return false;
}




