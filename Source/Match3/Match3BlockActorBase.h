// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Match3BlockActorBase.generated.h"

UCLASS(Abstract)
class MATCH3_API AMatch3BlockActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatch3BlockActorBase();
	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
		class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
		class UMaterialInstance* OrangeMaterial;

	/** Grid that owns us */
	UPROPERTY()
		class AMatch3BlockGrid* OwningGrid;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RenderComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/** Handle the block being clicked */
	UFUNCTION()
		virtual void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	virtual void HandleClicked() {};

	void SetBlockIndex(int index);

	int GetBlockIndex();
	
	
	void AddMoveDown();
	
	void SetBlockFactor(EBlockFactors Factor);
	
	UFUNCTION()
		virtual	void BeginMouseOverBlock(UPrimitiveComponent* TouchedComponent);
	

	
	UFUNCTION()
		virtual	void EndMouseOverBlock(UPrimitiveComponent* TouchedComponent);


	virtual void UpdatePosition();
	

	bool GetSelected();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Animate(float inSecs) {};
	virtual void FinishAnimation() {};

	virtual void Highlight(bool bOn);

private:

	//Game Parameters
	EBlockFactors BlockFactor;
	int BlockIndex;
	int MoveTo;
	bool Selected = false;


	//Flipbooks For State Animation
	UPaperFlipbook* IdleAnimation;
	UPaperFlipbook* HoverAnimation;
	UPaperFlipbook* SelectedAnimation;
	UPaperFlipbook* FallingAnimation;
	UPaperFlipbook* DeathAnimation;
	UPaperFlipbook* LockedAnimation;


	//Animation Parameters
	float Animtime = 0.0f;
	FVector StartPosition;
	FVector EndPosition;


	// Special Features TBD
	bool Special = false;
	bool Locked = false;



};
