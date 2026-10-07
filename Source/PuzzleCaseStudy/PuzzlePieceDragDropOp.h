#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "PuzzlePieceDragDropOp.generated.h"

/**
 * Payload carried while dragging a piece from the UI list onto the 3D board.
 * Create this in OnDragDetected; read PieceId in OnDrop.
 */
UCLASS(BlueprintType, Blueprintable)
class PUZZLECASESTUDY_API UPuzzlePieceDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()

public:
	/** Which puzzle piece (0–8) is being dragged from the list. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle|UI", meta = (ExposeOnSpawn = "true"))
	int32 PieceId = 0;
};
