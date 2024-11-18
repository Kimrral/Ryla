// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "RylaExperienceManagerComponent.generated.h"

// Forward Declare
class URylaExperienceDefinition;

enum class ERylaExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRylaExperienceLoaded, const URylaExperienceDefinition*);

/**
 * URylaExperienceManagerComponent
 * - 말 그대로, 해당 component는 game state를 owner로 가지면서, experience의 상태 정보를 가지고 있는 component이다
 * - 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */

UCLASS()
class RYLA_API URylaExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	URylaExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	/**
 * member methods
 */
	bool IsExperienceLoaded() const { return (LoadState == ERylaExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	/**
 * 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
 */
	void CallOrRegister_OnExperienceLoaded(FOnRylaExperienceLoaded::FDelegate&& Delegate);
	
	UPROPERTY()
	TObjectPtr<const URylaExperienceDefinition> CurrentExperience;

	/** Experience의 로딩 상태를 모니터링 */
	ERylaExperienceLoadState LoadState = ERylaExperienceLoadState::Unloaded;

	/** Experience 로딩이 완료된 이후, Broadcasting Delegate */
	FOnRylaExperienceLoaded OnExperienceLoaded;

	/** 활성화된 GameFeature Plugin들 */
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	
};
