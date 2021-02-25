// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3BlockActorBase.h"
#include "Match3BlockGrid.h"

// Sets default values
AMatch3BlockActorBase::AMatch3BlockActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMatch3BlockActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMatch3BlockActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMatch3BlockActorBase::BeginMouseOverBlock(UPrimitiveComponent* TouchedComponent) {
	if (!Selected)
	{
		RenderComponent->SetFlipbook(HoverAnimation);
		OwningGrid->CheckForHover(BlockIndex);
	}
	else
	{
		OwningGrid->CheckForUnSelected(BlockIndex);
	}
}

void AMatch3BlockActorBase::EndMouseOverBlock(UPrimitiveComponent* TouchedComponent)
{
	if (!Selected)
	{
		RenderComponent->SetFlipbook(IdleAnimation);
	}

}

void AMatch3BlockActorBase::UpdatePosition()
{
	if (MoveTo > 0) {
		FVector MoveDown = GetActorLocation() - FVector(MoveTo * OwningGrid->BlockSpacing, 0, 0);
		//SetActorLocation(MoveDown);
		int x, y;
		x = BlockIndex / OwningGrid->GridSize_H;
		y = BlockIndex % OwningGrid->GridSize_H;

		x = x - MoveTo;
		BlockIndex = (x * OwningGrid->GridSize_H) + y;
		//OwningGrid->BlockActors[BlockIndex] = this;
		StartPosition = GetActorLocation();
		EndPosition = MoveDown;
		RenderComponent->SetFlipbook(FallingAnimation);

	}
}

bool AMatch3BlockActorBase::GetSelected()
{
	return Selected;
}

void AMatch3BlockActorBase::Highlight(bool bOn)
{
	Selected = bOn;
	if (bOn)
	{
		BlockMesh->SetMaterial(0, OrangeMaterial);
		RenderComponent->SetFlipbook(SelectedAnimation);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
		RenderComponent->SetFlipbook(IdleAnimation);
	}
}

void AMatch3BlockActorBase::SetBlockFactor(EBlockFactors Factor)
{
	BlockFactor = Factor;
}

void AMatch3BlockActorBase::AddMoveDown()
{
	MoveTo++;
}

int AMatch3BlockActorBase::GetBlockIndex()
{
	return BlockIndex;
}

void AMatch3BlockActorBase::SetBlockIndex(int index)
{
	BlockIndex = index;
}


void AMatch3BlockActorBase::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}