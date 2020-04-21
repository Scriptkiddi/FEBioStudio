#pragma once
#include <QWidget>
#include <PostLib/FEPostModel.h>

namespace Post {
	class CImageModel;
}

class CImageViewer : public QWidget, public Post::FEModelDependant
{
	Q_OBJECT

protected:
	class Ui;

public:
	CImageViewer(QWidget* parent = nullptr);

	void SetImageModel(Post::CImageModel* img);

	void Update();

	void Update(Post::FEPostModel* fem) override;

private:
	void UpdatePath();

private slots:
	void onSliderChanged(int val);
	void onOverlayChanged(int val);

private:
	CImageViewer::Ui*	ui;
};
