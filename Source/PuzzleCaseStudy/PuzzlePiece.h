#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzlePiece.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class APuzzleSlot;

/**
 * Draggable puzzle cube. CorrectPieceId must match the slot's ExpectedPieceId when solved.
 * Shows the id as world text on top of the mesh after spawn.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzlePiece : public AActor
{
	GENERATED_BODY()

public:
	APuzzlePiece();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Piece")
	int32 GetCorrectPieceId() const { return CorrectPieceId; }

	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void SetCorrectPieceId(int32 InId);

	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void SetCurrentSlot(APuzzleSlot* Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Piece")
	APuzzleSlot* GetCurrentSlot() const { return CurrentSlot; }

	/** Moves the actor to the slot snap point (used after drop / swap). */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void SnapToSlot(APuzzleSlot* Slot);

	/** Swaps world/slot ownership with another piece (requirement: drop on piece). */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|Piece")
	void ExchangeWith(APuzzlePiece* Other);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Piece")
	UStaticMeshComponent* GetPieceMesh() const { return PieceMesh; }

protected:
	/** Updates the floating id label to match CorrectPieceId. */
	void RefreshIdLabel();

	/** Tilts the label toward the player camera so a 45° top view stays readable. */
	void OrientIdLabelToCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<UStaticMeshComponent> PieceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<UTextRenderComponent> IdText;

	/** Which slot index this piece belongs in when the puzzle is solved (0–8). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle|Piece")
	int32 CorrectPieceId = 0;

	UPROPERTY()
	TObjectPtr<APuzzleSlot> CurrentSlot;
};
