// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RylaAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RYLA_API URylaAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	URylaAssetManager();

	static URylaAssetManager& Get();

	// FScopeLogTime의 사용 여부 판단을 위해 사용
	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/** thread-safe way of adding asset to keep in memory [Thread-Safe] */
	void AddLoadedAsset(const UObject* Asset);

	// GetAsset은 정적 로딩으로 BP CLass와 Object 로딩
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMenory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	virtual void StartInitialLoading() override;
	virtual void FinishInitialLoading() override;

	// GC 제외 용도
	// 전역 멤버 컨테이너
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection SyncObject;
};

// 템플릿은 헤더에 정의하는 것이 좋다.
template <typename AssetType>
AssetType* URylaAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMenory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}
		if (LoadedAsset && bKeepInMenory)
		{
			// added to loaded asset list.
			// 여기서 AddLoadAsset은 메모리에 상주하기 위한 장치라고 생각하면 됨:
			// - 한번 등록되면 직접 내리지 않는한 Unload가 되지 않음 (== 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}
	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> URylaAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}
		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}
	return LoadedSubclass;
}
