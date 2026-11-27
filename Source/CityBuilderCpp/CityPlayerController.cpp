#include "CityPlayerController.h"
#include "Building.h"
#include "BuildingData.h"
#include "BuildPreview.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"
ACityPlayerController::ACityPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}
void ACityPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // On donne au joueur les touches du Mapping Context.
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem != nullptr && MappingContext != nullptr)
    {
        Subsystem->AddMappingContext(MappingContext, 0);
    }
    // La souris reste visible : on joue au clavier ET à la souris, et on clique sur l'interface.
    bShowMouseCursor = true;
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    Grid = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (PreviewClass != nullptr)
    {
        Preview = GetWorld()->SpawnActor<ABuildPreview>(PreviewClass);
        Preview->SetData(nullptr);
    }
    // Provisoire : on choisit d'office le premier bâtiment, en attendant les boutons du HUD.
    if (AvailableBuildings.Num() > 0)
    {
        SelectBuilding(AvailableBuildings[0]);
    }
}
void ACityPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
    if (Input == nullptr)
    {
        return;
    }
    // Started : une fois, au moment du clic.
    Input->BindAction(ClickAction, ETriggerEvent::Started, this, &ACityPlayerController::OnClickStarted);
    Input->BindAction(CancelAction, ETriggerEvent::Started, this, &ACityPlayerController::OnCancel);
    Input->BindAction(RotateBuildingAction, ETriggerEvent::Started, this, &ACityPlayerController::OnRotateBuilding);
}
void ACityPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdatePreview();
}
void ACityPlayerController::SelectBuilding(UBuildingData* Data)
{
    SelectedBuilding = Data;
    if (Preview != nullptr)
    {
        Preview->SetData(Data);
    }
}
bool ACityPlayerController::GetCellUnderCursor(FIntPoint& OutCell) const
{
    if (Grid == nullptr)
    {
        return false;
    }
    // Le rayon qui part de la caméra et passe par la souris...
    FVector RayOrigin;
    FVector RayDirection;
    if (!DeprojectMousePositionToWorld(RayOrigin, RayDirection))
    {
        return false;
    }
    // ... et l'endroit où il touche le plan du sol. Les bâtiments ne gênent pas le calcul.
    const FPlane GroundPlane(Grid->GetActorLocation(), FVector::UpVector);
    const FVector HitPoint = FMath::RayPlaneIntersection(RayOrigin, RayDirection, GroundPlane);
    OutCell = Grid->WorldToCell(HitPoint);
    return Grid->IsInGrid(OutCell);
}
void ACityPlayerController::UpdatePreview()
{
    if (Preview == nullptr || SelectedBuilding == nullptr)
    {
        return;
    }
    FIntPoint Cell;
    if (!GetCellUnderCursor(Cell))
    {
        Preview->SetActorHiddenInGame(true);
        return;
    }
    Preview->SetActorHiddenInGame(false);
    Preview->SetActorLocationAndRotation(Grid->CellToWorld(Cell), FRotator(0.f, PreviewYaw, 0.f));
    const bool bCanBuild = Grid->IsCellFree(Cell);
    Preview->SetValid(bCanBuild);
}
void ACityPlayerController::TryBuild()
{
    FIntPoint Cell;
    if (SelectedBuilding == nullptr || !GetCellUnderCursor(Cell) || !Grid->IsCellFree(Cell))
    {
        return;
    }
    Grid->PlaceBuilding(SelectedBuilding, Cell, PreviewYaw);
}
void ACityPlayerController::OnClickStarted()
{
    if (SelectedBuilding != nullptr)
    {
        TryBuild();
    }
}
void ACityPlayerController::OnCancel()
{
    // Clic droit avec un bâtiment choisi : on arrête de construire.
    if (SelectedBuilding != nullptr)
    {
        SelectBuilding(nullptr);
        return;
    }
    // Sinon : on détruit le bâtiment visé.
    FIntPoint Cell;
    if (!GetCellUnderCursor(Cell))
    {
        return;
    }
    ABuilding* Building = Grid->GetBuildingAt(Cell);
    if (Building != nullptr && Building->GetData() != nullptr)
    {
        Grid->RemoveBuilding(Cell);
    }
}
void ACityPlayerController::OnRotateBuilding()
{
    PreviewYaw = FMath::Fmod(PreviewYaw + 90.f, 360.f);
}