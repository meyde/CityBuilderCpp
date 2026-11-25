#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CityGameMode.generated.h"

//Les règles de la ville
UCLASS()
class CITYBUILDERCPP_API ACityGameMode : public AGameModeBase
{
	GENERATED_BODY()
	public:
		virtual void BeginPlay() override;
};