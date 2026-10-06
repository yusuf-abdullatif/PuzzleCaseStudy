#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzlePiece.generated.h"

class UStaticMeshComponent;
class APuzzleSlot;

/**
 * Draggable puzzle cube. CorrectPieceId must match the slot's ExpectedPieceId when solved.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzlePiece : public AActor
{
	GENERATED_BODY()

public:
	APuzzlePiece();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Piece")
	int32 GetCorrectPieceId() const { return CorrectPieceId; }

	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void SetCurrentSlot(APuzzleSlot* Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Piece")
	APuzzleSlot* GetCurrentSlot() const { return CurrentSlot; }

	/** Moves the actor to the slot snap point (used after drop / swap). */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void SnapToSlot(APuzzleSlot* Slot);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<UStaticMeshComponent> PieceMesh;

	/** Which slot index this piece belongs in when the puzzle is solved (0–8). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle|Piece")
	int32 CorrectPieceId = 0;

	UPROPERTY()
	TObjectPtr<APuzzleSlot> CurrentSlot;
};
