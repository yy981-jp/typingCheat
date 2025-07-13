#include <windows.h>
#include <gdiplus.h>


bool SaveBitmapToFile(HBITMAP hBitmap, const WCHAR* filename, const CLSID* clsid);
bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
int capture(int x, int y, int w, int h);
