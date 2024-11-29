#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RylaCharacter.generated.h"

/** forward declaration */
class URylaPawnExtensionComponent;
class URylaCameraComponent;

UCLASS()
class ARylaCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARylaCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialized();

	/**
	 * ACharacter interfaces
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	/**
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ryla|Character")
	TObjectPtr<URylaPawnExtensionComponent> PawnExtComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ryla|Character")
	// TObjectPtr<URylaCameraComponent> CameraComponent;

};