// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <windows.h>
#include <wingdi.h>

UENUM(BlueprintType)
enum class ImageType : uint8
{
	GrayScale UMETA(DisplayName = "GrayScale"),
	RGB UMETA(DisplayName = "RGB"),
	RGBA UMETA(DisplayName = "RGBA")
};

class IMAGE_COPIER_API Image_Utils
{
public:
	static void CopyImageToClipboard(const int32 InSizeX, const int32 InSizeY, const TArray<FColor> &InImageData, ImageType imageType)
	{
		// Convert FColor array to raw BGRA data
		TArray<uint8> RawData;

		switch (imageType)
		{
		case ImageType::GrayScale:
			RawData.SetNum(InSizeX * InSizeY); // 1 byte per pixel
			for (int32 i = 0; i < InImageData.Num(); ++i)
			{
				uint8 GrayValue = 0.299 * InImageData[i].R + 0.587 * InImageData[i].G + 0.114 * InImageData[i].B;
				RawData[i] = GrayValue;
			}
			break;
		case ImageType::RGB:
			RawData.SetNum(InSizeX * InSizeY * 3); // 3 bytes per pixel (RGB)
			for (int32 i = 0; i < InImageData.Num(); ++i)
			{
				RawData[i * 3 + 0] = InImageData[i].R;
				RawData[i * 3 + 1] = InImageData[i].G;
				RawData[i * 3 + 2] = InImageData[i].B;
			}
			break;
		case ImageType::RGBA:
			RawData.SetNum(InSizeX * InSizeY * 4); // 4 bytes per pixel (RGBA)
			for (int32 i = 0; i < InImageData.Num(); ++i)
			{
				RawData[i * 4 + 0] = InImageData[i].R;
				RawData[i * 4 + 1] = InImageData[i].G;
				RawData[i * 4 + 2] = InImageData[i].B;
				RawData[i * 4 + 3] = InImageData[i].A;
			}
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Unsupported image type"));
			return;
		}

		// Create a bitmap header
		BITMAPINFOHEADER BitmapInfoHeader = {};
		BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		BitmapInfoHeader.biWidth = InSizeX;
		BitmapInfoHeader.biHeight = -InSizeY; // Negative to indicate top-down bitmap
		BitmapInfoHeader.biPlanes = 1;
		BitmapInfoHeader.biBitCount = 32;
		BitmapInfoHeader.biCompression = BI_RGB;

		// Allocate global memory for the bitmap
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + RawData.Num());
		if (hGlobal)
		{
			void *pData = GlobalLock(hGlobal);
			if (pData)
			{
				// Copy header and pixel data to global memory
				memcpy(pData, &BitmapInfoHeader, sizeof(BITMAPINFOHEADER));
				memcpy((uint8 *)pData + sizeof(BITMAPINFOHEADER), RawData.GetData(), RawData.Num());
				GlobalUnlock(hGlobal);

				// Open clipboard and set the data
				if (OpenClipboard(nullptr))
				{
					EmptyClipboard();
					SetClipboardData(CF_DIB, hGlobal);
					CloseClipboard();
				}
			}
		}
	}
};
