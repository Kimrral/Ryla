// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaAssetManager.h"

#include "Ryla/RylaLogChannels.h"

URylaAssetManager::URylaAssetManager()
{
}

URylaAssetManager& URylaAssetManager::Get()
{
	check(GEngine);

	// AssetManager를 UEngine의 GEngine AssetManager Class를 Override 했기 때문에 GEngine에 Asset Manager가 존재함
	if (URylaAssetManager* Singleton = Cast<URylaAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogRyla, Fatal, TEXT("Invalid AssetManager Class Name in DefaultEngine.ini"))

	// Fatal 나면 올수 없는 구문이지만 컴파일 위한 더미 리턴
	return *NewObject<URylaAssetManager>();
}


// cmd 커맨드라인 Arguments를 언리얼에서도 이용할 수 있다.
bool URylaAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* URylaAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// Synchronous Load Asset이 불필요한 작업을 하지는 않는지 확인하기 위한 함수
	if (AssetPath.IsValid())
	{
		// FScopeLogTIme 확인
		if (ShouldLogAssetLoads())
		{
			// 동기 로딩이 얼마나 걸렸는지 초단위로 로깅 진행
			TUniquePtr<FScopeLogTime> LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronous Loaded Assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 1.  AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		// ReSharper disable once CppDeprecatedEntity
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// 최후의 수단 (굉장히 느림)
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void URylaAssetManager::AddLoadedAsset(const UObject* Asset)
{
	// 멀티스레드의 안전성을 보장하지 못하는 상황인 에디터 실행 전 상황
	// 락을 걸어 중복 접근 방지
	if (ensureAlways(Asset))
	{
		// for thread-safety, use lock
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

void URylaAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

void URylaAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

