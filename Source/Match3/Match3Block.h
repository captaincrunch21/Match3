// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Match3Block.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class AMatch3Block : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderComponent;

	

public:
	AMatch3Block();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class AMatch3BlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void Highlight(bool bOn);


	void SetBlockFactor(EBlockFactors Factor);
	EBlockFactors GetBlockFactor();

	void SetBlockIndex(int index)
	{
		BlockIndex = index;
	}

	int GetBlockIndex()
	{
		return BlockIndex;
	}

	void AddMoveDown()
	{
		MoveTo++;
	}

	UFUNCTION()
	void BeginMouseOverBlock(UPrimitiveComponent* TouchedComponent);
	
	UFUNCTION()
	void EndMouseOverBlock(UPrimitiveComponent* TouchedComponent);

	void UpdatePosition();

	bool GetSelected();

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

	void Animate(float inSecs);
	void FinishAnimation();


private:
	EBlockFactors BlockFactor;
	int BlockIndex;
	int MoveTo;
	bool Selected = false;

	UPaperFlipbook* p1Idle;
	UPaperFlipbook* p1Selected;
	UPaperFlipbook* p1SelesctedStart;
	UPaperFlipbook* p1Falling;

	UPaperFlipbook* p2Idle;
	UPaperFlipbook* p2Selected;
	UPaperFlipbook* p2SelesctedStart;
	UPaperFlipbook* p2Falling;

	UPaperFlipbook* p3Idle;
	UPaperFlipbook* p3Selected;
	UPaperFlipbook* p3SelesctedStart;
	UPaperFlipbook* p3Falling;

	UPaperFlipbook* p4Special;

	float Animtime = 0.0f;

	FVector StartPosition;
	FVector EndPosition;

	bool Special = false;
	bool Locked = false;

};