// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RylaGameModeBase.generated.h"

/**
 * 
 */
class URylaExperienceDefinition;
class URylaPawnData;

UCLASS()
class RYLA_API ARylaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARylaGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	
	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	
	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	const URylaPawnData* GetPawnDataForController(const AController* InController) const;

	void HandleMatchAssignmentIfNotExpectingOne();
	bool IsExperienceLoaded()  const;
	void OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience);
	
	
};
