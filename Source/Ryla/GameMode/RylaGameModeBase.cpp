// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaGameModeBase.h"

#include "RylaExperienceManagerComponent.h"
#include "RylaGameState.h"
#include "Ryla/Character/RylaCharacter.h"
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

void ARylaGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
}

void ARylaGameModeBase::OnExperienceLoaded(const URylaExperienceDefinition* CurrentExperience)
{
	// PlayerController를 순회하며
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}
