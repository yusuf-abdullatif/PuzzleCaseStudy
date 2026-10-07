#include "PuzzlePlayerController.h"
#include "PuzzleBoardDropWidget.h"
#include "PuzzleEndScreenWidget.h"
#include "PuzzleGameState.h"
#include "PuzzleHUDWidget.h"
#include "PuzzlePiece.h"
#include "PuzzlePieceListWidget.h"
#include "PuzzleSlot.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

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

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>())
	{
		DragPlaneZ = GS->GetAverageSlotHeight();
		GS->OnPuzzleWon.AddDynamic(this, &APuzzlePlayerController::HandlePuzzleWon);
	}

	// Drop target under the piece list so board area receives UI drops.
	if (BoardDropWidgetClass)
	{
		BoardDropWidget = CreateWidget<UPuzzleBoardDropWidget>(this, BoardDropWidgetClass);
		if (BoardDropWidget)
		{
			BoardDropWidget->AddToViewport(0);
		}
	}

	if (PieceListWidgetClass)
	{
		PieceListWidget = CreateWidget<UPuzzlePieceListWidget>(this, PieceListWidgetClass);
		if (PieceListWidget)
		{
			PieceListWidget->AddToViewport(10);
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UPuzzleHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport(20);
		}
	}
}

void APuzzlePlayerController::HandlePuzzleWon()
{
	if (!EndScreenWidgetClass)
	{
		return;
	}

	if (!EndScreenWidget)
	{
		EndScreenWidget = CreateWidget<UPuzzleEndScreenWidget>(this, EndScreenWidgetClass);
	}

	if (EndScreenWidget && !EndScreenWidget->IsInViewport())
	{
		EndScreenWidget->AddToViewport(100);
	}

	if (const APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>())
	{
		EndScreenWidget->ShowResults(GS->GetElapsedSeconds(), GS->GetMoveCount());
	}
}

void APuzzlePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (HUDWidget)
	{
		HUDWidget->RefreshFromGameState();
	}

	if (APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>(); GS && GS->IsPuzzleComplete())
	{
		return;
	}

	if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		HandleDragPress();
	}

	if (bIsDraggingPiece)
	{
		UpdateDraggedPieceLocation();
	}

	if (WasInputKeyJustReleased(EKeys::LeftMouseButton) && bIsDraggingPiece)
	{
		HandleDragRelease();
	}
}

void APuzzlePlayerController::HandleDragPress()
{
	APuzzlePiece* HitPiece = GetPieceUnderCursor();
	if (!HitPiece)
	{
		return;
	}

	DraggedPiece = HitPiece;
	bIsDraggingPiece = true;
}

void APuzzlePlayerController::UpdateDraggedPieceLocation()
{
	if (!IsValid(DraggedPiece))
	{
		bIsDraggingPiece = false;
		return;
	}

	FVector Location;
	if (GetBoardPlaneHitUnderCursor(Location))
	{
		DraggedPiece->SetActorLocation(Location);
	}
}

void APuzzlePlayerController::HandleDragRelease()
{
	CommitDrop();
	DraggedPiece = nullptr;
	bIsDraggingPiece = false;
}

void APuzzlePlayerController::CommitDrop()
{
	if (!IsValid(DraggedPiece))
	{
		return;
	}

	APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>();
	if (!GS)
	{
		return;
	}

	const FVector DropLocation = DraggedPiece->GetActorLocation();

	if (APuzzlePiece* OtherPiece = FindOtherPieceNear(DropLocation, PieceSwapRadius))
	{
		DraggedPiece->ExchangeWith(OtherPiece);
		GS->RegisterMove();
		return;
	}

	APuzzleSlot* NearestSlot = GS->FindNearestSlot(DropLocation, SlotSnapMaxDistance);
	if (!IsValid(NearestSlot))
	{
		DraggedPiece->SetCurrentSlot(nullptr);
		return;
	}

	if (APuzzlePiece* Occupant = NearestSlot->GetOccupyingPiece(); IsValid(Occupant) && Occupant != DraggedPiece)
	{
		DraggedPiece->ExchangeWith(Occupant);
	}
	else
	{
		DraggedPiece->SnapToSlot(NearestSlot);
	}

	GS->RegisterMove();
}

APuzzlePiece* APuzzlePlayerController::FindOtherPieceNear(const FVector& WorldLocation, float Radius) const
{
	const float RadiusSq = Radius * Radius;
	APuzzlePiece* Best = nullptr;
	float BestDistSq = RadiusSq;

	for (TActorIterator<APuzzlePiece> It(GetWorld()); It; ++It)
	{
		APuzzlePiece* Piece = *It;
		if (!IsValid(Piece) || Piece == DraggedPiece)
		{
			continue;
		}

		const FVector Delta = Piece->GetActorLocation() - WorldLocation;
		const float DistSq = Delta.X * Delta.X + Delta.Y * Delta.Y;
		if (DistSq <= BestDistSq)
		{
			BestDistSq = DistSq;
			Best = Piece;
		}
	}

	return Best;
}

APuzzlePiece* APuzzlePlayerController::GetPieceUnderCursor() const
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		return nullptr;
	}

	FHitResult Hit;
	FCollisionQueryParams Params(TEXT("PuzzlePick"), false);
	if (IsValid(DraggedPiece))
	{
		Params.AddIgnoredActor(DraggedPiece);
	}

	const FVector TraceEnd = WorldOrigin + WorldDirection * 100000.f;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, TraceEnd, ECC_Visibility, Params))
	{
		return nullptr;
	}

	return Cast<APuzzlePiece>(Hit.GetActor());
}

bool APuzzlePlayerController::ProjectRayToBoard(const FVector& WorldOrigin, const FVector& WorldDirection, FVector& OutLocation) const
{
	UWorld* World = GetWorld();
	if (!World || WorldDirection.IsNearlyZero())
	{
		return false;
	}

	// Trace what the cursor is actually looking at (handles 45° top-down correctly).
	FCollisionQueryParams Params(SCENE_QUERY_STAT(PuzzleBoardTrace), true, GetPawn());
	if (IsValid(DraggedPiece))
	{
		Params.AddIgnoredActor(DraggedPiece);
	}

	for (TActorIterator<APuzzlePiece> It(World); It; ++It)
	{
		if (IsValid(*It))
		{
			Params.AddIgnoredActor(*It);
		}
	}

	FHitResult Hit;
	const FVector TraceEnd = WorldOrigin + WorldDirection.GetSafeNormal() * 100000.f;
	if (World->LineTraceSingleByChannel(Hit, WorldOrigin, TraceEnd, ECC_Visibility, Params))
	{
		OutLocation = Hit.ImpactPoint + FVector(0.f, 0.f, PieceRestHeight);
		return true;
	}

	// Fallback: horizontal plane at slot height (only if nothing was hit).
	if (const APuzzleGameState* GS = World->GetGameState<APuzzleGameState>())
	{
		const_cast<APuzzlePlayerController*>(this)->DragPlaneZ = GS->GetAverageSlotHeight();
	}

	if (FMath::IsNearlyZero(WorldDirection.Z))
	{
		return false;
	}

	const float T = (DragPlaneZ - WorldOrigin.Z) / WorldDirection.Z;
	if (T < 0.f)
	{
		return false;
	}

	OutLocation = WorldOrigin + WorldDirection * T;
	return true;
}

bool APuzzlePlayerController::GetBoardPlaneHitUnderCursor(FVector& OutLocation) const
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		return false;
	}

	return ProjectRayToBoard(WorldOrigin, WorldDirection, OutLocation);
}

bool APuzzlePlayerController::DeprojectScreenToBoardPlane(const FVector2D& ViewportPixelPosition, FVector& OutWorldLocation) const
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportPixelPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	return ProjectRayToBoard(WorldOrigin, WorldDirection, OutWorldLocation);
}

APuzzlePiece* APuzzlePlayerController::SpawnPieceFromUI(int32 PieceId, const FVector& WorldLocation)
{
	APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>();
	if (!GS || !DefaultPieceClass)
	{
		return nullptr;
	}

	APuzzlePiece* Spawned = GS->SpawnPieceAtLocation(DefaultPieceClass, PieceId, WorldLocation);
	if (!Spawned)
	{
		return nullptr;
	}

	// Soft-snap to nearest slot so drops land on the board cell under the cursor.
	if (APuzzleSlot* Slot = GS->FindNearestSlot(WorldLocation, SlotSnapMaxDistance))
	{
		Spawned->SnapToSlot(Slot);
	}

	GS->RegisterMove();
	return Spawned;
}
