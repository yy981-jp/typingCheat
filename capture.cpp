#include <windows.h>
#include <leptonica/allheaders.h>
#include <vector>
#include "capture.h"

// HBITMAP → PIX*
PIX* bitmapToPix(HBITMAP hBitmap, int w, int h) {
	BITMAPFILEHEADER bfh = {};
	BITMAPINFOHEADER bih = {};
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = w;
	bih.biHeight = h;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;

	int bytesPerLine = ((w * 3 + 3) / 4) * 4;
	int imageSize = bytesPerLine * h;
	int fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;

	std::vector<uint8_t> bmpData(fileSize);
	bfh.bfType = 0x4D42; // 'BM'
	bfh.bfSize = fileSize;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	memcpy(bmpData.data(), &bfh, sizeof(bfh));
	memcpy(bmpData.data() + sizeof(bfh), &bih, sizeof(bih));

	// ピクセルデータを取得
	HDC hDC = GetDC(NULL);
	GetDIBits(hDC, hBitmap, 0, h, bmpData.data() + bfh.bfOffBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);

	// LeptonicaのPIXへ変換
	return pixReadMemBmp(bmpData.data(), bmpData.size());
}

// 指定範囲をキャプチャして PIX* を返す
PIX* capture(int x, int y, int w, int h) {
	HDC screenDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(screenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, w, h);
	SelectObject(memDC, hBitmap);

	// BitBltでキャプチャ
	BitBlt(memDC, 0, 0, w, h, screenDC, x, y, SRCCOPY);

	// 変換
	PIX* pix = bitmapToPix(hBitmap, w, h);

	// 後始末
	DeleteObject(hBitmap);
	DeleteDC(memDC);
	ReleaseDC(NULL, screenDC);

	return pix;
}
