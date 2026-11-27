#include "BuildPreview.h"
#include "BuildingData.h"
#include "Materials/MaterialInstanceDynamic.h"
ABuildPreview::ABuildPreview()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCastShadow(false);
}
void ABuildPreview::SetData(UBuildingData* Data)
{
	if (Data == nullptr || Data->Mesh == nullptr)
	{
		SetActorHiddenInGame(true);
		return;
	}
	SetActorHiddenInGame(false);
	// Même calcul que dans ABuilding : le mesh à sa taille, posé sur le sol.
	Mesh->SetStaticMesh(Data->Mesh);
	Mesh->SetRelativeScale3D(Data->MeshScale);
	const FBoxSphereBounds Bounds = Data->Mesh->GetBounds();
	const float Bottom = (Bounds.Origin.Z - Bounds.BoxExtent.Z) * Data->MeshScale.Z;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -Bottom + 2.f));
	if (DynamicMaterial == nullptr && GhostMaterial != nullptr)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterial, this);
	}
	Mesh->SetMaterial(0, DynamicMaterial);
}
void ABuildPreview::SetValid(bool bValid)
{
	if (DynamicMaterial != nullptr)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), bValid ? ValidColor : InvalidColor);
	}
}