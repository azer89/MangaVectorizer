
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "GLContainer.h"

#include "LightingEffect.h"
//#include "MeshDeformer.h"
#include "DeformationManager.h"
#include "PartExtractor.h"

GLContainer::GLContainer(QWidget *parent) : 
	QAbstractScrollArea (parent),
	ctrlPressed(false),
	mousePressed(false),
	sWidth(20),
	sHeight(20),
	prevNum(-1)
{
	QGLFormat format;
	format.setVersion(2, 0);
	format.setProfile(QGLFormat::CompatibilityProfile);
	format.setSampleBuffers(true);

	glWidget = new GLWidget(format);
	glWidget->setObjectName(QStringLiteral("myGLImageDisplay"));

	setViewport(glWidget);

	horizontalScrollBar()->setSingleStep(10);
	horizontalScrollBar()->setPageStep(100);
	horizontalScrollBar()->setVisible(false);

	verticalScrollBar()->setSingleStep(10);
	verticalScrollBar()->setPageStep(100);
	verticalScrollBar()->setVisible(false);

	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(HScrollChanged(int)));
	connect(verticalScrollBar(),   SIGNAL(valueChanged(int)),this, SLOT(VScrollChanged(int)));

	setMouseTracking(true);

	doubleClickTimer = new QTimer(this); connect(doubleClickTimer, SIGNAL(timeout()), this, SLOT(DummyFunction()));
	doubleClickTimeout = 100;
}

void GLContainer::UpdateViewport(bool putInMiddle)
{
	QSize barSize = QSize(this->width() - sWidth, this->height() - sHeight);

	float zoomFactor = glWidget->GetZoomFactor();
	QSize imageSize  = glWidget->GetImageSize();

	QSize borderLimit( 10, 10);

	int img_width =	imageSize.width()  * zoomFactor;
	int img_height = imageSize.height() * zoomFactor;

	float xSPos;
	float ySPos;
	if(!putInMiddle)
	{
		float xNormPos = mousePos.x() + xPrevF;
		float yNormPos = mousePos.y() + yPrevF;
		xNormPos /= prevZoomFactor;
		yNormPos /= prevZoomFactor;

		float xRev = xNormPos * zoomFactor;
		float yRev = yNormPos * zoomFactor;
		xSPos = xRev - mousePos.x();
		ySPos = yRev - mousePos.y();
	}		

	int leftRange = 0;
	int rightRange = 0;
	int upRange = 0;
	int downRange = 0;

	float hPos = 0;
	float vPos = 0;

	int xGap = abs(barSize.width() - img_width);
	int yGap = abs(barSize.height() - img_height);

	if(img_width <= barSize.width())
	{
		if(putInMiddle)hPos = -xGap * 0.5; //
		else hPos = xSPos;

		leftRange = -img_width - xGap;
		rightRange = img_width;
		
	}
	else
	{
		if(putInMiddle) hPos = xGap * 0.5;
		else hPos = xSPos;
		leftRange = -img_width + xGap;
		rightRange = img_width;		
	}

	if(img_height <= barSize.height())
	{
		if(putInMiddle) vPos = -yGap * 0.5;
		else vPos = ySPos;

		upRange =  -img_height -yGap;
		downRange = img_height;
		
	}
	else
	{
		if(putInMiddle) vPos = yGap * 0.5;
		else vPos = ySPos;
		upRange = -img_height + yGap;
		downRange = img_height;		
	}

	xPrevF = hPos;
	yPrevF = vPos;

	horizontalScrollBar()->setRange(leftRange, rightRange);
	verticalScrollBar()->setRange(upRange, downRange);
	
	horizontalScrollBar()->setSliderPosition(hPos);
	verticalScrollBar()->setSliderPosition(vPos);
}

void GLContainer::paintEvent(QPaintEvent *event)
{
	QAbstractScrollArea::paintEvent(event);
	this->viewport()->update();
	glWidget->updateGL();	
}

bool GLContainer::event(QEvent * event)
{
	bool evResult = QAbstractScrollArea::event(event);
	if(event->type() == QEvent::Resize) UpdateViewport(true);
	return evResult;
}

void GLContainer::SetScrolls()
{
	horizontalScrollBar()->setVisible(true);
	verticalScrollBar()->setVisible(true);

	prevZoomFactor = 1.0f;

	// nasty code here...
	bool shouldZoom = true;
	do 
	{
		int _w = this->width();
		int _h = this->height();
		QSize imgSize = glWidget->GetImageSize();
		double zoomFactor = glWidget->GetZoomFactor();

		if((double)this->width()  < (double)imgSize.width() * zoomFactor ||  (double)this->height() < (double)imgSize.height() *zoomFactor)
		{ glWidget->ZoomOut(); }
		else
		{ shouldZoom = false; }
	} while (shouldZoom);
	UpdateViewport(true);
}

void GLContainer::SetImage(QString image)
{
	glWidget->SetImage(image);	

	this->SetScrolls();
}

void GLContainer::VScrollChanged(int val)
{
	yPrevF = val;
	glWidget->VerticalScroll(val);
}

void GLContainer::HScrollChanged(int val)
{
	xPrevF = val;
	glWidget->HorizontalScroll(val);
}

void GLContainer::DummyFunction()
{
	doubleClickTimer->stop();
}

void GLContainer::mousePressEvent(QMouseEvent *event)
{
	if(doubleClickTimer->isActive())
	{
		doubleClickTimer->stop();
		glWidget->mouseDoubleClick(event->x(), event->y());
		//glWidget->GetPartExtractor()->DoPointLocation(event->x(), event->y());
	}
	else
	{
	}

	this->mousePressed = true;

	if(!this->ctrlPressed)		
	{ 
		glWidget->mousePressEvent(event->x(), event->y()); 
	}

	if(this->ctrlPressed)
	{
		this->prevMousePos = mousePos;
		this->prevScrollPos.setX(horizontalScrollBar()->sliderPosition());
		this->prevScrollPos.setY(verticalScrollBar()->sliderPosition());
	}

	emit UpdateThings();
}

void GLContainer::mouseMoveEvent(QMouseEvent *event)
{
	mousePos.setX(event->x());
	mousePos.setY(event->y());

	if(this->ctrlPressed && this->mousePressed)
	{
		int xDelta = mousePos.x() - prevMousePos.x();
		int yDelta = mousePos.y() - prevMousePos.y();
		horizontalScrollBar()->setSliderPosition(prevScrollPos.x() - xDelta);
		verticalScrollBar()->setSliderPosition(prevScrollPos.y() - yDelta);
	}

	glWidget->mouseMoveEvent(event->x(), event->y());
}

void GLContainer::mouseReleaseEvent(QMouseEvent *event)
{
	this->mousePressed = false;
	if(!this->ctrlPressed)
	{
		glWidget->mouseReleaseEvent(event->x(), event->y());
	}

	doubleClickTimer->start(doubleClickTimeout);	

	//emit StrokeRelease();
}

void GLContainer::wheelEvent(QWheelEvent* event)
{
	//QSize imgSize = glWidget->GetImageSize();
	//if(imgSize.width() == 0 && imgSize.height() == 0) return;
	
	bool scrollDir = (event->delta() > 0) ? true : false;	// negative means scroll down, positive is otherwise
	prevZoomFactor = glWidget->GetZoomFactor();		// for anchor zoom

	if(scrollDir) glWidget->ZoomOut();
	else glWidget->ZoomIn();

	float zoomFactor = glWidget->GetZoomFactor() * 100.0;
	emit SendMessage("Zoom: " + QString::number(zoomFactor) + "%");

	// update scrollbars
	UpdateViewport();
}

void GLContainer::keyPressEvent(QKeyEvent *event)
{
	//// draw mode
	if(event->key() == Qt::Key_1)      
		{ glWidget->SetDrawMode(DRAW_RASTER); emit SendMessage("Raster image"); }

	else if(event->key() == Qt::Key_2)
		{ glWidget->SetDrawMode(DRAW_VECTOR); emit SendMessage("Vector image"); }
	
	else if(event->key() == Qt::Key_3)
		{ glWidget->SetDrawMode(DRAW_DEFORMATION); emit SendMessage("Mesh deformation"); }
	
	else if(event->key() == Qt::Key_4)
		{ glWidget->SetDrawMode(DRAW_PART_EXTRACTION); emit SendMessage("Part completion"); }
	
	else if(event->key() == Qt::Key_5)
		{ glWidget->SetDrawMode(DRAW_LIGHTING_EFFECT); emit SendMessage("Lighting effect"); }
	
	if(event->key() == Qt::Key_C && glWidget->GetDrawMode() == DRAW_PART_EXTRACTION) 
	{ 
		emit SendMessage("Point match");
		this->glWidget->GetPartExtractor()->DoNonRigidCorrespondence(); 
	}
	else if(event->key() == Qt::Key_Backspace && glWidget->GetDrawMode() == DRAW_PART_EXTRACTION) 
		{ this->glWidget->GetPartExtractor()->DoUndo();  }
	else if(event->key() == Qt::Key_X && glWidget->GetDrawMode() == DRAW_PART_EXTRACTION) 
		{ this->glWidget->GetPartExtractor()->SwapStrokeType(); }
	else if(event->key() == Qt::Key_Shift && glWidget->GetDrawMode() == DRAW_PART_EXTRACTION) 
		{ this->glWidget->GetPartExtractor()->SetShiftButtonState(true); }

	if(event->key() == Qt::Key_Control) { this->ctrlPressed = true; }	
	else if(event->key() == Qt::Key_Tab)  { this->glWidget->CreateNewStrokeSegment(); }	
	else if(event->key() == Qt::Key_Backspace && glWidget->GetDrawMode() == DRAW_RASTER)  { this->glWidget->DeleteSegment(); }
	
	// mesh deform fix
	//else if(event->key() == Qt::Key_Backspace && glWidget->GetDrawMode() == DRAW_DEFORMATION)  { this->glWidget->GetMeshDeformer()->DeleteHandles(); }
	else if(event->key() == Qt::Key_Backspace && glWidget->GetDrawMode() == DRAW_DEFORMATION)  { this->glWidget->GetDeformManager()->DeleteHandles(); }

	if(event->key() == Qt::Key_BracketLeft && glWidget->GetDrawMode() == DRAW_LIGHTING_EFFECT)
		{ glWidget->GetLightingEffect()->DecreaseThickness(); }
	else if(event->key() == Qt::Key_BracketRight && glWidget->GetDrawMode() == DRAW_LIGHTING_EFFECT)
		{ glWidget->GetLightingEffect()->IncreaseThickness(); }

	if(event->key() == Qt::Key_BracketLeft && glWidget->GetDrawMode() == DRAW_DEFORMATION)
	{
		//std::cout << "test1\n";
		glWidget->GetDeformManager()->DecreaseThickness(); 
	}
	else if(event->key() == Qt::Key_BracketRight && glWidget->GetDrawMode() == DRAW_DEFORMATION)
	{ 
		//std::cout << "test2\n";
		glWidget->GetDeformManager()->IncreaseThickness(); 
	}

	//// for pseudo edge
	// mesh deform fix
	/*if(event->key() == Qt::Key_Shift)
	{ this->glWidget->GetMeshDeformer()->SetShiftButton(true); }*/
	if(event->key() == Qt::Key_Shift)
	{ this->glWidget->GetDeformManager()->SetShiftButton(true); }
	

	//// Recheck number of strokes
	this->prevNum = NumStrokes();

	emit UpdateThings();

	glWidget->updateGL();

	//else if(event->key() == Qt::Key_T)
	//	emit StrokeRelease(this->glWidget->GetStrokes());
}

void GLContainer::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Control) { this->ctrlPressed = false; }
	
	// mesh deform fix
	//if(event->key() == Qt::Key_Shift) { this->glWidget->GetMeshDeformer()->SetShiftButton(false); }
	if(event->key() == Qt::Key_Shift) { this->glWidget->GetDeformManager()->SetShiftButton(false); }

	if(event->key() == Qt::Key_Shift && glWidget->GetDrawMode() == DRAW_PART_EXTRACTION) 
		{ this->glWidget->GetPartExtractor()->SetShiftButtonState(false); }
}

int GLContainer::NumStrokes()
{
	int num = 0;
	std::vector<std::vector<CVSystem::MyLine>> lines = this->glWidget->GetStrokes();
	for(size_t a = 0; a < lines.size(); a++)
	{
		for(size_t b = 0; b < lines[a].size(); b++)
			{ num++;} 
	}
	return num;
}
