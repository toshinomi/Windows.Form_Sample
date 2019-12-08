#include "ColorReversal.h"
#include "ComFunc.h"
#include "ComInfo.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_bitmap">ビットマップ</param>
ColorReversal::ColorReversal(Bitmap^ _bitmap) : ComImgProc(_bitmap)
{
}

/// <summary>
/// デスクトラクタ
/// </summary>
ColorReversal::~ColorReversal()
{
}

/// <summary>
/// 初期化
/// </summary>
void ColorReversal::Init(void)
{
	this->Init();
}

/// <summary>
/// 色反転の実行
/// </summary>
/// <param name="_token">キャンセルトークン</param>
/// <returns>実行結果 成功/失敗</returns>
bool ColorReversal::GoImgProc(CancellationToken^ _token)
{
	bool bRst = true;

	Bitmap^ bitmap = this->GetBitmap();
	int nWidthSize = bitmap->Width;
	int nHeightSize = bitmap->Height;

	Bitmap^ bitmapAfter = gcnew Bitmap(bitmap);

	BitmapData^ bitmapData = bitmapAfter->LockBits(System::Drawing::Rectangle(0, 0, nWidthSize, nHeightSize), ImageLockMode::ReadWrite, PixelFormat::Format32bppArgb);

	int nIdxWidth;
	int nIdxHeight;

	for (nIdxHeight = 0; nIdxHeight < nHeightSize; nIdxHeight++)
	{
		if (_token->IsCancellationRequested)
		{
			bRst = false;
			break;
		}

		for (nIdxWidth = 0; nIdxWidth < nWidthSize; nIdxWidth++)
		{
			if (_token->IsCancellationRequested)
			{
				bRst = false;
				break;
			}

			Byte* pPixel = (Byte*)bitmapData->Scan0.ToPointer() + nIdxHeight * bitmapData->Stride + nIdxWidth * 4;

			pPixel[ComInfo::Pixel::Type::B] = (Byte)(255 - pPixel[ComInfo::Pixel::Type::B]);
			pPixel[ComInfo::Pixel::Type::G] = (Byte)(255 - pPixel[ComInfo::Pixel::Type::G]);
			pPixel[ComInfo::Pixel::Type::R] = (Byte)(255 - pPixel[ComInfo::Pixel::Type::R]);
		}
	}
	bitmapAfter->UnlockBits(bitmapData);
	this->m_bitmapAfter = (Bitmap^)bitmapAfter->Clone();
	delete bitmapAfter;

	return bRst;
}