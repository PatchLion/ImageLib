#include "ImageReader.h"
#include <QtNetwork>

void createSSLRequest(QNetworkRequest& request, const QUrl& url)
{
	request.setUrl(url);

	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	sslConfig.setProtocol(QSsl::AnyProtocol);

	request.setSslConfiguration(sslConfig);
}

ImageLib::CImageReader::CImageReader(const ImageLib::stReadParam& param)
	: m_param(param)
{

}

void ImageLib::CImageReader::loadImage(QImage& image, const ImageLib::stReadParam& param)
{
	if (param.fileUrl.isLocalFile())
	{
		const QString filePath = param.fileUrl.toLocalFile();

		QImageReader reader(filePath);

		m_result.isSuccess = reader.read(&image);
		if (!m_result.isSuccess)
		{
			m_result.error = reader.error();
		}
	}
	else
	{
		QEventLoop loop;
		QNetworkAccessManager network;
		QNetworkRequest request(param.fileUrl);
		if (param.fileUrl.scheme().toLower().trimmed() == "https")
		{
			createSSLRequest(request, param.fileUrl);
		}

		QNetworkReply* reply = network.get(request);
		QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		loop.exec();

		if (reply->error() == QNetworkReply::NoError)
		{
			m_result.isSuccess = true;
			image.loadFromData(reply->readAll());
		}
		else
		{
			m_result.isSuccess = false;
			m_result.error = QImageReader::DeviceError;
		}
	}

	if (m_result.isSuccess)
	{
		if (param.limitSize > 0 && (image.width() > param.limitSize || image.height() > param.limitSize))
		{
			image = image.scaled(param.limitSize, param.limitSize, Qt::KeepAspectRatio);
		}
	}
}

void ImageLib::CImageReader::loadImage(QImage& image)
{
	loadImage(image, m_param);
}
