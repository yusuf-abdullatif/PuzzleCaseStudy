#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleEndScreenWidget.generated.h"

/**
 * End screen shown when all pieces are correct; Blueprint lays out time and move labels.
 * Call RestartPuzzle from a Reset button to reload the current level.
 */
UCLASS(Abstract, Blueprintable)
class PUZZLECASESTUDY_API UPuzzleEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void ShowResults(float ElapsedSeconds, int32 MoveCount);

	/** Reloads the current map (fresh shuffle, empty board, new timer). Wire to Reset button. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void RestartPuzzle();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle|UI")
	void OnShowResults(float ElapsedSeconds, int32 MoveCount);
};
