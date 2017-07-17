#include "ImageWriter.h"

ImageLib::CImageWriter::CImageWriter(const ImageLib::stWriteParam& param)
	: m_param(param)
{

}

void ImageLib::CImageWriter::writeImage(const QImage& image, const ImageLib::stWriteParam& param)
{
	QImage tempImage = image;

	if (param.limitSize > 0 && image.width() != param.limitSize && image.height() != param.limitSize)
	{
		tempImage = tempImage.scaled(param.limitSize, param.limitSize, Qt::KeepAspectRatio);
	}
	

	QImageWriter writer(param.filePath);
	m_result.isSuccess = writer.write(image);
	if (!m_result.isSuccess)
	{
		m_result.error = writer.error();
	}
}

void ImageLib::CImageWriter::writeImage(const QImage& image)
{
	writeImage(image, m_param);
}

