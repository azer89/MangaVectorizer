
/**
 * Main user interface
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CVUserInterface.h"

#include "ScreentoneSegmentation.h"
#include "ScreentoneClassifier.h"
#include "Triangulator1.h"
#include "Triangulator2.h"
#include "MeshDeformer.h"
#include "LightingEffect.h"
#include "PartExtractor.h"
#include "IUI_StrokeType.h"
#include "DeformationManager.h"

#include "SystemParams.h"

CVUserInterface::CVUserInterface(QWidget *parent)
	: QMainWindow(parent),
	  scSegm(0),
	  scClassifier(0),
	  triangulator1(0),
	  triangulator2(0)
{
	ui.setupUi(this);

	this->showMaximized();

	//ui.sbLSTolerance->setValue(0.001);
	//ui.sbLSDistanceTime->setValue(0.0001);
	//ui.sbLSTolerance->setSingleStep(0.001);
	//ui.sbLSDistanceTime->setSingleStep(0.0001);

	ui.saveFileButton->setEnabled(false);
	ui.lsSegmButton->setEnabled(false);
	ui.ctSegmButton->setEnabled(false);
	ui.sVectorizeButton->setEnabled(false);
	ui.triangulationButton->setEnabled(false);
	ui.bindHandlesButton->setEnabled(false);
	ui.extractPartButton->setEnabled(false);
	ui.partsTriangulationButton->setEnabled(false);

	ui.gaborButton->setEnabled(false);
	ui.levelSetButton->setEnabled(false);
	ui.segmentationButton->setEnabled(false);
	ui.cartoonTextureButton->setEnabled(false);
	ui.renderingParamsButton->setEnabled(false);
	ui.triangulationParamsButton->setEnabled(false);
	ui.partExtractionParamsButton->setEnabled(false);

	ui.gaborParamsDock->hide();
	ui.levelSetParamsDock->hide();
	ui.segmentationParamsDock->hide();
	ui.cartoonTextureParamsDock->hide();
	ui.renderingParamsDock->hide();
	ui.triangulationParamsDock->hide();
	ui.partExtractorParamsDock->hide();

	//ui.cartoonTextureButton->installEventFilter(this);

	connect(ui.gaborButton,					SIGNAL(triggered()), this, SLOT(DisplayGaborParameters()));
	connect(ui.levelSetButton,				SIGNAL(triggered()), this, SLOT(DisplayLevelSetParameters()));
	connect(ui.segmentationButton,			SIGNAL(triggered()), this, SLOT(DisplaySegmentationParameters()));
	connect(ui.cartoonTextureButton,		SIGNAL(triggered()), this, SLOT(DisplayCartoonTextureParams()));
	connect(ui.renderingParamsButton,		SIGNAL(triggered()), this, SLOT(DisplayRenderingParameters()));
	connect(ui.triangulationParamsButton,	SIGNAL(triggered()), this, SLOT(DisplayTriangulationParams()));
	connect(ui.bindHandlesButton,			SIGNAL(triggered()), this, SLOT(DoHandlesBinding()));
	connect(ui.partExtractionParamsButton,	    SIGNAL(triggered()), this, SLOT(DisplayPartExtractionParams()));

	connect(ui.openFileButton,				SIGNAL(triggered()), this, SLOT(OpenFileAction()));
	connect(ui.openMyObjButton,				SIGNAL(triggered()), this, SLOT(OpenMYOBJFileAction()));
	connect(ui.saveFileButton,				SIGNAL(triggered()), this, SLOT(SaveFileAction()));
	connect(ui.lsSegmButton,				SIGNAL(triggered()), this, SLOT(DoLSSegmentation()));
	connect(ui.ctSegmButton,				SIGNAL(triggered()), this, SLOT(DoCTSegmentation()));
	connect(ui.triangulationButton,			SIGNAL(triggered()), this, SLOT(DoTriangulation()));
	connect(ui.sVectorizeButton,			SIGNAL(triggered()), this, SLOT(DoScreentoneVectorization()));
	connect(ui.extractPartButton,			SIGNAL(triggered()), this, SLOT(DoExtractPart()));
	connect(ui.partsTriangulationButton,	SIGNAL(triggered()), this, SLOT(DoPartsTriangulation()));
	connect(ui.frame,				SIGNAL(SendMessage(QString)), this, SLOT(DisplayMessage(QString)));
	connect(ui.frame,				SIGNAL(StrokeRelease()),	  this, SLOT(DoStrokeCalculation()));
	connect(ui.frame,				SIGNAL(UpdateThings()),		  this, SLOT(GLContainerUpdate()));

	connect(ui.gpuToggleButton,			SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	connect(ui.scRegionsToggleButton,	SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	connect(ui.usScribblesToggleButton,	SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	connect(ui.wireframeToggleButton,	SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	connect(ui.bbwHandlesToggleButton,	SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	connect(ui.bbwPEToggleButton,		SIGNAL(triggered()), this, SLOT(ToggleButtonsPressed()));
	
	// Rendering
	connect(ui.sbSTAA,			SIGNAL(valueChanged(double)), this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.sbSTScale,		SIGNAL(valueChanged(double)), this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.sbSTOrientation, SIGNAL(valueChanged(int)),    this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.sbSTBlur,		SIGNAL(valueChanged(double)), this, SLOT(RenderingParamsValuesChanged()));
	//connect(ui.sbCRBlur,		SIGNAL(valueChanged(double)), this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.sbSTBlackness,	SIGNAL(valueChanged(int)),    this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.sbCRBlackness,	SIGNAL(valueChanged(int)),    this, SLOT(RenderingParamsValuesChanged()));
	connect(ui.cbSTFixed,		SIGNAL(stateChanged(int)),    this, SLOT(RenderingParamsValuesChanged()));

	// Part Extraction
	connect(ui.sbPENIter,			SIGNAL(valueChanged(int)),	  this, SLOT(PartExtrationParamsChanged()));
	connect(ui.sbPESampleDist,		SIGNAL(valueChanged(double)), this, SLOT(PartExtrationParamsChanged()));
	connect(ui.sbPESearchRadius,	SIGNAL(valueChanged(double)), this, SLOT(PartExtrationParamsChanged()));
	connect(ui.sbPEThreshold,		SIGNAL(valueChanged(double)), this, SLOT(PartExtrationParamsChanged()));

	scSegm = new CVSystem::ScreentoneSegmentation();
	scClassifier = new CVSystem::ScreentoneClassifier();

	ofTimer = new QTimer(this); connect(ofTimer, SIGNAL(timeout()), this, SLOT(OpenFileThread()));
	ofMYOBJTimer = new QTimer(this); connect(ofMYOBJTimer, SIGNAL(timeout()), this, SLOT(OpenMYOBJFileThread()));
}

CVUserInterface::~CVUserInterface()
{
	if(scSegm)		  { delete scSegm; }
	if(triangulator1) { delete triangulator1; }
	if(triangulator2) { delete triangulator2; }
}

void CVUserInterface::OpenMYOBJFileAction()
{
	ui.saveFileButton->setEnabled(true);	// Enable save
	ui.renderingParamsButton->setEnabled(true);
	ui.partExtractionParamsButton->setEnabled(true);
	//ui.extractPartButton->setEnabled(true);

	QString qFilename = QFileDialog::getOpenFileName(this, "Open Mesh", "D:\\VG_Project\\ComicsVectorizer\\CVUserInterface\\Mesh");	
	if(qFilename.isEmpty()) { return; }
	strFilename = qFilename.toStdString();
	SystemParams::str_filename = strFilename;

	ofMYOBJTimer->start(1);		
}

void CVUserInterface::OpenMYOBJFileThread()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Opening mesh...", 100000);

	// do the actual thing
	
	if(triangulator1) delete triangulator1;
	triangulator1 = new CVSystem::Triangulator1();
	triangulator1->ReadFromMYOBJ(strFilename);

	ui.frame->GetGLWidget()->SetLSTriangles(triangulator1->GetLSTriangles());	
	ui.frame->GetGLWidget()->SetTriangles(triangulator1->GetPartOffset(),
										  triangulator1->GetVertexList(), 
										  triangulator1->GetIndexedTriangles(), 
										  triangulator1->GetBorderSCTriangles(), 
										  triangulator1->GetBorderWTriangles(), 
										  triangulator1->GetSCTriangles(), 
										  triangulator1->GetWTriangles(), 
										  triangulator1->GetBTriangles());
	ui.frame->GetGLWidget()->SetIndexedBezierCurves(triangulator1->GetIndexedBezierCurves());
	ui.frame->GetGLWidget()->SetLSQuads(triangulator1->GetLSQuads());
	//ui.frame->GetGLWidget()->SetDrawMode(DRAW_PART_EXTRACTION);
	ui.frame->GetGLWidget()->SetDrawMode(DRAW_VECTOR);

	// UI
	ui.frame->GetGLWidget()->SetWidthHeight(triangulator1->GetWidthScaled(), triangulator1->GetHeightScaled());
	ui.frame->SetScrolls();

	// screentone (which actually read from file)	
	if(triangulator1->GetSCTriangles().size() > 0)
		{ DoScreentoneVectorization(); }
	
	GLContainerUpdate();

	// done
	
	ui.statusBar->showMessage("Done", 2500);
	QApplication::restoreOverrideCursor();
	ofMYOBJTimer->stop();
}

void CVUserInterface::OpenFileThread()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Opening image...", 100000);

	QString qFilename = QString::fromStdString(strFilename);

	// Display image
	ui.frame->SetImage(qFilename);
	int imgWidth = ui.frame->GetGLWidget()->GetImageSize().width();
	int imgHeight = ui.frame->GetGLWidget()->GetImageSize().height();

	scSegm->Init(strFilename, imgWidth, imgHeight);

	ui.statusBar->showMessage("Done", 2500);
	QApplication::restoreOverrideCursor();

	// mark
	hasDoneSegmentation = false;

	// stop timer
	ofTimer->stop();
}

void CVUserInterface::OpenFileAction()
{
	// Get Filename
	QString qFilename = QFileDialog::getOpenFileName(this, "Open Image", "D:\\VG_Project\\_Data\\Data011");	
	if(qFilename.isEmpty()) return;
	strFilename = qFilename.toStdString();	
	SystemParams::str_filename = strFilename;
		
	ui.saveFileButton->setEnabled(true);	// Enable save
	ui.lsSegmButton->setEnabled(true);
	ui.ctSegmButton->setEnabled(true);
	ui.triangulationButton->setEnabled(true);
	ui.gaborButton->setEnabled(true);
	ui.levelSetButton->setEnabled(true);
	ui.segmentationButton->setEnabled(true);
	ui.cartoonTextureButton->setEnabled(true);
	ui.renderingParamsButton->setEnabled(true);
	ui.triangulationParamsButton->setEnabled(true);
	ui.partExtractionParamsButton->setEnabled(true);

	ofTimer->start(1);						// Use timer to enable cursor override
}


void CVUserInterface::SaveFileAction()
{
	QString filterEx = "PNG (*.png);;BMP (*.bmp);;JPG (*.jpg)";
	//
	//QString qFilename = QFileDialog::getSaveFileName(this, "Save Image", " ", filterEx);
	QString qFilename = QFileDialog::getSaveFileName(this, "Save Image", "C:\\Users\\Administrator\\Desktop\\Vectorization_Result", filterEx);
	if(qFilename.isEmpty()) return;
	std::string saveFilename = qFilename.toStdString();	
	ui.frame->GetGLWidget()->SaveImage(qFilename);
}


void CVUserInterface::DoStrokeCalculation()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Do Stroke Calculation...", 100000);

	// Read Parameter input
	SetParams();

	std::vector<std::vector<CVSystem::MyLine>> sLines = ui.frame->GetGLWidget()->GetStrokes();
	scSegm->ComputeDistanceMap(sLines);
	//segmEngine->ComputeSegmentation2(sLines);

	ui.statusBar->showMessage("Stroke Calculation done", 5000);
	QApplication::restoreOverrideCursor();
}


void CVUserInterface::DoCTSegmentation()
{
	std::cout << "Do Cartoon+Texture Segmentation\n";

	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();

	// Read Parameter input
	SetParams();

	ui.statusBar->showMessage("Running Segmentation...", 100000);
	scSegm->ComputeCTSegmentation();
	//ui.frame->GetGLWidget()->SetDilatedSegment(scSegm->GetDilatedLabelMap());
	ui.frame->GetGLWidget()->SetSegment(scSegm->GetLabelMap());

	hasDoneSegmentation = true;
	std::cout << "Segmentation done\n";
	ui.statusBar->showMessage("Segmentation done", 5000);
	QApplication::restoreOverrideCursor();

	// enable button to vectorize screentone
	//ui.sVectorizeButton->setEnabled(true);
}

void CVUserInterface::DoLSSegmentation()
{
	std::cout << "Do Level Set Segmentation\n";
	std::vector<std::vector<CVSystem::MyLine>> sLines = ui.frame->GetGLWidget()->GetStrokes();
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();

	// Read Parameter input
	SetParams();

	ui.statusBar->showMessage("Running Segmentation...", 100000);
	scSegm->ComputeLSSegmentation(sLines);
	//ui.frame->GetGLWidget()->SetDilatedSegment(scSegm->GetDilatedLabelMap());
	ui.frame->GetGLWidget()->SetSegment(scSegm->GetLabelMap());
		
	hasDoneSegmentation = true;
	std::cout << "Segmentation done\n";
	ui.statusBar->showMessage("Segmentation done", 5000);
	QApplication::restoreOverrideCursor();

	// enable button to vectorize screentone
	//ui.sVectorizeButton->setEnabled(true);
}

void CVUserInterface::DoScreentoneVectorization()
{
	// Read Parameter input
	SetParams();

	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Analyzing Screentone...", 100000);

	int* labelMap = scSegm->GetLabelMap();
	std::vector<double> params = scClassifier->GetParameters(strFilename, labelMap);
	ui.frame->GetGLWidget()->SetScreentoneParams(params);

	if(params.size() > 2) { ui.sbSTOrientation->setValue(params[1]); }

	ui.statusBar->showMessage("Screentone Vectorization done", 5000);
	QApplication::restoreOverrideCursor();
}

void CVUserInterface::DoHandlesBinding()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Do Handles Binding...", 100000);

	// mesh deform fix
	//ui.frame->GetGLWidget()->GetMeshDeformer()->BindHandles();
	ui.frame->GetGLWidget()->GetDeformManager()->BindHandles();

	ui.statusBar->showMessage("Handles Binding", 5000);
	QApplication::restoreOverrideCursor();
}

void CVUserInterface::DoExtractPart()
{
	ui.frame->GetGLWidget()->GetPartExtractor()->ExtractPart();
}

void CVUserInterface::DoPartsTriangulation()
{
	using namespace CVSystem;
	 
	std::vector<MyIndexedPolygon>      oriParts		  = ui.frame->GetGLWidget()->GetPartExtractor()->GetParts();
	std::vector<MyPoint>			   oriVertexList  = ui.frame->GetGLWidget()->GetVertexList();
	std::vector<MyIndexedBezierCurves> oriIdxBzCurves = ui.frame->GetGLWidget()->GetIndexedBezierCurves();
	std::vector<MyIndexedTriangle>	   oriBTriangles  = ui.frame->GetGLWidget()->GetBTriangles();
	std::vector<MyIndexedTriangle>	   oriWTriangles  = ui.frame->GetGLWidget()->GetWtriangles();
	std::vector<MyIndexedTriangle>	   oriSCTriangles = ui.frame->GetGLWidget()->GetSCtriangles();

	std::vector<CVSystem::MyIndexedLine>	oriIdxPartitionLines = ui.frame->GetGLWidget()->GetPartExtractor()->GetPartitionLines();
	std::vector<IUI_StrokeType>				oriIdxPartitionLineTypes = ui.frame->GetGLWidget()->GetPartExtractor()->GetPartitionLineTypes();

	QSize imgSz = ui.frame->GetGLWidget()->GetImageSize();

	if(triangulator2) { delete triangulator2; }
	triangulator2  = new Triangulator2();

	triangulator2->SetData(oriParts,
						   oriVertexList,
						   oriIdxBzCurves,
						   oriBTriangles,
						   oriWTriangles,
						   oriSCTriangles,
						   oriIdxPartitionLines,
						   oriIdxPartitionLineTypes,
						   imgSz.width(),
						   imgSz.height());

	//triangulator2->TriangulateOriMesh();
	//triangulator2->TriangulateParts();
	triangulator2->TriangulateAll();
}

void CVUserInterface::DoTriangulation()
{
	// Read Parameter input
	SetParams();

	QApplication::setOverrideCursor(Qt::WaitCursor);
	QApplication::processEvents();
	ui.statusBar->showMessage("Do Triangulation...", 100000);
	
	cv::Mat oriImg = cv::imread(strFilename, CV_LOAD_IMAGE_GRAYSCALE);
	
	int* labelMap = 0;
	int* dilatedLabelMap = 0;
	if(hasDoneSegmentation)
	{
		labelMap = scSegm->GetLabelMap();
		dilatedLabelMap = scSegm->GetDilatedLabelMap();
	}

	if(triangulator1) delete triangulator1;
	triangulator1 = new CVSystem::Triangulator1();
	triangulator1->TraceImage(strFilename, oriImg, labelMap, dilatedLabelMap);
	
	// Original Tracing
	//tgr->OCalculate();
	//ui.frame->GetGLWidget()->PushOTriangles(tgr->GetOTriangles());
	//ui.frame->GetGLWidget()->PushOQuads(tgr->GetOQuads());

	// Least Square Fitting
	//tgr->LSCalculate1();
	triangulator1->LSCalculate2();
	ui.frame->GetGLWidget()->SetLSTriangles(triangulator1->GetLSTriangles());
	ui.frame->GetGLWidget()->SetLSQuads(triangulator1->GetLSQuads());
	ui.frame->GetGLWidget()->SetTriangles(triangulator1->GetPartOffset(),
										  triangulator1->GetVertexList(), 
										  triangulator1->GetIndexedTriangles(), 
										  triangulator1->GetBorderSCTriangles(), 
										  triangulator1->GetBorderWTriangles(), 
										  triangulator1->GetSCTriangles(), 
										  triangulator1->GetWTriangles(), 
										  triangulator1->GetBTriangles());

	ui.frame->GetGLWidget()->SetIndexedBezierCurves(triangulator1->GetIndexedBezierCurves());

	// Since now, the corner detection is done internally
	//ui.frame->GetGLWidget()->SetCornerList(tgr->GetCornerList());	// Fucking delete this
	//ui.frame->GetGLWidget()->GetCurveCompletionUI()->SetCornerIndices(tgr->GetCornerIndices());
	//ui.frame->GetGLWidget()->GetCurveCompletionUI()->SetCornerEdges(tgr->GetCornerEdges());
	
	ui.frame->GetGLWidget()->SetDrawMode(DRAW_VECTOR);
	ui.statusBar->showMessage("Triangulation done", 5000);
	QApplication::restoreOverrideCursor();

	// enable button to vectorize screentone
	ui.sVectorizeButton->setEnabled(true);

	//ui.extractPartButton->setEnabled(true);

}

void CVUserInterface::DisplayMessage(QString msg)
{
	// timeout after 2.5 seconds
	ui.statusBar->showMessage(msg, 2500);
}

// Please check SystemParams.cpp
void CVUserInterface::SetParams()
{
	// Manjunath - Ma
	if(ui.gaborParamsDock->isVisible())
	{
		SystemParams::g_scale		= ui.sbGScale->value();
		SystemParams::g_orientation = ui.sbGOrientation->value();
		SystemParams::g_total_dim	= SystemParams::g_scale * SystemParams::g_orientation;
		SystemParams::g_window		= ui.sbGWindow->value();
		SystemParams::g_ul			= ui.sbGMinFreq->value();
		SystemParams::g_uh			= ui.sbGMaxFreq->value();
		SystemParams::g_side		= ui.sbGSide->value();
		SystemParams::g_flag		= ui.cbGFlag->checkState() == Qt::Checked;
	}

	// Segmentation parameters
	if(ui.segmentationParamsDock->isVisible())
	{
		SystemParams::s_km_n_clusters = ui.sbSNCluster->value();
		SystemParams::s_km_max_iter   = ui.sbSMaxIter->value();	
		SystemParams::s_min_size_area = ui.sbSMinArea->value();
	}

	// Chan-Vese IPOL
	if(ui.levelSetParamsDock->isVisible())
	{
		SystemParams::cv_ip_Tol     = ui.sbLSTolerance->value() * 0.1;
		SystemParams::cv_ip_MaxIter = ui.sbLSMaxIter->value();
		SystemParams::cv_ip_Mu      = ui.sbLSMu->value();
		SystemParams::cv_ip_Nu      = ui.sbLSNu->value();	
		SystemParams::cv_ip_Lambda1 = ui.sbLSLambda1->value();
		SystemParams::cv_ip_Lambda2 = ui.sbLSLambda2->value();
		SystemParams::cv_ip_dt      = ui.sbLSDistanceTime->value() * 0.1;
	}

	// Cartoon+Texture
	if(ui.cartoonTextureParamsDock->isVisible())
	{
		SystemParams::ct_f_sigma = ui.sbCTSigma->value();
	}

	// Triangulation
	if(ui.triangulationParamsDock->isVisible())
	{
		SystemParams::t_scale_factor = ui.spTRScale->value();
		SystemParams::t_rdp_epsilon = ui.spTRRDPEpsilon->value() * SystemParams::t_scale_factor;
		SystemParams::css_sigma = ui.spTRCSSSigma->value();
		SystemParams::t_delaunay_max_length = ui.spTREdgeLength->value();
	}

	std::cout << SystemParams::css_sigma << "\n";
}

void CVUserInterface::ToggleButtonsPressed()
{
	SystemParams::gr_gpu = ui.gpuToggleButton->isChecked();
	SystemParams::gr_sc_regions = ui.scRegionsToggleButton->isChecked();
	SystemParams::gr_user_scribbles = ui.usScribblesToggleButton->isChecked();
	SystemParams::gr_show_wireframe = ui.wireframeToggleButton->isChecked();
	SystemParams::gr_show_bbw_handles = ui.bbwHandlesToggleButton->isChecked();
	SystemParams::gr_show_bbw_pseudo_edge = ui.bbwPEToggleButton->isChecked();
	ui.frame->GetGLWidget()->updateGL();
}

/*
void CVUserInterface::WireframeToggleButtonPressed()
{
	SystemParams::gr_show_wireframe = ui.wireframeToggleButton->isChecked();
	ui.frame->GetGLWidget()->updateGL();
}
*/

/*
void CVUserInterface::BBWHandlesToggleButtonPressed()
{
	SystemParams::gr_show_bbw_handles = ui.bbwHandlesToggleButton->isChecked();
	ui.frame->GetGLWidget()->updateGL();
}
*/

/*
void CVUserInterface::BBWPEToggleButtonPressed()
{
	SystemParams::gr_show_bbw_pseudo_edge = ui.bbwPEToggleButton->isChecked();
	ui.frame->GetGLWidget()->updateGL();
}
*/

void CVUserInterface::DisplayGaborParameters() 
{ 
	ui.gaborParamsDock->show(); 
	ui.gaborParamsContent->setMinimumHeight(this->height() / 4);
}

void CVUserInterface::DisplayLevelSetParameters() 
{ 
	ui.levelSetParamsDock->show(); 
	ui.levelSetParamsContent->setMinimumHeight(this->height() / 4);
}

void CVUserInterface::DisplaySegmentationParameters() 
{ 
	ui.segmentationParamsDock->show(); 
	ui.segmentationParamsContent->setMinimumHeight(this->height() / 4);
}

void CVUserInterface::DisplayRenderingParameters() 
{ 
	ui.renderingParamsDock->show(); 	
	ui.renderingParamsContent->setMinimumHeight(this->height() / 5);
}

void CVUserInterface::DisplayCartoonTextureParams() 
{ 
	ui.cartoonTextureParamsDock->show(); 
	ui.cartoonTextureParamsContent->setMinimumHeight(this->height() / 5);
}

void CVUserInterface::DisplayTriangulationParams()
{
	ui.triangulationParamsDock->show();
	ui.triangulationParamsContent->setMinimumHeight(this->height() / 5);
}

void CVUserInterface::DisplayPartExtractionParams()
{
	ui.partExtractorParamsDock->show();
	ui.partExtractorParamsContent->setMinimumHeight(this->height() / 5);
}

void CVUserInterface::RenderingParamsValuesChanged()
{	
	//std::cout << "renedering params\n";

	//std::cout << ui.sbSTBlackness->value() << "\n";

	SystemParams::sr_aa_value = ui.sbSTAA->value();
	SystemParams::sr_tone_frequency = ui.sbSTScale->value();
	ui.frame->GetGLWidget()->SetScreentoneOrientation(ui.sbSTOrientation->value());
	SystemParams::sr_blur = ui.sbSTBlur->value();
	SystemParams::sr_blackness = ui.sbSTBlackness->value();
	//SystemParams::cr_blur = ui.sbCRBlur->value();
	SystemParams::cr_blackness = (double)ui.sbCRBlackness->value() / 255.0 ;
	
	bool isFixed = (ui.cbSTFixed->checkState() == Qt::Checked);

	if(isFixed != SystemParams::sr_fixed)
	{
		if(isFixed)
		{
			SystemParams::sr_tone_frequency = 0.5;
			ui.sbSTScale->setValue(0.5);
		}
		else
		{
			SystemParams::sr_tone_frequency = 1.0;
			ui.sbSTScale->setValue(1.0);
		}
	}

	SystemParams::sr_fixed = isFixed;

	ui.frame->GetGLWidget()->updateGL();
}

void CVUserInterface::PartExtrationParamsChanged()
{
	SystemParams::pe_num_iteration   = ui.sbPENIter->value();
	SystemParams::pe_sample_distance = ui.sbPESampleDist->value();
	SystemParams::pe_search_radius   = ui.sbPESearchRadius->value();
	SystemParams::pe_threshold       = ui.sbPEThreshold->value();

	//std::cout << "pe_num_iteration "   << SystemParams::pe_num_iteration << "\n";
	//std::cout << "pe_sample_distance " << SystemParams::pe_sample_distance << "\n";
	//std::cout << "pe_search_radius "   << SystemParams::pe_search_radius << "\n";
}

/*void CVUserInterface::keyPressEvent(QKeyEvent *event)
{
	std::cout << "CVUserInterface, keyPress\n";
}*/

void CVUserInterface::GLContainerUpdate()
{
	// mesh deform fix
	/*if(ui.frame->GetGLWidget()->GetDrawMode() == DRAW_DEFORMATION &&
	ui.frame->GetGLWidget()->GetMeshDeformer()->BindReady() )
	{ ui.bindHandlesButton->setEnabled(true); }
	else
	{ ui.bindHandlesButton->setEnabled(false); }
	*/
	if(ui.frame->GetGLWidget()->GetDrawMode() == DRAW_DEFORMATION &&
		ui.frame->GetGLWidget()->GetDeformManager()->BindReady() )
	{ ui.bindHandlesButton->setEnabled(true); }
	else
	{ ui.bindHandlesButton->setEnabled(false); }

	if(ui.frame->GetGLWidget()->GetDrawMode() == DRAW_PART_EXTRACTION && ui.frame->GetGLWidget()->GetPartExtractor()->HasData())
	{ 
		ui.extractPartButton->setEnabled(true); 
		ui.partsTriangulationButton->setEnabled(true); 
	}
	else
		
	{ 
		ui.extractPartButton->setEnabled(false); 
		ui.partsTriangulationButton->setEnabled(false); 
	}
}