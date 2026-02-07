// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize timer values
	MaxTime = 300.0f; // 5 minutes
	RemainingTime = MaxTime;
	TimeReductionPerHit = 3.0f; // 3 seconds per hit
	bIsDefeated = false;
}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Reset timer at start
	RemainingTime = MaxTime;
	bIsDefeated = false;

	// Broadcast initial timer value
	if (OnBossTimerChanged.IsBound())
	{
		OnBossTimerChanged.Broadcast(RemainingTime);
	}
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Don't tick timer if already defeated
	if (bIsDefeated)
	{
		return;
	}

	// Decrease remaining time
	RemainingTime -= DeltaTime;

	// Broadcast timer change
	if (OnBossTimerChanged.IsBound())
	{
		OnBossTimerChanged.Broadcast(RemainingTime);
	}

	// Check if boss is defeated (timer reached 0)
	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0.0f;
		HandleDefeat();
	}
}

void ABossCharacter::TakeHit(float Damage)
{
	if (bIsDefeated)
	{
		return;
	}

	// Reduce timer by hit damage (default 3 seconds)
	RemainingTime -= Damage;

	// Broadcast timer change
	if (OnBossTimerChanged.IsBound())
	{
		OnBossTimerChanged.Broadcast(RemainingTime);
	}

	// Check if this hit defeated the boss
	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0.0f;
		HandleDefeat();
	}
}

float ABossCharacter::GetTimePercentage() const
{
	if (MaxTime <= 0.0f)
	{
		return 0.0f;
	}
	return FMath::Clamp(RemainingTime / MaxTime, 0.0f, 1.0f);
}

void ABossCharacter::HandleDefeat()
{
	if (bIsDefeated)
	{
		return;
	}

	bIsDefeated = true;

	// Broadcast defeat event
	if (OnBossDefeated.IsBound())
	{
		OnBossDefeated.Broadcast();
	}

	// Optional: Disable boss movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
	}

	// Optional: Play defeat animation or effects here
}
