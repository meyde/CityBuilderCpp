#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CityPlayerController.generated.h"

class ABuildPreview;
class AGridManager;
class UBuildingData;
class UInputAction;
class UInputMappingContext;

// Le joueur : ses touches et sa souris.
// Le joueur : ses touches, sa souris et le bâtiment qu'il a choisi.
UCLASS()
class CITYBUILDERCPP_API ACityPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    ACityPlayerController();
    virtual void Tick(float DeltaTime) override;
    // Choisit le bâtiment à construire. Avec nullptr, on ne construit plus.
    void SelectBuilding(UBuildingData* Data);
    const TArray<TObjectPtr<UBuildingData>>& GetAvailableBuildings() const { return AvailableBuildings; }
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    // La case sous la souris. Renvoie faux si la souris n'est pas sur la grille.
    bool GetCellUnderCursor(FIntPoint& OutCell) const;
    // Place l'aperçu sur la case visée, en vert ou en rouge.
    void UpdatePreview();
    // Construit le bâtiment choisi sur la case visée, si c'est possible.
    void TryBuild();
    // Les fonctions appelées par les Input Actions.
    void OnClickStarted();
    void OnCancel();
    void OnRotateBuilding();
    // --- Les touches ---
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> MappingContext;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ClickAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> CancelAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RotateBuildingAction;
    // --- La construction ---
    // Les bâtiments proposés dans la barre de construction.
    UPROPERTY(EditDefaultsOnly, Category = "City")
    TArray<TObjectPtr<UBuildingData>> AvailableBuildings;
    UPROPERTY(EditDefaultsOnly, Category = "City")
    TSubclassOf<ABuildPreview> PreviewClass;
    UPROPERTY()
    TObjectPtr<AGridManager> Grid;
    UPROPERTY()
    TObjectPtr<ABuildPreview> Preview;
    UPROPERTY()
    TObjectPtr<UBuildingData> SelectedBuilding;
    // La rotation de l'aperçu, en degrés (0, 90, 180 ou 270).
    float PreviewYaw = 0.f;
};