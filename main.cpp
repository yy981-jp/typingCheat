#include <iostream>

#include <QtCore/QRect>

#include <leptonica/allheaders.h>

#include <yy981/time.h>

#include "selectRect.h"
#include "capture.h"
#include "OCR.h"


int main() {
	QRect rect = getRect();
	// std::cout << "DBG::main::rect after\n";
	sleepc(tu::s,1); // CWidget close 待機
	PIX* img = capture(rect.x(), rect.y(), rect.width(), rect.height());
	pixWrite("debug.png", img, IFF_PNG);//DEBUG
	std::string text = OCR(img);
	std::cout << text;
}