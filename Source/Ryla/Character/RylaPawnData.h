#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RylaPawnData.generated.h"

/**
 * URylaPawnData
 * - non-mutable data asset that contains properties used to define a pawn
 */
UCLASS(BlueprintType)
class URylaPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URylaPawnData(const FObjectInitializer& ObjectInitializer);

	/** Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ryla|Pawn")
	TSubclassOf<APawn> PawnClass;
};
