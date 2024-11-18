// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaPlayerState.h"

#include "GameFramework/GameStateBase.h"
#include "Ryla/GameMode/RylaExperienceManagerComponent.h"

ARylaPlayerState::ARylaPlayerState(const FObjectInitializer& ObjectInitializer)
{
}

void ARylaPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	URylaExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<URylaExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnRylaExperienceLoaded::FDelegate::CreateUObject(this, &ARylaPlayerState::OnExperienceLoaded));
}

void ARylaPlayerState::OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience)
{
}

void ARylaPlayerState::SetPawnData(const URylaPawnData* InPawnData)
{
}
