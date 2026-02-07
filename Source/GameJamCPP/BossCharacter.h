// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDefeatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossTimerChangedSignature, float, RemainingTime);

/**
 * Boss Character with timer-based HP system
 * Timer starts at 5 minutes (300 seconds)
 * Player hits reduce timer by 3 seconds
 * When timer reaches 0, player wins
 */
UCLASS()
class GAMEJAMCPP_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Called when boss takes a hit */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void TakeHit(float Damage = 3.0f);

	/** Get remaining time on boss timer */
	UFUNCTION(BlueprintPure, Category = "Boss")
	float GetRemainingTime() const { return RemainingTime; }

	/** Get max time (initial timer value) */
	UFUNCTION(BlueprintPure, Category = "Boss")
	float GetMaxTime() const { return MaxTime; }

	/** Get time percentage (0-1) */
	UFUNCTION(BlueprintPure, Category = "Boss")
	float GetTimePercentage() const;

	/** Check if boss is defeated */
	UFUNCTION(BlueprintPure, Category = "Boss")
	bool IsDefeated() const { return bIsDefeated; }

	/** Event called when boss is defeated */
	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossDefeatedSignature OnBossDefeated;

	/** Event called when timer changes */
	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossTimerChangedSignature OnBossTimerChanged;

protected:
	/** Maximum time for the boss (5 minutes = 300 seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MaxTime;

	/** Current remaining time */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
	float RemainingTime;

	/** Time reduced per hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float TimeReductionPerHit;

	/** Is the boss defeated? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
	bool bIsDefeated;

	/** Handle boss defeat */
	void HandleDefeat();
};
