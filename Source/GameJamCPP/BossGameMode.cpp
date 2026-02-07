// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossGameMode.h"
#include "BossCharacter.h"
#include "GameJamCPPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ABossGameMode::ABossGameMode()
{
	// Set default pawn class to the player character
	DefaultPawnClass = AGameJamCPPCharacter::StaticClass();

	bGameOver = false;
	BossCharacter = nullptr;
}

void ABossGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find the boss character in the level
	FindBossCharacter();

	// Bind to boss defeated event
	if (BossCharacter)
	{
		BossCharacter->OnBossDefeated.AddDynamic(this, &ABossGameMode::OnBossDefeated);
	}
}

void ABossGameMode::FindBossCharacter()
{
	// Search for boss character in the level
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<ABossCharacter> It(World); It; ++It)
	{
		BossCharacter = *It;
		break; // Get the first boss found
	}

	if (!BossCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Boss Character found in the level!"));
	}
}

void ABossGameMode::OnBossDefeated()
{
	if (bGameOver)
	{
		return;
	}

	bGameOver = true;
	OnPlayerWin();
}

void ABossGameMode::OnPlayerWin()
{
	UE_LOG(LogTemp, Log, TEXT("Player Wins! Boss Defeated!"));

	// Pause the game
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Show victory message or UI
	// This can be extended in Blueprint or with a custom widget
}
