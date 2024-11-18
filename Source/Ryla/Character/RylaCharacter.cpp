// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaCharacter.h"

// Sets default values
ARylaCharacter::ARylaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARylaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARylaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARylaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

