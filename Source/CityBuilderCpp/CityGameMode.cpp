#include "CityGameMode.h"
#include "Building.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"


void ACityGameMode::BeginPlay()
{
	Super::BeginPlay();
	Money = StartMoney;
	OnMoneyChanged.Broadcast(Money);
	OnStatsChanged.Broadcast();
	GetWorldTimerManager().SetTimer(EconomyTimer, this, &ACityGameMode::EconomyTick, EconomyInterval, true);
}
AGridManager* ACityGameMode::GetGrid()
{
	if (Grid == nullptr)
	{
		Grid = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	}
	return Grid;
}
bool ACityGameMode::TrySpend(int32 Amount)
{
	if (Money < Amount)
	{
		return false;
	}
	AddMoney(-Amount);
	return true;
}
void ACityGameMode::AddMoney(int32 Amount)
{
	Money += Amount;
	OnMoneyChanged.Broadcast(Money);
}
void ACityGameMode::EconomyTick()
{
	if (GetGrid() == nullptr)
	{
		return;
	}
	TArray<ABuilding*> Buildings;
	Grid->GetAllBuildings(Buildings);
	// On fait les comptes.
	int32 Income = 0;
	int32 Upkeep = 0;
	for (ABuilding* Building : Buildings)
	{
		Income += Building->GetIncome();
		Upkeep += Building->GetUpkeep();
	}
	LastBalance = Income - Upkeep;
	AddMoney(LastBalance);
	OnStatsChanged.Broadcast();
}