#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_youTubeAPI.h"
#include <QtNetwork>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <iostream>
#include <qpixmap.h>
#include <qmenu.h>
//#include <QtGui/qevent.h>
#include <qevent.h>

#include <qfiledialog.h>


class youTubeAPI : public QMainWindow
{
	Q_OBJECT

public:

	QString key_API = "AIzaSyBmRI3ITSvgWISPP42wto7f1lPKUe2Hpf8";
	youTubeAPI( QWidget *parent = nullptr );

	~youTubeAPI();
	// Њолучить список видео по просмотрам или релевантности
	void get_video_data();
	void get_video_view();

private:
	void create_video_list();
	void get_info_video( int countVideo );
	QBuffer *get_pic( QString str1 );
	void clear_all_buff();


private:

	//Чтруктура данных видео
	struct videoData {

		QStringList id_video_list;
		QStringList name_video_list;
		QStringList pic_video_list;

	};

	Ui::youTubeAPIClass ui;

	QByteArray rspD;
	QUrl urlAPI;
	int maxResult = 9;


	//®кзампл§ры
	videoData vData;
	




	QStringList id_video_list;
	QStringList name_video_list;
	QStringList pic_video_list;
	QStringList stat_video_list;

	//QString id_video_string;
	QString search;
	QPixmap pic_save;

protected:
	void mousePressEvent( QMouseEvent *event ) override;
	//void mousePressEvent( QMouseEvent *event ) override;

private slots:

	void test_save();
	void search_sl();
	void click_on_list( int );
	//void click_on_list( QListWidgetItem * );


	//void click_on_list( QListWidgetItem * );

};
