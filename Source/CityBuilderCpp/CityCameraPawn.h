#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CityCameraPawn.generated.h"

class AGridManager;
class UCameraComponent;
class UFloatingPawnMovement;
class UInputAction;
class USpringArmComponent;
struct FInputActionValue;

//ZQSD: movement; AE: rotate; wheel:zoom
UCLASS()
class CITYBUILDERCPP_API ACityCameraPawn : public APawn
{
	GENERATED_BODY()
public:
	ACityCameraPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	
	//fonction input actions
	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	
	//camera locked to the grid
	void ClampToGrid();
	
	//component references 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UFloatingPawnMovement> Movement;
	
	//input actions references
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RotateAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ZoomAction;
	
	//movespeed at default spring arm length
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MoveSpeed = 2000.f;
	// rotate speed in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float RotateSpeed = 90.f;
	// zoom strenght per wheel scroll
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomStep = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	
	float MinZoom = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	
	float MaxZoom = 6000.f;
	//zoom speed modifier for a smoother shift
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomSmoothness = 8.f;
	// target length that is gradually achieved.
	float TargetZoom = 3000.f;
	//GridManager reference
	UPROPERTY()
	TObjectPtr<AGridManager> Grid;
};
