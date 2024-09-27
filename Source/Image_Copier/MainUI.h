// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Types/SlateEnums.h"
#include "Components/ComboBoxString.h"
#include "HAL/PlatformApplicationMisc.h"
#include "UObject/EnumProperty.h"
#include "Image_Utils.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
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

private:
	FString GetImageTypeAsString(ImageType ImageTypeValue);
	ImageType GetImageTypeFromString(const FString &EnumName);
	void ShowAndHideWarningtext();
	void HideWarningtext();

	int32 InSizeX = 256;
	int32 InSizeY = 256;

	UFUNCTION()
	void TakeScreenShot();

	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnInputXChanged(const FText &Text);

	UFUNCTION()
	void OnInputYChanged(const FText &Text);

	// ScreenShotButton
	UButton *ssButton;

	// Image-type
	ImageType imageType;

	UComboBoxString *imageTypeSelector;

	UEditableTextBox *inputX;

	UEditableTextBox *inputY;

	UTextBlock *warningText;
};