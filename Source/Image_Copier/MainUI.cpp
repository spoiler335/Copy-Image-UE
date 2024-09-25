// Fill out your copyright notice in the Description page of Project Settings.

#include "MainUI.h"

void UMainUI::NativeConstruct()
{
    Super::NativeConstruct();
    GetOwningPlayer()->bShowMouseCursor = true;
    ssButton = Cast<UButton>(GetWidgetFromName("ScreenShotButton"));
    if (ssButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("SSButton is Present"));
        ssButton->OnClicked.AddDynamic(this, &UMainUI::TakeScreenShot);
    }
}

void UMainUI::TakeScreenShot()
{

    UE_LOG(LogTemp, Warning, TEXT("TakeSS"));

    // Define the size of the image
    const int32 InSizeX = 256;
    const int32 InSizeY = 256;

    // Create a mock image data array
    TArray<FColor> InImageData;
    InImageData.SetNum(InSizeX * InSizeY);

    // Fill the array with some color data (e.g., red color)
    for (int32 i = 0; i < InImageData.Num(); ++i)
    {
        InImageData[i] = FColor::Red;
    }

    // Call the function with the mock data
    CopyImageToClipboard(InSizeX, InSizeY, InImageData);
}
