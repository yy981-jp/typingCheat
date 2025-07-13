#include <windows.h>
#include <gdiplus.h>
#include <iostream>

#include "capture.h"

using namespace Gdiplus;

// BMP/PNG保存用
bool SaveBitmapToFile(HBITMAP hBitmap, const WCHAR* filename, const CLSID* clsid) {
	Gdiplus::Bitmap bmp(hBitmap, NULL);
	return bmp.Save(filename, clsid, NULL) == Ok;
}

// PNG用のCLSID取得
bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	UINT num = 0, size = 0;
	GetImageEncodersSize(&num, &size);
	if (size == 0) return false;

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(size);
	if (!pImageCodecInfo) return false;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT i = 0; i < num; ++i) {
		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return true;
		}
	}
	free(pImageCodecInfo);
	return false;
}

int capture(int x, int y, int w, int h) {
	// 座標とサイズ指定

	// GDI+初期化
	ULONG_PTR token;
	GdiplusStartupInput gpsi;
	GdiplusStartup(&token, &gpsi, NULL);

	// スクリーンDC
	HDC screenDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(screenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, w, h);
	SelectObject(memDC, hBitmap);

	// 指定範囲をキャプチャ
	BitBlt(memDC, 0, 0, w, h, screenDC, x, y, SRCCOPY);

	// PNGとして保存
	CLSID pngClsid;
	if (GetEncoderClsid(L"image/png", &pngClsid)) {
		if (SaveBitmapToFile(hBitmap, L"capture.png", &pngClsid)) {
			std::wcout << L"保存成功: capture.png" << std::endl;
		} else {
			std::wcerr << L"保存失敗" << std::endl;
		}
	} else {
		std::wcerr << L"PNGエンコーダが見つかりません" << std::endl;
	}

	// 後始末
	DeleteObject(hBitmap);
	DeleteDC(memDC);
	ReleaseDC(NULL, screenDC);
	GdiplusShutdown(token);

	return 0;
}
