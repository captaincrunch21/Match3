// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3Block.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Match3BlockGrid.h"
#include "Math/UnrealMathUtility.h"

AMatch3Block::AMatch3Block()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		
		
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p1Idle;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p1Selected;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p1SelesctedStart;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p1Falling;
		
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p2Idle;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p2Selected;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p2SelesctedStart;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p2Falling;
		
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p3Idle;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p3Selected;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p3SelesctedStart;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p3Falling;

		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> p4Special;

		


		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Materials/BlueMaterial_Inst.BlueMaterial_Inst"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		
			, p1Idle(TEXT("/Game/Sprites_Data/p1_Flipbooks/p1IdleFlipbook.p1IdleFlipbook"))
			, p1Selected(TEXT("/Game/Sprites_Data/p1_Flipbooks/p1SelectedStayFlipbook.p1SelectedStayFlipbook"))
			, p1SelesctedStart(TEXT("/Game/Sprites_Data/p1_Flipbooks/p1SelectedFlipbook.p1SelectedFlipbook"))
			, p1Falling(TEXT("/Game/Sprites_Data/p1_Flipbooks/p1FallFlipbook.p1FallFlipbook"))

			, p2Idle(TEXT("/Game/Sprites_Data/p2_Flipbooks/p1IdleFlipbook.p1IdleFlipbook"))
			, p2Selected(TEXT("/Game/Sprites_Data/p2_Flipbooks/p1SelectedStayFlipbook.p1SelectedStayFlipbook"))
			, p2SelesctedStart(TEXT("/Game/Sprites_Data/p2_Flipbooks/p1SelectedFlipbook.p1SelectedFlipbook"))
			, p2Falling(TEXT("/Game/Sprites_Data/p2_Flipbooks/p1FallFlipbook.p1FallFlipbook"))

			, p3Idle(TEXT("/Game/Sprites_Data/p3_Flipbooks/p1IdleFlipbook.p1IdleFlipbook"))
			, p3Selected(TEXT("/Game/Sprites_Data/p3_Flipbooks/p1SelectedStayFlipbook.p1SelectedStayFlipbook"))
			, p3SelesctedStart(TEXT("/Game/Sprites_Data/p3_Flipbooks/p1SelectedFlipbook.p1SelectedFlipbook"))
			, p3Falling(TEXT("/Game/Sprites_Data/p3_Flipbooks/p1FallFlipbook.p1FallFlipbook"))

			, p4Special(TEXT("/Game/Characters/Rainbow.Rainbow"))

			
		
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(0.80,0.80f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMatch3Block::BlockClicked);
	//BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMatch3Block::OnFingerPressedBlock);
	BlockMesh->OnBeginCursorOver.AddDynamic(this, &AMatch3Block::BeginMouseOverBlock);
	BlockMesh->OnEndCursorOver.AddDynamic(this, &AMatch3Block::EndMouseOverBlock);

	BlockMesh->bOverrideLightMapRes = 127;
	BlockMesh->CastShadow = 0;
	BlockMesh->SetMobility(EComponentMobility::Movable);


	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();

	RenderComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderComponent"));

	 p1Idle			 = ConstructorStatics.p1Idle.Get();
	 p1Selected		 = ConstructorStatics.p1Selected.Get();
	 p1SelesctedStart = ConstructorStatics.p1SelesctedStart.Get();
	 p1Falling		 = ConstructorStatics.p1Falling.Get();
	 
	 p2Idle			 = ConstructorStatics.p2Idle.Get();
	 p2Selected		 = ConstructorStatics.p2Selected.Get();
	 p2SelesctedStart = ConstructorStatics.p2SelesctedStart.Get();
	 p2Falling		 = ConstructorStatics.p2Falling.Get();
	 
	 p3Idle			 = ConstructorStatics.p3Idle.Get();
	 p3Selected		 = ConstructorStatics.p3Selected.Get();
	 p3SelesctedStart = ConstructorStatics.p3SelesctedStart.Get();
	 p3Falling		 = ConstructorStatics.p3Falling.Get();

	 p4Special = ConstructorStatics.p4Special.Get();
	 
	 RenderComponent->SetWorldRotation(FRotator(0, -90, 90));
	RenderComponent->SetupAttachment(DummyRoot);
	RenderComponent->SetRelativeLocation(FVector(0, 0, 30));
	RenderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//RenderComponent->SetRelativeRotation(FRotator(90.0, 0.0, -90.0));
	RenderComponent->SetFlipbook(p1Idle);
	//BlockMesh->SetVisibility(false);

	//RenderComponent->SetRelativeScale3D(FVector(0.1,0.1,0.1));

	MoveTo = 0;


}

void AMatch3Block::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMatch3Block::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMatch3Block::HandleClicked()
{
	// Check we are not already active
	if (BlockFactor != EBlockFactors::PLAYER4) 
	{

	

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->CheckAndSetFirstClick(BlockIndex);
		}
	}
}

void AMatch3Block::Highlight(bool bOn)
{
	Selected = bOn;
	if (bOn)
	{
		BlockMesh->SetMaterial(0, OrangeMaterial);

		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1Selected);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2Selected);
		}
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3Selected);
		}
		else
		{

		}

	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1Idle);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2Idle);
		}		
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3Idle);
		}
		else
		{

		}
	}
}

void AMatch3Block::SetBlockFactor(EBlockFactors Factor)
{
	BlockFactor = Factor;
	if (BlockFactor == EBlockFactors::PLAYER1)
	{
		RenderComponent->SetFlipbook(p1Idle);
	}
	else if (BlockFactor == EBlockFactors::PLAYER2)
	{
		RenderComponent->SetFlipbook(p2Idle);
	}
	else if (BlockFactor == EBlockFactors::PLAYER3)
	{
		RenderComponent->SetFlipbook(p3Idle);
	}
	else
	{
		Special = true;
		RenderComponent->SetFlipbook(p4Special);
		RenderComponent->SetRelativeScale3D(FVector(0.1));
	}
}

EBlockFactors AMatch3Block::GetBlockFactor()
{
	return BlockFactor;
}

void AMatch3Block::BeginMouseOverBlock(UPrimitiveComponent * TouchedComponent)
{
	if (!Selected)
	{
		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1SelesctedStart);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2SelesctedStart);
		}
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3SelesctedStart);
		}
		else
		{

		}

		OwningGrid->CheckForHover(BlockIndex);




	}
	else
	{
		OwningGrid->CheckForUnSelected(BlockIndex);
	}
}

void AMatch3Block::EndMouseOverBlock(UPrimitiveComponent * TouchedComponent)
{
	if (!Selected)
	{
		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1Idle);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2Idle);
		}
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3Idle);
		}
		else
		{

		}

	}
}

void AMatch3Block::UpdatePosition()
{
	if (MoveTo > 0) {
		FVector MoveDown = GetActorLocation() - FVector( MoveTo * OwningGrid->BlockSpacing, 0,0);
		//SetActorLocation(MoveDown);
		int x, y;
		x = BlockIndex / OwningGrid->GridSize_H;
		y = BlockIndex % OwningGrid->GridSize_H;
		
		x = x - MoveTo;
		BlockIndex = (x * OwningGrid->GridSize_H) + y;
		OwningGrid->BlockActors[BlockIndex] = this;
		StartPosition = GetActorLocation();
		EndPosition = MoveDown;


		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1Falling);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2Falling);
		}
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3Falling);
		}
		else
		{

		}



	}
}

bool AMatch3Block::GetSelected()
{
	return Selected;
}

void AMatch3Block::Animate(float inSecs)
{
	if (MoveTo > 0)
	{
		Animtime += inSecs;
		auto Pos = FMath::Lerp(StartPosition, EndPosition, Animtime/OwningGrid->animTime);

		SetActorLocation(Pos);
	}
}

void AMatch3Block::FinishAnimation()
{
	if (MoveTo > 0)
	{
		SetActorLocation(EndPosition);
		MoveTo = 0;
		Animtime = 0.0f;
		if (BlockFactor == EBlockFactors::PLAYER1)
		{
			RenderComponent->SetFlipbook(p1Idle);
		}
		else if (BlockFactor == EBlockFactors::PLAYER2)
		{
			RenderComponent->SetFlipbook(p2Idle);
		}
		else if (BlockFactor == EBlockFactors::PLAYER3)
		{
			RenderComponent->SetFlipbook(p3Idle);
		}
		else
		{

		}
	}
}

