#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PuzzleGameState.generated.h"

class APuzzleSlot;
class APuzzlePiece;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleWon);

/**
 * Replicated-friendly run stats and win tracking for the placement puzzle.
 * Slots register themselves when placed in the level.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzleGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	APuzzleGameState();

	virtual void Tick(float DeltaSeconds) override;

	/** Called by APuzzleSlot when it enters play. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|State")
	void RegisterSlot(APuzzleSlot* Slot);

	/** Increments move counter (piece placed, swap, etc.). */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|State")
	void RegisterMove();

	/** Returns true when every registered slot holds the correct piece. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	bool AreAllPiecesCorrect() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	int32 GetMoveCount() const { return MoveCount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	float GetElapsedSeconds() const { return ElapsedSeconds; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	bool IsPuzzleComplete() const { return bPuzzleComplete; }

	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Events")
	FOnPuzzleWon OnPuzzleWon;

protected:
	void EvaluateWinCondition();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	int32 MoveCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	float ElapsedSeconds = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	bool bPuzzleComplete = false;

	UPROPERTY()
	TArray<TObjectPtr<APuzzleSlot>> RegisteredSlots;
};
