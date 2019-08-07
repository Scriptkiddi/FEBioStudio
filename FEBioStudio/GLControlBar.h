#pragma once
#include <QWidget>

class CMainWindow;
class CGLControlBar_UI;

class CGLControlBar : public QWidget
{
	Q_OBJECT

public:
	CGLControlBar(CMainWindow* wnd, QWidget* parent = 0);

	void Update();

	int views();
	int getActiveView();
	void setActiveView(int n);
	void addView(const std::string& name, bool makeActive = true);

private slots:
	void onPivotChanged();
	void onPivotClicked(bool b);
	void onSnapToGridClicked(bool b);
	void onSnapToNodeClicked(bool b);
	void onToggleVisibleClicked(bool b);
	void onMeshButtonClicked(int n);
	void onSelectConnected(bool b);
	void onSelectClosestPath(bool b);
	void onMaxAngleChanged(double v);
	void onSelectBackfacing(bool b);
	void onHideSelection(bool b);
	void onShowAll(bool b);
	void onZoomSelectClicked(bool b);
	void onZoomAllClicked(bool b);
	void onCurrentViewChanged(int n);

signals:
	void currentViewChanged(int n);

private:
	CGLControlBar_UI*	ui;
};
