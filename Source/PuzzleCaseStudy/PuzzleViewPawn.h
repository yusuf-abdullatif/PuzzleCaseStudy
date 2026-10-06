#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PuzzleViewPawn.generated.h"

class UCameraComponent;

/**
 * Fixed-view camera pawn for the puzzle board.
 * Place one in the level and set Auto Possess Player = Player 0 (see setup guide).
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzleViewPawn : public APawn
{
	GENERATED_BODY()

public:
	APuzzleViewPawn();

protected:
	/** World-facing camera used for mouse raycasts from the player controller. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
