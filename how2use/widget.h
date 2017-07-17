#ifndef HTIMAGELIB_TEST_H
#define HTIMAGELIB_TEST_H

#include <QtWidgets/QWidget>
#include "ui_HTImageLib_Test.h"
#include "HTImage.h"

class CHTImageReadThread;
class CHTImageSaveThread;
class HTImageLib_Test : public QWidget, public Ui_AdvShowWidget
{
	Q_OBJECT

public:
	HTImageLib_Test(QWidget *parent = 0);
	~HTImageLib_Test();
    
    
protected:
    void paintEvent(QPaintEvent* event);
	bool eventFilter(QObject *object, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void updateExifInformation();
    
protected Q_SLOTS:
    void onImageLoaded(bool isSuccess, HTImage::ImageResult result);
	void onImageSaved();
	void on_pushButtonLoad_clicked(bool checked);
	void on_pushButtonSave_clicked(bool checked);

private:
	static QString outputPath();

private:
	CHTImageReadThread* imageReadThread;
	CHTImageSaveThread* imageSaveThread;
	QString m_originPath;
    HTImage::CHTImage m_htImage;
	HTImage::CHTImage m_savedPixmap;
	bool m_isPressed;
	QString m_originExif;
	QString m_savedImageExif;
};

#endif // HTIMAGELIB_TEST_H
