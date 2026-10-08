#include "PuzzleEndScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UPuzzleEndScreenWidget::ShowResults(float ElapsedSeconds, int32 MoveCount)
{
	OnShowResults(ElapsedSeconds, MoveCount);
}

void UPuzzleEndScreenWidget::RestartPuzzle()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(World, true);
	if (LevelName.IsEmpty())
	{
		return;
	}

	UGameplayStatics::OpenLevel(World, FName(*LevelName));
}
