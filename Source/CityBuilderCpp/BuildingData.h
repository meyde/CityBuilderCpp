#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingData.generated.h"
class ABuilding;
// La fiche d'un bâtiment. Chaque Data Asset (DA_House, DA_Road...) en est une copie remplie dans l'éditeur.
UCLASS(BlueprintType)
class CITYBUILDERCPP_API UBuildingData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// Le nom affiché sur le bouton et dans le panneau d'information.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	FText DisplayName;
	// Une phrase qui explique à quoi sert le bâtiment.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building", meta = (MultiLine = true))
	FText Description;
	// La classe à faire apparaître (BP_House, BP_Road...).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TSubclassOf<ABuilding> BuildingClass;
	// Le mesh, son matériau, sa couleur et sa taille.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look")
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look")
	TObjectPtr<UMaterialInterface> Material;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look")
	FLinearColor Color = FLinearColor::White;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look")
	FVector MeshScale = FVector(3.f, 3.f, 2.f);
	// Ce que coûte le bâtiment à la construction.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economy")
	int32 Cost = 100;
	// Ce qu'il rapporte à chaque tour d'économie (s'il est relié à une route).
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economy")
    int32 Income = 0;
    // Ce qu'il coûte à chaque tour d'économie, relié ou non.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economy")
    int32 Upkeep = 0;
};