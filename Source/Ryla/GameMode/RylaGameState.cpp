// Fill out your copyright notice in the Description page of Project Settings.


#include "RylaGameState.h"
#include "RylaExperienceManagerComponent.h"


ARylaGameState::ARylaGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<URylaExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
