
/**
 *
 * This is the main UI form
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __CV_User_Interface__
#define __CV_User_Interface__

#include <QTimer>

#include <QtWidgets/QMainWindow>
#include "ui_CVUserInterface.h"

#include "SystemParams.h"

// forward declaration
namespace CVSystem
{
	class ScreentoneSegmentation;
	class ScreentoneClassifier;
	class Triangulator1;
	class Triangulator2;
}

// forward declaration
class MeshDeformer;	
class LightingEffect;

class CVUserInterface : public QMainWindow
{
	Q_OBJECT

public:
	CVUserInterface(QWidget *parent = 0);
	~CVUserInterface();

private:
	std::string strFilename;
	Ui::CVUserInterfaceClass ui;
	
	CVSystem::ScreentoneSegmentation*	scSegm;
	CVSystem::ScreentoneClassifier*		scClassifier;
	CVSystem::Triangulator1*			triangulator1;
	CVSystem::Triangulator2*			triangulator2;

	QTimer* ofTimer;
	QTimer* ofMYOBJTimer;
	bool hasDoneSegmentation;


private:
	// et pipeline params
	void SetParams();

private slots:
	// trigger thread to open image
	void OpenFileAction();	
	// thread to open image
	void OpenFileThread();
	// trigger thread to open .myobj
	void OpenMYOBJFileAction();
	// thread to open .myobj
	void OpenMYOBJFileThread();
	// save image drawn in display
	void SaveFileAction();	
	// display message in bottom left
	void DisplayMessage(QString msg);


	// level set segmentation
	void DoLSSegmentation();
	// cartoon+texture segmentation
	void DoCTSegmentation();
	// strokes of level set
	void DoStrokeCalculation();
	// perform triangulation to produce mesh
	void DoTriangulation();
	// LBP and property extraction
	void DoScreentoneVectorization();
	// bind handle using BBW
	void DoHandlesBinding();
	// contextual component extraction
	void DoExtractPart();
	// triangulate contexts after completion
	void DoPartsTriangulation();


	//// Displaying menu
	// gabor wavelet menu
	void DisplayGaborParameters();
	// level set menu
	void DisplayLevelSetParameters();
	// segmentation menu
	void DisplaySegmentationParameters();
	// rendering menu (screentone and bezier curve)
	void DisplayRenderingParameters();
	// cartoon+texture menu
	void DisplayCartoonTextureParams();
	// triangulation menu
	void DisplayTriangulationParams();
	// context completion menu
	void DisplayPartExtractionParams();

	// if values in menu changed
	void RenderingParamsValuesChanged();
	// if values in menu changed
	void PartExtrationParamsChanged();
	// if values in menu changed
	void ToggleButtonsPressed();
	// listening to opengl container
	void GLContainerUpdate();
};

#endif // CVUSERINTERFACE_H
