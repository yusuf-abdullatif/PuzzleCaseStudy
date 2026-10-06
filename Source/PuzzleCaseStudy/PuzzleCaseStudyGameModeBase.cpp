#include "PuzzleCaseStudyGameModeBase.h"
#include "PuzzleGameState.h"
#include "PuzzlePlayerController.h"
#include "PuzzleViewPawn.h"

APuzzleCaseStudyGameModeBase::APuzzleCaseStudyGameModeBase()
{
	DefaultPawnClass = APuzzleViewPawn::StaticClass();
	PlayerControllerClass = APuzzlePlayerController::StaticClass();
	GameStateClass = APuzzleGameState::StaticClass();
}
