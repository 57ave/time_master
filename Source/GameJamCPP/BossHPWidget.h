// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPWidget.generated.h"

class ABossCharacter;
class UProgressBar;
class UTextBlock;

/**
 * Widget to display boss HP/Timer bar
 */
UCLASS()
class GAMEJAMCPP_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Set the boss character to track */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SetBossCharacter(ABossCharacter* InBoss);

	/** Update the HP bar display */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void UpdateDisplay();

protected:
	/** The boss character being tracked */
	UPROPERTY(BlueprintReadOnly, Category = "Boss")
	ABossCharacter* BossCharacter;

	/** Progress bar widget (bind this in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	/** Text block for time remaining (bind this in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;

	/** Text block for boss name (bind this in Blueprint) */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BossNameText;

	/** Called when boss timer changes */
	UFUNCTION()
	void OnTimerChanged(float RemainingTime);

	/** Find boss character in the level */
	void FindBossCharacter();

	/** Format time as MM:SS */
	FText FormatTime(float TimeInSeconds) const;
};
