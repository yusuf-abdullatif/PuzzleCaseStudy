#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleEndScreenWidget.generated.h"

/**
 * End screen shown when all pieces are correct; Blueprint lays out time and move labels.
 */
UCLASS(Abstract, Blueprintable)
class PUZZLECASESTUDY_API UPuzzleEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void ShowResults(float ElapsedSeconds, int32 MoveCount);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle|UI")
	void OnShowResults(float ElapsedSeconds, int32 MoveCount);
};
