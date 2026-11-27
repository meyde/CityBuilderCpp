#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class ABuilding;
class UBuildingData;
class UInstancedStaticMeshComponent;
// La grille de la ville : les cases, et ce qui est construit sur chaque case.
UCLASS()
class CITYBUILDERCPP_API AGridManager : public AActor
{
	GENERATED_BODY()
public:
	AGridManager();
	// --- Les cases ---
	// La case qui contient ce point du monde.
	UFUNCTION(BlueprintPure, Category = "Grid")
	FIntPoint WorldToCell(const FVector& WorldLocation) const;
	// Le centre de cette case, dans le monde.
	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector CellToWorld(FIntPoint Cell) const;
	// Vrai si la case est bien dans la grille.
	UFUNCTION(BlueprintPure, Category = "Grid")
	bool IsInGrid(FIntPoint Cell) const;
	float GetCellSize() const { return CellSize; }
	// Les coins de la grille dans le monde, pour limiter la caméra.
	FVector GetMinCorner() const;
	FVector GetMaxCorner() const;
	// --- Les bâtiments ---
	// Le bâtiment posé sur cette case, ou nullptr si elle est vide.
	ABuilding* GetBuildingAt(FIntPoint Cell) const;
	// Vrai si la case est dans la grille et que rien n'y est construit.
	bool IsCellFree(FIntPoint Cell) const;
	// Fait apparaître le bâtiment de cette fiche sur la case. Ne vérifie pas l'argent.
	ABuilding* PlaceBuilding(UBuildingData* Data, FIntPoint Cell, float Yaw);
	// Détruit le bâtiment posé sur la case. Renvoie vrai s'il y en avait un.
	bool RemoveBuilding(FIntPoint Cell);	
	// Tous les bâtiments posés.
    void GetAllBuildings(TArray<ABuilding*>& OutBuildings) const;
        
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 Width = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 Height = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	float CellSize = 400.f;
	// Le sol sous toute la grille.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Ground;
	// Une tuile par case, toutes dessinées d'un coup.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> Tiles;
	// Ce qui est construit sur chaque case.
	UPROPERTY()
	TMap<FIntPoint, TObjectPtr<ABuilding>> Buildings;
};