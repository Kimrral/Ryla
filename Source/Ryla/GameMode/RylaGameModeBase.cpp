// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaGameModeBase.h"

#include "RylaExperienceDefinition.h"
#include "RylaExperienceManagerComponent.h"
#include "RylaGameState.h"
#include "Ryla/Character/RylaCharacter.h"
#include "Ryla/Character/RylaPawnData.h"
#include "Ryla/Player/RylaPlayerController.h"
#include "Ryla/Player/RylaPlayerState.h"


ARylaGameModeBase::ARylaGameModeBase()
{
	GameStateClass = ARylaGameState::StaticClass();
	PlayerControllerClass = ARylaPlayerController::StaticClass();
	PlayerStateClass = ARylaPlayerState::StaticClass();
	DefaultPawnClass = ARylaCharacter::StaticClass();
}

void ARylaGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로, 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다:
	// - 이를 처리하기 위해, 한프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ARylaGameModeBase::HandleMatchAssignmentIfNotExpectingOne);
}

void ARylaGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 Delegate를 준비한다:
	URylaExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<URylaExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnRylaExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ARylaGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 활용하여, PawnData로부터 PawnClass를 유도하자
	if (const URylaPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ARylaGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ARylaGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

const URylaPawnData* ARylaGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData가 오버라이드 되었을 경우, PawnData는 PlayerState에서 가져오게 됨
	if (InController)
	{
		if (const ARylaPlayerState* HakPS = InController->GetPlayerState<ARylaPlayerState>())
		{
			// GetPawnData 구현
			if (const URylaPawnData* PawnData = HakPS->GetPawnData<URylaPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정
	check(GameState);
	const URylaExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<URylaExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		// GetExperienceChecked 구현
		if (const URylaExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked(); Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되었으면 nullptr
	return nullptr;
}

void ARylaGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
}

bool ARylaGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	const URylaExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<URylaExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ARylaGameModeBase::OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience)
{
	// PlayerController를 순회하며
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
		if (APlayerController* PC = Cast<APlayerController>(*Iterator); PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}
