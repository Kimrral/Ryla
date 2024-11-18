// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RylaPawnData.generated.h"

/**
 * 
 */
UCLASS()
class RYLA_API URylaPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URylaPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RylaPawn")
	TSubclassOf<APawn> PawnClass;
	
};
