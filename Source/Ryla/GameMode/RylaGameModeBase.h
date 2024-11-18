// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RylaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RYLA_API ARylaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARylaGameModeBase();

	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	void OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience)

	void HandleMatchAssignmentIfNotExpectingOne();
	
	
};
