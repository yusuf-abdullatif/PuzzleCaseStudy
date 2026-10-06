#include "PuzzlePiece.h"
#include "PuzzleSlot.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

APuzzlePiece::APuzzlePiece()
{
	PrimaryActorTick.bCanEverTick = false;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	SetRootComponent(PieceMesh);
	PieceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PieceMesh->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		PieceMesh->SetStaticMesh(CubeMesh.Object);
		PieceMesh->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
	}
}

void APuzzlePiece::SetCurrentSlot(APuzzleSlot* Slot)
{
	if (IsValid(CurrentSlot))
	{
		CurrentSlot->SetOccupyingPiece(nullptr);
	}

	CurrentSlot = Slot;

	if (IsValid(CurrentSlot))
	{
		CurrentSlot->SetOccupyingPiece(this);
	}
}

void APuzzlePiece::SnapToSlot(APuzzleSlot* Slot)
{
	if (!IsValid(Slot))
	{
		return;
	}

	SetCurrentSlot(Slot);
	SetActorLocation(Slot->GetSnapLocation());
}
