#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>

int OCR() {
	// Tesseract APIのインスタンス作成
	tesseract::TessBaseAPI api;

	// 初期化（第1引数: データパス or NULL, 第2引数: 言語）
	if (api.Init(NULL, "eng")) {
		std::cerr << "Tesseractの初期化に失敗しました。" << std::endl;
		return 1;
	}

	// 画像の読み込み（Leptonica使用）
	Pix *image = pixRead("test.png");
	if (!image) {
		std::cerr << "画像の読み込みに失敗しました。" << std::endl;
		return 1;
	}

	// 画像をTesseractに渡す
	api.SetImage(image);

	// OCRを実行して文字列を取得（UTF-8で返ってくる）
	char *outText = api.GetUTF8Text();

	// 結果を表示
	std::cout << "OCR結果:\n" << outText << std::endl;

	// 後処理
	delete[] outText;
	pixDestroy(&image);
	api.End();

	return 0;
}
