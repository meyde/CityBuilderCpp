#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CityGameMode.generated.h"


class AGridManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCityStatsChanged);
// Les règles de la ville : l'argent et les comptes à chaque tour.


UCLASS()
class CITYBUILDERCPP_API ACityGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	// La grille du niveau (cherchée la première fois qu'on la demande).
	AGridManager* GetGrid();
	// --- L'argent ---
	int32 GetMoney() const { return Money; }
	// Retire l'argent si on en a assez. Renvoie faux sinon, et rien n'est retiré.
	bool TrySpend(int32 Amount);
	void AddMoney(int32 Amount);
	// --- Les statistiques, mises à jour à chaque tour ---
	int32 GetLastBalance() const { return LastBalance; }
	UPROPERTY(BlueprintAssignable, Category = "City")
	FOnMoneyChanged OnMoneyChanged;
	UPROPERTY(BlueprintAssignable, Category = "City")
	FOnCityStatsChanged OnStatsChanged;
protected:
	// Toutes les EconomyInterval secondes : chaque bâtiment rapporte et coûte.
	void EconomyTick();
	UPROPERTY(EditDefaultsOnly, Category = "City")
	int32 StartMoney = 1500;
	UPROPERTY(EditDefaultsOnly, Category = "City")
	float EconomyInterval = 3.f;
	UPROPERTY()
	TObjectPtr<AGridManager> Grid;
	int32 Money = 0;
	int32 LastBalance = 0;
	FTimerHandle EconomyTimer;
	
};