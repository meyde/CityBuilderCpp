#include "CityCameraPawn.h"

#include "EnhancedInputComponent.h"
#include "GridManager.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ACityCameraPawn::ACityCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    //Root creation
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;
    
    // Spring arm creation
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Root);
    SpringArm->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
    SpringArm->TargetArmLength = TargetZoom;
    SpringArm->bDoCollisionTest = false;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 10.f;
    
    // Camera creation
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    
    // movement component creation
    Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
    Movement->Acceleration = 10000.f;
    Movement->Deceleration = 10000.f;
}


void ACityCameraPawn::BeginPlay()
{
    Super::BeginPlay();
    //If blueprint changed default value:
    TargetZoom = SpringArm->TargetArmLength;
    
    //GridManager attribution
    Grid = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
}


void ACityCameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // smooth shift toward target zoom
    SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetZoom, DeltaTime, ZoomSmoothness);
    
    // moveSpeed adjusted with zoom
    Movement->MaxSpeed = MoveSpeed * SpringArm->TargetArmLength / 2000.f;
    
    ClampToGrid();
}


void ACityCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (Input == nullptr)
    {
        return;
    }
    Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACityCameraPawn::Move);
    Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ACityCameraPawn::Rotate);
    Input->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACityCameraPawn::Zoom);
}
void ACityCameraPawn::Move(const FInputActionValue& Value)
{
    // X : left-right, Y : front-behind.  local directions.
    const FVector2D Input = Value.Get<FVector2D>();
    AddMovementInput(GetActorRightVector(), Input.X);
    AddMovementInput(GetActorForwardVector(), Input.Y);
}
void ACityCameraPawn::Rotate(const FInputActionValue& Value)
{
    // rotation speed independent of game speed
    const float DeltaTime = GetWorld()->DeltaRealTimeSeconds;
    AddActorWorldRotation(FRotator(0.f, Value.Get<float>() * RotateSpeed * DeltaTime, 0.f));
}
void ACityCameraPawn::Zoom(const FInputActionValue& Value)
{

    TargetZoom = FMath::Clamp(TargetZoom - Value.Get<float>() * ZoomStep, MinZoom, MaxZoom);
}

void ACityCameraPawn::ClampToGrid()
{
    if (Grid == nullptr)
    {
        return;
    }
    const FVector Min = Grid->GetMinCorner();
    const FVector Max = Grid->GetMaxCorner();
    FVector Location = GetActorLocation();
    Location.X = FMath::Clamp(Location.X, Min.X, Max.X);
    Location.Y = FMath::Clamp(Location.Y, Min.Y, Max.Y);
    SetActorLocation(Location);
}