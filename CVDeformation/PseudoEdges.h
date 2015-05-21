
/**
 * Implementation of handles and pseudo edges
 * Please read "Jacobson 2011" for more detail
 *
 * Jacobson, A.; Baran, I.; Popović, J. & Sorkine, O. 
 * Bounded Biharmonic Weights for Real-Time Deformation 
 * ACM Transactions on Graphics (proceedings of ACM SIGGRAPH), 2011, 30, 78:1-78:8
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Pseudo_Edges__
#define __Pseudo_Edges__

#include <vector>
#include "MyPoint.h"

typedef std::pair<int, int> PE_Pair_Index;


class PseudoEdges
{
public:
	
	// Constructor
	PseudoEdges();

	// Destructor
	~PseudoEdges();	

	// Set up candidate indices
	void SetCandidateIndices(int index1, int index2);

	// Add a pseudo edge
	void AddEdge(double x, double y, std::vector<CVSystem::MyPoint> vertexList);

	// Get neighboring indices within a pseudo edge
	void GetNeighborIndices(int index1, int index2, int& prevIdx, int& nextIdx);

	// Get neighbor points within a pseudo edge
	void GetNeighborPoints(std::vector<CVSystem::MyPoint> vertexList, 
						   int index1, 
						   int index2, 
						   CVSystem::MyPoint& curPt, 
						   CVSystem::MyPoint& prevPt, 
						   CVSystem::MyPoint& nextPt);

	// Check given indices whether belong to any pseudo edge
	bool IsPartOfPseudoEdge(int index1, int index2);

	// Get candidate index1
	int GetCandidateIndex1(){return _candidateIndex1;}

	// Get candidate index2
	int GetCandidateIndex2(){return _candidateIndex2;}

	// Invalidate candidate indices (assign them -1)
	void InvalidateCandidateIndices()
	{
		_candidateIndex1 = -1;
		_candidateIndex2 = -1;
	};

	// Delete handles (press backspace)
	void DeleteHandles();

	// Get index2 from input index1
	int GetSecondHandleIndex(int index1);

	// Check whether an index1 is a handle or not
	bool DoesHandleExist(int index);

	// allocate pair list
	void UpdatePairIndex();

public:
	CVSystem::MyPoint			  _peCurrentPoint;	// draw purpose

	std::vector<int>			  _handleIndices;		// index list referencing _vertexList 	
	std::vector<std::vector<int>> _peList;			// index list referencing _vertexList 	
	std::vector<PE_Pair_Index>	  _pePairIndexList;	// index list referencing _peList  

	int	_handleIndex1;
	int	_handleIndex2;

	int	_candidateIndex1;		// candidate indices referencing _vertexList
	int	_candidateIndex2;		// candidate index referencing _handleIndices	
	
};

#endif