#include "PuzzleSlot.h"
#include "PuzzleGameState.h"
#include "PuzzlePiece.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

APuzzleSlot::APuzzleSlot()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh"));
	SlotMesh->SetupAttachment(Root);
	SlotMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SlotMesh->SetGenerateOverlapEvents(false);

	// Flat cube from Starter Content — swap mesh/material on a Blueprint subclass if you prefer.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		SlotMesh->SetStaticMesh(CubeMesh.Object);
		SlotMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.05f));
	}

	SnapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SnapVolume"));
	SnapVolume->SetupAttachment(Root);
	SnapVolume->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	SnapVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SnapVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void APuzzleSlot::BeginPlay()
{
	Super::BeginPlay();

	if (APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>())
	{
		GS->RegisterSlot(this);
	}
}

FVector APuzzleSlot::GetSnapLocation() const
{
	return GetActorLocation() + FVector(0.f, 0.f, 50.f);
}

void APuzzleSlot::SetOccupyingPiece(APuzzlePiece* Piece)
{
	OccupyingPiece = Piece;
}

bool APuzzleSlot::IsCorrectlyFilled() const
{
	if (!IsValid(OccupyingPiece))
	{
		return false;
	}
	return OccupyingPiece->GetCorrectPieceId() == ExpectedPieceId;
}
