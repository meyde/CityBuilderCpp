#include "GridManager.h"
#include "Building.h"
#include "BuildingData.h"
#include "Components/InstancedStaticMeshComponent.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;
    // La racine est le coin de la grille : la case (0, 0) commence ici.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
    Ground->SetupAttachment(RootComponent);
    Ground->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Tiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tiles"));
    Tiles->SetupAttachment(RootComponent);
    Tiles->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
FIntPoint AGridManager::WorldToCell(const FVector& WorldLocation) const
{
    const FVector Local = WorldLocation - GetActorLocation();
    return FIntPoint(FMath::FloorToInt(Local.X / CellSize), FMath::FloorToInt(Local.Y / CellSize));
}
FVector AGridManager::CellToWorld(FIntPoint Cell) const
{
    return GetActorLocation() + FVector((Cell.X + 0.5f) * CellSize, (Cell.Y + 0.5f) * CellSize, 0.f);
}
bool AGridManager::IsInGrid(FIntPoint Cell) const
{
    return Cell.X >= 0 && Cell.X < Width && Cell.Y >= 0 && Cell.Y < Height;
}
FVector AGridManager::GetMinCorner() const
{
    return GetActorLocation();
}
FVector AGridManager::GetMaxCorner() const
{
    return GetActorLocation() + FVector(Width * CellSize, Height * CellSize, 0.f);
}
ABuilding* AGridManager::GetBuildingAt(FIntPoint Cell) const
{
    const TObjectPtr<ABuilding>* Found = Buildings.Find(Cell);
    return Found ? Found->Get() : nullptr;
}
bool AGridManager::IsCellFree(FIntPoint Cell) const
{
    return IsInGrid(Cell) && GetBuildingAt(Cell) == nullptr;
}
ABuilding* AGridManager::PlaceBuilding(UBuildingData* Data, FIntPoint Cell, float Yaw)
{
    if (Data == nullptr || Data->BuildingClass == nullptr || !IsCellFree(Cell))
    {
        return nullptr;
    }
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ABuilding* Building = GetWorld()->SpawnActor<ABuilding>(Data->BuildingClass, CellToWorld(Cell), FRotator(0.f, Yaw, 0.f), Params);
    if (Building == nullptr)
    {
        return nullptr;
    }
    Buildings.Add(Cell, Building);
    Building->Init(Data, Cell, this);
    return Building;
}
bool AGridManager::RemoveBuilding(FIntPoint Cell)
{
    ABuilding* Building = GetBuildingAt(Cell);
    if (Building == nullptr)
    {
        return false;
    }
    Buildings.Remove(Cell);
    Building->Destroy();
    return true;
}

void AGridManager::GetAllBuildings(TArray<ABuilding*>& OutBuildings) const
{
    OutBuildings.Reset();
    for (const TPair<FIntPoint, TObjectPtr<ABuilding>>& Pair : Buildings)
    {
        if (Pair.Value)
        {
            OutBuildings.Add(Pair.Value);
        }
    }
}


void AGridManager::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    // Le sol : un plan de 100 x 100 cm, placé au milieu de la grille et étiré sur toute sa taille.
    Ground->SetRelativeLocation(FVector(Width * CellSize / 2.f, Height * CellSize / 2.f, -1.f));
    Ground->SetRelativeScale3D(FVector(Width * CellSize / 100.f, Height * CellSize / 100.f, 1.f));
    // Une tuile un peu plus petite que la case : l'écart entre deux tuiles dessine les lignes.
    Tiles->ClearInstances();
    const float TileScale = CellSize * 0.96f / 100.f;
    for (int32 X = 0; X < Width; X++)
    {
        for (int32 Y = 0; Y < Height; Y++)
        {
            const FVector Location = CellToWorld(FIntPoint(X, Y)) + FVector(0.f, 0.f, 1.f);
            Tiles->AddInstance(FTransform(FRotator::ZeroRotator, Location, FVector(TileScale)), true);
        }
    }
}