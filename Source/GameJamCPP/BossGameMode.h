// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BossGameMode.generated.h"

class ABossCharacter;

/**
 * Game Mode for Boss Fight
 * Manages win/lose conditions based on boss timer
 */
UCLASS()
class GAMEJAMCPP_API ABossGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABossGameMode();

protected:
	virtual void BeginPlay() override;

public:
	/** Called when player wins (boss defeated) */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnPlayerWin();

	/** Get reference to the boss character */
	UFUNCTION(BlueprintPure, Category = "Game")
	ABossCharacter* GetBossCharacter() const { return BossCharacter; }

	/** Check if game is over */
	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameOver() const { return bGameOver; }

protected:
	/** Reference to the boss character in the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	ABossCharacter* BossCharacter;

	/** Is the game over? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bGameOver;

	/** Find the boss character in the level */
	void FindBossCharacter();

	/** Bind to boss events */
	UFUNCTION()
	void OnBossDefeated();
};
