#include "PuzzlePlayerController.h"

APuzzlePlayerController::APuzzlePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void APuzzlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Game + UI only; no keyboard focus required for puzzle interaction.
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
