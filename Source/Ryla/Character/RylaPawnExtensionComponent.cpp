#include "RylaPawnExtensionComponent.h"
#include "RylaPawnData.h"
#include "Ryla/RylaGameplayTags.h"

/** feature name을 component 단위니깐 component를 빼고 pawn extension만 넣은 것을 확인할 수 있다 */
const FName URylaPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

URylaPawnExtensionComponent::URylaPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본적으로 Tick을 꺼버리자:
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void URylaPawnExtensionComponent::SetPawnData(const URylaPawnData* InPawnData)
{
	// Pawn에 대해 Authority가 없을 경우, SetPawnData는 진행하지 않음
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	// PawnData 업데이트
	PawnData = InPawnData;
}

void URylaPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate로 다시 InitState 상태 변환 시작 (연결 고리)
	CheckDefaultInitialization();
}

void URylaPawnExtensionComponent::InitializeAbilitySystem(URylaAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	// check(InASC && InOwnerActor);
	//
	// if (AbilitySystemComponent == InASC)
	// {
	// 	return;
	// }
	//
	// if (AbilitySystemComponent)
	// {
	// 	UninitializeAbilitySystem();
	// }
	//
	// APawn* Pawn = GetPawnChecked<APawn>();
	// const AActor* ExistingAvatar = InASC->GetAvatarActor();
	// check(!ExistingAvatar);
	//
	// // ASC를 업데이트하고, InitAbilityActorInfo를 Pawn과 같이 호출하여, AvatarActor를 Pawn으로 업데이트 해준다
	// AbilitySystemComponent = InASC;
	// AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
	//
	// // OnAbilitySystemInitialized에 바인딩된 Delegate 호출
	// OnAbilitySystemInitialized.Broadcast();
}

void URylaPawnExtensionComponent::UninitializeAbilitySystem()
{
	// if (!AbilitySystemComponent)
	// {
	// 	return;
	// }
	//
	// if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	// {
	// 	// OnAbilitySystemUninitialized에 바인딩된 Delegate 호출
	// 	OnAbilitySystemUninitialized.Broadcast();
	// }
	//
	// AbilitySystemComponent = nullptr;
}

PRAGMA_DISABLE_OPTIMIZATION

void URylaPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	// OnAbilitySystemInitialized에 UObject가 바인딩되어 있지 않으면 추가 (Uniqueness)
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	// // 이미 ASC가 설정되었으면, Delegate에 추가하는게 아닌 바로 호출 (이미 초기화되어 있으니깐!)
	// if (AbilitySystemComponent)
	// {
	// 	Delegate.Execute();
	// }
}

void URylaPawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}

#include "Components/GameFrameworkComponentManager.h"

void URylaPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			//UE_LOG(LogRyla, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록 진행:
	// - 등록은 상속받았던 IGameFrameworkInitStateInterface 메서드 RegisterInitStateFeature()를 활용
	// - 해당 함수를 간단히 보기
	RegisterInitStateFeature();

	// 디버깅을 위한 함수
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

PRAGMA_ENABLE_OPTIMIZATION

void URylaPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// FeatureName에 NAME_None을 넣으면, Actor에 등록된 Feature Component의 InitState 상태를 관찰하겠다는 의미:
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// InitState_Spawned로 상태 변환:
	// - TryToChangeInitState는 아래와 같이 진행된다:
	//   1. CanChangeInitState로 상태 변환 가능성 유무 판단
	//   2. HandleChangeInitState로 내부 상태 변경 (Feature Component)
	//   3. BindOnActorInitStateChanged로 Bind된 Delegate를 조건에 맞게 호출해 줌
	//      - RylaPawnExtensionComponent의 경우, 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged()가 호출됨
	ensure(TryToChangeInitState(FRylaGameplayTags::Get().InitState_Spawned));

	// 해당 함수는 우리가 오버라이딩 한다:
	// - 이 함수를 ForceUpdateInitState와 같은 느낌으로 이해해주면 된다 
	// - 현재 강제 업데이트 진행 (물론 CanChangeInitState와 HandleChangleInitState를 진행해준다)
	CheckDefaultInitialization();
}

void URylaPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()의 쌍을 맞추어주자
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void URylaPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// RylaPawnExtensionComponent는 다른 Feature Component들의 상태가 DataAvailable를 관찰하여, Sync를 맞추는 구간이 있었다 (CanChangeInitState)
		// - 이를 가능케하기 위해, OnActorInitStateChanged에서는 DataAvailable에 대해 지속적으로 CheckDefaultInitialization을 호출하여, 상태를 확인한다
		const FRylaGameplayTags& InitTags = FRylaGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool URylaPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FRylaGameplayTags& InitTags = FRylaGameplayTags::Get();

	// InitState_Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 잘 세팅만 되어있으면 바로 Spawned로 넘어감!
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// 아마 PawnData를 누군가 설정하는 모양이다
		if (!PawnData)
		{
			return false;
		}

		// LocallyControlled인 Pawn이 Controller가 없으면 에러!
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor에 바인드된 모든 Feature들이 DataAvailable 상태일 때, DataInitialized로 넘어감:
		// - HaveAllFeaturesReachedInitState 확인
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// 위의 선형적인(linear) transition이 아니면 false!
	return false;
}

void URylaPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장하는 Component이다:
	// - 따라서, Actor에 바인딩된 Feature Component들에 대해 CheckDefaultInitialization을 호출해주도록 한다 (ForceUpdate 느낌?)
	// - 이 메서드를 IGameFrameworkInitStateInterface가 제공하는데, CheckDefaultInitializationForImplementers이다:
	// - 간단히 CheckDefaultInitializationForImplementers 보자:
	CheckDefaultInitializationForImplementers();

	const FRylaGameplayTags& InitTags = FRylaGameplayTags::Get();

	// 사용자 정의 InitState를 직접 넘겨줘야 한다... (사실 이건 좀 알아서 할 수 있을거 같은데 굳이...)
	static const TArray<FGameplayTag> StateChain = {InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady};

	// CanChangeInitState()와 HandleChangeInitState() 그리고 ChangeFeatureInitState 호출을 통한 OnActorInitStateChanged Delegate 호출까지 진행해준다:
	// 아래의 코드를 간단히 보자:
	// - 해당 코드까지 보면, 우리는 간단히 짐작할 수 있다:
	//   - 계속 상태를 가능할때까지 한번 업데이트 한다
	//   - InitState에 대한 변화는 Linear(선형적)임을!... -> 나중에 Diagram으로 정리하면서 보자
	//     - 업데이트가 멈추면 누군가 시작해줘야 함을 의미! (chaining)
	ContinueInitStateChain(StateChain);
}
