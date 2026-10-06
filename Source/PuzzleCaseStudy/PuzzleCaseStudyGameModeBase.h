#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PuzzleCaseStudyGameModeBase.generated.h"

/**
 * Sets default pawn, controller, and game state classes for the puzzle case study.
 * Override in a Blueprint (BP_PuzzleGameMode) to set as project default game mode.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzleCaseStudyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APuzzleCaseStudyGameModeBase();
};
