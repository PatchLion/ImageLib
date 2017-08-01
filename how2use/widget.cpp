#include "widget.h"


Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	connect(&m_queue, &ImageLib::CImageReadQueue::loadFinished, this, &Widget::onLoadFinished);
}

Widget::~Widget()
{
	
}

void Widget::showEvent(QShowEvent *event)
{
	m_queue.addLoadTask(ImageLib::stReadParam("https://ss2.baidu.com/6ONYsjip0QIZ8tyhnq/it/u=2215229663,342384076&fm=173&s=E0D283679E2088550874BC920100C093&w=472&h=639&img.JPEG", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img4.imgtn.bdimg.com/it/u=841795147,2679362733&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img1.imgtn.bdimg.com/it/u=3507947669,2220898782&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img3.imgtn.bdimg.com/it/u=4117556643,3146907580&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img2.imgtn.bdimg.com/it/u=407745084,769758410&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img3.imgtn.bdimg.com/it/u=3835451652,579825967&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img2.imgtn.bdimg.com/it/u=2680232412,816547010&fm=26&gp=0.jpg", false));
	m_queue.addLoadTask(ImageLib::stReadParam("http://img3.imgtn.bdimg.com/it/u=3835451652,579825967&fm=26&gp=0.jpg", false));

}

void Widget::onLoadFinished(const QString& taskID, bool success, const QImage& image)
{
	if (success)
	{
		image.save(taskID + ".jpg");
	}
}
