#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzlePieceListWidget.generated.h"

/**
 * Piece list UI base. Builds the shuffled list in C++ so Blueprint never needs a For Loop node.
 * Reparent WBP_PieceList to this class, then implement OnAddPieceRow / OnPieceRowUsed.
 */
UCLASS(Abstract, Blueprintable)
class PUZZLECASESTUDY_API UPuzzlePieceListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Call from Event Construct: reads shuffled ids from PuzzleGameState and fires OnAddPieceRow for each. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void PopulateFromGameState();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Implement in Blueprint: create a row widget for this PieceId and add it to your Vertical Box. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle|UI")
	void OnAddPieceRow(int32 PieceId);

	/** Implement in Blueprint: darken / disable the row for this PieceId after it was spawned. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle|UI")
	void OnPieceRowUsed(int32 PieceId);

	UFUNCTION()
	void HandlePieceSpawned(int32 PieceId);
};
