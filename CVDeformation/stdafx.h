// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// LIBIGL http://igl.ethz.ch/projects/libigl/
#include <igl/pathinfo.h>
#include <igl/readOBJ.h>
#include <igl/readOFF.h>
#include <igl/readMESH.h>
#include <igl/tetgen/mesh_with_skeleton.h>
#include <igl/faces_first.h>
#include <igl/readTGF.h>
#include <igl/launch_medit.h>
#include <igl/boundary_conditions.h>
#include <igl/bbw/bbw.h>
#include <igl/writeDMAT.h>
#include <igl/writeMESH.h>
#include <igl/normalize_row_sums.h>

// Eigen
#include <Eigen/Dense>

// C++
#include <iostream>
#include <string>

// OpenGL
#include <QtOpenGL/QGLWidget>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// KD Tree
#include "NanoFLANN.h"

#ifndef M_EPS
#define M_EPS		1e-8
#endif
