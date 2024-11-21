#pragma once

#include "RylaGameInstance.generated.h"

/**
 * GameInstance는 게임 프로세스(.exe)에서 하나만 존재하는 객체로 생각하면 된다 (== High-level Manager Object)
 * - 게임이 켜질때, 만들어지고, 게임이 꺼지기 전까지 살아있다
 * - Editor 상에서는 PIE로 실행 될때마다 하나씩 생성된다: 즉, 에디터에서는 복수개의 GameInstance가 존재 가능하다!
 */
UCLASS()
class URylaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	URylaGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * UGameInstance's interfaces
	 */ 
	virtual void Init() override;
	virtual void Shutdown() override;
};