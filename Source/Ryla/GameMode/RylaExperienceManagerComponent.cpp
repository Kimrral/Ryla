// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaExperienceManagerComponent.h"
#include "RylaExperienceDefinition.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Ryla/System/RylaAssetManager.h"

URylaExperienceManagerComponent::URylaExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URylaExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnRylaExperienceLoaded::FDelegate&& Delegate)
{
	// IsExperienceLoaded() 구현
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		/**
		 * 참고로, 여러분들은 Delegate 객체를 자세히 살펴보면, 내부적으로 필요한 변수들은 메모리 할당해놓는다:
		 * TArray<int> a = {1, 2, 3, 4};
		 * delegate_type delegate = [a](){
		 *	return a.Num();
		 * }
		 * a는 delegate_type 내부에 new로 할당되어 있다. 복사 비용을 낮추기 위해 Move를 통해 하는 것을 잊지 말자!
		 */
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}


const URylaExperienceDefinition* URylaExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ERylaExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}


void URylaExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId)
{
	const URylaAssetManager& AssetManager = URylaAssetManager::Get();

	TSubclassOf<URylaExperienceDefinition> AssetClass;
	{
		const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// 왜 CDO를 가져오는 걸까?
	const URylaExperienceDefinition* Experience = GetDefault<URylaExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 그리고 CDO로 CurrentExperience를 설정한다!
		// 어떤 의도로 이렇게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad까지 읽어보자) 다시 생각해보자:
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void URylaExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ERylaExperienceLoadState::Unloaded);

	LoadState = ERylaExperienceLoadState::Loading;

	URylaAssetManager& AssetManager = URylaAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	// 이미 앞서, ServerSetCurrentExperience에서 우리는 ExperienceId를 넘겨주었는데, 여기서 CDO를 활용하여, GetPrimaryAssetId를 로딩할 대상으로 넣는다!
	// - 왜 이렇게 하는걸까?
	// - GetPrimaryAssetId를 좀 더 자세히보자:
	// - GetPrimaryAssetId를 살펴봄으로써, 아래의 두가지를 알 수 있다:
	//   1. 우리는 B_HakDefaultExperience를 BP로 만든 이유
	//   2. CDO를 가져와서, GetPrimaryAssetId를 호출한 이유

	// 우리는 앞서 이미 CDO로 로딩하여, CDO를 사용하지 않고 CDO를 사용하여 로딩할 에셋을 지정하여, BundleAssetList에 추가해준다!
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// // ExperienceActionSet의 순회하며, BundleAssetList로 추가하자:
	// for (const TObjectPtr<UHakExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	// {
	// 	if (ActionSet)
	// 	{
	// 		// 앞서, 우리가 생성한 HAS_Shooter_SharedHUD가 추가되겠다 (물론 추가적인 HAS_Shooter_XXX)도 추가될거다
	// 		// - BundleAssetList는 Bundle로 등록할 Root의 PrimaryDataAsset를 추가하는 과정이다
	// 		//   (->??? 무슨말인가 싶을건데 ChangeBundleStateForPrimaryAssets)을 살펴보면서 이해하자
	// 		BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
	// 	}
	// }

	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다:
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이라고 생각하면 된다 (일단 지금은 넘어가자 후일, 또 다룰 것이다!)
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);

		// 오늘! 아래의 의미를 알게 될 것이다:
		// - LoadStateClient = "Client", LoadStateServer = "Server"
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 아래도, 후일 Bundle을 우리가 GameFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_HakDefaultExperience를 로딩해주는 함수로 생각하자
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void URylaExperienceManagerComponent::OnExperienceLoadComplete()
{
	// FrameNumber
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 호출
	OnExperienceFullLoadCompleted();
}

void URylaExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ERylaExperienceLoadState::Loaded);

	LoadState = ERylaExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}
