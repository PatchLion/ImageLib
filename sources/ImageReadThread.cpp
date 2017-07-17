#include "ImageReadThread.h"
#include "ImageReader.h"


ImageLib::CImageReadThread::CImageReadThread(QObject *parent /*= 0*/)
	: QThread(parent)
	, m_reader(0)
	, m_id(QUuid::createUuid().toString())
{

}

ImageLib::CImageReadThread::CImageReadThread(const ImageLib::stReadParam& param, QObject *parent /*= 0*/)
	: QThread(parent)
	, m_reader(0)
	, m_param(param)
	, m_id(QUuid::createUuid().toString())
{

}


ImageLib::CImageReadThread::~CImageReadThread()
{
	quit();
	wait();
	if (m_reader)
	{
		delete m_reader;
		m_reader = 0;
	}
}

void ImageLib::CImageReadThread::run()
{
	if (!m_reader)
	{
		m_reader = new CImageReader(m_param);
	}

	m_reader->loadImage(m_image);

	m_result = m_reader->result();
}
