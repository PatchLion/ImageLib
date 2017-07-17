#include "HTImageLib_Test.h"
#include <QtWidgets/QApplication>
//#include "log4qtConfig.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Log4QtConfig::config("HTImageLibTest", ".", Log4Qt::Level::DEBUG_INT);

	HTImageLib_Test w;
	w.show();
	return a.exec();
}
