#include "HTImageLib_Test.h"
#include <QtGui>
#include <QtWidgets>
#include "HTImageReader.h"
#include "HTImageReadThread.h"
#include "HTImageSaveThread.h"
#include "HTImageConfig.h"


const QString kTitle = QString::fromLocal8Bit("HTImageLib");
const QString kTip = QString::fromLocal8Bit("23333");

HTImageLib_Test::HTImageLib_Test(QWidget *parent)
	: QWidget(parent)
	, m_isPressed(false)
{
	setupUi(this);
	HTImageConfig::setProfileFilePath(HTImage::sRGB, "E:\\projects\\_works\\Fotor3_basefrom3.1.0_153.34_fixedBug\\resources\\build_in\\ColorSpace\\sRGB.icc");
	HTImageConfig::setProfileFilePath(HTImage::AdobeRGB, "E:\\projects\\_works\\Fotor3_basefrom3.1.0_153.34_fixedBug\\resources\\build_in\\ColorSpace\\AdobeRGB.icc");
	HTImageConfig::setProfileFilePath(HTImage::ProPhoto, "E:\\projects\\_works\\Fotor3_basefrom3.1.0_153.34_fixedBug\\resources\\build_in\\ColorSpace\\Prophoto.icc");
	//HTImageConfig::setRawLoaderResourceRootDir("E:\\projects\\_works\\Fotor3_basefrom3.1.0_153.34_fixedBug\\resources\\build_in");

	imageSaveThread = new CHTImageSaveThread;
	connect(imageSaveThread, &CHTImageSaveThread::finished, this, &HTImageLib_Test::onImageSaved);

	imageReadThread = new CHTImageReadThread;
	connect(imageReadThread, &CHTImageReadThread::imageLoadFinished, this, &HTImageLib_Test::onImageLoaded);
    
#ifdef Q_OS_MAC
    //HTImage::CHTImageReader reader("/Users/fotor-desktop/Pictures/7.jpg");
    //QImage image;
    //reader.loadThumb(image);
    
    //printf("load image: %d - %d", image.width(), image.height());
#else
    
#endif
	updateExifInformation();

	widgetOpenImageArea->installEventFilter(this);
	labelTip->clear();

	showMaximized();

	pushButtonSave->setEnabled(false);

}

HTImageLib_Test::~HTImageLib_Test()
{
	if (imageSaveThread)
	{
		delete imageSaveThread;
		imageSaveThread = 0;
	}

	if (imageReadThread)
	{
		delete imageReadThread;
		imageReadThread = 0;
	}
}

void HTImageLib_Test::paintEvent(QPaintEvent* event)
{

	if (m_htImage.image().width() > 0 && m_htImage.image().height() > 0)
	{
		QPainter painter(this);

		QRect drawRect = widgetOpenImageArea->geometry();

		QPixmap pix;
		if (!m_isPressed)
		{
			pix = QPixmap::fromImage(m_htImage.image()).scaled(drawRect.size(), Qt::KeepAspectRatio);
		}
		else
		{
			pix = QPixmap::fromImage(m_savedPixmap.image()).scaled(drawRect.size(), Qt::KeepAspectRatio);
		}

		painter.drawPixmap(drawRect.center().x() - pix.width() / 2.0, drawRect.center().y() - pix.height() / 2.0, pix);
	}
}

void HTImageLib_Test::updateExifInformation()
{
	textBrowserExif->setText(m_isPressed ? m_savedImageExif : m_originExif);

	setWindowTitle(m_isPressed ? kTitle + QString::fromLocal8Bit(" 保存后的图片") : kTitle + QString::fromLocal8Bit(" 原始图片"));
}

void HTImageLib_Test::onImageLoaded(bool isSuccess, HTImage::ImageResult result)
{
	const QString path = outputPath() + "/HTImageLib_Output";

	if (!QDir().exists(path))
	{
		QDir().mkpath(path);
	}

	labelOutputPath->setText(path + "/" + QFileInfo(imageReadThread->argument().filePath).baseName() + ".jpg");

    if (imageReadThread) {
        if (HTImage::Result_NoError == imageReadThread->result().code()) {
            m_htImage = imageReadThread->image();
            
			m_originExif = m_htImage.exif().description();

			updateExifInformation();

			if (imageSaveThread)
			{
				imageSaveThread->write(m_htImage, labelOutputPath->text());
			}

            update();
        }
        else{
			setEnabled(true);
			pushButtonSave->setEnabled(false);
            //printf("[Error] failed to read image!\n");
			
			QMessageBox::warning(this, "Error", QString("Failed to read image! %1").arg(result.text()));
        }
    }
}

void HTImageLib_Test::on_pushButtonLoad_clicked(bool checked)
{

	m_originPath.clear();

	labelOutputPath->clear();
	m_savedPixmap = QImage();
	m_htImage = QImage();
	m_originExif.clear();
	m_savedImageExif.clear();
	updateExifInformation();
	update();

	if (imageReadThread)
	{
#ifdef Q_OS_WIN
		m_originPath = QFileDialog::getOpenFileName(this, "", QString::fromLocal8Bit("D:\\images\\最全single"), "*.*");
#else
        m_originPath = QFileDialog::getOpenFileName(this, "", "/Users/fotor-desktop/Pictures", "*.*");
#endif
		if (!m_originPath.isEmpty())
		{
			if (checkBoxLoadThumb->isChecked())
			{
				imageReadThread->loadThumb(m_originPath, 600);
			}
			else
			{
				imageReadThread->loadImage(m_originPath, true);
			}
			

			setEnabled(false);
		}
	}

	labelTip->clear();

}

void HTImageLib_Test::onImageSaved()
{
	setEnabled(true);
	pushButtonSave->setEnabled(true);
	if (HTImage::Result_NoError == imageSaveThread->result().code()) {
		HTImage::CHTImageReader reader(labelOutputPath->text());
		HTImage::CHTImage image;
		if (reader.loadImage(image, true))
		{
			labelTip->setText(kTip);
			m_savedPixmap = QPixmap::fromImage(image.image());

			m_savedImageExif = image.exif().description();

			updateExifInformation();
		}
		
	}
	else
	{
		QMessageBox::warning(this, "Failed", QString("Failed to save image: %1").arg(imageSaveThread->result().text()));
	}
}

QString HTImageLib_Test::outputPath()
{
	return QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
}

void HTImageLib_Test::mousePressEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		m_isPressed = true;

		updateExifInformation();

		update();
	}
}

void HTImageLib_Test::mouseReleaseEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		m_isPressed = false;

		updateExifInformation();

		update();
	}
}

bool HTImageLib_Test::eventFilter(QObject *object, QEvent *event)
{
	if (isEnabled() && widgetOpenImageArea == object)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			m_isPressed = true;
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			m_isPressed = false;
		}

		updateExifInformation();

		update();
	}

	return QWidget::eventFilter(object, event);
}

void HTImageLib_Test::on_pushButtonSave_clicked(bool checked)
{
	const QString filepath = QFileDialog::getSaveFileName(this, "Save", QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0], "png (*.png);;jpg (*jpg)");

	if (!filepath.isEmpty())
	{
		if (imageSaveThread)
		{
			imageSaveThread->write(m_htImage, filepath);
			setEnabled(false);
		}
	}
}
