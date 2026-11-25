#include "CityGameMode.h"

void ACityGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("City Started"));
}