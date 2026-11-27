#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildPreview.generated.h"
class UBuildingData;
// L'aperçu transparent du bâtiment choisi, vert si on peut le poser, rouge sinon.
UCLASS()
class CITYBUILDERCPP_API ABuildPreview : public AActor
{
	GENERATED_BODY()
public:
	ABuildPreview();
	// Montre le mesh de cette fiche. Avec nullptr, l'aperçu se cache.
	void SetData(UBuildingData* Data);
	// Vert ou rouge.
	void SetValid(bool bValid);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	// Un matériau transparent avec un paramètre « Color ».
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TObjectPtr<UMaterialInterface> GhostMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	FLinearColor ValidColor = FLinearColor(0.1f, 1.f, 0.2f);
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	FLinearColor InvalidColor = FLinearColor(1.f, 0.1f, 0.1f);
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
};