// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RylaUserFacingExperience.generated.h"

/** forward declarations */
class UCommonSession_HostSessionRequest;

/**
 * URylaUserFacingExperienceDefinition
 * - description of settings used to display experiences in the UI and start a new session
 */
UCLASS(BlueprintType)
class RYLA_API URylaUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/**
	 * Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여, HostSessionRequest 생성
	 */
	//UFUNCTION(BlueprintCallable, BlueprintPure=false)
	//UCommonSession_HostSessionRequest* CreateHostingRequest() const;

	/**
	 * member variables
	 */

	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	/** the gameplay expierence to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="RylaExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
};
