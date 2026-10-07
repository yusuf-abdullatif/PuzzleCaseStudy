#include "PuzzleBoardDropWidget.h"
#include "PuzzlePieceDragDropOp.h"
#include "PuzzlePlayerController.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"

bool UPuzzleBoardDropWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return InOperation && InOperation->IsA(UPuzzlePieceDragDropOp::StaticClass());
}

bool UPuzzleBoardDropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UPuzzlePieceDragDropOp* PieceOp = Cast<UPuzzlePieceDragDropOp>(InOperation);
	if (!PieceOp)
	{
		return false;
	}

	APuzzlePlayerController* PC = Cast<APuzzlePlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return false;
	}

	// DragDrop gives absolute desktop coords; Deproject expects viewport pixels.
	// Using absolute coords with a 45° camera looks like the piece lands "below" the cursor.
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(this, InDragDropEvent.GetScreenSpacePosition(), PixelPosition, ViewportPosition);

	FVector WorldLocation;
	if (!PC->DeprojectScreenToBoardPlane(PixelPosition, WorldLocation))
	{
		// Fallback: current mouse in viewport space.
		float MouseX = 0.f;
		float MouseY = 0.f;
		if (!PC->GetMousePosition(MouseX, MouseY) || !PC->DeprojectScreenToBoardPlane(FVector2D(MouseX, MouseY), WorldLocation))
		{
			return false;
		}
	}

	return PC->SpawnPieceFromUI(PieceOp->PieceId, WorldLocation) != nullptr;
}
