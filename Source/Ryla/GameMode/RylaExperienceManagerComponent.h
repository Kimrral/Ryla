#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "RylaExperienceManagerComponent.generated.h"

/** forward declaration */
class URylaExperienceDefinition;

enum class ERylaExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRylaExperienceLoaded, const URylaExperienceDefinition*);

/**
 * RylaExperienceManagerComponent
 * - 말 그대로, 해당 component는 game state를 owner로 가지면서, experience의 상태 정보를 가지고 있는 component이다
 * - 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */
UCLASS()
class URylaExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	URylaExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member methods
	 */
	bool IsExperienceLoaded() { return (LoadState == ERylaExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	/**
	 * 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	 */
	void CallOrRegister_OnExperienceLoaded(FOnRylaExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const URylaExperienceDefinition* GetCurrentExperienceChecked() const;

	/**
	 * member variables
	 */

	/**
	 * 참고로 해당 멤버 변수는 Lyra에서는 'ReplicatedUsing='으로 선언되어있다:
	 * - 현재 우리는 아직 Replication을 신경쓰지 않을 것이기에, 최대한 네트워크 서버 코드를 배제하도록 하겠다
	 */
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