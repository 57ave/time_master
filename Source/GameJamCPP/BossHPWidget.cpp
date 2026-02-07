// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossHPWidget.h"
#include "BossCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void UBossHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Find the boss character
	FindBossCharacter();

	// Bind to boss events
	if (BossCharacter)
	{
		BossCharacter->OnBossTimerChanged.AddDynamic(this, &UBossHPWidget::OnTimerChanged);
		UpdateDisplay();
	}
}

void UBossHPWidget::NativeDestruct()
{
	// Unbind from boss events
	if (BossCharacter)
	{
		BossCharacter->OnBossTimerChanged.RemoveDynamic(this, &UBossHPWidget::OnTimerChanged);
	}

	Super::NativeDestruct();
}

void UBossHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update display each frame
	if (BossCharacter && !BossCharacter->IsDefeated())
	{
		UpdateDisplay();
	}
}

void UBossHPWidget::SetBossCharacter(ABossCharacter* InBoss)
{
	// Unbind from old boss
	if (BossCharacter)
	{
		BossCharacter->OnBossTimerChanged.RemoveDynamic(this, &UBossHPWidget::OnTimerChanged);
	}

	BossCharacter = InBoss;

	// Bind to new boss
	if (BossCharacter)
	{
		BossCharacter->OnBossTimerChanged.AddDynamic(this, &UBossHPWidget::OnTimerChanged);
		UpdateDisplay();
	}
}

void UBossHPWidget::UpdateDisplay()
{
	if (!BossCharacter)
	{
		return;
	}

	// Update progress bar
	if (HPBar)
	{
		float Percentage = BossCharacter->GetTimePercentage();
		HPBar->SetPercent(Percentage);

		// Change color based on remaining time
		if (Percentage > 0.5f)
		{
			HPBar->SetFillColorAndOpacity(FLinearColor::Green);
		}
		else if (Percentage > 0.25f)
		{
			HPBar->SetFillColorAndOpacity(FLinearColor::Yellow);
		}
		else
		{
			HPBar->SetFillColorAndOpacity(FLinearColor::Red);
		}
	}

	// Update time text
	if (TimeText)
	{
		float RemainingTime = BossCharacter->GetRemainingTime();
		TimeText->SetText(FormatTime(RemainingTime));
	}

	// Update boss name
	if (BossNameText)
	{
		BossNameText->SetText(FText::FromString(TEXT("BOSS")));
	}
}

void UBossHPWidget::OnTimerChanged(float RemainingTime)
{
	UpdateDisplay();
}

void UBossHPWidget::FindBossCharacter()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Search for boss character in the level
	for (TActorIterator<ABossCharacter> It(World); It; ++It)
	{
		BossCharacter = *It;
		break; // Get the first boss found
	}

	if (!BossCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossHPWidget: No Boss Character found in the level!"));
	}
}

FText UBossHPWidget::FormatTime(float TimeInSeconds) const
{
	// Clamp to 0 minimum
	TimeInSeconds = FMath::Max(0.0f, TimeInSeconds);

	// Convert to minutes and seconds
	int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
	int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;

	// Format as MM:SS
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds));
}
