#include <iostream>
#include <windows.h>

#include <QtCore/QRect>

#include <leptonica/allheaders.h>

#include <yy981/time.h>
#include <yy981/string.h>

#include "selectRect.h"
#include "capture.h"
#include "OCR.h"
#include "input.h"
#include "proc.h"

bool enable = false;
int wait = 1000;


void eventloop() {
	if (!RegisterHotKey(NULL, 98107137, MOD_NOREPEAT, VK_F7)) {std::cerr << "グローバルホットキーの登録に失敗しました-7" << std::endl;std::cin.get();exit(1);}
	if (!RegisterHotKey(NULL, 98107138, MOD_NOREPEAT, VK_F8)) {std::cerr << "グローバルホットキーの登録に失敗しました-8" << std::endl;std::cin.get();exit(1);}
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		// std::cout << "MSG: " << std::hex << msg.message << " wParam=" << msg.wParam << std::endl;
		if (msg.message == WM_HOTKEY) {
			if (msg.wParam == 98107137) enable=true;
			if (msg.wParam == 98107138) enable=false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int main(int argc, char* argv[]) {
	std::thread(eventloop).detach();
	
	if (GetParentProcessName()=="explorer.exe") {
		std::cout << "キー入力間隔を指定してください(標準=" << wait << "ms): ";
		std::string in;
		std::cin >> in;
		wait = st::toi(in);
	} else if (argc > 1) wait = st::toi(std::string(argv[1]));



	QRect rect = getRect();
	sleepc(tu::s,1); // CWidget close 待機

	std::cout << "待機時間" << wait << "ms   F7で開始 F8で一時停止\n";

	while(true) {
		if (enable) {
			PIX* img = capture(rect.x(), rect.y(), rect.width(), rect.height());
			std::string text = OCR(img);
			std::cout << text;
			inputParse(text,wait);
		} else while(!enable) sleepc(tu::s,1);
	}
	
}