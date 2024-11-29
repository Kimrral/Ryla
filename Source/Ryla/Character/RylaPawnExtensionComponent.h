#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GameFramework/Actor.h"
#include "RylaPawnExtensionComponent.generated.h"

/** forward declaration */
class URylaPawnData;
class URylaAbilitySystemComponent;

/**
 * 초기화 전반을 조정하는 컴포넌트
 */

UCLASS()
class URylaPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	URylaPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	/**
	 * member methods
	 */
	static URylaPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URylaPawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const URylaPawnData* InPawnData);
	void SetupPlayerInputComponent();
	//URylaAbilitySystemComponent* GetRylaAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	/** AbilitySystemComponent의 AvatorActor 대상 초기화/해제 호출 */
	void InitializeAbilitySystem(URylaAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	/** OnAbilitySystem[Initialized|Uninitialized] Delegate에 추가: */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	/**
	 * UPawnComponent interfaces
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	 * IGameFrameworkInitStateInterface
	 */
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	/**
	 * Pawn을 생성한 데이터를 캐싱
	 */
	UPROPERTY(EditInstanceOnly, Category = "Ryla|Pawn")
	TObjectPtr<const URylaPawnData> PawnData;

	// /** AbilitySystemComponent 캐싱 */
	// UPROPERTY()
	// TObjectPtr<URylaAbilitySystemComponent> AbilitySystemComponent;

	/** ASC Init과 Uninit의 Delegate 추가 */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
};