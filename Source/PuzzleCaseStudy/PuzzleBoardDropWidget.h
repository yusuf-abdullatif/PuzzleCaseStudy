#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleBoardDropWidget.generated.h"

/**
 * Full-screen drop catcher for UI → world piece spawn.
 * Reparent WBP_BoardDropTarget to this; NativeOnDrop handles spawn in C++.
 */
UCLASS(Abstract, Blueprintable)
class PUZZLECASESTUDY_API UPuzzleBoardDropWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
