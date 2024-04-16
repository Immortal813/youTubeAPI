#include "youTubeAPI.h"
// API AIzaSyBmRI3ITSvgWISPP42wto7f1lPKUe2Hpf8
// API2 AIzaSyCCFU-tfL3-8OXtkfDRpdWwfIzHRbqFqPE
// My first commit

youTubeAPI::youTubeAPI( QWidget *parent )
	: QMainWindow( parent )
{


	ui.setupUi( this );

	connect( ui.pushButton, SIGNAL( clicked() ), this, SLOT( search_sl() ) );

	// Использовать при поиске проблемы с 256
	//connect( ui.listWidget, &QListWidget::itemClicked, this, &youTubeAPI::click_on_list );

	//connect( ui.listWidget, &QListWidget::currentRowChanged, this, &youTubeAPI::click_on_list );
	connect( ui.listWidget, &QListWidget::currentRowChanged, this, &youTubeAPI::click_on_list );



}

void youTubeAPI::save_pic_slot() {

	//QString filePath = QFileDialog::getSaveFileName( this, "Save Pic", "", tr("Images (*.png, *.jpg"));
	QString filePath = QFileDialog::getSaveFileName( this, "Save Pic", "", "Images( *.png *.jpg )" );
	QMessageBox msgBox;

	if ( !filePath.isEmpty() ) {

		if ( pic_save.save( filePath ) ) {

			//qDebug() << "SAVE COMPLETE!";
			//msgBox.warning()
			//msgBox.setText( "Save pic complete!" );
			msgBox.warning( this, "YouTubeAPI", "SAVE COMPLETE" );
		}
		else {
			//qDebug() << "IMG SAVE ERROR!";
			msgBox.warning( this, "YouTubeAPI", "Save pic Error" );
			//msgBox.setText( "Save pic error!" );

		}
	}
	else {
		//qDebug() << "TEST_SAVE ERROR!";
		msgBox.warning( this, "YouTubeAPI", "Path is not fount or window was close" );
		//msgBox.setText( "Path is not fount or window was close" );
	}

	//msgBox.exec();
	qDebug() << "TEST";

}

// Контекстное меню
void youTubeAPI::mousePressEvent( QMouseEvent *event ) {

	if ( event->button() == Qt::RightButton ) {

		QPoint pos = event->pos();

		if ( ui.sear_label->geometry().contains( pos ) ) {

			QMenu menu( this );
			//menu.addAction( "Act" );
			QAction *newAct = new QAction( "Save", this );
			menu.addAction( newAct );
			connect( newAct, &QAction::triggered, this, &youTubeAPI::save_pic_slot );
			menu.exec( mapToGlobal( event->pos() ) );

		}

	}

	if ( event->button() == Qt::LeftButton ) {
	
		QPoint pos = event->pos();

		if ( ui.sear_label->geometry().contains( pos ) ) {

			//https://www.youtube.com/watch?v=
			//QDesktopServices::openUrl( QUrl( "https://qt.io" ) );
			QDesktopServices::openUrl( QUrl( QString("https://www.youtube.com/watch?v=%1").arg(url_id_video)));


		}

	
	}


}

youTubeAPI::~youTubeAPI()
{}

void youTubeAPI::create_video_list() {


	ui.listWidget->blockSignals( true );
	ui.listWidget->clear();

	//ui.listWidget->clearSelection();

	qDebug() << "IN CREATE_VIDEO. SIZE PIC: " << name_video_list.size();

	for ( int i = 0; i < name_video_list.size(); i++ ) {


		QListWidgetItem *wid_item = new QListWidgetItem( ui.listWidget );
		qDebug() << "STAT VIDO LIST SIZE: " << stat_video_list.size();
		QString str = name_video_list[ i ] + " View Count: " + stat_video_list[ i ];
		//wid_item->setText( name_video_list[ i ] );
		wid_item->setText( str );

		ui.listWidget->addItem( wid_item );


	}

	ui.listWidget->blockSignals( false );

}

void youTubeAPI::clear_all_buff() {

	id_video_list.clear();
	name_video_list.clear();
	pic_video_list.clear();
	rspD.clear();

}

//Получить картинку
QBuffer *youTubeAPI::get_pic( QString str1 ) {

	QNetworkAccessManager *manager = new QNetworkAccessManager();
	QEventLoop eventLoop;



	QNetworkRequest request( str1 );
	QNetworkReply *reply = manager->get( request );

	QObject::connect( reply, SIGNAL( finished() ), &eventLoop, SLOT( quit() ) );
	eventLoop.exec();


	QBuffer *qBuf = new QBuffer;

	qBuf->setData( reply->readAll() );

	qDebug() << "NEW CLASS!";

	qDebug() << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
	qDebug() << qBuf;

	return qBuf;

}



// Получение ID видео
void youTubeAPI::get_info_video( int countVideo ) {

	id_video_list.clear();
	name_video_list.clear();
	pic_video_list.clear();

	QJsonDocument jDoc = QJsonDocument::fromJson( rspD );
	QJsonObject jObj = jDoc.object();
	QJsonArray items = jObj[ "items" ].toArray();


	//qDebug() << "Size BUFFER RSP_D: " << rspD.size();
	qDebug() << "SIZE ITEMS IN GET_INFO: " << items.size();

	//qDebug() << "GET ID VIDEO SOURCE:" << rspD;


	////// СДЕЛАТЬ МАССИВ СТРИНГОВ хехе стринги


	// По дефолту 9, но нужно тестить
	for ( int i = 0; i < items.size(); i++ ) {

		QJsonObject video = items[ i ].toObject();

		//QString videoName = video[ "snippet" ].toObject()[ "description" ].toString();
		//QString videoId = video[ "id" ].toObject()[ "videoId" ].toString();

		id_video_list.append( video[ "id" ].toObject()[ "videoId" ].toString() );
		name_video_list.append( video[ "snippet" ].toObject()[ "title" ].toString() );
		pic_video_list.append( video[ "snippet" ].toObject()[ "thumbnails" ].toObject()[ "medium" ].toObject()[ "url" ].toString() );
		//id_video_list.append( videoId );
		//qDebug() << "ID Video List: " << videoId;


	}


	qDebug() << "PIC URL: " << pic_video_list;

}

// Получить количество просмотров на видео
void youTubeAPI::get_video_view() {

	stat_video_list.clear();
	QNetworkAccessManager *manager = new QNetworkAccessManager();

	QUrl url( "https://www.googleapis.com/youtube/v3/videos" );
	QUrlQuery query;
	QEventLoop eventLoop;
	//5dRbzlXeY74  qu3GD9lDAHA b7EdOq3vRQI

	qDebug() << "IN VIEW FUNC!";

	for ( int i = 0; i < id_video_list.size(); i++ ) {

		qDebug() << "ID VIDEO:" << i << id_video_list[ i ];

	}

	QString id_video_string = id_video_list.join( "," );
	//Запрос на получение кол-ва просмотров на видео
	query.addQueryItem( "part", "statistics" );
	query.addQueryItem( "id", id_video_string );
	query.addQueryItem( "key", key_API );

	url.setQuery( query );

	qDebug() << "URL: " << url;
	qDebug() << "STRING ID VIDEO LIST: " << id_video_string;

	QNetworkRequest request( url );
	QNetworkReply *reply = manager->get( request );


	////////
	QObject::connect( reply, SIGNAL( finished() ), &eventLoop, SLOT( quit() ) );
	eventLoop.exec();

	if ( reply->error() == QNetworkReply::NoError ) {

		QByteArray respData = reply->readAll();
		QJsonDocument jDoc = QJsonDocument::fromJson( respData );

		QJsonObject jObj = jDoc.object();
		QJsonArray items = jObj[ "items" ].toArray();


		qDebug() << "ITEMS SIZE GET VIDEO VIEW: " << items.size();

		if ( !items.isEmpty() ) {

			// По дефолту 9, но нужно тестить
			for ( int i = 0; i < items.size(); i++ ) {

				QJsonObject stat = items[ i ].toObject()[ "statistics" ].toObject();
				QString viewCount = stat[ "viewCount" ].toString();

				stat_video_list.append( viewCount );
				//stat_video_list.append(stat[ "viewCount"].toString());
				qDebug() << "NAME VIDEO LIST SIZE IN GET VIDEO: " << name_video_list.size();
				qDebug() << i << "." << name_video_list[ i ] << " VIEW COUNT: " << viewCount;

			}

		}
		else
			qDebug() << "Video is not find!";


	}
	//ERROR
	else
		QMessageBox::critical( this, "Error in get_video_data", reply->errorString() );
		//qDebug() << "ERROR IN GET_VIDEO_VIEW" << reply->errorString();


	qDebug() << "END GET VIDEO VIEW!";
}

// Получить начальную информацию о видео, в данном случае ID
void youTubeAPI::get_video_data()
{

	QNetworkAccessManager *manager = new QNetworkAccessManager();
	QUrl url( "https://www.googleapis.com/youtube/v3/search" );
	QUrlQuery query;
	QEventLoop eventLoop;

	// Параметры запроса

	query.addQueryItem( "part", "snippet" );
	query.addQueryItem( "q", search );
	query.addQueryItem( "type", "video" );
	query.addQueryItem( "maxResults", QString::number( maxResult ) );
	query.addQueryItem( "order", "viewCount" );
	query.addQueryItem( "key", key_API );

	qDebug() << "REQUEST: " << search;
	url.setQuery( query );
	qDebug() << url;
	//Отправка запроса
	QNetworkRequest request( url );
	QNetworkReply *reply = manager->get( request );
	//QObject::connect( manager, SIGNAL( finished( QNetworkReply * ) ), &eventLoop1, SLOT( quit() ) );


	//Обработка ответа
	QObject::connect( reply, SIGNAL( finished() ), &eventLoop, SLOT( quit() ) );
	//QObject::connect( reply, SIGNAL( finished() ) ,SLOT( quit() ) );
	eventLoop.exec();

	if ( reply->error() == QNetworkReply::NoError ) {

		qDebug() << "GET ALL DATA!";
		rspD = reply->readAll();



	}
	else
		QMessageBox::critical( this, "Error in get_video_data", reply->errorString() );
		//qDebug() << "ERROR IN GET_VIDEO_DATA: " << reply->errorString();




	qDebug() << "GET VIDEO DATA END";

	get_info_video( maxResult );

	get_video_view();

}

//////SLOTS
//Загружает картинку по выбранному айтему\тексту
void youTubeAPI::click_on_list( int item ) {
	//void youTubeAPI::click_on_list( QListWidgetItem *item ) {

		// Получаю индек листа\видео
		//int index_pic = item->data( Qt::UserRole + 1 ).toInt() ;
		//int index_pic = item->data( Qt::UserRole ).toInt() ;
		// 
		//ui.listWidget->blockSignals( true );
		//ui.listWidget->clearSelection();

		//ui.listWidget->blockSignals( false );
	if ( item >= 0 ) {

		qDebug() << "CLICK ON LIST ITEM SIZE: " << item;
		qDebug() << "CLICK ON LIST! LIST SIZE: " << pic_video_list.size();
		QBuffer *data_pic = new QBuffer( this );
		data_pic->setData( get_pic( pic_video_list[ item ] )->buffer() );
		QPixmap img;
		url_id_video = id_video_list[ item ];
		img.loadFromData( data_pic->buffer() );
		pic_save = img;
		ui.sear_label->setPixmap( img );

	}
	else {

		qDebug() << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		qDebug() << "ITEM SIZE < 0 " << "ITEM:" << item;

	}
	//ui.listWidget->blockSignals( true );

	/*QListWidgetItem *items = ui.listWidget->item( item );
	items->setSelected( false );*/

	//ui.listWidget->blockSignals( false );
}


// КНОПКА ПОИСКА
void youTubeAPI::search_sl() {

	search = ui.lineEdit->text();

	get_video_data();

	create_video_list();

	//ui.sear_label->setText( QString::number( bbb ) );
	//bbb += 1;



	// Расскоментировать, там находятся данные, но появляется ошибка при длинном запросе!


	qDebug() << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";



	//ui.sear_label->setPixmap( img );

	//ui.scrollArea->setWidget( ui.sear_label );
	//ui.pic_label = new QLabel;
	//ui.pic_label->setPixmap( img );
	//ui.scrollArea->setWidget( ui.pic_label );
	//qDebug() << data_pic;


	rspD.clear();



}
