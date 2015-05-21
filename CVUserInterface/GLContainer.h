
/**
 * OpenGL rendering container
 * Is responsible for:
 *		- keyboard input
 *		- mouse input
 *		- scrolling
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __GL_Container__
#define __GL_Container__

#include <vector>
#include <QScrollBar>
#include "GLWidget.h"

// forward declaration
class MeshDeformer;	
class LightingEffect;

class GLContainer : public QAbstractScrollArea
{
	Q_OBJECT

public:
	// constructor
	GLContainer(QWidget *parent = 0);
	
	// set bitmap image to be displayed
	void SetImage(QString image);

	// for Qt internal use
	void setWidget(QWidget *widget);
	QWidget *widget() const;	

	// get renderer
	GLWidget* GetGLWidget() { return this->glWidget; }

	// set up scrolls (vertical and horizontal)
	void SetScrolls();

protected:
	// global event
	bool event( QEvent * event );

	// draw
	void paintEvent(QPaintEvent *event);

	// mouse events
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	// mouse wheel
	void wheelEvent(QWheelEvent* event); 

	// keyboard
	void keyPressEvent(QKeyEvent *event);  
	void keyReleaseEvent(QKeyEvent *event);

private:
	GLWidget* glWidget;	
	QPoint prevScrollPos;
	QPoint prevMousePos;
	bool ctrlPressed;
	bool mousePressed;

	int sWidth;
	int sHeight;
	QPoint mousePos;

	// for updating opengl when scrolling display
	float xPrevF;
	float yPrevF;	
	float prevZoomFactor;

	// previous number of strokes (not used)
	int prevNum;

	// timer for double click
	QTimer* doubleClickTimer;

	// timing for double click
	int     doubleClickTimeout;

private:
	// update opengl viewport
	void UpdateViewport(bool putInMiddle = false);

	// number of drawing strokes (not used)
	int NumStrokes();

signals:
	// send message to UI (bottom left)
	void SendMessage(QString msg);

	// drawing stroke is released (not used)
	void StrokeRelease(); 

	// tell system to update
	void UpdateThings();

private slots:
	// vertical scroll
	void VScrollChanged(int val);

	// horizontal scroll
	void HScrollChanged(int val);

	// for double click effect
	void DummyFunction();
};

#endif