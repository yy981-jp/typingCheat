#include <string>
#include <windows.h>


void sendKey(WORD key) {
	INPUT inputs[2] = {};

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = key;
	inputs[0].ki.dwFlags = 0;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = key;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(2, inputs, sizeof(INPUT));
}

void inputParse(const std::string& text) {
	for (const char c: text) {
		switch (c) {
			case '.': sendKey(VK_OEM_PERIOD); break;
			default: sendKey(c);
		}
	}
}
