#include <QtWidgets/QApplication>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QScreen>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QDebug>

class CaptureWidget : public QWidget {
	Q_OBJECT

public:
	CaptureWidget() {
		setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
		setAttribute(Qt::WA_TranslucentBackground);
		setStyleSheet("background: rgba(0, 0, 0, 64);");

		QRect allScreens;
		for (QScreen *screen : QGuiApplication::screens())
			allScreens = allScreens.united(screen->geometry());
		setGeometry(allScreens);

		band = new QRubberBand(QRubberBand::Rectangle, this);
	}

protected:
	void mousePressEvent(QMouseEvent *event) override {
		start = event->globalPos(); // ←グローバル座標でキャプチャするため
		band->setGeometry(QRect(start, QSize()));
		band->show();
	}

	void mouseMoveEvent(QMouseEvent *event) override {
		band->setGeometry(QRect(start, event->globalPos()).normalized());
	}

	void mouseReleaseEvent(QMouseEvent *event) override {
		band->hide();
		QRect rect = QRect(start, event->globalPos()).normalized();

		// キャプチャ
		QScreen *screen = QGuiApplication::primaryScreen(); // デフォルトスクリーン（全体ならwinId()=0）
		if (!screen) return;
		QPixmap full = screen->grabWindow(0); // デスクトップ全体
		QPixmap cropped = full.copy(rect);

		// 保存
		QString filename = "capture.png";
		if (cropped.save(filename)) {
			qDebug() << "保存成功:" << filename;
		} else {
			qDebug() << "保存失敗!";
		}

		close();
	}

private:
	QPoint start;
	QRubberBand *band;
};

#include "read.moc"

int main(int argc, char *argv[]) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	CaptureWidget selector;
	selector.show();

	return app.exec();
}
