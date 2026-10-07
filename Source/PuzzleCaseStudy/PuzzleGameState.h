#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PuzzleGameState.generated.h"

class APuzzleSlot;
class APuzzlePiece;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleWon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzlePieceSpawned, int32, PieceId);

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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Called by APuzzleSlot when it enters play. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|State")
	void RegisterSlot(APuzzleSlot* Slot);

	/** Shuffles 0..NumPieces-1 once per run (UI list order). */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|State")
	void InitializeShuffledPieceOrder(int32 NumPieces = 9);

	/** Copy of shuffled ids for UMG (Blueprint-safe by-value return). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	TArray<int32> GetShuffledPieceIds() const { return ShuffledPieceIds; }

	/** Use with Blueprint "For Loop" if "For Each Loop" does not appear in search. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	int32 GetShuffledPieceCount() const { return ShuffledPieceIds.Num(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	int32 GetShuffledPieceIdAt(int32 Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	bool IsPieceAlreadySpawned(int32 PieceId) const { return SpawnedPieceIds.Contains(PieceId); }

	/** Spawns a piece if not already spawned; sets CorrectPieceId from PieceId. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|State")
	APuzzlePiece* SpawnPieceAtLocation(TSubclassOf<APuzzlePiece> PieceClass, int32 PieceId, const FVector& WorldLocation);

	/** Finds the closest registered slot within MaxDistance (XY plane). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	APuzzleSlot* FindNearestSlot(const FVector& WorldLocation, float MaxDistance) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|State")
	float GetAverageSlotHeight() const;

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

	/** Fired after a UI piece is spawned into the world (for list row styling). */
	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Events")
	FOnPuzzlePieceSpawned OnPieceSpawned;

protected:
	void EvaluateWinCondition();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	int32 MoveCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	float ElapsedSeconds = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	bool bPuzzleComplete = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|State")
	TArray<int32> ShuffledPieceIds;

	UPROPERTY()
	TArray<TObjectPtr<APuzzleSlot>> RegisteredSlots;

	UPROPERTY()
	TSet<int32> SpawnedPieceIds;
};
