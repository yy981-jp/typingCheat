#include <iostream>
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>

#include <yy981/time.h>

#include "capture.h"


class CWidget: public QLabel {
public:
	CWidget() {
		// setWindowFlags(Qt::FramelessWindowHint);
		setAlignment(Qt::AlignCenter);
		resize(500,300);
		setText(QString::fromLocal8Bit("このウィンドウを読み取り場所に配置してください\n(タイトルバーは含まれません)"));
		show();
	}

	void mousePressEvent(QMouseEvent *event) {
		QRect rect = geometry();
		QScreen *screen = this->screen();
		qreal dpr = screen->devicePixelRatio(); // DPI倍率（例: 1.5）

		int x = rect.x() * dpr;
		int y = rect.y() * dpr;
		int w = rect.width() * dpr;
		int h = rect.height() * dpr;
		hide();
		sleepc(tu::s,1);
		capture(x,y,w,h);
		std::cout << "キャプチャ成功";
		deleteLater();
	}
};

int main() {
	int argc = 1;
	char *argv[] = { (char*)"dummy" };
	QApplication app(argc, argv);
	CWidget window;

	return app.exec();
}