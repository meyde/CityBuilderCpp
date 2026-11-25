#include "CityPlayerController.h"

#include "EnhancedInputSubsystems.h"


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
}

