
/**
 * Class needed to create gradation mask for Lighting effect
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Lighting_Effect__
#define __Lighting_Effect__

#include <vector>

#include "MyPoint.h"
#include "MyIndexedTriangle.h"

class LightingEffect
{
public:
	// constructor
	LightingEffect();

	// destructor
	~LightingEffect();

	// mouse press
	void mousePressEvent(int x, int y);
	// mouse move
	void mouseMoveEvent(int x, int y);
	// mouse release
	void mouseReleaseEvent(int x, int y);

	// draw UI
	void paintGL();
	// init things
	void Init();

	// set UI texture
	void SetUISystem(int _img_width, int _img_height);
	// set rendering params
	void SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, /*int _img_width, int _img_height,*/ double _zoom_factor);

	// change lighting strength
	void IncreaseThickness() { _thicknessFactor += 0.05;}
	void DecreaseThickness() { _thicknessFactor -= 0.05;}
	// get current thickness
	double GetThickness() { return _thicknessFactor;}

	// Ui texture
	QImage _imgOriginal;
	QImage _imgGL;
	GLuint _imgID;

private:
	// draw light icon (praise the sun !)
	void DrawLightIcon(GLuint texId, double width, double height, double xPos, double yPos);
	// draw a texture
	void DrawTexture(GLuint texId, double width, double height);

private:
	// icon position in (x, y)
	CVSystem::MyPoint _iconPosition;

	double _scaleFactor;		// current scaling
	double _thicknessFactor;	// current thickness

	int    _img_width;		// image witdh
	int    _img_height;		// image height

	std::vector<CVSystem::MyPoint>			 _vertexList;			// vertex data
	std::vector<CVSystem::MyIndexedTriangle> _borderSCTriangles;	// triangles in border
	std::vector<CVSystem::MyIndexedTriangle> _scTriangles;			// screentone triangles
	
	// rendering params
	int    _x_offset;		// x scroll offset
	int    _y_offset;		// y scroll offset
	int    _frame_width;	// display width
	int    _frame_height;	// display height
	double _zoom_factor;	// zoom factor

	bool _mouseDown;		// mouse pressed ?

	// lighting icon
	QImage _imgIconOriginal;
	QImage _imgIconGL;
	GLuint _imgIconID;
};

#endif