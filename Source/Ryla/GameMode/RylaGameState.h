// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RylaGameState.generated.h"

/**
 * 
 */
UCLASS()
class RYLA_API ARylaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ARylaGameState();

	UPROPERTY()
	TObjectPtr<class URylaExperienceManagerComponent> ExperienceManagerComponent;
	
};
