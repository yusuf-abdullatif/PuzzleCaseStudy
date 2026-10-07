#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleHUDWidget.generated.h"

/**
 * Base HUD widget; Blueprint implements OnStatsUpdated to show timer and move count.
 */
UCLASS(Abstract, Blueprintable)
class PUZZLECASESTUDY_API UPuzzleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Puzzle|HUD")
	void RefreshFromGameState();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle|HUD")
	void OnStatsUpdated(float ElapsedSeconds, int32 MoveCount);
};
