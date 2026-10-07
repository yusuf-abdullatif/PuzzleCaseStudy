#include "PuzzleHUDWidget.h"
#include "PuzzleGameState.h"
void UPuzzleHUDWidget::RefreshFromGameState()
{
	if (const APuzzleGameState* GS = GetWorld()->GetGameState<APuzzleGameState>())
	{
		if (!GS->IsPuzzleComplete())
		{
			OnStatsUpdated(GS->GetElapsedSeconds(), GS->GetMoveCount());
		}
	}
}
