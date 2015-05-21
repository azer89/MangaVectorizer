/********************************************************************************
** Form generated from reading UI file 'CVUserInterface.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVUSERINTERFACE_H
#define UI_CVUSERINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "GLContainer.h"

QT_BEGIN_NAMESPACE

class Ui_CVUserInterfaceClass
{
public:
    QAction *openFileButton;
    QAction *saveFileButton;
    QAction *lsSegmButton;
    QAction *triangulationButton;
    QAction *gaborButton;
    QAction *levelSetButton;
    QAction *cartoonTextureButton;
    QAction *renderingParamsButton;
    QAction *segmentationButton;
    QAction *sVectorizeButton;
    QAction *ctSegmButton;
    QAction *triangulationParamsButton;
    QAction *bindHandlesButton;
    QAction *wireframeToggleButton;
    QAction *bbwHandlesToggleButton;
    QAction *openMyObjButton;
    QAction *bbwPEToggleButton;
    QAction *scRegionsToggleButton;
    QAction *gpuToggleButton;
    QAction *usScribblesToggleButton;
    QAction *partExtractionParamsButton;
    QAction *extractPartButton;
    QAction *partsTriangulationButton;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    GLContainer *frame;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuView_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *gaborParamsDock;
    QWidget *gaborParamsContent;
    QFormLayout *formLayout_3;
    QLabel *lblGScale;
    QSpinBox *sbGScale;
    QLabel *lblGOrientation;
    QSpinBox *sbGOrientation;
    QLabel *lblGWindow;
    QSpinBox *sbGWindow;
    QLabel *lblGMinFreq;
    QDoubleSpinBox *sbGMinFreq;
    QLabel *lblGMaxFreq;
    QDoubleSpinBox *sbGMaxFreq;
    QLabel *lblGSide;
    QSpinBox *sbGSide;
    QCheckBox *cbGFlag;
    QDockWidget *levelSetParamsDock;
    QWidget *levelSetParamsContent;
    QFormLayout *formLayout_2;
    QLabel *lblLSTolerance;
    QDoubleSpinBox *sbLSTolerance;
    QLabel *lblLSMaxIter;
    QSpinBox *sbLSMaxIter;
    QLabel *lblLSMu;
    QDoubleSpinBox *sbLSMu;
    QLabel *lblLSNu;
    QDoubleSpinBox *sbLSNu;
    QLabel *lblLSLambda1;
    QDoubleSpinBox *sbLSLambda1;
    QLabel *lblLSLambda2;
    QDoubleSpinBox *sbLSLambda2;
    QLabel *lblLSDistanceTime;
    QDoubleSpinBox *sbLSDistanceTime;
    QDockWidget *segmentationParamsDock;
    QWidget *segmentationParamsContent;
    QFormLayout *formLayout;
    QLabel *lblSNCluster;
    QSpinBox *sbSNCluster;
    QLabel *lblSMaxIter;
    QSpinBox *sbSMaxIter;
    QLabel *lblSCutoff;
    QDoubleSpinBox *sbSCutoff;
    QCheckBox *cbSNormalize;
    QLabel *lblSMinArea;
    QSpinBox *sbSMinArea;
    QDockWidget *renderingParamsDock;
    QWidget *renderingParamsContent;
    QFormLayout *formLayout_4;
    QLabel *label;
    QLabel *lblSTAA;
    QDoubleSpinBox *sbSTAA;
    QLabel *lblSTScale;
    QDoubleSpinBox *sbSTScale;
    QLabel *lblSTOrientation;
    QSpinBox *sbSTOrientation;
    QLabel *lblSTBlackness;
    QSpinBox *sbSTBlackness;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *sbCRBlackness;
    QCheckBox *cbSTFixed;
    QLabel *label_4;
    QDoubleSpinBox *sbSTBlur;
    QDockWidget *cartoonTextureParamsDock;
    QWidget *cartoonTextureParamsContent;
    QFormLayout *formLayout_5;
    QLabel *lblCTSigma;
    QDoubleSpinBox *sbCTSigma;
    QDockWidget *triangulationParamsDock;
    QWidget *triangulationParamsContent;
    QFormLayout *formLayout_6;
    QLabel *lblTRCSSSigma;
    QSpinBox *spTRCSSSigma;
    QLabel *lblTRScale;
    QDoubleSpinBox *spTRScale;
    QLabel *lblTRRDPEpsilon;
    QDoubleSpinBox *spTRRDPEpsilon;
    QLabel *lblTREdgeLength;
    QSpinBox *spTREdgeLength;
    QDockWidget *partExtractorParamsDock;
    QWidget *partExtractorParamsContent;
    QFormLayout *formLayout_7;
    QLabel *lblPESampleDist;
    QDoubleSpinBox *sbPESampleDist;
    QLabel *lblPENIter;
    QSpinBox *sbPENIter;
    QLabel *lblPESearchRadius;
    QDoubleSpinBox *sbPESearchRadius;
    QLabel *lblPEThreshold;
    QDoubleSpinBox *sbPEThreshold;

    void setupUi(QMainWindow *CVUserInterfaceClass)
    {
        if (CVUserInterfaceClass->objectName().isEmpty())
            CVUserInterfaceClass->setObjectName(QStringLiteral("CVUserInterfaceClass"));
        CVUserInterfaceClass->resize(900, 679);
        QIcon icon;
        icon.addFile(QStringLiteral(":/CVUserInterface/Image/ballon.png"), QSize(), QIcon::Normal, QIcon::Off);
        CVUserInterfaceClass->setWindowIcon(icon);
        openFileButton = new QAction(CVUserInterfaceClass);
        openFileButton->setObjectName(QStringLiteral("openFileButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/CVUserInterface/Image/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        openFileButton->setIcon(icon1);
        saveFileButton = new QAction(CVUserInterfaceClass);
        saveFileButton->setObjectName(QStringLiteral("saveFileButton"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/CVUserInterface/Image/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveFileButton->setIcon(icon2);
        lsSegmButton = new QAction(CVUserInterfaceClass);
        lsSegmButton->setObjectName(QStringLiteral("lsSegmButton"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/CVUserInterface/Image/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        lsSegmButton->setIcon(icon3);
        triangulationButton = new QAction(CVUserInterfaceClass);
        triangulationButton->setObjectName(QStringLiteral("triangulationButton"));
        triangulationButton->setIcon(icon3);
        gaborButton = new QAction(CVUserInterfaceClass);
        gaborButton->setObjectName(QStringLiteral("gaborButton"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/CVUserInterface/Image/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        gaborButton->setIcon(icon4);
        levelSetButton = new QAction(CVUserInterfaceClass);
        levelSetButton->setObjectName(QStringLiteral("levelSetButton"));
        levelSetButton->setIcon(icon4);
        cartoonTextureButton = new QAction(CVUserInterfaceClass);
        cartoonTextureButton->setObjectName(QStringLiteral("cartoonTextureButton"));
        cartoonTextureButton->setIcon(icon4);
        renderingParamsButton = new QAction(CVUserInterfaceClass);
        renderingParamsButton->setObjectName(QStringLiteral("renderingParamsButton"));
        renderingParamsButton->setIcon(icon4);
        segmentationButton = new QAction(CVUserInterfaceClass);
        segmentationButton->setObjectName(QStringLiteral("segmentationButton"));
        segmentationButton->setIcon(icon4);
        sVectorizeButton = new QAction(CVUserInterfaceClass);
        sVectorizeButton->setObjectName(QStringLiteral("sVectorizeButton"));
        sVectorizeButton->setIcon(icon3);
        ctSegmButton = new QAction(CVUserInterfaceClass);
        ctSegmButton->setObjectName(QStringLiteral("ctSegmButton"));
        ctSegmButton->setIcon(icon3);
        triangulationParamsButton = new QAction(CVUserInterfaceClass);
        triangulationParamsButton->setObjectName(QStringLiteral("triangulationParamsButton"));
        triangulationParamsButton->setIcon(icon4);
        bindHandlesButton = new QAction(CVUserInterfaceClass);
        bindHandlesButton->setObjectName(QStringLiteral("bindHandlesButton"));
        bindHandlesButton->setIcon(icon3);
        wireframeToggleButton = new QAction(CVUserInterfaceClass);
        wireframeToggleButton->setObjectName(QStringLiteral("wireframeToggleButton"));
        wireframeToggleButton->setCheckable(true);
        wireframeToggleButton->setChecked(false);
        bbwHandlesToggleButton = new QAction(CVUserInterfaceClass);
        bbwHandlesToggleButton->setObjectName(QStringLiteral("bbwHandlesToggleButton"));
        bbwHandlesToggleButton->setCheckable(true);
        bbwHandlesToggleButton->setChecked(true);
        openMyObjButton = new QAction(CVUserInterfaceClass);
        openMyObjButton->setObjectName(QStringLiteral("openMyObjButton"));
        openMyObjButton->setIcon(icon1);
        bbwPEToggleButton = new QAction(CVUserInterfaceClass);
        bbwPEToggleButton->setObjectName(QStringLiteral("bbwPEToggleButton"));
        bbwPEToggleButton->setCheckable(true);
        bbwPEToggleButton->setChecked(true);
        scRegionsToggleButton = new QAction(CVUserInterfaceClass);
        scRegionsToggleButton->setObjectName(QStringLiteral("scRegionsToggleButton"));
        scRegionsToggleButton->setCheckable(true);
        scRegionsToggleButton->setChecked(true);
        gpuToggleButton = new QAction(CVUserInterfaceClass);
        gpuToggleButton->setObjectName(QStringLiteral("gpuToggleButton"));
        gpuToggleButton->setCheckable(true);
        gpuToggleButton->setChecked(false);
        usScribblesToggleButton = new QAction(CVUserInterfaceClass);
        usScribblesToggleButton->setObjectName(QStringLiteral("usScribblesToggleButton"));
        usScribblesToggleButton->setCheckable(true);
        usScribblesToggleButton->setChecked(true);
        partExtractionParamsButton = new QAction(CVUserInterfaceClass);
        partExtractionParamsButton->setObjectName(QStringLiteral("partExtractionParamsButton"));
        partExtractionParamsButton->setIcon(icon4);
        extractPartButton = new QAction(CVUserInterfaceClass);
        extractPartButton->setObjectName(QStringLiteral("extractPartButton"));
        extractPartButton->setIcon(icon3);
        partsTriangulationButton = new QAction(CVUserInterfaceClass);
        partsTriangulationButton->setObjectName(QStringLiteral("partsTriangulationButton"));
        partsTriangulationButton->setIcon(icon3);
        centralWidget = new QWidget(CVUserInterfaceClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(5, 5, 5, 5);
        frame = new GLContainer(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(1);

        gridLayout_2->addWidget(frame, 0, 1, 1, 1);

        CVUserInterfaceClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CVUserInterfaceClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuBar->setAcceptDrops(false);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuView_2 = new QMenu(menuBar);
        menuView_2->setObjectName(QStringLiteral("menuView_2"));
        CVUserInterfaceClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CVUserInterfaceClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
        mainToolBar->setIconSize(QSize(12, 12));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        mainToolBar->setFloatable(false);
        CVUserInterfaceClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CVUserInterfaceClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CVUserInterfaceClass->setStatusBar(statusBar);
        gaborParamsDock = new QDockWidget(CVUserInterfaceClass);
        gaborParamsDock->setObjectName(QStringLiteral("gaborParamsDock"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gaborParamsDock->sizePolicy().hasHeightForWidth());
        gaborParamsDock->setSizePolicy(sizePolicy);
        gaborParamsDock->setMinimumSize(QSize(191, 205));
        gaborParamsDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
        gaborParamsDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        gaborParamsContent = new QWidget();
        gaborParamsContent->setObjectName(QStringLiteral("gaborParamsContent"));
        formLayout_3 = new QFormLayout(gaborParamsContent);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_3->setContentsMargins(5, 5, 5, 5);
        lblGScale = new QLabel(gaborParamsContent);
        lblGScale->setObjectName(QStringLiteral("lblGScale"));
        lblGScale->setMinimumSize(QSize(75, 0));
        lblGScale->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, lblGScale);

        sbGScale = new QSpinBox(gaborParamsContent);
        sbGScale->setObjectName(QStringLiteral("sbGScale"));
        sbGScale->setMinimumSize(QSize(100, 0));
        sbGScale->setMaximumSize(QSize(100, 16777215));
        sbGScale->setReadOnly(false);
        sbGScale->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        sbGScale->setMinimum(1);
        sbGScale->setMaximum(10);
        sbGScale->setValue(4);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, sbGScale);

        lblGOrientation = new QLabel(gaborParamsContent);
        lblGOrientation->setObjectName(QStringLiteral("lblGOrientation"));
        lblGOrientation->setMinimumSize(QSize(75, 0));
        lblGOrientation->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, lblGOrientation);

        sbGOrientation = new QSpinBox(gaborParamsContent);
        sbGOrientation->setObjectName(QStringLiteral("sbGOrientation"));
        sbGOrientation->setMinimumSize(QSize(100, 0));
        sbGOrientation->setMaximumSize(QSize(100, 16777215));
        sbGOrientation->setReadOnly(false);
        sbGOrientation->setMinimum(1);
        sbGOrientation->setMaximum(10);
        sbGOrientation->setValue(6);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, sbGOrientation);

        lblGWindow = new QLabel(gaborParamsContent);
        lblGWindow->setObjectName(QStringLiteral("lblGWindow"));
        lblGWindow->setMinimumSize(QSize(75, 0));
        lblGWindow->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, lblGWindow);

        sbGWindow = new QSpinBox(gaborParamsContent);
        sbGWindow->setObjectName(QStringLiteral("sbGWindow"));
        sbGWindow->setMinimumSize(QSize(100, 0));
        sbGWindow->setMaximumSize(QSize(100, 16777215));
        sbGWindow->setMinimum(1);
        sbGWindow->setMaximum(21);
        sbGWindow->setValue(1);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, sbGWindow);

        lblGMinFreq = new QLabel(gaborParamsContent);
        lblGMinFreq->setObjectName(QStringLiteral("lblGMinFreq"));
        lblGMinFreq->setMinimumSize(QSize(75, 0));
        lblGMinFreq->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, lblGMinFreq);

        sbGMinFreq = new QDoubleSpinBox(gaborParamsContent);
        sbGMinFreq->setObjectName(QStringLiteral("sbGMinFreq"));
        sbGMinFreq->setMinimumSize(QSize(100, 0));
        sbGMinFreq->setMaximumSize(QSize(100, 16777215));
        sbGMinFreq->setMinimum(0);
        sbGMinFreq->setMaximum(50);
        sbGMinFreq->setSingleStep(0.05);
        sbGMinFreq->setValue(0.2);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, sbGMinFreq);

        lblGMaxFreq = new QLabel(gaborParamsContent);
        lblGMaxFreq->setObjectName(QStringLiteral("lblGMaxFreq"));
        lblGMaxFreq->setMinimumSize(QSize(75, 0));
        lblGMaxFreq->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, lblGMaxFreq);

        sbGMaxFreq = new QDoubleSpinBox(gaborParamsContent);
        sbGMaxFreq->setObjectName(QStringLiteral("sbGMaxFreq"));
        sbGMaxFreq->setMinimumSize(QSize(100, 0));
        sbGMaxFreq->setMaximumSize(QSize(100, 16777215));
        sbGMaxFreq->setMinimum(0.1);
        sbGMaxFreq->setMaximum(50);
        sbGMaxFreq->setSingleStep(0.05);
        sbGMaxFreq->setValue(0.8);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, sbGMaxFreq);

        lblGSide = new QLabel(gaborParamsContent);
        lblGSide->setObjectName(QStringLiteral("lblGSide"));
        lblGSide->setMinimumSize(QSize(75, 0));
        lblGSide->setMaximumSize(QSize(75, 16777215));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, lblGSide);

        sbGSide = new QSpinBox(gaborParamsContent);
        sbGSide->setObjectName(QStringLiteral("sbGSide"));
        sbGSide->setMinimumSize(QSize(100, 0));
        sbGSide->setMaximumSize(QSize(100, 16777215));
        sbGSide->setReadOnly(true);
        sbGSide->setMinimum(1);
        sbGSide->setMaximum(200);
        sbGSide->setValue(60);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, sbGSide);

        cbGFlag = new QCheckBox(gaborParamsContent);
        cbGFlag->setObjectName(QStringLiteral("cbGFlag"));
        cbGFlag->setCheckable(false);

        formLayout_3->setWidget(6, QFormLayout::LabelRole, cbGFlag);

        gaborParamsDock->setWidget(gaborParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), gaborParamsDock);
        levelSetParamsDock = new QDockWidget(CVUserInterfaceClass);
        levelSetParamsDock->setObjectName(QStringLiteral("levelSetParamsDock"));
        sizePolicy.setHeightForWidth(levelSetParamsDock->sizePolicy().hasHeightForWidth());
        levelSetParamsDock->setSizePolicy(sizePolicy);
        levelSetParamsDock->setMinimumSize(QSize(191, 208));
        levelSetParamsDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        levelSetParamsContent = new QWidget();
        levelSetParamsContent->setObjectName(QStringLiteral("levelSetParamsContent"));
        formLayout_2 = new QFormLayout(levelSetParamsContent);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(5, 5, 5, 5);
        lblLSTolerance = new QLabel(levelSetParamsContent);
        lblLSTolerance->setObjectName(QStringLiteral("lblLSTolerance"));
        lblLSTolerance->setMinimumSize(QSize(75, 0));
        lblLSTolerance->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, lblLSTolerance);

        sbLSTolerance = new QDoubleSpinBox(levelSetParamsContent);
        sbLSTolerance->setObjectName(QStringLiteral("sbLSTolerance"));
        sbLSTolerance->setMinimumSize(QSize(100, 0));
        sbLSTolerance->setMaximumSize(QSize(100, 16777215));
        sbLSTolerance->setReadOnly(false);
        sbLSTolerance->setMaximum(0.1);
        sbLSTolerance->setSingleStep(0.05);
        sbLSTolerance->setValue(0.01);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, sbLSTolerance);

        lblLSMaxIter = new QLabel(levelSetParamsContent);
        lblLSMaxIter->setObjectName(QStringLiteral("lblLSMaxIter"));
        lblLSMaxIter->setMinimumSize(QSize(75, 0));
        lblLSMaxIter->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, lblLSMaxIter);

        sbLSMaxIter = new QSpinBox(levelSetParamsContent);
        sbLSMaxIter->setObjectName(QStringLiteral("sbLSMaxIter"));
        sbLSMaxIter->setMinimumSize(QSize(100, 0));
        sbLSMaxIter->setMaximumSize(QSize(100, 16777215));
        sbLSMaxIter->setMinimum(1);
        sbLSMaxIter->setMaximum(5000);
        sbLSMaxIter->setValue(1000);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, sbLSMaxIter);

        lblLSMu = new QLabel(levelSetParamsContent);
        lblLSMu->setObjectName(QStringLiteral("lblLSMu"));
        lblLSMu->setMinimumSize(QSize(75, 0));
        lblLSMu->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, lblLSMu);

        sbLSMu = new QDoubleSpinBox(levelSetParamsContent);
        sbLSMu->setObjectName(QStringLiteral("sbLSMu"));
        sbLSMu->setMinimumSize(QSize(100, 0));
        sbLSMu->setMaximumSize(QSize(100, 16777215));
        sbLSMu->setMaximum(2000);
        sbLSMu->setValue(0.01);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, sbLSMu);

        lblLSNu = new QLabel(levelSetParamsContent);
        lblLSNu->setObjectName(QStringLiteral("lblLSNu"));
        lblLSNu->setMinimumSize(QSize(75, 0));
        lblLSNu->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, lblLSNu);

        sbLSNu = new QDoubleSpinBox(levelSetParamsContent);
        sbLSNu->setObjectName(QStringLiteral("sbLSNu"));
        sbLSNu->setMinimumSize(QSize(100, 0));
        sbLSNu->setMaximumSize(QSize(100, 16777215));
        sbLSNu->setMinimum(-5000);
        sbLSNu->setMaximum(5000);
        sbLSNu->setValue(0);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, sbLSNu);

        lblLSLambda1 = new QLabel(levelSetParamsContent);
        lblLSLambda1->setObjectName(QStringLiteral("lblLSLambda1"));
        lblLSLambda1->setMinimumSize(QSize(75, 0));
        lblLSLambda1->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, lblLSLambda1);

        sbLSLambda1 = new QDoubleSpinBox(levelSetParamsContent);
        sbLSLambda1->setObjectName(QStringLiteral("sbLSLambda1"));
        sbLSLambda1->setMinimumSize(QSize(100, 0));
        sbLSLambda1->setMaximumSize(QSize(100, 16777215));
        sbLSLambda1->setMinimum(-5000);
        sbLSLambda1->setMaximum(5000);
        sbLSLambda1->setValue(1);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, sbLSLambda1);

        lblLSLambda2 = new QLabel(levelSetParamsContent);
        lblLSLambda2->setObjectName(QStringLiteral("lblLSLambda2"));
        lblLSLambda2->setMinimumSize(QSize(75, 0));
        lblLSLambda2->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, lblLSLambda2);

        sbLSLambda2 = new QDoubleSpinBox(levelSetParamsContent);
        sbLSLambda2->setObjectName(QStringLiteral("sbLSLambda2"));
        sbLSLambda2->setMinimumSize(QSize(100, 0));
        sbLSLambda2->setMaximumSize(QSize(100, 16777215));
        sbLSLambda2->setMinimum(-5000);
        sbLSLambda2->setMaximum(5000);
        sbLSLambda2->setValue(1);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, sbLSLambda2);

        lblLSDistanceTime = new QLabel(levelSetParamsContent);
        lblLSDistanceTime->setObjectName(QStringLiteral("lblLSDistanceTime"));
        lblLSDistanceTime->setMinimumSize(QSize(75, 0));
        lblLSDistanceTime->setMaximumSize(QSize(75, 16777215));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, lblLSDistanceTime);

        sbLSDistanceTime = new QDoubleSpinBox(levelSetParamsContent);
        sbLSDistanceTime->setObjectName(QStringLiteral("sbLSDistanceTime"));
        sbLSDistanceTime->setMinimumSize(QSize(100, 0));
        sbLSDistanceTime->setMaximumSize(QSize(100, 16777215));
        sbLSDistanceTime->setReadOnly(false);
        sbLSDistanceTime->setValue(0.01);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, sbLSDistanceTime);

        levelSetParamsDock->setWidget(levelSetParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), levelSetParamsDock);
        segmentationParamsDock = new QDockWidget(CVUserInterfaceClass);
        segmentationParamsDock->setObjectName(QStringLiteral("segmentationParamsDock"));
        sizePolicy.setHeightForWidth(segmentationParamsDock->sizePolicy().hasHeightForWidth());
        segmentationParamsDock->setSizePolicy(sizePolicy);
        segmentationParamsDock->setMinimumSize(QSize(180, 153));
        segmentationParamsDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        segmentationParamsContent = new QWidget();
        segmentationParamsContent->setObjectName(QStringLiteral("segmentationParamsContent"));
        formLayout = new QFormLayout(segmentationParamsContent);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setContentsMargins(5, 5, 5, 5);
        lblSNCluster = new QLabel(segmentationParamsContent);
        lblSNCluster->setObjectName(QStringLiteral("lblSNCluster"));
        lblSNCluster->setMinimumSize(QSize(75, 0));
        lblSNCluster->setMaximumSize(QSize(75, 16777215));

        formLayout->setWidget(0, QFormLayout::LabelRole, lblSNCluster);

        sbSNCluster = new QSpinBox(segmentationParamsContent);
        sbSNCluster->setObjectName(QStringLiteral("sbSNCluster"));
        sbSNCluster->setMaximumSize(QSize(100, 16777215));
        sbSNCluster->setValue(3);

        formLayout->setWidget(0, QFormLayout::FieldRole, sbSNCluster);

        lblSMaxIter = new QLabel(segmentationParamsContent);
        lblSMaxIter->setObjectName(QStringLiteral("lblSMaxIter"));
        lblSMaxIter->setMinimumSize(QSize(75, 0));
        lblSMaxIter->setMaximumSize(QSize(75, 16777215));

        formLayout->setWidget(1, QFormLayout::LabelRole, lblSMaxIter);

        sbSMaxIter = new QSpinBox(segmentationParamsContent);
        sbSMaxIter->setObjectName(QStringLiteral("sbSMaxIter"));
        sbSMaxIter->setMaximumSize(QSize(100, 16777215));
        sbSMaxIter->setMaximum(1000);
        sbSMaxIter->setValue(100);

        formLayout->setWidget(1, QFormLayout::FieldRole, sbSMaxIter);

        lblSCutoff = new QLabel(segmentationParamsContent);
        lblSCutoff->setObjectName(QStringLiteral("lblSCutoff"));
        lblSCutoff->setMinimumSize(QSize(75, 0));
        lblSCutoff->setMaximumSize(QSize(75, 16777215));

        formLayout->setWidget(3, QFormLayout::LabelRole, lblSCutoff);

        sbSCutoff = new QDoubleSpinBox(segmentationParamsContent);
        sbSCutoff->setObjectName(QStringLiteral("sbSCutoff"));
        sbSCutoff->setEnabled(false);
        sbSCutoff->setMaximumSize(QSize(100, 16777215));
        sbSCutoff->setMaximum(1);
        sbSCutoff->setSingleStep(0.1);
        sbSCutoff->setValue(0);

        formLayout->setWidget(3, QFormLayout::FieldRole, sbSCutoff);

        cbSNormalize = new QCheckBox(segmentationParamsContent);
        cbSNormalize->setObjectName(QStringLiteral("cbSNormalize"));
        cbSNormalize->setEnabled(false);
        cbSNormalize->setChecked(false);

        formLayout->setWidget(4, QFormLayout::LabelRole, cbSNormalize);

        lblSMinArea = new QLabel(segmentationParamsContent);
        lblSMinArea->setObjectName(QStringLiteral("lblSMinArea"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lblSMinArea);

        sbSMinArea = new QSpinBox(segmentationParamsContent);
        sbSMinArea->setObjectName(QStringLiteral("sbSMinArea"));
        sbSMinArea->setMaximum(1000000);
        sbSMinArea->setSingleStep(500);
        sbSMinArea->setValue(500);

        formLayout->setWidget(2, QFormLayout::FieldRole, sbSMinArea);

        segmentationParamsDock->setWidget(segmentationParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), segmentationParamsDock);
        renderingParamsDock = new QDockWidget(CVUserInterfaceClass);
        renderingParamsDock->setObjectName(QStringLiteral("renderingParamsDock"));
        renderingParamsDock->setMinimumSize(QSize(175, 285));
        renderingParamsDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        renderingParamsContent = new QWidget();
        renderingParamsContent->setObjectName(QStringLiteral("renderingParamsContent"));
        renderingParamsContent->setMinimumSize(QSize(175, 0));
        formLayout_4 = new QFormLayout(renderingParamsContent);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_4->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(renderingParamsContent);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(8);
        font.setUnderline(true);
        label->setFont(font);
        label->setTextFormat(Qt::AutoText);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label);

        lblSTAA = new QLabel(renderingParamsContent);
        lblSTAA->setObjectName(QStringLiteral("lblSTAA"));
        lblSTAA->setMinimumSize(QSize(75, 0));
        lblSTAA->setMaximumSize(QSize(75, 16777215));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, lblSTAA);

        sbSTAA = new QDoubleSpinBox(renderingParamsContent);
        sbSTAA->setObjectName(QStringLiteral("sbSTAA"));
        sbSTAA->setMinimum(0.1);
        sbSTAA->setMaximum(10);
        sbSTAA->setSingleStep(0.1);
        sbSTAA->setValue(0.5);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, sbSTAA);

        lblSTScale = new QLabel(renderingParamsContent);
        lblSTScale->setObjectName(QStringLiteral("lblSTScale"));
        lblSTScale->setMinimumSize(QSize(75, 0));
        lblSTScale->setMaximumSize(QSize(75, 16777215));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, lblSTScale);

        sbSTScale = new QDoubleSpinBox(renderingParamsContent);
        sbSTScale->setObjectName(QStringLiteral("sbSTScale"));
        sbSTScale->setMinimum(0.1);
        sbSTScale->setMaximum(10);
        sbSTScale->setSingleStep(0.1);
        sbSTScale->setValue(1);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, sbSTScale);

        lblSTOrientation = new QLabel(renderingParamsContent);
        lblSTOrientation->setObjectName(QStringLiteral("lblSTOrientation"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, lblSTOrientation);

        sbSTOrientation = new QSpinBox(renderingParamsContent);
        sbSTOrientation->setObjectName(QStringLiteral("sbSTOrientation"));
        sbSTOrientation->setMaximum(180);
        sbSTOrientation->setSingleStep(5);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, sbSTOrientation);

        lblSTBlackness = new QLabel(renderingParamsContent);
        lblSTBlackness->setObjectName(QStringLiteral("lblSTBlackness"));

        formLayout_4->setWidget(5, QFormLayout::LabelRole, lblSTBlackness);

        sbSTBlackness = new QSpinBox(renderingParamsContent);
        sbSTBlackness->setObjectName(QStringLiteral("sbSTBlackness"));
        sbSTBlackness->setMaximum(255);
        sbSTBlackness->setValue(75);

        formLayout_4->setWidget(5, QFormLayout::FieldRole, sbSTBlackness);

        label_2 = new QLabel(renderingParamsContent);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font1;
        font1.setUnderline(true);
        label_2->setFont(font1);

        formLayout_4->setWidget(7, QFormLayout::SpanningRole, label_2);

        label_3 = new QLabel(renderingParamsContent);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_4->setWidget(8, QFormLayout::LabelRole, label_3);

        sbCRBlackness = new QSpinBox(renderingParamsContent);
        sbCRBlackness->setObjectName(QStringLiteral("sbCRBlackness"));
        sbCRBlackness->setMaximum(255);
        sbCRBlackness->setValue(25);

        formLayout_4->setWidget(8, QFormLayout::FieldRole, sbCRBlackness);

        cbSTFixed = new QCheckBox(renderingParamsContent);
        cbSTFixed->setObjectName(QStringLiteral("cbSTFixed"));

        formLayout_4->setWidget(6, QFormLayout::LabelRole, cbSTFixed);

        label_4 = new QLabel(renderingParamsContent);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_4);

        sbSTBlur = new QDoubleSpinBox(renderingParamsContent);
        sbSTBlur->setObjectName(QStringLiteral("sbSTBlur"));
        sbSTBlur->setSingleStep(0.1);
        sbSTBlur->setValue(1);

        formLayout_4->setWidget(4, QFormLayout::FieldRole, sbSTBlur);

        renderingParamsDock->setWidget(renderingParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), renderingParamsDock);
        cartoonTextureParamsDock = new QDockWidget(CVUserInterfaceClass);
        cartoonTextureParamsDock->setObjectName(QStringLiteral("cartoonTextureParamsDock"));
        cartoonTextureParamsDock->setMinimumSize(QSize(175, 52));
        cartoonTextureParamsContent = new QWidget();
        cartoonTextureParamsContent->setObjectName(QStringLiteral("cartoonTextureParamsContent"));
        formLayout_5 = new QFormLayout(cartoonTextureParamsContent);
        formLayout_5->setSpacing(6);
        formLayout_5->setContentsMargins(11, 11, 11, 11);
        formLayout_5->setObjectName(QStringLiteral("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_5->setContentsMargins(5, 5, 5, 5);
        lblCTSigma = new QLabel(cartoonTextureParamsContent);
        lblCTSigma->setObjectName(QStringLiteral("lblCTSigma"));
        lblCTSigma->setMinimumSize(QSize(75, 0));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, lblCTSigma);

        sbCTSigma = new QDoubleSpinBox(cartoonTextureParamsContent);
        sbCTSigma->setObjectName(QStringLiteral("sbCTSigma"));
        sbCTSigma->setMinimum(0.1);
        sbCTSigma->setSingleStep(0.5);
        sbCTSigma->setValue(5.5);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, sbCTSigma);

        cartoonTextureParamsDock->setWidget(cartoonTextureParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), cartoonTextureParamsDock);
        triangulationParamsDock = new QDockWidget(CVUserInterfaceClass);
        triangulationParamsDock->setObjectName(QStringLiteral("triangulationParamsDock"));
        triangulationParamsDock->setMinimumSize(QSize(175, 130));
        triangulationParamsContent = new QWidget();
        triangulationParamsContent->setObjectName(QStringLiteral("triangulationParamsContent"));
        formLayout_6 = new QFormLayout(triangulationParamsContent);
        formLayout_6->setSpacing(6);
        formLayout_6->setContentsMargins(11, 11, 11, 11);
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        formLayout_6->setContentsMargins(5, 5, 5, 5);
        lblTRCSSSigma = new QLabel(triangulationParamsContent);
        lblTRCSSSigma->setObjectName(QStringLiteral("lblTRCSSSigma"));
        lblTRCSSSigma->setMinimumSize(QSize(75, 0));

        formLayout_6->setWidget(2, QFormLayout::LabelRole, lblTRCSSSigma);

        spTRCSSSigma = new QSpinBox(triangulationParamsContent);
        spTRCSSSigma->setObjectName(QStringLiteral("spTRCSSSigma"));
        spTRCSSSigma->setMaximumSize(QSize(100, 16777215));
        spTRCSSSigma->setMaximum(10000);
        spTRCSSSigma->setSingleStep(10);
        spTRCSSSigma->setValue(20);

        formLayout_6->setWidget(2, QFormLayout::FieldRole, spTRCSSSigma);

        lblTRScale = new QLabel(triangulationParamsContent);
        lblTRScale->setObjectName(QStringLiteral("lblTRScale"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, lblTRScale);

        spTRScale = new QDoubleSpinBox(triangulationParamsContent);
        spTRScale->setObjectName(QStringLiteral("spTRScale"));
        spTRScale->setMinimum(0.1);
        spTRScale->setMaximum(1);
        spTRScale->setSingleStep(0.1);
        spTRScale->setValue(1);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, spTRScale);

        lblTRRDPEpsilon = new QLabel(triangulationParamsContent);
        lblTRRDPEpsilon->setObjectName(QStringLiteral("lblTRRDPEpsilon"));

        formLayout_6->setWidget(1, QFormLayout::LabelRole, lblTRRDPEpsilon);

        spTRRDPEpsilon = new QDoubleSpinBox(triangulationParamsContent);
        spTRRDPEpsilon->setObjectName(QStringLiteral("spTRRDPEpsilon"));
        spTRRDPEpsilon->setMinimum(0);
        spTRRDPEpsilon->setMaximum(100);
        spTRRDPEpsilon->setValue(2);

        formLayout_6->setWidget(1, QFormLayout::FieldRole, spTRRDPEpsilon);

        lblTREdgeLength = new QLabel(triangulationParamsContent);
        lblTREdgeLength->setObjectName(QStringLiteral("lblTREdgeLength"));

        formLayout_6->setWidget(3, QFormLayout::LabelRole, lblTREdgeLength);

        spTREdgeLength = new QSpinBox(triangulationParamsContent);
        spTREdgeLength->setObjectName(QStringLiteral("spTREdgeLength"));
        spTREdgeLength->setMaximum(1000);
        spTREdgeLength->setSingleStep(5);

        formLayout_6->setWidget(3, QFormLayout::FieldRole, spTREdgeLength);

        triangulationParamsDock->setWidget(triangulationParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), triangulationParamsDock);
        partExtractorParamsDock = new QDockWidget(CVUserInterfaceClass);
        partExtractorParamsDock->setObjectName(QStringLiteral("partExtractorParamsDock"));
        partExtractorParamsDock->setMinimumSize(QSize(175, 138));
        partExtractorParamsDock->setFeatures(QDockWidget::DockWidgetClosable);
        partExtractorParamsDock->setAllowedAreas(Qt::RightDockWidgetArea);
        partExtractorParamsContent = new QWidget();
        partExtractorParamsContent->setObjectName(QStringLiteral("partExtractorParamsContent"));
        partExtractorParamsContent->setMinimumSize(QSize(175, 0));
        formLayout_7 = new QFormLayout(partExtractorParamsContent);
        formLayout_7->setSpacing(6);
        formLayout_7->setContentsMargins(11, 11, 11, 11);
        formLayout_7->setObjectName(QStringLiteral("formLayout_7"));
        formLayout_7->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        lblPESampleDist = new QLabel(partExtractorParamsContent);
        lblPESampleDist->setObjectName(QStringLiteral("lblPESampleDist"));
        lblPESampleDist->setMinimumSize(QSize(75, 0));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, lblPESampleDist);

        sbPESampleDist = new QDoubleSpinBox(partExtractorParamsContent);
        sbPESampleDist->setObjectName(QStringLiteral("sbPESampleDist"));
        sbPESampleDist->setMinimum(0.01);
        sbPESampleDist->setMaximum(100);
        sbPESampleDist->setValue(2.5);

        formLayout_7->setWidget(0, QFormLayout::FieldRole, sbPESampleDist);

        lblPENIter = new QLabel(partExtractorParamsContent);
        lblPENIter->setObjectName(QStringLiteral("lblPENIter"));
        lblPENIter->setMinimumSize(QSize(75, 0));

        formLayout_7->setWidget(2, QFormLayout::LabelRole, lblPENIter);

        sbPENIter = new QSpinBox(partExtractorParamsContent);
        sbPENIter->setObjectName(QStringLiteral("sbPENIter"));
        sbPENIter->setMinimum(1);
        sbPENIter->setMaximum(10000);
        sbPENIter->setValue(1);

        formLayout_7->setWidget(2, QFormLayout::FieldRole, sbPENIter);

        lblPESearchRadius = new QLabel(partExtractorParamsContent);
        lblPESearchRadius->setObjectName(QStringLiteral("lblPESearchRadius"));

        formLayout_7->setWidget(1, QFormLayout::LabelRole, lblPESearchRadius);

        sbPESearchRadius = new QDoubleSpinBox(partExtractorParamsContent);
        sbPESearchRadius->setObjectName(QStringLiteral("sbPESearchRadius"));
        sbPESearchRadius->setMinimum(0.1);
        sbPESearchRadius->setMaximum(10000);
        sbPESearchRadius->setSingleStep(0.5);
        sbPESearchRadius->setValue(5);

        formLayout_7->setWidget(1, QFormLayout::FieldRole, sbPESearchRadius);

        lblPEThreshold = new QLabel(partExtractorParamsContent);
        lblPEThreshold->setObjectName(QStringLiteral("lblPEThreshold"));

        formLayout_7->setWidget(3, QFormLayout::LabelRole, lblPEThreshold);

        sbPEThreshold = new QDoubleSpinBox(partExtractorParamsContent);
        sbPEThreshold->setObjectName(QStringLiteral("sbPEThreshold"));
        sbPEThreshold->setMaximum(10000);
        sbPEThreshold->setValue(2.5);

        formLayout_7->setWidget(3, QFormLayout::FieldRole, sbPEThreshold);

        partExtractorParamsDock->setWidget(partExtractorParamsContent);
        CVUserInterfaceClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), partExtractorParamsDock);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuView_2->menuAction());
        menuFile->addAction(openFileButton);
        menuFile->addSeparator();
        menuFile->addAction(openMyObjButton);
        menuFile->addSeparator();
        menuFile->addAction(saveFileButton);
        menuView->addAction(gaborButton);
        menuView->addAction(levelSetButton);
        menuView->addAction(cartoonTextureButton);
        menuView->addAction(segmentationButton);
        menuView->addAction(triangulationParamsButton);
        menuView->addAction(renderingParamsButton);
        menuView->addAction(partExtractionParamsButton);
        menuView_2->addAction(wireframeToggleButton);
        menuView_2->addAction(bbwHandlesToggleButton);
        menuView_2->addAction(bbwPEToggleButton);
        menuView_2->addAction(gpuToggleButton);
        menuView_2->addAction(scRegionsToggleButton);
        menuView_2->addAction(usScribblesToggleButton);
        mainToolBar->addAction(lsSegmButton);
        mainToolBar->addAction(ctSegmButton);
        mainToolBar->addAction(triangulationButton);
        mainToolBar->addAction(sVectorizeButton);
        mainToolBar->addAction(bindHandlesButton);
        mainToolBar->addAction(extractPartButton);
        mainToolBar->addAction(partsTriangulationButton);

        retranslateUi(CVUserInterfaceClass);

        QMetaObject::connectSlotsByName(CVUserInterfaceClass);
    } // setupUi

    void retranslateUi(QMainWindow *CVUserInterfaceClass)
    {
        CVUserInterfaceClass->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Comics Vectorizer", 0));
        openFileButton->setText(QApplication::translate("CVUserInterfaceClass", "Open image", 0));
        saveFileButton->setText(QApplication::translate("CVUserInterfaceClass", "Save", 0));
        lsSegmButton->setText(QApplication::translate("CVUserInterfaceClass", "Level Set Segmentation", 0));
        triangulationButton->setText(QApplication::translate("CVUserInterfaceClass", "Triangulation", 0));
        gaborButton->setText(QApplication::translate("CVUserInterfaceClass", "Gabor Wavelet", 0));
        gaborButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+1", 0));
        levelSetButton->setText(QApplication::translate("CVUserInterfaceClass", "Level Set", 0));
        levelSetButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+2", 0));
        cartoonTextureButton->setText(QApplication::translate("CVUserInterfaceClass", "Cartoon+Texture", 0));
        cartoonTextureButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+5", 0));
        renderingParamsButton->setText(QApplication::translate("CVUserInterfaceClass", "Rendering", 0));
        renderingParamsButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+4", 0));
        segmentationButton->setText(QApplication::translate("CVUserInterfaceClass", "Segmentation", 0));
        segmentationButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+3", 0));
        sVectorizeButton->setText(QApplication::translate("CVUserInterfaceClass", "Vectorize Screentone", 0));
        ctSegmButton->setText(QApplication::translate("CVUserInterfaceClass", "Cartoon+Texture Filter", 0));
        ctSegmButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+C", 0));
        triangulationParamsButton->setText(QApplication::translate("CVUserInterfaceClass", "Triangulation", 0));
        triangulationParamsButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+6", 0));
        bindHandlesButton->setText(QApplication::translate("CVUserInterfaceClass", "Bind Handles", 0));
        bindHandlesButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+B", 0));
        wireframeToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "Wireframe", 0));
        wireframeToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "W", 0));
        bbwHandlesToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "BBW Handles", 0));
        bbwHandlesToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "H", 0));
        openMyObjButton->setText(QApplication::translate("CVUserInterfaceClass", "Open .myobj", 0));
        bbwPEToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "BBW Pseudo Edges", 0));
        bbwPEToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "P", 0));
        scRegionsToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "Screentone Regions", 0));
        scRegionsToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "S", 0));
        gpuToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "GPU", 0));
        gpuToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "G", 0));
        usScribblesToggleButton->setText(QApplication::translate("CVUserInterfaceClass", "Users Scribbles", 0));
        usScribblesToggleButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "U", 0));
        partExtractionParamsButton->setText(QApplication::translate("CVUserInterfaceClass", "Part Extraction", 0));
        partExtractionParamsButton->setShortcut(QApplication::translate("CVUserInterfaceClass", "Ctrl+6", 0));
        extractPartButton->setText(QApplication::translate("CVUserInterfaceClass", "Extract Part", 0));
#ifndef QT_NO_TOOLTIP
        extractPartButton->setToolTip(QApplication::translate("CVUserInterfaceClass", "Extract Part", 0));
#endif // QT_NO_TOOLTIP
        partsTriangulationButton->setText(QApplication::translate("CVUserInterfaceClass", "Parts Triangulation", 0));
        menuFile->setTitle(QApplication::translate("CVUserInterfaceClass", "File", 0));
        menuView->setTitle(QApplication::translate("CVUserInterfaceClass", "Parameters", 0));
        menuView_2->setTitle(QApplication::translate("CVUserInterfaceClass", "View", 0));
        gaborParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Gabor Wavelet", 0));
        lblGScale->setText(QApplication::translate("CVUserInterfaceClass", "Scale", 0));
        lblGOrientation->setText(QApplication::translate("CVUserInterfaceClass", "Orientation", 0));
        lblGWindow->setText(QApplication::translate("CVUserInterfaceClass", "Window", 0));
        lblGMinFreq->setText(QApplication::translate("CVUserInterfaceClass", "Min f", 0));
        lblGMaxFreq->setText(QApplication::translate("CVUserInterfaceClass", "Max f", 0));
        lblGSide->setText(QApplication::translate("CVUserInterfaceClass", "Side", 0));
        cbGFlag->setText(QApplication::translate("CVUserInterfaceClass", "Flag", 0));
        levelSetParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Level Set", 0));
        lblLSTolerance->setText(QApplication::translate("CVUserInterfaceClass", "Tolerance", 0));
        lblLSMaxIter->setText(QApplication::translate("CVUserInterfaceClass", "Max Iteration", 0));
        lblLSMu->setText(QApplication::translate("CVUserInterfaceClass", "\316\274", 0));
        lblLSNu->setText(QApplication::translate("CVUserInterfaceClass", "\316\275", 0));
        lblLSLambda1->setText(QApplication::translate("CVUserInterfaceClass", "\316\2731", 0));
        lblLSLambda2->setText(QApplication::translate("CVUserInterfaceClass", "\316\2732", 0));
        lblLSDistanceTime->setText(QApplication::translate("CVUserInterfaceClass", "Distance Time", 0));
        segmentationParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Segmentation", 0));
        lblSNCluster->setText(QApplication::translate("CVUserInterfaceClass", "N Cluster", 0));
        lblSMaxIter->setText(QApplication::translate("CVUserInterfaceClass", "Max Iteration", 0));
        lblSCutoff->setText(QApplication::translate("CVUserInterfaceClass", "Cut off", 0));
        cbSNormalize->setText(QApplication::translate("CVUserInterfaceClass", "Normalize", 0));
        lblSMinArea->setText(QApplication::translate("CVUserInterfaceClass", "Min Area", 0));
        renderingParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Rendering", 0));
        label->setText(QApplication::translate("CVUserInterfaceClass", "Screentone", 0));
        lblSTAA->setText(QApplication::translate("CVUserInterfaceClass", "Anti Aliasing", 0));
        lblSTScale->setText(QApplication::translate("CVUserInterfaceClass", "Scale", 0));
        lblSTOrientation->setText(QApplication::translate("CVUserInterfaceClass", "Orientation", 0));
        lblSTBlackness->setText(QApplication::translate("CVUserInterfaceClass", "Grayscale", 0));
        label_2->setText(QApplication::translate("CVUserInterfaceClass", "Bezier Curve", 0));
        label_3->setText(QApplication::translate("CVUserInterfaceClass", "Grayscale", 0));
        cbSTFixed->setText(QApplication::translate("CVUserInterfaceClass", "Fixed", 0));
        label_4->setText(QApplication::translate("CVUserInterfaceClass", "Blur", 0));
        cartoonTextureParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Cartoon+Texture", 0));
        lblCTSigma->setText(QApplication::translate("CVUserInterfaceClass", "Filter \317\203", 0));
        triangulationParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Triangulation", 0));
        lblTRCSSSigma->setText(QApplication::translate("CVUserInterfaceClass", "CSS \317\203", 0));
        lblTRScale->setText(QApplication::translate("CVUserInterfaceClass", "Scale", 0));
        lblTRRDPEpsilon->setText(QApplication::translate("CVUserInterfaceClass", "RDP \316\265", 0));
        lblTREdgeLength->setText(QApplication::translate("CVUserInterfaceClass", "Edge Length", 0));
        partExtractorParamsDock->setWindowTitle(QApplication::translate("CVUserInterfaceClass", "Part Extractor", 0));
        lblPESampleDist->setText(QApplication::translate("CVUserInterfaceClass", "Sample Dist.", 0));
        lblPENIter->setText(QApplication::translate("CVUserInterfaceClass", "N iteration", 0));
        lblPESearchRadius->setText(QApplication::translate("CVUserInterfaceClass", "Search Radius", 0));
        lblPEThreshold->setText(QApplication::translate("CVUserInterfaceClass", "Threshold", 0));
    } // retranslateUi

};

namespace Ui {
    class CVUserInterfaceClass: public Ui_CVUserInterfaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVUSERINTERFACE_H
