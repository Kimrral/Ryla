#pragma once

#include "Engine/DataAsset.h"
#include "RylaExperienceDefinition.generated.h"

/** forward declaration */
class URylaPawnData;
class UGameFeatureAction;

/**
 * URylaExperienceDefinition
 * - definition of an experience
 */
UCLASS()
class URylaExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URylaExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	/** the default pawn class to spawn for players */
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<URylaPawnData> DefaultPawnData;

	/** lost if game feature plugins this experience wants to have active */
	// 해당 property는 단순히 마킹 및 기억용으로 남겨놓도록 하겠다:
	// - GameMode에 따라 필요한 GameFeature plugin들을 로딩하는데 이에 대한 연결고리로 생각하면 된다 (현재는 쓰지 않음)
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
	
	// /** ExperienceActionSet은 UGameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	// UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	// TArray<TObjectPtr<URylaExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};