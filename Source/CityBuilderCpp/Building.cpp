#include "Building.h"
#include "BuildingData.h"
#include "Materials/MaterialInstanceDynamic.h"
ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ABuilding::Init(UBuildingData* InData, FIntPoint InCell, AGridManager* InGrid)
{
	Data = InData;
	Cell = InCell;
	Grid = InGrid;
	ApplyMesh();
	OnBuilt();
}
void ABuilding::ApplyMesh()
{
	if (Data == nullptr || Data->Mesh == nullptr)
	{
		return;
	}
	const FVector Scale = Data->MeshScale;
	Mesh->SetStaticMesh(Data->Mesh);
	Mesh->SetRelativeScale3D(Scale);
	// On remonte le mesh pour que le bas de sa boîte touche le sol, quel que soit son pivot.
	const FBoxSphereBounds Bounds = Data->Mesh->GetBounds();
	const float Bottom = (Bounds.Origin.Z - Bounds.BoxExtent.Z) * Scale.Z;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -Bottom));
	if (Data->Material != nullptr)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Data->Material, this);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Data->Color);
		Mesh->SetMaterial(0, DynamicMaterial);
	}
}
