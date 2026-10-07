#include "PuzzleGameState.h"
#include "PuzzlePiece.h"
#include "PuzzleSlot.h"
#include "Engine/World.h"

APuzzleGameState::APuzzleGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APuzzleGameState::BeginPlay()
{
	Super::BeginPlay();
	InitializeShuffledPieceOrder(9);
}

void APuzzleGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bPuzzleComplete)
	{
		ElapsedSeconds += DeltaSeconds;
	}
}

void APuzzleGameState::RegisterSlot(APuzzleSlot* Slot)
{
	if (Slot && !RegisteredSlots.Contains(Slot))
	{
		RegisteredSlots.Add(Slot);
	}
}

void APuzzleGameState::InitializeShuffledPieceOrder(int32 NumPieces)
{
	ShuffledPieceIds.Reset();
	for (int32 Id = 0; Id < NumPieces; ++Id)
	{
		ShuffledPieceIds.Add(Id);
	}

	// Fisher–Yates shuffle for UI listing order each run.
	for (int32 I = ShuffledPieceIds.Num() - 1; I > 0; --I)
	{
		const int32 J = FMath::RandRange(0, I);
		ShuffledPieceIds.Swap(I, J);
	}
}

int32 APuzzleGameState::GetShuffledPieceIdAt(int32 Index) const
{
	if (!ShuffledPieceIds.IsValidIndex(Index))
	{
		return INDEX_NONE;
	}
	return ShuffledPieceIds[Index];
}

APuzzlePiece* APuzzleGameState::SpawnPieceAtLocation(TSubclassOf<APuzzlePiece> PieceClass, int32 PieceId, const FVector& WorldLocation)
{
	if (bPuzzleComplete || !PieceClass || SpawnedPieceIds.Contains(PieceId))
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APuzzlePiece* NewPiece = World->SpawnActor<APuzzlePiece>(PieceClass, WorldLocation, FRotator::ZeroRotator, SpawnParams);
	if (!NewPiece)
	{
		return nullptr;
	}

	NewPiece->SetCorrectPieceId(PieceId);
	SpawnedPieceIds.Add(PieceId);
	OnPieceSpawned.Broadcast(PieceId);
	return NewPiece;
}

APuzzleSlot* APuzzleGameState::FindNearestSlot(const FVector& WorldLocation, float MaxDistance) const
{
	APuzzleSlot* BestSlot = nullptr;
	float BestDistSq = MaxDistance * MaxDistance;

	for (const APuzzleSlot* Slot : RegisteredSlots)
	{
		if (!IsValid(Slot))
		{
			continue;
		}

		const FVector Delta = Slot->GetActorLocation() - WorldLocation;
		const float DistSq2D = Delta.X * Delta.X + Delta.Y * Delta.Y;
		if (DistSq2D <= BestDistSq)
		{
			BestDistSq = DistSq2D;
			BestSlot = const_cast<APuzzleSlot*>(Slot);
		}
	}

	return BestSlot;
}

float APuzzleGameState::GetAverageSlotHeight() const
{
	if (RegisteredSlots.Num() == 0)
	{
		return 0.f;
	}

	// Use snap height so deprojected cursor matches where pieces sit (not Z=0 under the board).
	float Sum = 0.f;
	int32 Count = 0;
	for (const APuzzleSlot* Slot : RegisteredSlots)
	{
		if (IsValid(Slot))
		{
			Sum += Slot->GetSnapLocation().Z;
			++Count;
		}
	}
	return Count > 0 ? Sum / static_cast<float>(Count) : 0.f;
}

void APuzzleGameState::RegisterMove()
{
	if (bPuzzleComplete)
	{
		return;
	}

	++MoveCount;
	EvaluateWinCondition();
}

void APuzzleGameState::EvaluateWinCondition()
{
	if (bPuzzleComplete || RegisteredSlots.Num() == 0)
	{
		return;
	}

	if (AreAllPiecesCorrect())
	{
		bPuzzleComplete = true;
		OnPuzzleWon.Broadcast();
	}
}

bool APuzzleGameState::AreAllPiecesCorrect() const
{
	for (const APuzzleSlot* Slot : RegisteredSlots)
	{
		if (!IsValid(Slot) || !Slot->IsCorrectlyFilled())
		{
			return false;
		}
	}
	return true;
}
