#include "ImageReadQueue.h"
#include "ImageReadThread.h"

const int kDefaultPoolSize = 20;

ImageLib::CImageReadQueue::CImageReadQueue(int poolSize, QObject *parent /*= 0*/)
	: QObject(parent)
{
	connect(this, &CImageReadQueue::threadFinished, this, &CImageReadQueue::onThreadFinished);

	if (poolSize >= 1)
	{
		m_poolSize = poolSize;
	}
	else
	{
		m_poolSize = kDefaultPoolSize;
	}
}

ImageLib::CImageReadQueue::~CImageReadQueue()
{

}


QString ImageLib::CImageReadQueue::addLoadTask(const ImageLib::stReadParam& param)
{
	QMutexLocker locker(&m_mutex);

	m_loaders.push_back(stLoader(param));

	startNextTask();

	return true;
}

ImageLib::CImageReadThread* ImageLib::CImageReadQueue::createImageLoader(const ImageLib::stReadParam& param)
{
	ImageLib::CImageReadThread *loader = new ImageLib::CImageReadThread(param);
	connect(loader, &CImageReadThread::finished, this, &CImageReadQueue::onImageLoadFinished);
	return loader;
}

void ImageLib::CImageReadQueue::onImageLoadFinished()
{
	CImageReadThread* loader = dynamic_cast<CImageReadThread*>(sender());

	Q_ASSERT(loader);

	Q_EMIT threadFinished(loader);
}

void ImageLib::CImageReadQueue::onThreadFinished(CImageReadThread* loader)
{
	Q_ASSERT(loader);
	
	const ImageLib::stReadResult& result = loader->result();

	Q_EMIT loadFinished(loader->id(), result.isSuccess, loader->image());
		
	for (int i = 0; i < m_loaders.size(); i++)
	{
		if (m_loaders[i].reader == loader)
		{
			m_loaders[i].state = result.isSuccess ? Loaded : LoadFailed;
			break;
		}
	}
	
	QMutexLocker locker(&m_mutex);

	startNextTask();
}

void ImageLib::CImageReadQueue::releaseTask(const QString& taskID)
{
	if (!taskID.isEmpty())
	{
		QMutexLocker locker(&m_mutex);


		for (int i = 0; i < m_loaders.size(); i++)
		{
			stLoader& load = m_loaders[i];
			if (load.reader && load.reader->id() == taskID)
			{
				if (load.reader->isRunning())
				{
					load.reader->quit();
					load.reader->wait();
				}

				qDebug() << "release loader: " << taskID;
				delete load.reader;
				load.reader = 0;
				m_loaders.removeAt(i);
				return;
			}
		}
	}
}

bool ImageLib::CImageReadQueue::isAllFinished() const
{
	bool finished = true;

	Q_FOREACH(stLoader loader, m_loaders)
	{
		if (Loading == loader.state)
		{
			finished = false;
			break;
		}
	}

	return finished;
}

void ImageLib::CImageReadQueue::startNextTask()
{
	if (m_loaders.isEmpty())
	{
		return;
	}

	int runningTaskCount = 0;
	for (int i = 0; i < m_loaders.size(); i++)
	{
		stLoader& load = m_loaders[i];
		if (Loading == load.state)
		{
			runningTaskCount++;
		}
	}

	qDebug() << "startNextTask " << runningTaskCount << "/" << poolSize();
	if (runningTaskCount >= poolSize())
	{
		//³¬³öÏÞÖÆ
		return;
	}

	int startCount = (poolSize() - runningTaskCount);
	for (int i = 0; i < m_loaders.size(); i++)
	{
		if (startCount <= 0)
		{
			break;
		}
		stLoader& load = m_loaders[i];
		if (Ready == load.state && !load.reader)
		{
			load.reader = createImageLoader(load.param);
			load.state = Loading;
			startCount--;
			load.reader->start();
		}
	}
}

