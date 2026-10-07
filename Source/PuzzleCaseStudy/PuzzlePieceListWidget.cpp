#include "PuzzlePieceListWidget.h"
#include "PuzzleGameState.h"

void UPuzzlePieceListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APuzzleGameState* GS = GetWorld() ? GetWorld()->GetGameState<APuzzleGameState>() : nullptr)
	{
		GS->OnPieceSpawned.AddDynamic(this, &UPuzzlePieceListWidget::HandlePieceSpawned);
	}
}

void UPuzzlePieceListWidget::NativeDestruct()
{
	if (APuzzleGameState* GS = GetWorld() ? GetWorld()->GetGameState<APuzzleGameState>() : nullptr)
	{
		GS->OnPieceSpawned.RemoveDynamic(this, &UPuzzlePieceListWidget::HandlePieceSpawned);
	}

	Super::NativeDestruct();
}

void UPuzzlePieceListWidget::HandlePieceSpawned(int32 PieceId)
{
	OnPieceRowUsed(PieceId);
}

void UPuzzlePieceListWidget::PopulateFromGameState()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const APuzzleGameState* GS = World->GetGameState<APuzzleGameState>();
	if (!GS)
	{
		return;
	}

	const TArray<int32> Ids = GS->GetShuffledPieceIds();
	for (const int32 PieceId : Ids)
	{
		OnAddPieceRow(PieceId);
	}
}
