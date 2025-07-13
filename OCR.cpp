#include <iostream>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <yy981/return.h>

std::string OCR(PIX* image) {
	tesseract::TessBaseAPI api;

	// 初期化 (データパスorNULL,言語)
	if (api.Init(NULL, "eng")) return_e("Tesseractの初期化に失敗しました");
	api.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.");

	// Leptonica
	if (!image) return_e("画像の読み込みに失敗しました");

	// OCR (UTF-8)
	api.SetImage(image);
	char *outText = api.GetUTF8Text();
	std::string result(outText);

	// 後処理
	delete[] outText;
	pixDestroy(&image);
	api.End();

	return result;
}
