// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <windows.h>
#include <wingdi.h>
#include "Components/Button.h"
#include "HAL/PlatformApplicationMisc.h"
#include "MainUI.generated.h"

/**
 *
 */
UCLASS()
class IMAGE_COPIER_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	static void CopyImageToClipboard(const int32 InSizeX, const int32 InSizeY, const TArray<FColor> &InImageData)
	{
		// Convert FColor array to raw BGRA data
		TArray<uint8> RawData;
		RawData.SetNum(InSizeX * InSizeY * 4); // 4 bytes per pixel (BGRA)

		for (int32 i = 0; i < InImageData.Num(); ++i)
		{
			RawData[i * 4 + 0] = InImageData[i].B;
			RawData[i * 4 + 1] = InImageData[i].G;
			RawData[i * 4 + 2] = InImageData[i].R;
			RawData[i * 4 + 3] = InImageData[i].A;
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

private:
	UFUNCTION()
	void TakeScreenShot();

	// ScreenShotButton
	UButton *ssButton;
};
