#include "RylaGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Ryla/RylaGameplayTags.h"

URylaGameInstance::URylaGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)	
{
}

void URylaGameInstance::Init()
{
	Super::Init();

	// 앞서 정의한 InitState의 GameplayTags 등록:
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FRylaGameplayTags& GameplayTags = FRylaGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void URylaGameInstance::Shutdown()
{
	Super::Shutdown();
}