#include <iostream>
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>

#include <yy981/time.h>

class CWidget: public QLabel {
Q_OBJECT
public:
	CWidget() {
		// setWindowFlags(Qt::FramelessWindowHint);
		setAlignment(Qt::AlignCenter);
		resize(500,300);
		setText(QString::fromLocal8Bit("このウィンドウを読み取り場所に配置し、\nウィンドウ内をクリックしてください\n(タイトルバーは含まれません)"));
		show();
	}

	void mousePressEvent(QMouseEvent *event) {
		QRect rect = geometry();
		QScreen *screen = this->screen();
		qreal dpr = screen->devicePixelRatio(); // DPI倍率
		hide();
		emit rectSelected(QRect(rect.x()*dpr, rect.y()*dpr, rect.width()*dpr, rect.height()*dpr));
	}
signals:
	void rectSelected(QRect rect);
};
#include "selectRect.moc"

QRect getRect() {
	int argc = 1;
	char *argv[] = { (char*)"dummy" };
	QApplication app(argc, argv);
	CWidget* window = new CWidget;
	QRect r_rect;
	QObject::connect(window, &CWidget::rectSelected, [&r_rect,window](QRect rect){
		r_rect = rect;
		// std::cout << "選択成功";
		window->deleteLater();
		// std::cout << "DBG::deleteLater\n";
		QApplication::quit();
		// std::cout << "DBG::quit\n";
	});

	app.exec();
	return r_rect;
}