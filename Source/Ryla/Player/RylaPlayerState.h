// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RylaPlayerState.generated.h"

class URylaPawnData;
class URylaExperienceDefinition;

/**
 * 
 */
UCLASS()
class RYLA_API ARylaPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARylaPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * AActor's interface
	 */
	virtual void PostInitializeComponents() final;

	/**
	 * member methods
	 */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience);
	void SetPawnData(const URylaPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const URylaPawnData> PawnData;
};
