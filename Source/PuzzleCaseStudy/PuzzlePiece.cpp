#include "PuzzlePiece.h"
#include "PuzzleSlot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

APuzzlePiece::APuzzlePiece()
{
	PrimaryActorTick.bCanEverTick = false;

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	SetRootComponent(PieceMesh);
	PieceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PieceMesh->SetSimulatePhysics(false);
	PieceMesh->SetCollisionObjectType(ECC_WorldDynamic);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		PieceMesh->SetStaticMesh(CubeMesh.Object);
		PieceMesh->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
	}

	// World-space number drawn on top of the cube (readable after UI spawn).
	IdText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IdText"));
	IdText->SetupAttachment(PieceMesh);
	IdText->SetUsingAbsoluteScale(true);
	IdText->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	IdText->SetHorizontalAlignment(EHTA_Center);
	IdText->SetVerticalAlignment(EVRTA_TextCenter);
	IdText->SetWorldSize(48.f);
	IdText->SetTextRenderColor(FColor::White);
	IdText->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RefreshIdLabel();
}

void APuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	RefreshIdLabel();
	OrientIdLabelToCamera();
}

void APuzzlePiece::SetCorrectPieceId(int32 InId)
{
	CorrectPieceId = InId;
	RefreshIdLabel();
	OrientIdLabelToCamera();
}

void APuzzlePiece::RefreshIdLabel()
{
	if (IdText)
	{
		IdText->SetText(FText::AsNumber(CorrectPieceId));
	}
}

void APuzzlePiece::OrientIdLabelToCamera()
{
	if (!IdText || !GetWorld())
	{
		return;
	}

	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	FVector CamLocation;
	FRotator CamRotation;
	PC->GetPlayerViewPoint(CamLocation, CamRotation);

	const FVector TextLocation = IdText->GetComponentLocation();
	const FVector ToCamera = (CamLocation - TextLocation).GetSafeNormal();
	if (!ToCamera.IsNearlyZero())
	{
		// Face the camera so the digit stays upright in the 45° top view.
		IdText->SetWorldRotation(ToCamera.Rotation());
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

void APuzzlePiece::ExchangeWith(APuzzlePiece* Other)
{
	if (!IsValid(Other) || Other == this)
	{
		return;
	}

	APuzzleSlot* SlotA = CurrentSlot;
	APuzzleSlot* SlotB = Other->CurrentSlot;
	const FVector WorldA = GetActorLocation();
	const FVector WorldB = Other->GetActorLocation();

	SetCurrentSlot(nullptr);
	Other->SetCurrentSlot(nullptr);

	if (IsValid(SlotB))
	{
		SetActorLocation(SlotB->GetSnapLocation());
		SetCurrentSlot(SlotB);
	}
	else
	{
		SetActorLocation(WorldB);
	}

	if (IsValid(SlotA))
	{
		Other->SetActorLocation(SlotA->GetSnapLocation());
		Other->SetCurrentSlot(SlotA);
	}
	else
	{
		Other->SetActorLocation(WorldA);
	}
}
