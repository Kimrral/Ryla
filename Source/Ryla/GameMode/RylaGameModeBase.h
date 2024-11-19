// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RylaGameModeBase.generated.h"

/**
 * forward declaration
 */
class URylaExperienceDefinition;
class URylaPawnData;

/**
 * RylaGameMode
 */
UCLASS()
class ARylaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARylaGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * AGameModeBase interface
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;
	virtual void InitGameState() final;
	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/**
	 * member methods
	 */
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience);
	const URylaPawnData* GetPawnDataForController(const AController* InController) const;
};
