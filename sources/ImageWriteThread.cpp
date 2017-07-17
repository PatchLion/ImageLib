#include "ImageWriteThread.h"
#include "ImageWriter.h"

ImageLib::CImageWriteThread::CImageWriteThread(QObject *parent /*= 0*/)
	: QThread(parent)
	, m_writer(0)
	, m_id(QUuid::createUuid().toString())
{

}

ImageLib::CImageWriteThread::CImageWriteThread(const ImageLib::stWriteParam& param, QObject *parent /*= 0*/)
	: QThread(parent)
	, m_writer(0)
	, m_param(param)
	, m_id(QUuid::createUuid().toString())
{

}

ImageLib::CImageWriteThread::~CImageWriteThread()
{
	if (m_writer)
	{
		delete m_writer;
		m_writer = 0;
	}
}

void ImageLib::CImageWriteThread::run()
{
	if (!m_writer)
	{
		m_writer = new CImageWriter(m_param);
	}

	m_writer->writeImage(m_image, m_param);

	m_result = m_writer->result();
}

