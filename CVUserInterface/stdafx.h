
#pragma once

// Qt OpenGL
#include <QtOpenGL/QGLWidget>
#include <QGLShader>
#include <QGLFramebufferObject>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtGui/QMouseEvent>

#include <QAbstractScrollArea>
#include <QColor>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QScrollArea>
#include <QStyle>
#include <QTimer>
#include <QWidget>

#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <sstream>
#include <cfloat>
#include <ctime>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <GL/glu.h>
#include <GL/glut.h>

// KD Tree (somehow this should be put in stdafx.h)
#include "NanoFLANN.h"

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Eigen for matrix operation
#include <Eigen/Dense>
#include <Eigen/Geometry>

#ifndef M_EPS
#define M_EPS		1e-8
#endif

//#ifndef __Max_Min__
//#define __Max_Min__
#define my_min(a,b) ((a)<(b) ? (a) : (b))
#define my_max(a,b) ((a)>(b) ? (a) : (b))
//#endif

//#ifndef __CV_System_Data_Types__
//#define __CV_System_Data_Types__

//#include "MyCorner.h"
#include "MyCurveTriangle.h"
#include "MyIndexedBezierCurves.h"
#include "MyIndexedLine.h"
#include "MyIndexedTriangle.h"
#include "MyLine.h"
#include "MyPoint.h"
#include "MyQuad.h"
#include "MyTriangle.h"

//#endif // !__CV_System__



