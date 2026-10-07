#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzlePlayerController.generated.h"

class APuzzlePiece;
class APuzzleSlot;
class UPuzzleHUDWidget;
class UPuzzleEndScreenWidget;
class UPuzzlePieceListWidget;
class UPuzzleBoardDropWidget;

/**
 * Mouse drag for world pieces: snap to slots, swap on piece overlap, counts moves via game state.
 */
UCLASS(Blueprintable)
class PUZZLECASESTUDY_API APuzzlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APuzzlePlayerController();

	/** Spawns a piece from the UI list at a world location on the board plane. */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|Spawn")
	APuzzlePiece* SpawnPieceFromUI(int32 PieceId, const FVector& WorldLocation);

	/**
	 * Viewport-pixel screen position → board world position.
	 * Prefers a visibility line-trace (correct under angled cameras), then plane fallback.
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle|Spawn")
	bool DeprojectScreenToBoardPlane(const FVector2D& ViewportPixelPosition, FVector& OutWorldLocation) const;

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	void HandleDragPress();
	void HandleDragRelease();
	void UpdateDraggedPieceLocation();
	bool GetBoardPlaneHitUnderCursor(FVector& OutLocation) const;
	bool ProjectRayToBoard(const FVector& WorldOrigin, const FVector& WorldDirection, FVector& OutLocation) const;
	APuzzlePiece* GetPieceUnderCursor() const;
	APuzzlePiece* FindOtherPieceNear(const FVector& WorldLocation, float Radius) const;
	void CommitDrop();

	/** Lift above floor/slot hit so the cube rests on the surface (matches slot snap feel). */
	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|Interaction")
	float PieceRestHeight = 50.f;

	UFUNCTION()
	void HandlePuzzleWon();

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|Classes")
	TSubclassOf<APuzzlePiece> DefaultPieceClass;

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|UI")
	TSubclassOf<UPuzzleHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|UI")
	TSubclassOf<UPuzzleEndScreenWidget> EndScreenWidgetClass;

	/** Full-screen drop target (WBP_BoardDropTarget). ZOrder below the piece list. */
	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|UI")
	TSubclassOf<UPuzzleBoardDropWidget> BoardDropWidgetClass;

	/** Shuffled piece list (WBP_PieceList). */
	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|UI")
	TSubclassOf<UPuzzlePieceListWidget> PieceListWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|Interaction")
	float SlotSnapMaxDistance = 120.f;

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle|Interaction")
	float PieceSwapRadius = 70.f;

	UPROPERTY(BlueprintReadOnly, Category = "Puzzle|Interaction")
	TObjectPtr<APuzzlePiece> DraggedPiece;

	UPROPERTY(BlueprintReadOnly, Category = "Puzzle|Interaction")
	bool bIsDraggingPiece = false;

	UPROPERTY()
	TObjectPtr<UPuzzleHUDWidget> HUDWidget;

	UPROPERTY()
	TObjectPtr<UPuzzleEndScreenWidget> EndScreenWidget;

	UPROPERTY()
	TObjectPtr<UPuzzleBoardDropWidget> BoardDropWidget;

	UPROPERTY()
	TObjectPtr<UPuzzlePieceListWidget> PieceListWidget;

	float DragPlaneZ = 0.f;
};
