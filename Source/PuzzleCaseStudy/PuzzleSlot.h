#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleSlot.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class APuzzlePiece;

/**
 * One cell on the 3x3 board. ExpectedPieceId must match APuzzlePiece::CorrectPieceId when solved.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzleSlot : public AActor
{
	GENERATED_BODY()

public:
	APuzzleSlot();

	virtual void BeginPlay() override;

	/** Index 0–8 for a 3x3 board; which piece belongs here when solved. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle|Slot")
	int32 ExpectedPieceId = 0;

	/** World transform where a piece should sit when snapped into this slot. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Slot")
	FVector GetSnapLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Puzzle|Slot")
	void SetOccupyingPiece(APuzzlePiece* Piece);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Slot")
	APuzzlePiece* GetOccupyingPiece() const { return OccupyingPiece; }

	/** True if the occupying piece's CorrectPieceId matches this slot's ExpectedPieceId. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Slot")
	bool IsCorrectlyFilled() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<UStaticMeshComponent> SlotMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
	TObjectPtr<UBoxComponent> SnapVolume;

	UPROPERTY()
	TObjectPtr<APuzzlePiece> OccupyingPiece;
};
