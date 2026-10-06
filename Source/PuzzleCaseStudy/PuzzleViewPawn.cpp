#include "PuzzleViewPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

APuzzleViewPawn::APuzzleViewPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	// Slight downward tilt; fine-tune rotation in the editor on the placed actor.
	CameraComponent->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
}
