#include "youTubeAPI.h"
#include <QtWidgets/QApplication>

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );
	youTubeAPI w;

	//QString search = "Cat";
	//w.get_video_inf(  );
	w.show();

	return a.exec();
}
