#ifndef widget_h__
#define widget_h__


#include <QtWidgets>
#include "ui_widget.h"
#include "ImageReadQueue.h"

class Widget : public QWidget, public Ui_Widget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = 0);
	~Widget();

protected:
	void showEvent(QShowEvent *event);

private Q_SLOTS:
	//º”‘ÿÕÍ≥…
	void onLoadFinished(const QString& taskID, bool success, const QImage& image);

    
private:
	ImageLib::CImageReadQueue m_queue;
};
#endif // widget_h__
