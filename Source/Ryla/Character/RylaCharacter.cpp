#include "RylaCharacter.h"
#include "RylaPawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(RylaCharacter)

ARylaCharacter::ARylaCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tick을 비활성화
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<URylaPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	{
		PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
		PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	}

	// // CameraComponent 생성
	// {
	// 	CameraComponent = CreateDefaultSubobject<URylaCameraComponent>(TEXT("CameraComponent"));
	// 	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	// }
	//
	// // HealthComponent 생성
	// {
	// 	HealthComponent = CreateDefaultSubobject<URylaHealthComponent>(TEXT("HealthComponent"));
	// }
}

void ARylaCharacter::OnAbilitySystemInitialized()
{
	URylaAbilitySystemComponent* RylaASC = Cast<URylaAbilitySystemComponent>(GetAbilitySystemComponent());
	check(RylaASC);

	// HealthComponent의 ASC를 통한 초기화
	//HealthComponent->InitializeWithAbilitySystem(RylaASC);
}

void ARylaCharacter::OnAbilitySystemUninitialized()
{
	//HealthComponent->UninitializeWithAbilitySystem();
}

void ARylaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음:
	// - SetupPlayerInputComponent 확인
	PawnExtComponent->SetupPlayerInputComponent();
}

UAbilitySystemComponent* ARylaCharacter::GetAbilitySystemComponent() const
{
	// 앞서, 우리는 PawnExtensionComponent에 AbilitySystemComponent를 캐싱하였다
	//return PawnExtComponent->GetRylaAbilitySystemComponent();
	return nullptr;
}