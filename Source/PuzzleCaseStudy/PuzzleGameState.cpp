#include "PuzzleGameState.h"
#include "PuzzleSlot.h"

APuzzleGameState::APuzzleGameState()
{
	PrimaryActorTick.bCanEverTick = true;
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
