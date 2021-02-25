// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3Block.h"
#include "Match3BlockGrid.generated.h"







/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMatch3BlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* MovesText;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* RestartSpriteComponent;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
		class UPaperSprite* RestartSprite;

public:
	AMatch3BlockGrid();

	/** How many blocks have been clicked */
	int Score;

	//Grid Properties
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int GridSize_W;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int GridSize_H;

	/** Size will be width* heigth **/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int Size;
	
	/* Collection of all the Block Actors */
	TArray<AMatch3Block *> BlockActors;

	/* Grid Handling */
	void UpdateGrid();
	
	/* Updating Block actors on Grid  */
	void DestroyBlocks(TArray<int>& Blockindices) {}
	void SpawnReplacementBlocks();

	TArray<int> CalculateReplacementIndices();
		
	bool CheckBlockForSelection(int BlockIndex) {}
	void ClearSelection() {}

	void CheckAndSetFirstClick(int BlockIndex);
	
	void CheckForHover(int BlockIndex);
	
	/* Input Handling  */
	void HandleInputPressed();
	void HandleInputRelease();
	

	UFUNCTION()
	void RestartClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);


	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	int updateBlock(int OldId, int decrement);

	void SetScoreSprites();
	
	void SetMovesSprites();
	
protected:
	// Begin AActor interface
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	// End AActor interface

private:
	TArray<int> SelectedBlocks;
	EBlockFactors CurrentFactor;

	int MinRunLength;

	EGameState GameState;



public:

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }
	FORCEINLINE class UTextRenderComponent* GetMovesText() const { return MovesText; }

	float animTime = 0.5f;

	void FinishAnimating();

	int MovesLeft = 30;


	UPaperFlipbook* Score1;
	
	UPaperFlipbook* Moves1;
	
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderScore1Component;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderScore2Component;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderScore3Component;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderScore4Component;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderScore5Component;


	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderMoves1Component;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderMoves2Component;
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderMoves3Component;

	void AddPowerBricks(int BlockIndex);

	bool HasPower = false;

	void CheckForUnSelected(int BlockIndex);

	void GameOverAnimation() {}
	void RestartGame();
	void SpawnAllBlocks();
};



