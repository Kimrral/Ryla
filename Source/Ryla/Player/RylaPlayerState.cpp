// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaPlayerState.h"

#include "GameFramework/GameStateBase.h"
#include "Ryla/GameMode/RylaExperienceManagerComponent.h"
#include "Ryla/GameMode/RylaGameModeBase.h"

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

void ARylaPlayerState::SetPawnData(const URylaPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;

	// PawnData의 AbilitySet을 순회하며, ASC에 Ability를 할당(Give)한다
	// - 이 과정에서 ASC의 ActivatableAbilities에 추가된다
	// for (UHakAbilitySet* AbilitySet : PawnData->AbilitySets)
	// {
	// 	if (AbilitySet)
	// 	{
	// 		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
	// 	}
	// }
}


void ARylaPlayerState::OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience)
{
	if (ARylaGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARylaGameModeBase>())
	{
		// AHakGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const URylaPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

