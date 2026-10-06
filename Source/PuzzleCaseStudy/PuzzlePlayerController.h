#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzlePlayerController.generated.h"

/**
 * Mouse-only controller: cursor visible, click/mouse-over enabled for UI and world picks later.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APuzzlePlayerController();

protected:
	virtual void BeginPlay() override;
};
