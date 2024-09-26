// Fill out your copyright notice in the Description page of Project Settings.

#include "MainUI.h"
#include "Image_Utils.h"
#include "Math/UnrealMathUtility.h"

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
    imageTypeSelector = Cast<UComboBoxString>(GetWidgetFromName("ImageTypeDropBox-1"));
    if (imageTypeSelector)
    {
        UE_LOG(LogTemp, Warning, TEXT("ImageTypeComboBox is Present"));
        imageTypeSelector->ClearOptions();
        imageTypeSelector->AddOption(GetImageTypeAsString(ImageType::RGB));
        imageTypeSelector->AddOption(GetImageTypeAsString(ImageType::GrayScale));
        imageTypeSelector->AddOption(GetImageTypeAsString(ImageType::RGBA));
        imageTypeSelector->SetSelectedOption(GetImageTypeAsString(ImageType::RGB));

        // Set the callback for selection change
        imageTypeSelector->OnSelectionChanged.AddDynamic(this, &UMainUI::OnSelectionChanged);
    }
    imageType = GetImageTypeFromString(imageTypeSelector->GetSelectedOption());
}

FString UMainUI::GetImageTypeAsString(ImageType ImageTypeValue)
{
    const UEnum *EnumPtr = StaticEnum<ImageType>();
    if (!EnumPtr)
    {
        return FString("Invalid");
    }
    return EnumPtr->GetNameStringByValue(static_cast<int64>(ImageTypeValue));
}

ImageType UMainUI::GetImageTypeFromString(const FString &EnumName)
{
    const UEnum *EnumPtr = StaticEnum<ImageType>();
    if (!EnumPtr)
    {
        UE_LOG(LogTemp, Error, TEXT("Enum not found!"));
        return ImageType::RGB; // Default value or handle error appropriately
    }

    int32 EnumValue = EnumPtr->GetValueByName(FName(*EnumName));
    if (EnumValue == INDEX_NONE)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid enum name!"));
        return ImageType::RGB; // Default value or handle error appropriately
    }

    return static_cast<ImageType>(EnumValue);
}

void UMainUI::TakeScreenShot()
{

    UE_LOG(LogTemp, Warning, TEXT("TakeSS"));

    // Define the size of the image
    const int32 InSizeX = 512;
    const int32 InSizeY = 256;

    // Create a mock image data array
    TArray<FColor> InImageData;
    InImageData.SetNum(InSizeX * InSizeY);

    // Fill the array with some color data (e.g., red color)
    for (int32 i = 0; i < InImageData.Num(); ++i)
    {
        InImageData[i] = FColor(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255));
    }

    // Call the function with the mock data
    Image_Utils::CopyImageToClipboard(InSizeX, InSizeY, InImageData, imageType);
}

void UMainUI::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    UE_LOG(LogTemp, Warning, TEXT("Selection changed to: %s"), *SelectedItem);
    imageType = GetImageTypeFromString(SelectedItem);
}
