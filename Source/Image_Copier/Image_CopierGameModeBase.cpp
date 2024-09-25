// Copyright Epic Games, Inc. All Rights Reserved.

#include "Image_CopierGameModeBase.h"
#include "GameFramework/GameUserSettings.h"

void AImage_CopierGameModeBase::BeginPlay()
{
    UGameUserSettings *UserSettings = GEngine->GetGameUserSettings();
    UserSettings->SetFullscreenMode(EWindowMode::Windowed);
    UserSettings->ApplySettings(false);
}
