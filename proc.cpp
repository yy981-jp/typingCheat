#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

// WCHAR* → std::string（Shift-JIS変換。UTF-8なら CP_UTF8 に変えてね）
std::string wcharToString(const WCHAR* wstr) {
	int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (len == 0) return "";
	std::string result(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, result.data(), len, nullptr, nullptr);
	result.pop_back(); // null終端削除
	return result;
}

std::string GetParentProcessName() {
	DWORD pid = GetCurrentProcessId();
	DWORD ppid = 0;

	// スナップショット取得
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return "";

	PROCESSENTRY32W pe = {}; // Wつきに変更
	pe.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(hSnapshot, &pe)) {
		do {
			if (pe.th32ProcessID == pid) {
				ppid = pe.th32ParentProcessID;
				break;
			}
		} while (Process32NextW(hSnapshot, &pe));
	}

	// 親プロセス名を取得
	std::string parentName = "";
	pe.dwSize = sizeof(PROCESSENTRY32W);
	if (Process32FirstW(hSnapshot, &pe)) {
		do {
			if (pe.th32ProcessID == ppid) {
				parentName = wcharToString(pe.szExeFile);
				break;
			}
		} while (Process32NextW(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return parentName;
}
