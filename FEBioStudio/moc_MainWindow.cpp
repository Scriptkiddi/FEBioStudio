/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CMainWindow_t {
    QByteArrayData data[256];
    char stringdata0[6019];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMainWindow_t qt_meta_stringdata_CMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CMainWindow"
QT_MOC_LITERAL(1, 12, 27), // "on_actionNewModel_triggered"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 29), // "on_actionNewProject_triggered"
QT_MOC_LITERAL(4, 71, 30), // "on_actionOpenProject_triggered"
QT_MOC_LITERAL(5, 102, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(6, 126, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(7, 150, 25), // "on_actionSaveAs_triggered"
QT_MOC_LITERAL(8, 176, 26), // "on_actionSaveAll_triggered"
QT_MOC_LITERAL(9, 203, 27), // "on_actionCloseAll_triggered"
QT_MOC_LITERAL(10, 231, 27), // "on_actionSnapShot_triggered"
QT_MOC_LITERAL(11, 259, 30), // "on_actionSaveProject_triggered"
QT_MOC_LITERAL(12, 290, 32), // "on_actionExportFEModel_triggered"
QT_MOC_LITERAL(13, 323, 33), // "on_actionImportGeometry_trigg..."
QT_MOC_LITERAL(14, 357, 33), // "on_actionExportGeometry_trigg..."
QT_MOC_LITERAL(15, 391, 32), // "on_actionImportProject_triggered"
QT_MOC_LITERAL(16, 424, 32), // "on_actionExportProject_triggered"
QT_MOC_LITERAL(17, 457, 30), // "on_actionImportImage_triggered"
QT_MOC_LITERAL(18, 488, 29), // "on_actionConvertFeb_triggered"
QT_MOC_LITERAL(19, 518, 29), // "on_actionConvertGeo_triggered"
QT_MOC_LITERAL(20, 548, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(21, 572, 24), // "on_recentFiles_triggered"
QT_MOC_LITERAL(22, 597, 8), // "QAction*"
QT_MOC_LITERAL(23, 606, 6), // "action"
QT_MOC_LITERAL(24, 613, 26), // "on_recentFEFiles_triggered"
QT_MOC_LITERAL(25, 640, 28), // "on_recentGeomFiles_triggered"
QT_MOC_LITERAL(26, 669, 23), // "on_actionUndo_triggered"
QT_MOC_LITERAL(27, 693, 23), // "on_actionRedo_triggered"
QT_MOC_LITERAL(28, 717, 34), // "on_actionInvertSelection_trig..."
QT_MOC_LITERAL(29, 752, 33), // "on_actionClearSelection_trigg..."
QT_MOC_LITERAL(30, 786, 34), // "on_actionDeleteSelection_trig..."
QT_MOC_LITERAL(31, 821, 32), // "on_actionHideSelection_triggered"
QT_MOC_LITERAL(32, 854, 33), // "on_actionHideUnselected_trigg..."
QT_MOC_LITERAL(33, 888, 28), // "on_actionUnhideAll_triggered"
QT_MOC_LITERAL(34, 917, 23), // "on_actionFind_triggered"
QT_MOC_LITERAL(35, 941, 30), // "on_actionSelectRange_triggered"
QT_MOC_LITERAL(36, 972, 32), // "on_actionToggleVisible_triggered"
QT_MOC_LITERAL(37, 1005, 32), // "on_actionNameSelection_triggered"
QT_MOC_LITERAL(38, 1038, 28), // "on_actionTransform_triggered"
QT_MOC_LITERAL(39, 1067, 36), // "on_actionCollapseTransform_tr..."
QT_MOC_LITERAL(40, 1104, 24), // "on_actionClone_triggered"
QT_MOC_LITERAL(41, 1129, 28), // "on_actionCloneGrid_triggered"
QT_MOC_LITERAL(42, 1158, 31), // "on_actionCloneRevolve_triggered"
QT_MOC_LITERAL(43, 1190, 24), // "on_actionMerge_triggered"
QT_MOC_LITERAL(44, 1215, 24), // "on_actionPurge_triggered"
QT_MOC_LITERAL(45, 1240, 25), // "on_actionDetach_triggered"
QT_MOC_LITERAL(46, 1266, 26), // "on_actionExtract_triggered"
QT_MOC_LITERAL(47, 1293, 30), // "on_actionEditProject_triggered"
QT_MOC_LITERAL(48, 1324, 29), // "on_actionFaceToElem_triggered"
QT_MOC_LITERAL(49, 1354, 32), // "on_actionSelectOverlap_triggered"
QT_MOC_LITERAL(50, 1387, 24), // "on_actionAddBC_triggered"
QT_MOC_LITERAL(51, 1412, 31), // "on_actionAddNodalLoad_triggered"
QT_MOC_LITERAL(52, 1444, 30), // "on_actionAddSurfLoad_triggered"
QT_MOC_LITERAL(53, 1475, 30), // "on_actionAddBodyLoad_triggered"
QT_MOC_LITERAL(54, 1506, 24), // "on_actionAddIC_triggered"
QT_MOC_LITERAL(55, 1531, 29), // "on_actionAddContact_triggered"
QT_MOC_LITERAL(56, 1561, 32), // "on_actionAddConstraint_triggered"
QT_MOC_LITERAL(57, 1594, 37), // "on_actionAddRigidConstraint_t..."
QT_MOC_LITERAL(58, 1632, 36), // "on_actionAddRigidConnector_tr..."
QT_MOC_LITERAL(59, 1669, 30), // "on_actionAddMaterial_triggered"
QT_MOC_LITERAL(60, 1700, 26), // "on_actionAddStep_triggered"
QT_MOC_LITERAL(61, 1727, 30), // "on_actionAddReaction_triggered"
QT_MOC_LITERAL(62, 1758, 30), // "on_actionSoluteTable_triggered"
QT_MOC_LITERAL(63, 1789, 27), // "on_actionSBMTable_triggered"
QT_MOC_LITERAL(64, 1817, 30), // "on_actionCurveEditor_triggered"
QT_MOC_LITERAL(65, 1848, 32), // "on_actionMeshInspector_triggered"
QT_MOC_LITERAL(66, 1881, 38), // "on_actionElasticityConvertor_..."
QT_MOC_LITERAL(67, 1920, 32), // "on_actionUnitConverter_triggered"
QT_MOC_LITERAL(68, 1953, 26), // "on_actionKinemat_triggered"
QT_MOC_LITERAL(69, 1980, 26), // "on_actionPlotMix_triggered"
QT_MOC_LITERAL(70, 2007, 27), // "on_actionFEBioRun_triggered"
QT_MOC_LITERAL(71, 2035, 28), // "on_actionFEBioStop_triggered"
QT_MOC_LITERAL(72, 2064, 32), // "on_actionFEBioOptimize_triggered"
QT_MOC_LITERAL(73, 2097, 26), // "on_actionOptions_triggered"
QT_MOC_LITERAL(74, 2124, 28), // "on_actionLayerInfo_triggered"
QT_MOC_LITERAL(75, 2153, 27), // "on_actionPlaneCut_triggered"
QT_MOC_LITERAL(76, 2181, 30), // "on_actionMirrorPlane_triggered"
QT_MOC_LITERAL(77, 2212, 29), // "on_actionVectorPlot_triggered"
QT_MOC_LITERAL(78, 2242, 29), // "on_actionTensorPlot_triggered"
QT_MOC_LITERAL(79, 2272, 33), // "on_actionIsosurfacePlot_trigg..."
QT_MOC_LITERAL(80, 2306, 28), // "on_actionSlicePlot_triggered"
QT_MOC_LITERAL(81, 2335, 34), // "on_actionDisplacementMap_trig..."
QT_MOC_LITERAL(82, 2370, 33), // "on_actionStreamLinePlot_trigg..."
QT_MOC_LITERAL(83, 2404, 35), // "on_actionParticleFlowPlot_tri..."
QT_MOC_LITERAL(84, 2440, 33), // "on_actionVolumeFlowPlot_trigg..."
QT_MOC_LITERAL(85, 2474, 30), // "on_actionImageSlicer_triggered"
QT_MOC_LITERAL(86, 2505, 31), // "on_actionVolumeRender_triggered"
QT_MOC_LITERAL(87, 2537, 32), // "on_actionMarchingCubes_triggered"
QT_MOC_LITERAL(88, 2570, 24), // "on_actionGraph_triggered"
QT_MOC_LITERAL(89, 2595, 26), // "on_actionSummary_triggered"
QT_MOC_LITERAL(90, 2622, 24), // "on_actionStats_triggered"
QT_MOC_LITERAL(91, 2647, 28), // "on_actionIntegrate_triggered"
QT_MOC_LITERAL(92, 2676, 31), // "on_actionImportPoints_triggered"
QT_MOC_LITERAL(93, 2708, 30), // "on_actionImportLines_triggered"
QT_MOC_LITERAL(94, 2739, 28), // "on_actionRecordNew_triggered"
QT_MOC_LITERAL(95, 2768, 30), // "on_actionRecordStart_triggered"
QT_MOC_LITERAL(96, 2799, 30), // "on_actionRecordPause_triggered"
QT_MOC_LITERAL(97, 2830, 29), // "on_actionRecordStop_triggered"
QT_MOC_LITERAL(98, 2860, 33), // "on_actionUndoViewChange_trigg..."
QT_MOC_LITERAL(99, 2894, 33), // "on_actionRedoViewChange_trigg..."
QT_MOC_LITERAL(100, 2928, 29), // "on_actionZoomSelect_triggered"
QT_MOC_LITERAL(101, 2958, 30), // "on_actionZoomExtents_triggered"
QT_MOC_LITERAL(102, 2989, 28), // "on_actionViewCapture_toggled"
QT_MOC_LITERAL(103, 3018, 8), // "bchecked"
QT_MOC_LITERAL(104, 3027, 22), // "on_actionOrtho_toggled"
QT_MOC_LITERAL(105, 3050, 1), // "b"
QT_MOC_LITERAL(106, 3052, 25), // "on_actionShowGrid_toggled"
QT_MOC_LITERAL(107, 3078, 30), // "on_actionShowMeshLines_toggled"
QT_MOC_LITERAL(108, 3109, 30), // "on_actionShowEdgeLines_toggled"
QT_MOC_LITERAL(109, 3140, 32), // "on_actionBackfaceCulling_toggled"
QT_MOC_LITERAL(110, 3173, 27), // "on_actionViewSmooth_toggled"
QT_MOC_LITERAL(111, 3201, 28), // "on_actionShowNormals_toggled"
QT_MOC_LITERAL(112, 3230, 27), // "on_actionShowFibers_toggled"
QT_MOC_LITERAL(113, 3258, 28), // "on_actionShowMatAxes_toggled"
QT_MOC_LITERAL(114, 3287, 29), // "on_actionShowDiscrete_toggled"
QT_MOC_LITERAL(115, 3317, 30), // "on_actionToggleLight_triggered"
QT_MOC_LITERAL(116, 3348, 24), // "on_actionFront_triggered"
QT_MOC_LITERAL(117, 3373, 23), // "on_actionBack_triggered"
QT_MOC_LITERAL(118, 3397, 23), // "on_actionLeft_triggered"
QT_MOC_LITERAL(119, 3421, 24), // "on_actionRight_triggered"
QT_MOC_LITERAL(120, 3446, 22), // "on_actionTop_triggered"
QT_MOC_LITERAL(121, 3469, 25), // "on_actionBottom_triggered"
QT_MOC_LITERAL(122, 3495, 26), // "on_actionWireframe_toggled"
QT_MOC_LITERAL(123, 3522, 25), // "on_actionSnap3D_triggered"
QT_MOC_LITERAL(124, 3548, 22), // "on_actionTrack_toggled"
QT_MOC_LITERAL(125, 3571, 29), // "on_actionViewVPSave_triggered"
QT_MOC_LITERAL(126, 3601, 29), // "on_actionViewVPPrev_triggered"
QT_MOC_LITERAL(127, 3631, 29), // "on_actionViewVPNext_triggered"
QT_MOC_LITERAL(128, 3661, 28), // "on_actionSyncViews_triggered"
QT_MOC_LITERAL(129, 3690, 34), // "on_actionToggleConnected_trig..."
QT_MOC_LITERAL(130, 3725, 27), // "on_actionFEBioURL_triggered"
QT_MOC_LITERAL(131, 3753, 29), // "on_actionFEBioForum_triggered"
QT_MOC_LITERAL(132, 3783, 33), // "on_actionFEBioResources_trigg..."
QT_MOC_LITERAL(133, 3817, 28), // "on_actionFEBioPubs_triggered"
QT_MOC_LITERAL(134, 3846, 24), // "on_actionAbout_triggered"
QT_MOC_LITERAL(135, 3871, 23), // "on_actionSelect_toggled"
QT_MOC_LITERAL(136, 3895, 26), // "on_actionTranslate_toggled"
QT_MOC_LITERAL(137, 3922, 23), // "on_actionRotate_toggled"
QT_MOC_LITERAL(138, 3946, 22), // "on_actionScale_toggled"
QT_MOC_LITERAL(139, 3969, 34), // "on_selectCoord_currentIndexCh..."
QT_MOC_LITERAL(140, 4004, 1), // "n"
QT_MOC_LITERAL(141, 4006, 30), // "on_actionSelectObjects_toggled"
QT_MOC_LITERAL(142, 4037, 28), // "on_actionSelectParts_toggled"
QT_MOC_LITERAL(143, 4066, 31), // "on_actionSelectSurfaces_toggled"
QT_MOC_LITERAL(144, 4098, 29), // "on_actionSelectCurves_toggled"
QT_MOC_LITERAL(145, 4128, 28), // "on_actionSelectNodes_toggled"
QT_MOC_LITERAL(146, 4157, 31), // "on_actionSelectDiscrete_toggled"
QT_MOC_LITERAL(147, 4189, 21), // "on_selectRect_toggled"
QT_MOC_LITERAL(148, 4211, 23), // "on_selectCircle_toggled"
QT_MOC_LITERAL(149, 4235, 21), // "on_selectFree_toggled"
QT_MOC_LITERAL(150, 4257, 30), // "on_actionMeasureTool_triggered"
QT_MOC_LITERAL(151, 4288, 31), // "on_actionPlaneCutTool_triggered"
QT_MOC_LITERAL(152, 4320, 25), // "on_postSelectRect_toggled"
QT_MOC_LITERAL(153, 4346, 27), // "on_postSelectCircle_toggled"
QT_MOC_LITERAL(154, 4374, 25), // "on_postSelectFree_toggled"
QT_MOC_LITERAL(155, 4400, 34), // "on_postActionMeasureTool_trig..."
QT_MOC_LITERAL(156, 4435, 33), // "on_selectData_currentValueCha..."
QT_MOC_LITERAL(157, 4469, 1), // "i"
QT_MOC_LITERAL(158, 4471, 21), // "on_actionPlay_toggled"
QT_MOC_LITERAL(159, 4493, 26), // "on_actionRefresh_triggered"
QT_MOC_LITERAL(160, 4520, 24), // "on_actionFirst_triggered"
QT_MOC_LITERAL(161, 4545, 23), // "on_actionPrev_triggered"
QT_MOC_LITERAL(162, 4569, 23), // "on_actionNext_triggered"
QT_MOC_LITERAL(163, 4593, 23), // "on_actionLast_triggered"
QT_MOC_LITERAL(164, 4617, 31), // "on_actionTimeSettings_triggered"
QT_MOC_LITERAL(165, 4649, 25), // "on_actionColorMap_toggled"
QT_MOC_LITERAL(166, 4675, 26), // "on_selectTime_valueChanged"
QT_MOC_LITERAL(167, 4702, 31), // "on_fontStyle_currentFontChanged"
QT_MOC_LITERAL(168, 4734, 4), // "font"
QT_MOC_LITERAL(169, 4739, 24), // "on_fontSize_valueChanged"
QT_MOC_LITERAL(170, 4764, 19), // "on_fontBold_toggled"
QT_MOC_LITERAL(171, 4784, 7), // "checked"
QT_MOC_LITERAL(172, 4792, 21), // "on_fontItalic_toggled"
QT_MOC_LITERAL(173, 4814, 29), // "on_actionProperties_triggered"
QT_MOC_LITERAL(174, 4844, 21), // "on_tab_currentChanged"
QT_MOC_LITERAL(175, 4866, 24), // "on_tab_tabCloseRequested"
QT_MOC_LITERAL(176, 4891, 24), // "on_welcome_anchorClicked"
QT_MOC_LITERAL(177, 4916, 4), // "link"
QT_MOC_LITERAL(178, 4921, 15), // "on_clearProject"
QT_MOC_LITERAL(179, 4937, 15), // "on_closeProject"
QT_MOC_LITERAL(180, 4953, 12), // "on_closeFile"
QT_MOC_LITERAL(181, 4966, 4), // "file"
QT_MOC_LITERAL(182, 4971, 15), // "on_addToProject"
QT_MOC_LITERAL(183, 4987, 24), // "OnPostObjectStateChanged"
QT_MOC_LITERAL(184, 5012, 24), // "OnPostObjectPropsChanged"
QT_MOC_LITERAL(185, 5037, 9), // "FSObject*"
QT_MOC_LITERAL(186, 5047, 2), // "po"
QT_MOC_LITERAL(187, 5050, 35), // "on_modelViewer_currentObjectC..."
QT_MOC_LITERAL(188, 5086, 17), // "OnSelectMeshLayer"
QT_MOC_LITERAL(189, 5104, 2), // "ac"
QT_MOC_LITERAL(190, 5107, 30), // "OnSelectObjectTransparencyMode"
QT_MOC_LITERAL(191, 5138, 9), // "CloseView"
QT_MOC_LITERAL(192, 5148, 10), // "forceClose"
QT_MOC_LITERAL(193, 5159, 10), // "CDocument*"
QT_MOC_LITERAL(194, 5170, 3), // "doc"
QT_MOC_LITERAL(195, 5174, 15), // "SetCurrentState"
QT_MOC_LITERAL(196, 5190, 10), // "closeEvent"
QT_MOC_LITERAL(197, 5201, 12), // "QCloseEvent*"
QT_MOC_LITERAL(198, 5214, 2), // "ev"
QT_MOC_LITERAL(199, 5217, 13), // "keyPressEvent"
QT_MOC_LITERAL(200, 5231, 10), // "QKeyEvent*"
QT_MOC_LITERAL(201, 5242, 22), // "on_finishedReadingFile"
QT_MOC_LITERAL(202, 5265, 7), // "success"
QT_MOC_LITERAL(203, 5273, 11), // "errorString"
QT_MOC_LITERAL(204, 5285, 19), // "finishedReadingFile"
QT_MOC_LITERAL(205, 5305, 11), // "QueuedFile&"
QT_MOC_LITERAL(206, 5317, 5), // "qfile"
QT_MOC_LITERAL(207, 5323, 17), // "checkFileProgress"
QT_MOC_LITERAL(208, 5341, 13), // "StopAnimation"
QT_MOC_LITERAL(209, 5355, 7), // "onTimer"
QT_MOC_LITERAL(210, 5363, 21), // "on_glview_pointPicked"
QT_MOC_LITERAL(211, 5385, 5), // "vec3d"
QT_MOC_LITERAL(212, 5391, 1), // "r"
QT_MOC_LITERAL(213, 5393, 26), // "on_glview_selectionChanged"
QT_MOC_LITERAL(214, 5420, 13), // "onRunFinished"
QT_MOC_LITERAL(215, 5434, 8), // "exitCode"
QT_MOC_LITERAL(216, 5443, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(217, 5464, 2), // "es"
QT_MOC_LITERAL(218, 5467, 11), // "onReadyRead"
QT_MOC_LITERAL(219, 5479, 15), // "onErrorOccurred"
QT_MOC_LITERAL(220, 5495, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(221, 5518, 3), // "err"
QT_MOC_LITERAL(222, 5522, 17), // "onExportMaterials"
QT_MOC_LITERAL(223, 5540, 18), // "vector<GMaterial*>"
QT_MOC_LITERAL(224, 5559, 7), // "matList"
QT_MOC_LITERAL(225, 5567, 20), // "onExportAllMaterials"
QT_MOC_LITERAL(226, 5588, 17), // "onImportMaterials"
QT_MOC_LITERAL(227, 5606, 18), // "DeleteAllMaterials"
QT_MOC_LITERAL(228, 5625, 11), // "DeleteAllBC"
QT_MOC_LITERAL(229, 5637, 14), // "DeleteAllLoads"
QT_MOC_LITERAL(230, 5652, 11), // "DeleteAllIC"
QT_MOC_LITERAL(231, 5664, 16), // "DeleteAllContact"
QT_MOC_LITERAL(232, 5681, 20), // "DeleteAllConstraints"
QT_MOC_LITERAL(233, 5702, 25), // "DeleteAllRigidConstraints"
QT_MOC_LITERAL(234, 5728, 24), // "DeleteAllRigidConnectors"
QT_MOC_LITERAL(235, 5753, 14), // "DeleteAllSteps"
QT_MOC_LITERAL(236, 5768, 9), // "GetGLView"
QT_MOC_LITERAL(237, 5778, 8), // "CGLView*"
QT_MOC_LITERAL(238, 5787, 15), // "GetCurrentModel"
QT_MOC_LITERAL(239, 5803, 15), // "Post::CGLModel*"
QT_MOC_LITERAL(240, 5819, 17), // "UpdateFontToolbar"
QT_MOC_LITERAL(241, 5837, 11), // "RunFEBioJob"
QT_MOC_LITERAL(242, 5849, 10), // "CFEBioJob*"
QT_MOC_LITERAL(243, 5860, 3), // "job"
QT_MOC_LITERAL(244, 5864, 8), // "autoSave"
QT_MOC_LITERAL(245, 5873, 15), // "NextSSHFunction"
QT_MOC_LITERAL(246, 5889, 12), // "CSSHHandler*"
QT_MOC_LITERAL(247, 5902, 12), // "ShowProgress"
QT_MOC_LITERAL(248, 5915, 4), // "show"
QT_MOC_LITERAL(249, 5920, 7), // "message"
QT_MOC_LITERAL(250, 5928, 25), // "ShowIndeterminateProgress"
QT_MOC_LITERAL(251, 5954, 14), // "UpdateProgress"
QT_MOC_LITERAL(252, 5969, 12), // "DoModelCheck"
QT_MOC_LITERAL(253, 5982, 15), // "CModelDocument*"
QT_MOC_LITERAL(254, 5998, 11), // "toggleOrtho"
QT_MOC_LITERAL(255, 6010, 8) // "autosave"

    },
    "CMainWindow\0on_actionNewModel_triggered\0"
    "\0on_actionNewProject_triggered\0"
    "on_actionOpenProject_triggered\0"
    "on_actionOpen_triggered\0on_actionSave_triggered\0"
    "on_actionSaveAs_triggered\0"
    "on_actionSaveAll_triggered\0"
    "on_actionCloseAll_triggered\0"
    "on_actionSnapShot_triggered\0"
    "on_actionSaveProject_triggered\0"
    "on_actionExportFEModel_triggered\0"
    "on_actionImportGeometry_triggered\0"
    "on_actionExportGeometry_triggered\0"
    "on_actionImportProject_triggered\0"
    "on_actionExportProject_triggered\0"
    "on_actionImportImage_triggered\0"
    "on_actionConvertFeb_triggered\0"
    "on_actionConvertGeo_triggered\0"
    "on_actionExit_triggered\0"
    "on_recentFiles_triggered\0QAction*\0"
    "action\0on_recentFEFiles_triggered\0"
    "on_recentGeomFiles_triggered\0"
    "on_actionUndo_triggered\0on_actionRedo_triggered\0"
    "on_actionInvertSelection_triggered\0"
    "on_actionClearSelection_triggered\0"
    "on_actionDeleteSelection_triggered\0"
    "on_actionHideSelection_triggered\0"
    "on_actionHideUnselected_triggered\0"
    "on_actionUnhideAll_triggered\0"
    "on_actionFind_triggered\0"
    "on_actionSelectRange_triggered\0"
    "on_actionToggleVisible_triggered\0"
    "on_actionNameSelection_triggered\0"
    "on_actionTransform_triggered\0"
    "on_actionCollapseTransform_triggered\0"
    "on_actionClone_triggered\0"
    "on_actionCloneGrid_triggered\0"
    "on_actionCloneRevolve_triggered\0"
    "on_actionMerge_triggered\0"
    "on_actionPurge_triggered\0"
    "on_actionDetach_triggered\0"
    "on_actionExtract_triggered\0"
    "on_actionEditProject_triggered\0"
    "on_actionFaceToElem_triggered\0"
    "on_actionSelectOverlap_triggered\0"
    "on_actionAddBC_triggered\0"
    "on_actionAddNodalLoad_triggered\0"
    "on_actionAddSurfLoad_triggered\0"
    "on_actionAddBodyLoad_triggered\0"
    "on_actionAddIC_triggered\0"
    "on_actionAddContact_triggered\0"
    "on_actionAddConstraint_triggered\0"
    "on_actionAddRigidConstraint_triggered\0"
    "on_actionAddRigidConnector_triggered\0"
    "on_actionAddMaterial_triggered\0"
    "on_actionAddStep_triggered\0"
    "on_actionAddReaction_triggered\0"
    "on_actionSoluteTable_triggered\0"
    "on_actionSBMTable_triggered\0"
    "on_actionCurveEditor_triggered\0"
    "on_actionMeshInspector_triggered\0"
    "on_actionElasticityConvertor_triggered\0"
    "on_actionUnitConverter_triggered\0"
    "on_actionKinemat_triggered\0"
    "on_actionPlotMix_triggered\0"
    "on_actionFEBioRun_triggered\0"
    "on_actionFEBioStop_triggered\0"
    "on_actionFEBioOptimize_triggered\0"
    "on_actionOptions_triggered\0"
    "on_actionLayerInfo_triggered\0"
    "on_actionPlaneCut_triggered\0"
    "on_actionMirrorPlane_triggered\0"
    "on_actionVectorPlot_triggered\0"
    "on_actionTensorPlot_triggered\0"
    "on_actionIsosurfacePlot_triggered\0"
    "on_actionSlicePlot_triggered\0"
    "on_actionDisplacementMap_triggered\0"
    "on_actionStreamLinePlot_triggered\0"
    "on_actionParticleFlowPlot_triggered\0"
    "on_actionVolumeFlowPlot_triggered\0"
    "on_actionImageSlicer_triggered\0"
    "on_actionVolumeRender_triggered\0"
    "on_actionMarchingCubes_triggered\0"
    "on_actionGraph_triggered\0"
    "on_actionSummary_triggered\0"
    "on_actionStats_triggered\0"
    "on_actionIntegrate_triggered\0"
    "on_actionImportPoints_triggered\0"
    "on_actionImportLines_triggered\0"
    "on_actionRecordNew_triggered\0"
    "on_actionRecordStart_triggered\0"
    "on_actionRecordPause_triggered\0"
    "on_actionRecordStop_triggered\0"
    "on_actionUndoViewChange_triggered\0"
    "on_actionRedoViewChange_triggered\0"
    "on_actionZoomSelect_triggered\0"
    "on_actionZoomExtents_triggered\0"
    "on_actionViewCapture_toggled\0bchecked\0"
    "on_actionOrtho_toggled\0b\0"
    "on_actionShowGrid_toggled\0"
    "on_actionShowMeshLines_toggled\0"
    "on_actionShowEdgeLines_toggled\0"
    "on_actionBackfaceCulling_toggled\0"
    "on_actionViewSmooth_toggled\0"
    "on_actionShowNormals_toggled\0"
    "on_actionShowFibers_toggled\0"
    "on_actionShowMatAxes_toggled\0"
    "on_actionShowDiscrete_toggled\0"
    "on_actionToggleLight_triggered\0"
    "on_actionFront_triggered\0"
    "on_actionBack_triggered\0on_actionLeft_triggered\0"
    "on_actionRight_triggered\0"
    "on_actionTop_triggered\0on_actionBottom_triggered\0"
    "on_actionWireframe_toggled\0"
    "on_actionSnap3D_triggered\0"
    "on_actionTrack_toggled\0"
    "on_actionViewVPSave_triggered\0"
    "on_actionViewVPPrev_triggered\0"
    "on_actionViewVPNext_triggered\0"
    "on_actionSyncViews_triggered\0"
    "on_actionToggleConnected_triggered\0"
    "on_actionFEBioURL_triggered\0"
    "on_actionFEBioForum_triggered\0"
    "on_actionFEBioResources_triggered\0"
    "on_actionFEBioPubs_triggered\0"
    "on_actionAbout_triggered\0"
    "on_actionSelect_toggled\0"
    "on_actionTranslate_toggled\0"
    "on_actionRotate_toggled\0on_actionScale_toggled\0"
    "on_selectCoord_currentIndexChanged\0n\0"
    "on_actionSelectObjects_toggled\0"
    "on_actionSelectParts_toggled\0"
    "on_actionSelectSurfaces_toggled\0"
    "on_actionSelectCurves_toggled\0"
    "on_actionSelectNodes_toggled\0"
    "on_actionSelectDiscrete_toggled\0"
    "on_selectRect_toggled\0on_selectCircle_toggled\0"
    "on_selectFree_toggled\0"
    "on_actionMeasureTool_triggered\0"
    "on_actionPlaneCutTool_triggered\0"
    "on_postSelectRect_toggled\0"
    "on_postSelectCircle_toggled\0"
    "on_postSelectFree_toggled\0"
    "on_postActionMeasureTool_triggered\0"
    "on_selectData_currentValueChanged\0i\0"
    "on_actionPlay_toggled\0on_actionRefresh_triggered\0"
    "on_actionFirst_triggered\0"
    "on_actionPrev_triggered\0on_actionNext_triggered\0"
    "on_actionLast_triggered\0"
    "on_actionTimeSettings_triggered\0"
    "on_actionColorMap_toggled\0"
    "on_selectTime_valueChanged\0"
    "on_fontStyle_currentFontChanged\0font\0"
    "on_fontSize_valueChanged\0on_fontBold_toggled\0"
    "checked\0on_fontItalic_toggled\0"
    "on_actionProperties_triggered\0"
    "on_tab_currentChanged\0on_tab_tabCloseRequested\0"
    "on_welcome_anchorClicked\0link\0"
    "on_clearProject\0on_closeProject\0"
    "on_closeFile\0file\0on_addToProject\0"
    "OnPostObjectStateChanged\0"
    "OnPostObjectPropsChanged\0FSObject*\0"
    "po\0on_modelViewer_currentObjectChanged\0"
    "OnSelectMeshLayer\0ac\0"
    "OnSelectObjectTransparencyMode\0CloseView\0"
    "forceClose\0CDocument*\0doc\0SetCurrentState\0"
    "closeEvent\0QCloseEvent*\0ev\0keyPressEvent\0"
    "QKeyEvent*\0on_finishedReadingFile\0"
    "success\0errorString\0finishedReadingFile\0"
    "QueuedFile&\0qfile\0checkFileProgress\0"
    "StopAnimation\0onTimer\0on_glview_pointPicked\0"
    "vec3d\0r\0on_glview_selectionChanged\0"
    "onRunFinished\0exitCode\0QProcess::ExitStatus\0"
    "es\0onReadyRead\0onErrorOccurred\0"
    "QProcess::ProcessError\0err\0onExportMaterials\0"
    "vector<GMaterial*>\0matList\0"
    "onExportAllMaterials\0onImportMaterials\0"
    "DeleteAllMaterials\0DeleteAllBC\0"
    "DeleteAllLoads\0DeleteAllIC\0DeleteAllContact\0"
    "DeleteAllConstraints\0DeleteAllRigidConstraints\0"
    "DeleteAllRigidConnectors\0DeleteAllSteps\0"
    "GetGLView\0CGLView*\0GetCurrentModel\0"
    "Post::CGLModel*\0UpdateFontToolbar\0"
    "RunFEBioJob\0CFEBioJob*\0job\0autoSave\0"
    "NextSSHFunction\0CSSHHandler*\0ShowProgress\0"
    "show\0message\0ShowIndeterminateProgress\0"
    "UpdateProgress\0DoModelCheck\0CModelDocument*\0"
    "toggleOrtho\0autosave"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     218,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0, 1104,    2, 0x0a /* Public */,
       3,    0, 1105,    2, 0x0a /* Public */,
       4,    0, 1106,    2, 0x0a /* Public */,
       5,    0, 1107,    2, 0x0a /* Public */,
       6,    0, 1108,    2, 0x0a /* Public */,
       7,    0, 1109,    2, 0x0a /* Public */,
       8,    0, 1110,    2, 0x0a /* Public */,
       9,    0, 1111,    2, 0x0a /* Public */,
      10,    0, 1112,    2, 0x0a /* Public */,
      11,    0, 1113,    2, 0x0a /* Public */,
      12,    0, 1114,    2, 0x0a /* Public */,
      13,    0, 1115,    2, 0x0a /* Public */,
      14,    0, 1116,    2, 0x0a /* Public */,
      15,    0, 1117,    2, 0x0a /* Public */,
      16,    0, 1118,    2, 0x0a /* Public */,
      17,    0, 1119,    2, 0x0a /* Public */,
      18,    0, 1120,    2, 0x0a /* Public */,
      19,    0, 1121,    2, 0x0a /* Public */,
      20,    0, 1122,    2, 0x0a /* Public */,
      21,    1, 1123,    2, 0x0a /* Public */,
      24,    1, 1126,    2, 0x0a /* Public */,
      25,    1, 1129,    2, 0x0a /* Public */,
      26,    0, 1132,    2, 0x0a /* Public */,
      27,    0, 1133,    2, 0x0a /* Public */,
      28,    0, 1134,    2, 0x0a /* Public */,
      29,    0, 1135,    2, 0x0a /* Public */,
      30,    0, 1136,    2, 0x0a /* Public */,
      31,    0, 1137,    2, 0x0a /* Public */,
      32,    0, 1138,    2, 0x0a /* Public */,
      33,    0, 1139,    2, 0x0a /* Public */,
      34,    0, 1140,    2, 0x0a /* Public */,
      35,    0, 1141,    2, 0x0a /* Public */,
      36,    0, 1142,    2, 0x0a /* Public */,
      37,    0, 1143,    2, 0x0a /* Public */,
      38,    0, 1144,    2, 0x0a /* Public */,
      39,    0, 1145,    2, 0x0a /* Public */,
      40,    0, 1146,    2, 0x0a /* Public */,
      41,    0, 1147,    2, 0x0a /* Public */,
      42,    0, 1148,    2, 0x0a /* Public */,
      43,    0, 1149,    2, 0x0a /* Public */,
      44,    0, 1150,    2, 0x0a /* Public */,
      45,    0, 1151,    2, 0x0a /* Public */,
      46,    0, 1152,    2, 0x0a /* Public */,
      47,    0, 1153,    2, 0x0a /* Public */,
      48,    0, 1154,    2, 0x0a /* Public */,
      49,    0, 1155,    2, 0x0a /* Public */,
      50,    0, 1156,    2, 0x0a /* Public */,
      51,    0, 1157,    2, 0x0a /* Public */,
      52,    0, 1158,    2, 0x0a /* Public */,
      53,    0, 1159,    2, 0x0a /* Public */,
      54,    0, 1160,    2, 0x0a /* Public */,
      55,    0, 1161,    2, 0x0a /* Public */,
      56,    0, 1162,    2, 0x0a /* Public */,
      57,    0, 1163,    2, 0x0a /* Public */,
      58,    0, 1164,    2, 0x0a /* Public */,
      59,    0, 1165,    2, 0x0a /* Public */,
      60,    0, 1166,    2, 0x0a /* Public */,
      61,    0, 1167,    2, 0x0a /* Public */,
      62,    0, 1168,    2, 0x0a /* Public */,
      63,    0, 1169,    2, 0x0a /* Public */,
      64,    0, 1170,    2, 0x0a /* Public */,
      65,    0, 1171,    2, 0x0a /* Public */,
      66,    0, 1172,    2, 0x0a /* Public */,
      67,    0, 1173,    2, 0x0a /* Public */,
      68,    0, 1174,    2, 0x0a /* Public */,
      69,    0, 1175,    2, 0x0a /* Public */,
      70,    0, 1176,    2, 0x0a /* Public */,
      71,    0, 1177,    2, 0x0a /* Public */,
      72,    0, 1178,    2, 0x0a /* Public */,
      73,    0, 1179,    2, 0x0a /* Public */,
      74,    0, 1180,    2, 0x0a /* Public */,
      75,    0, 1181,    2, 0x0a /* Public */,
      76,    0, 1182,    2, 0x0a /* Public */,
      77,    0, 1183,    2, 0x0a /* Public */,
      78,    0, 1184,    2, 0x0a /* Public */,
      79,    0, 1185,    2, 0x0a /* Public */,
      80,    0, 1186,    2, 0x0a /* Public */,
      81,    0, 1187,    2, 0x0a /* Public */,
      82,    0, 1188,    2, 0x0a /* Public */,
      83,    0, 1189,    2, 0x0a /* Public */,
      84,    0, 1190,    2, 0x0a /* Public */,
      85,    0, 1191,    2, 0x0a /* Public */,
      86,    0, 1192,    2, 0x0a /* Public */,
      87,    0, 1193,    2, 0x0a /* Public */,
      88,    0, 1194,    2, 0x0a /* Public */,
      89,    0, 1195,    2, 0x0a /* Public */,
      90,    0, 1196,    2, 0x0a /* Public */,
      91,    0, 1197,    2, 0x0a /* Public */,
      92,    0, 1198,    2, 0x0a /* Public */,
      93,    0, 1199,    2, 0x0a /* Public */,
      94,    0, 1200,    2, 0x0a /* Public */,
      95,    0, 1201,    2, 0x0a /* Public */,
      96,    0, 1202,    2, 0x0a /* Public */,
      97,    0, 1203,    2, 0x0a /* Public */,
      98,    0, 1204,    2, 0x0a /* Public */,
      99,    0, 1205,    2, 0x0a /* Public */,
     100,    0, 1206,    2, 0x0a /* Public */,
     101,    0, 1207,    2, 0x0a /* Public */,
     102,    1, 1208,    2, 0x0a /* Public */,
     104,    1, 1211,    2, 0x0a /* Public */,
     106,    1, 1214,    2, 0x0a /* Public */,
     107,    1, 1217,    2, 0x0a /* Public */,
     108,    1, 1220,    2, 0x0a /* Public */,
     109,    1, 1223,    2, 0x0a /* Public */,
     110,    1, 1226,    2, 0x0a /* Public */,
     111,    1, 1229,    2, 0x0a /* Public */,
     112,    1, 1232,    2, 0x0a /* Public */,
     113,    1, 1235,    2, 0x0a /* Public */,
     114,    1, 1238,    2, 0x0a /* Public */,
     115,    0, 1241,    2, 0x0a /* Public */,
     116,    0, 1242,    2, 0x0a /* Public */,
     117,    0, 1243,    2, 0x0a /* Public */,
     118,    0, 1244,    2, 0x0a /* Public */,
     119,    0, 1245,    2, 0x0a /* Public */,
     120,    0, 1246,    2, 0x0a /* Public */,
     121,    0, 1247,    2, 0x0a /* Public */,
     122,    1, 1248,    2, 0x0a /* Public */,
     123,    0, 1251,    2, 0x0a /* Public */,
     124,    1, 1252,    2, 0x0a /* Public */,
     125,    0, 1255,    2, 0x0a /* Public */,
     126,    0, 1256,    2, 0x0a /* Public */,
     127,    0, 1257,    2, 0x0a /* Public */,
     128,    0, 1258,    2, 0x0a /* Public */,
     129,    0, 1259,    2, 0x0a /* Public */,
     130,    0, 1260,    2, 0x0a /* Public */,
     131,    0, 1261,    2, 0x0a /* Public */,
     132,    0, 1262,    2, 0x0a /* Public */,
     133,    0, 1263,    2, 0x0a /* Public */,
     134,    0, 1264,    2, 0x0a /* Public */,
     135,    1, 1265,    2, 0x0a /* Public */,
     136,    1, 1268,    2, 0x0a /* Public */,
     137,    1, 1271,    2, 0x0a /* Public */,
     138,    1, 1274,    2, 0x0a /* Public */,
     139,    1, 1277,    2, 0x0a /* Public */,
     141,    1, 1280,    2, 0x0a /* Public */,
     142,    1, 1283,    2, 0x0a /* Public */,
     143,    1, 1286,    2, 0x0a /* Public */,
     144,    1, 1289,    2, 0x0a /* Public */,
     145,    1, 1292,    2, 0x0a /* Public */,
     146,    1, 1295,    2, 0x0a /* Public */,
     147,    1, 1298,    2, 0x0a /* Public */,
     148,    1, 1301,    2, 0x0a /* Public */,
     149,    1, 1304,    2, 0x0a /* Public */,
     150,    0, 1307,    2, 0x0a /* Public */,
     151,    0, 1308,    2, 0x0a /* Public */,
     152,    1, 1309,    2, 0x0a /* Public */,
     153,    1, 1312,    2, 0x0a /* Public */,
     154,    1, 1315,    2, 0x0a /* Public */,
     155,    0, 1318,    2, 0x0a /* Public */,
     156,    1, 1319,    2, 0x0a /* Public */,
     158,    1, 1322,    2, 0x0a /* Public */,
     159,    0, 1325,    2, 0x0a /* Public */,
     160,    0, 1326,    2, 0x0a /* Public */,
     161,    0, 1327,    2, 0x0a /* Public */,
     162,    0, 1328,    2, 0x0a /* Public */,
     163,    0, 1329,    2, 0x0a /* Public */,
     164,    0, 1330,    2, 0x0a /* Public */,
     165,    1, 1331,    2, 0x0a /* Public */,
     166,    1, 1334,    2, 0x0a /* Public */,
     167,    1, 1337,    2, 0x0a /* Public */,
     169,    1, 1340,    2, 0x0a /* Public */,
     170,    1, 1343,    2, 0x0a /* Public */,
     172,    1, 1346,    2, 0x0a /* Public */,
     173,    0, 1349,    2, 0x0a /* Public */,
     174,    1, 1350,    2, 0x0a /* Public */,
     175,    1, 1353,    2, 0x0a /* Public */,
     176,    1, 1356,    2, 0x0a /* Public */,
     178,    0, 1359,    2, 0x0a /* Public */,
     179,    0, 1360,    2, 0x0a /* Public */,
     180,    1, 1361,    2, 0x0a /* Public */,
     182,    1, 1364,    2, 0x0a /* Public */,
     183,    0, 1367,    2, 0x0a /* Public */,
     184,    1, 1368,    2, 0x0a /* Public */,
     187,    1, 1371,    2, 0x0a /* Public */,
     188,    1, 1374,    2, 0x0a /* Public */,
     190,    1, 1377,    2, 0x0a /* Public */,
     191,    2, 1380,    2, 0x0a /* Public */,
     191,    1, 1385,    2, 0x2a /* Public | MethodCloned */,
     191,    1, 1388,    2, 0x0a /* Public */,
     195,    1, 1391,    2, 0x0a /* Public */,
     196,    1, 1394,    2, 0x0a /* Public */,
     199,    1, 1397,    2, 0x0a /* Public */,
     201,    2, 1400,    2, 0x0a /* Public */,
     204,    3, 1405,    2, 0x0a /* Public */,
     207,    0, 1412,    2, 0x0a /* Public */,
     208,    0, 1413,    2, 0x0a /* Public */,
     209,    0, 1414,    2, 0x0a /* Public */,
     210,    1, 1415,    2, 0x0a /* Public */,
     213,    0, 1418,    2, 0x0a /* Public */,
     214,    2, 1419,    2, 0x0a /* Public */,
     218,    0, 1424,    2, 0x0a /* Public */,
     219,    1, 1425,    2, 0x0a /* Public */,
     222,    1, 1428,    2, 0x0a /* Public */,
     225,    0, 1431,    2, 0x0a /* Public */,
     226,    0, 1432,    2, 0x0a /* Public */,
     227,    0, 1433,    2, 0x0a /* Public */,
     228,    0, 1434,    2, 0x0a /* Public */,
     229,    0, 1435,    2, 0x0a /* Public */,
     230,    0, 1436,    2, 0x0a /* Public */,
     231,    0, 1437,    2, 0x0a /* Public */,
     232,    0, 1438,    2, 0x0a /* Public */,
     233,    0, 1439,    2, 0x0a /* Public */,
     234,    0, 1440,    2, 0x0a /* Public */,
     235,    0, 1441,    2, 0x0a /* Public */,
     236,    0, 1442,    2, 0x0a /* Public */,
     238,    0, 1443,    2, 0x0a /* Public */,
     240,    0, 1444,    2, 0x0a /* Public */,
     241,    2, 1445,    2, 0x0a /* Public */,
     241,    1, 1450,    2, 0x2a /* Public | MethodCloned */,
     245,    1, 1453,    2, 0x0a /* Public */,
     247,    2, 1456,    2, 0x0a /* Public */,
     247,    1, 1461,    2, 0x2a /* Public | MethodCloned */,
     250,    2, 1464,    2, 0x0a /* Public */,
     250,    1, 1469,    2, 0x2a /* Public | MethodCloned */,
     251,    1, 1472,    2, 0x0a /* Public */,
     252,    1, 1475,    2, 0x0a /* Public */,
     254,    0, 1478,    2, 0x0a /* Public */,
     255,    0, 1479,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  103,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  103,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void, QMetaType::Bool,  105,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  157,
    QMetaType::Void, QMetaType::Bool,  103,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  103,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, QMetaType::QFont,  168,
    QMetaType::Void, QMetaType::Int,  157,
    QMetaType::Void, QMetaType::Bool,  171,
    QMetaType::Void, QMetaType::Bool,  103,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, QMetaType::QUrl,  177,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,  181,
    QMetaType::Void, QMetaType::QString,  181,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 185,  186,
    QMetaType::Void, 0x80000000 | 185,  186,
    QMetaType::Void, 0x80000000 | 22,  189,
    QMetaType::Void, 0x80000000 | 22,  189,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,  140,  192,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, 0x80000000 | 193,  194,
    QMetaType::Void, QMetaType::Int,  140,
    QMetaType::Void, 0x80000000 | 197,  198,
    QMetaType::Void, 0x80000000 | 200,  198,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,  202,  203,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 205, QMetaType::QString,  202,  206,  203,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 211,  212,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 216,  215,  217,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 220,  221,
    QMetaType::Void, 0x80000000 | 223,  224,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 237,
    0x80000000 | 239,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 242, QMetaType::Bool,  243,  244,
    QMetaType::Void, 0x80000000 | 242,  243,
    QMetaType::Void, 0x80000000 | 246,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,  248,  249,
    QMetaType::Void, QMetaType::Bool,  248,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,  248,  249,
    QMetaType::Void, QMetaType::Bool,  248,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Bool, 0x80000000 | 253,  194,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionNewModel_triggered(); break;
        case 1: _t->on_actionNewProject_triggered(); break;
        case 2: _t->on_actionOpenProject_triggered(); break;
        case 3: _t->on_actionOpen_triggered(); break;
        case 4: _t->on_actionSave_triggered(); break;
        case 5: _t->on_actionSaveAs_triggered(); break;
        case 6: _t->on_actionSaveAll_triggered(); break;
        case 7: _t->on_actionCloseAll_triggered(); break;
        case 8: _t->on_actionSnapShot_triggered(); break;
        case 9: _t->on_actionSaveProject_triggered(); break;
        case 10: _t->on_actionExportFEModel_triggered(); break;
        case 11: _t->on_actionImportGeometry_triggered(); break;
        case 12: _t->on_actionExportGeometry_triggered(); break;
        case 13: _t->on_actionImportProject_triggered(); break;
        case 14: _t->on_actionExportProject_triggered(); break;
        case 15: _t->on_actionImportImage_triggered(); break;
        case 16: _t->on_actionConvertFeb_triggered(); break;
        case 17: _t->on_actionConvertGeo_triggered(); break;
        case 18: _t->on_actionExit_triggered(); break;
        case 19: _t->on_recentFiles_triggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 20: _t->on_recentFEFiles_triggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 21: _t->on_recentGeomFiles_triggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 22: _t->on_actionUndo_triggered(); break;
        case 23: _t->on_actionRedo_triggered(); break;
        case 24: _t->on_actionInvertSelection_triggered(); break;
        case 25: _t->on_actionClearSelection_triggered(); break;
        case 26: _t->on_actionDeleteSelection_triggered(); break;
        case 27: _t->on_actionHideSelection_triggered(); break;
        case 28: _t->on_actionHideUnselected_triggered(); break;
        case 29: _t->on_actionUnhideAll_triggered(); break;
        case 30: _t->on_actionFind_triggered(); break;
        case 31: _t->on_actionSelectRange_triggered(); break;
        case 32: _t->on_actionToggleVisible_triggered(); break;
        case 33: _t->on_actionNameSelection_triggered(); break;
        case 34: _t->on_actionTransform_triggered(); break;
        case 35: _t->on_actionCollapseTransform_triggered(); break;
        case 36: _t->on_actionClone_triggered(); break;
        case 37: _t->on_actionCloneGrid_triggered(); break;
        case 38: _t->on_actionCloneRevolve_triggered(); break;
        case 39: _t->on_actionMerge_triggered(); break;
        case 40: _t->on_actionPurge_triggered(); break;
        case 41: _t->on_actionDetach_triggered(); break;
        case 42: _t->on_actionExtract_triggered(); break;
        case 43: _t->on_actionEditProject_triggered(); break;
        case 44: _t->on_actionFaceToElem_triggered(); break;
        case 45: _t->on_actionSelectOverlap_triggered(); break;
        case 46: _t->on_actionAddBC_triggered(); break;
        case 47: _t->on_actionAddNodalLoad_triggered(); break;
        case 48: _t->on_actionAddSurfLoad_triggered(); break;
        case 49: _t->on_actionAddBodyLoad_triggered(); break;
        case 50: _t->on_actionAddIC_triggered(); break;
        case 51: _t->on_actionAddContact_triggered(); break;
        case 52: _t->on_actionAddConstraint_triggered(); break;
        case 53: _t->on_actionAddRigidConstraint_triggered(); break;
        case 54: _t->on_actionAddRigidConnector_triggered(); break;
        case 55: _t->on_actionAddMaterial_triggered(); break;
        case 56: _t->on_actionAddStep_triggered(); break;
        case 57: _t->on_actionAddReaction_triggered(); break;
        case 58: _t->on_actionSoluteTable_triggered(); break;
        case 59: _t->on_actionSBMTable_triggered(); break;
        case 60: _t->on_actionCurveEditor_triggered(); break;
        case 61: _t->on_actionMeshInspector_triggered(); break;
        case 62: _t->on_actionElasticityConvertor_triggered(); break;
        case 63: _t->on_actionUnitConverter_triggered(); break;
        case 64: _t->on_actionKinemat_triggered(); break;
        case 65: _t->on_actionPlotMix_triggered(); break;
        case 66: _t->on_actionFEBioRun_triggered(); break;
        case 67: _t->on_actionFEBioStop_triggered(); break;
        case 68: _t->on_actionFEBioOptimize_triggered(); break;
        case 69: _t->on_actionOptions_triggered(); break;
        case 70: _t->on_actionLayerInfo_triggered(); break;
        case 71: _t->on_actionPlaneCut_triggered(); break;
        case 72: _t->on_actionMirrorPlane_triggered(); break;
        case 73: _t->on_actionVectorPlot_triggered(); break;
        case 74: _t->on_actionTensorPlot_triggered(); break;
        case 75: _t->on_actionIsosurfacePlot_triggered(); break;
        case 76: _t->on_actionSlicePlot_triggered(); break;
        case 77: _t->on_actionDisplacementMap_triggered(); break;
        case 78: _t->on_actionStreamLinePlot_triggered(); break;
        case 79: _t->on_actionParticleFlowPlot_triggered(); break;
        case 80: _t->on_actionVolumeFlowPlot_triggered(); break;
        case 81: _t->on_actionImageSlicer_triggered(); break;
        case 82: _t->on_actionVolumeRender_triggered(); break;
        case 83: _t->on_actionMarchingCubes_triggered(); break;
        case 84: _t->on_actionGraph_triggered(); break;
        case 85: _t->on_actionSummary_triggered(); break;
        case 86: _t->on_actionStats_triggered(); break;
        case 87: _t->on_actionIntegrate_triggered(); break;
        case 88: _t->on_actionImportPoints_triggered(); break;
        case 89: _t->on_actionImportLines_triggered(); break;
        case 90: _t->on_actionRecordNew_triggered(); break;
        case 91: _t->on_actionRecordStart_triggered(); break;
        case 92: _t->on_actionRecordPause_triggered(); break;
        case 93: _t->on_actionRecordStop_triggered(); break;
        case 94: _t->on_actionUndoViewChange_triggered(); break;
        case 95: _t->on_actionRedoViewChange_triggered(); break;
        case 96: _t->on_actionZoomSelect_triggered(); break;
        case 97: _t->on_actionZoomExtents_triggered(); break;
        case 98: _t->on_actionViewCapture_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 99: _t->on_actionOrtho_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 100: _t->on_actionShowGrid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 101: _t->on_actionShowMeshLines_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 102: _t->on_actionShowEdgeLines_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 103: _t->on_actionBackfaceCulling_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 104: _t->on_actionViewSmooth_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 105: _t->on_actionShowNormals_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 106: _t->on_actionShowFibers_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 107: _t->on_actionShowMatAxes_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 108: _t->on_actionShowDiscrete_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 109: _t->on_actionToggleLight_triggered(); break;
        case 110: _t->on_actionFront_triggered(); break;
        case 111: _t->on_actionBack_triggered(); break;
        case 112: _t->on_actionLeft_triggered(); break;
        case 113: _t->on_actionRight_triggered(); break;
        case 114: _t->on_actionTop_triggered(); break;
        case 115: _t->on_actionBottom_triggered(); break;
        case 116: _t->on_actionWireframe_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 117: _t->on_actionSnap3D_triggered(); break;
        case 118: _t->on_actionTrack_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 119: _t->on_actionViewVPSave_triggered(); break;
        case 120: _t->on_actionViewVPPrev_triggered(); break;
        case 121: _t->on_actionViewVPNext_triggered(); break;
        case 122: _t->on_actionSyncViews_triggered(); break;
        case 123: _t->on_actionToggleConnected_triggered(); break;
        case 124: _t->on_actionFEBioURL_triggered(); break;
        case 125: _t->on_actionFEBioForum_triggered(); break;
        case 126: _t->on_actionFEBioResources_triggered(); break;
        case 127: _t->on_actionFEBioPubs_triggered(); break;
        case 128: _t->on_actionAbout_triggered(); break;
        case 129: _t->on_actionSelect_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 130: _t->on_actionTranslate_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 131: _t->on_actionRotate_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 132: _t->on_actionScale_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 133: _t->on_selectCoord_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 134: _t->on_actionSelectObjects_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 135: _t->on_actionSelectParts_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 136: _t->on_actionSelectSurfaces_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 137: _t->on_actionSelectCurves_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 138: _t->on_actionSelectNodes_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 139: _t->on_actionSelectDiscrete_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 140: _t->on_selectRect_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 141: _t->on_selectCircle_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 142: _t->on_selectFree_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 143: _t->on_actionMeasureTool_triggered(); break;
        case 144: _t->on_actionPlaneCutTool_triggered(); break;
        case 145: _t->on_postSelectRect_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 146: _t->on_postSelectCircle_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 147: _t->on_postSelectFree_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 148: _t->on_postActionMeasureTool_triggered(); break;
        case 149: _t->on_selectData_currentValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 150: _t->on_actionPlay_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 151: _t->on_actionRefresh_triggered(); break;
        case 152: _t->on_actionFirst_triggered(); break;
        case 153: _t->on_actionPrev_triggered(); break;
        case 154: _t->on_actionNext_triggered(); break;
        case 155: _t->on_actionLast_triggered(); break;
        case 156: _t->on_actionTimeSettings_triggered(); break;
        case 157: _t->on_actionColorMap_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 158: _t->on_selectTime_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 159: _t->on_fontStyle_currentFontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 160: _t->on_fontSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 161: _t->on_fontBold_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 162: _t->on_fontItalic_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 163: _t->on_actionProperties_triggered(); break;
        case 164: _t->on_tab_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 165: _t->on_tab_tabCloseRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 166: _t->on_welcome_anchorClicked((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 167: _t->on_clearProject(); break;
        case 168: _t->on_closeProject(); break;
        case 169: _t->on_closeFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 170: _t->on_addToProject((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 171: _t->OnPostObjectStateChanged(); break;
        case 172: _t->OnPostObjectPropsChanged((*reinterpret_cast< FSObject*(*)>(_a[1]))); break;
        case 173: _t->on_modelViewer_currentObjectChanged((*reinterpret_cast< FSObject*(*)>(_a[1]))); break;
        case 174: _t->OnSelectMeshLayer((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 175: _t->OnSelectObjectTransparencyMode((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 176: _t->CloseView((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 177: _t->CloseView((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 178: _t->CloseView((*reinterpret_cast< CDocument*(*)>(_a[1]))); break;
        case 179: _t->SetCurrentState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 180: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 181: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 182: _t->on_finishedReadingFile((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 183: _t->finishedReadingFile((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QueuedFile(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 184: _t->checkFileProgress(); break;
        case 185: _t->StopAnimation(); break;
        case 186: _t->onTimer(); break;
        case 187: _t->on_glview_pointPicked((*reinterpret_cast< const vec3d(*)>(_a[1]))); break;
        case 188: _t->on_glview_selectionChanged(); break;
        case 189: _t->onRunFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 190: _t->onReadyRead(); break;
        case 191: _t->onErrorOccurred((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 192: _t->onExportMaterials((*reinterpret_cast< const vector<GMaterial*>(*)>(_a[1]))); break;
        case 193: _t->onExportAllMaterials(); break;
        case 194: _t->onImportMaterials(); break;
        case 195: _t->DeleteAllMaterials(); break;
        case 196: _t->DeleteAllBC(); break;
        case 197: _t->DeleteAllLoads(); break;
        case 198: _t->DeleteAllIC(); break;
        case 199: _t->DeleteAllContact(); break;
        case 200: _t->DeleteAllConstraints(); break;
        case 201: _t->DeleteAllRigidConstraints(); break;
        case 202: _t->DeleteAllRigidConnectors(); break;
        case 203: _t->DeleteAllSteps(); break;
        case 204: { CGLView* _r = _t->GetGLView();
            if (_a[0]) *reinterpret_cast< CGLView**>(_a[0]) = std::move(_r); }  break;
        case 205: { Post::CGLModel* _r = _t->GetCurrentModel();
            if (_a[0]) *reinterpret_cast< Post::CGLModel**>(_a[0]) = std::move(_r); }  break;
        case 206: _t->UpdateFontToolbar(); break;
        case 207: _t->RunFEBioJob((*reinterpret_cast< CFEBioJob*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 208: _t->RunFEBioJob((*reinterpret_cast< CFEBioJob*(*)>(_a[1]))); break;
        case 209: _t->NextSSHFunction((*reinterpret_cast< CSSHHandler*(*)>(_a[1]))); break;
        case 210: _t->ShowProgress((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 211: _t->ShowProgress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 212: _t->ShowIndeterminateProgress((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 213: _t->ShowIndeterminateProgress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 214: _t->UpdateProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 215: { bool _r = _t->DoModelCheck((*reinterpret_cast< CModelDocument*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 216: _t->toggleOrtho(); break;
        case 217: _t->autosave(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CMainWindow.data,
    qt_meta_data_CMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 218)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 218;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 218)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 218;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
