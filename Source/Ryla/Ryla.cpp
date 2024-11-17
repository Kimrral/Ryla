// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ryla.h"

#include "RylaLogChannels.h"
#include "Modules/ModuleManager.h"

class FRylaGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FRylaGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	UE_LOG(LogRyla, Warning, TEXT("Start up Module!!"))

	// 통상적으로 사용하는 에셋 로딩법은 하드 코딩이다.
	// 해당 부분을 해결하기 위해 에셋 매니저가 필요하다.
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Starter..."));
	// if(SphereVisualAsset.Succeeded())
	// {
	// 	//....
	// }
}

void FRylaGameModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

// Custom Module Settings
// Double Primary Module cannot exist
IMPLEMENT_PRIMARY_GAME_MODULE(FRylaGameModule, Ryla, "Ryla");
