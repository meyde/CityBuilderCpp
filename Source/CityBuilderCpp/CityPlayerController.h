#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CityPlayerController.generated.h"

class UInputMappingContext;

// Le joueur : ses touches et sa souris.
UCLASS()
class CITYBUILDERCPP_API ACityPlayerController : public APlayerController
{
	GENERATED_BODY()
protected: 
	virtual void BeginPlay() override;
	
	//Les touches
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;
};
