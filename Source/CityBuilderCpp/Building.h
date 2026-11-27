#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"
class AGridManager;
class UBuildingData;
// Un bâtiment posé sur une case. C'est le parent de la route, des maisons, des commerces...
UCLASS()
class CITYBUILDERCPP_API ABuilding : public AActor
{
	GENERATED_BODY()
public:
	ABuilding();
	// Appelée par la grille juste après l'apparition : le bâtiment lit sa fiche.
	virtual void Init(UBuildingData* InData, FIntPoint InCell, AGridManager* InGrid);
	UBuildingData* GetData() const { return Data; }
	FIntPoint GetCell() const { return Cell; }
	// --- Ce que le bâtiment apporte à la ville. Les enfants les redéfinissent. ---
	// L'argent gagné à chaque tour d'économie.
	virtual int32 GetIncome() const;
	// L'argent dépensé à chaque tour d'économie.
	virtual int32 GetUpkeep() const;
	// Un event que le Blueprint enfant peut remplir (une animation, un son...).
	UFUNCTION(BlueprintImplementableEvent, Category = "Building")
	void OnBuilt();
protected:
	// Met le mesh de la fiche, à sa taille, posé sur le sol, avec sa couleur.
	void ApplyMesh();
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleInstanceOnly, Category = "Building")
	TObjectPtr<UBuildingData> Data;
	UPROPERTY(VisibleInstanceOnly, Category = "Building")
	FIntPoint Cell;
	UPROPERTY()
	TObjectPtr<AGridManager> Grid;
	// Le matériau du mesh, qu'on peut recolorer en jeu.
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
};
