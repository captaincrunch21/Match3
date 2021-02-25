// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3BlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Match3Pawn.h"
#include "TimerManager.h"
#include "Match3BlockActorBase.h"
#include "PaperSpriteComponent.h"


#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMatch3BlockGrid::AMatch3BlockGrid()
{

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> Score1;
		

		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> Moves1;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> RestartSprite;
		
		FConstructorStatics() 
			: Score1(TEXT("/Game/Numbers/YellowNumbers.YellowNumbers"))
			, Moves1(TEXT("/Game/Numbers/GreenNumbers.GreenNumbers"))
			, RestartSprite(TEXT("/Game/cfbcd4336297cdd_Sprite_0.cfbcd4336297cdd_Sprite_0"))

		{
		}


	};
	static FConstructorStatics ConstructorStatics;


	Score1 = ConstructorStatics.Score1.Get();			  							   
	Moves1 = ConstructorStatics.Moves1.Get();

	RenderScore1Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderScore1Component"));
	RenderScore2Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderScore2Component"));
	RenderScore3Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderScore3Component"));
	RenderScore4Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderScore4Component"));
	RenderScore5Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderScore5Component"));
	RenderMoves1Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderMoves1Component"));
	RenderMoves2Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderMoves2Component"));
	RenderMoves3Component = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RenderMoves3Component"));

	RestartSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RestartSprite"));
	RestartSpriteComponent->SetSprite(RestartSprite);

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	RenderScore1Component->SetupAttachment(DummyRoot);
	RenderScore1Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore1Component->SetPlayRate(0.0);

	RenderScore2Component->SetupAttachment(DummyRoot);
	RenderScore2Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore2Component->SetPlayRate(0.0);

	RenderScore3Component->SetupAttachment(DummyRoot);
	RenderScore3Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore3Component->SetPlayRate(0.0);

	RenderScore4Component->SetupAttachment(DummyRoot);
	RenderScore4Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore4Component->SetPlayRate(0.0);

	RenderScore5Component->SetupAttachment(DummyRoot);
	RenderScore5Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore5Component->SetPlayRate(0.0);

	RenderMoves1Component->SetupAttachment(DummyRoot);
	RenderMoves1Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderMoves1Component->SetPlayRate(0.0);

	RenderMoves2Component->SetupAttachment(DummyRoot);
	RenderMoves2Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderMoves2Component->SetPlayRate(0.0);

	RenderMoves3Component->SetupAttachment(DummyRoot);
	RenderMoves3Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderMoves3Component->SetPlayRate(0.0);
	
	RestartSpriteComponent->SetupAttachment(DummyRoot);
	RestartSpriteComponent->SetWorldRotation(FRotator(0, -90, -90));
	RestartSpriteComponent->SetWorldLocation(FVector(-464,-1130, -53)); 
	RestartSpriteComponent->SetRelativeScale3D(FVector(.35f));

	RestartSpriteComponent->OnClicked.AddDynamic(this, &AMatch3BlockGrid::RestartClicked);

	// Create static mesh component
	/*ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);


	MovesText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MovesText0"));
	MovesText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	MovesText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	MovesText->SetText(FText::Format(LOCTEXT("MovesFmt", "Moves Left: {0}"), FText::AsNumber(MovesLeft)));
	MovesText->SetupAttachment(DummyRoot);*/

	// Set defaults

	GridSize_W = 10;
	GridSize_H = 8;
	Size = GridSize_H*GridSize_W;
	BlockSpacing = 95.0f;
	CurrentFactor = EBlockFactors::NONE;
	GameState = EGameState::IDLE;

	PrimaryActorTick.bCanEverTick = true;

	MinRunLength = 2;

}


int AMatch3BlockGrid::updateBlock(int OldId, int decrement)
{
	return 0;
}

void AMatch3BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	
	SpawnAllBlocks();

	SetMovesSprites();
}

void AMatch3BlockGrid::RestartGame()
{
	MovesLeft = 30;
	for (auto block : BlockActors)
	{
		block->Destroy();
	}
	Score = 0;
	SetScoreSprites();
	SetMovesSprites();
	SpawnAllBlocks();
}


void AMatch3BlockGrid::SpawnAllBlocks()
{
	// Number of blocks
	const int NumBlocks = Size;
	BlockActors.Empty();
	auto Match3Pawn = (AMatch3Pawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Match3Pawn->SetGameGrid(this);


	// Loop to spawn each block
	for (int BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / GridSize_H) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%GridSize_H) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

		//Improve Random Function
		int SelectedPower = FMath::RandRange(0, 40);
		if (SelectedPower < 38)
		{
			int BFint = FMath::RandRange(0, 2);
			switch (BFint)
			{
			case 0:
				NewBlock->SetBlockFactor(EBlockFactors::PLAYER1);
				break;
			case 1:
				NewBlock->SetBlockFactor(EBlockFactors::PLAYER2);
				break;
			case 2:
				NewBlock->SetBlockFactor(EBlockFactors::PLAYER3);
				break;
			default:
				break;
			}
		}
		else {
			NewBlock->SetBlockFactor(EBlockFactors::PLAYER4);
		}

		BlockActors.Add(NewBlock);
		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			NewBlock->SetBlockIndex(BlockIndex);
		}
	}


	const float XOffset = (GridSize_W*0.7) * BlockSpacing; // Divide by dimension
	const float YOffset = (GridSize_H*1.3) * BlockSpacing;
	const FVector TextLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
	//ScoreText->SetWorldLocation(TextLocation);
	//MovesText->SetWorldLocation(TextLocation + FVector(-30, 0, 0));

	FVector OffsetWidget = FVector(0, -60, 0);

	FVector TextSpacing = FVector(0, 66, 0);
	RenderScore5Component->SetWorldLocation(TextSpacing * 5 + TextLocation + OffsetWidget);
	RenderScore4Component->SetWorldLocation(TextSpacing * 4 + TextLocation + OffsetWidget);
	RenderScore3Component->SetWorldLocation(TextSpacing * 3 + TextLocation + OffsetWidget);
	RenderScore2Component->SetWorldLocation(TextSpacing * 2 + TextLocation + OffsetWidget);
	RenderScore1Component->SetWorldLocation(TextSpacing * 1 + TextLocation + OffsetWidget);

	RenderMoves3Component->SetWorldLocation(TextSpacing * 3 + TextLocation + OffsetWidget + FVector(-85, 0, 0));
	RenderMoves2Component->SetWorldLocation(TextSpacing * 2 + TextLocation + OffsetWidget + FVector(-85, 0, 0));
	RenderMoves1Component->SetWorldLocation(TextSpacing * 1 + TextLocation + OffsetWidget + FVector(-85, 0, 0));
	RenderScore5Component->SetFlipbook(Score1);
	RenderScore4Component->SetFlipbook(Score1);
	RenderScore3Component->SetFlipbook(Score1);
	RenderScore2Component->SetFlipbook(Score1);
	RenderScore1Component->SetFlipbook(Score1);

	RenderMoves3Component->SetFlipbook(Moves1);
	RenderMoves2Component->SetFlipbook(Moves1);
	RenderMoves1Component->SetFlipbook(Moves1);


	RenderScore1Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore2Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore3Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore4Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderScore5Component->SetWorldRotation(FRotator(0, 90, -90));

	RenderMoves1Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderMoves2Component->SetWorldRotation(FRotator(0, 90, -90));
	RenderMoves3Component->SetWorldRotation(FRotator(0, 90, -90));


}

void AMatch3BlockGrid::SetScoreSprites()
{
	int NScore = Score;
	int digit;
	if (NScore == 0)
	{
		digit = 1;
		RenderScore5Component->SetPlaybackPositionInFrames(digit, true);
		RenderScore4Component->SetPlaybackPositionInFrames(digit, true);
		RenderScore3Component->SetPlaybackPositionInFrames(digit, true);
		RenderScore2Component->SetPlaybackPositionInFrames(digit, true);
		RenderScore1Component->SetPlaybackPositionInFrames(digit, true);


		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderScore5Component->SetPlaybackPositionInFrames(digit, true);

	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderScore4Component->SetPlaybackPositionInFrames(digit, true);

	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderScore3Component->SetPlaybackPositionInFrames(digit, true);

	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderScore2Component->SetPlaybackPositionInFrames(digit, true);

	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderScore1Component->SetPlaybackPositionInFrames(digit, true);

	return;


}

void AMatch3BlockGrid::SetMovesSprites()
{
	int NScore = MovesLeft;
	int digit;
	if (NScore == 0)
	{
		RenderMoves3Component->SetPlaybackPositionInFrames(0, true);
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderMoves3Component->SetPlaybackPositionInFrames(digit, true);


	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderMoves2Component->SetPlaybackPositionInFrames(digit, true);


	if (NScore == 0)
	{
		return;
	}

	digit = NScore % 10;
	NScore = NScore / 10;
	digit++;
	RenderMoves1Component->SetPlaybackPositionInFrames(digit, true);
}

void AMatch3BlockGrid::Tick(float DeltaSeconds)
{
	if(GameState==EGameState::ANIMATING)
	{
		for (auto block : BlockActors)
		{
			block->Animate(DeltaSeconds);
		}
	}


}


void AMatch3BlockGrid::AddScore()
{
	// Increment score
	Score+=20;

	// Update text
	/*ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));

	MovesText->SetText(FText::Format(LOCTEXT("MovesFmt", "Moves Left: {0}"), FText::AsNumber(MovesLeft)));
*/

}


void AMatch3BlockGrid::FinishAnimating()
{
	GameState = EGameState::IDLE;
	for (auto block:BlockActors)
	{
		block->FinishAnimation();
	}

}

void AMatch3BlockGrid::AddPowerBricks(int BlockIndex)
{
	int block_x = BlockIndex / GridSize_H;
	int block_y = BlockIndex % GridSize_H;
	for (int i = block_x - 2; i < block_x + 3; i++) {
		for (int j = block_y - 2; j < block_y + 3; j++) {
			if (i >= 0 && i < GridSize_W && j >= 0 && j < GridSize_H) {
				int index = i * GridSize_H + j;
				if (BlockActors[index])
				{
					if (!BlockActors[index]->GetSelected()) 
					{
						BlockActors[index]->Highlight(true);
						SelectedBlocks.Add(index);
					}
				}
			}
		
		}
	}

}


void AMatch3BlockGrid::UpdateGrid()
{
	HasPower = false;
	bool destroy = false;
	if (SelectedBlocks.Num() > MinRunLength)
	{
		destroy = true;
		MovesLeft--;
		//MovesText->SetText(FText::Format(LOCTEXT("MovesFmt", "Moves Left: {0}"), FText::AsNumber(MovesLeft)));

	}




	for (auto id : SelectedBlocks)
	{
		if (destroy)
		{
			BlockActors[id]->Destroy();
			AddScore();
		}
		else
		{
			BlockActors[id]->Highlight(false);	
		}
	}
	if (destroy) 
	{
		SpawnReplacementBlocks();
		SetScoreSprites();
		SetMovesSprites();
	}
	SelectedBlocks.Empty();

}

void AMatch3BlockGrid::CheckAndSetFirstClick(int BlockIndex)
{
	if (MovesLeft > 0) {
		if (CurrentFactor == EBlockFactors::NONE)
		{
			CurrentFactor = BlockActors[BlockIndex]->GetBlockFactor();
			BlockActors[BlockIndex]->Highlight(true);
			SelectedBlocks.Add(BlockIndex);
		}
	}
}

void AMatch3BlockGrid::CheckForHover(int BlockIndex)
{
	if (!HasPower) {


		if (SelectedBlocks.Num() == 0)
		{
			return;
		}



		TEnumAsByte<EGameState> GameStateee = GameState;
		TEnumAsByte<EBlockFactors> EBlockFactorsddd = CurrentFactor;


		UE_LOG(LogTemp, Warning, TEXT("GameState: %i"), GameStateee);
		UE_LOG(LogTemp, Warning, TEXT("CurrentFactor: %i"), EBlockFactorsddd);

		int PreviousIndex = SelectedBlocks[SelectedBlocks.Num() - 1];

		if (PreviousIndex == BlockIndex)
		{
			return;
		}


		int previous_x = PreviousIndex / GridSize_H;
		int previous_y = PreviousIndex % GridSize_H;

		int block_x = BlockIndex / GridSize_H;
		int block_y = BlockIndex % GridSize_H;

		bool adj = false;
		if ((abs(block_x - previous_x) == 1 || abs(block_y - previous_y) == 1) && (abs(block_y - previous_y) * abs(block_x - previous_x) == 0))
		{
			adj = true;
		}

		if (adj) {
			if ((BlockActors[BlockIndex]->GetBlockFactor() == CurrentFactor || BlockActors[BlockIndex]->GetBlockFactor() == EBlockFactors::PLAYER4) && (GameState == EGameState::SELECTING))
			{
				BlockActors[BlockIndex]->Highlight(true);
				SelectedBlocks.Add(BlockIndex);
				if (BlockActors[BlockIndex]->GetBlockFactor() == EBlockFactors::PLAYER4)
				{
					AddPowerBricks(BlockIndex);
					HasPower = true;
				}
			}
		}
	}
}

void AMatch3BlockGrid::HandleInputPressed()
{
	GameState = EGameState::SELECTING;
}

void AMatch3BlockGrid::HandleInputRelease()
{
	
	GameState = EGameState::IDLE;
	CurrentFactor = EBlockFactors::NONE;
	
	UpdateGrid();
	

}

void AMatch3BlockGrid::RestartClicked(UPrimitiveComponent * ClickedComp, FKey ButtonClicked)
{
	RestartGame();
}



TArray<int> AMatch3BlockGrid::CalculateReplacementIndices()
{
	TArray<int> SpawnNumbers;
	SpawnNumbers.AddZeroed(GridSize_H);

	for (auto id : SelectedBlocks)
	{
		int x, y;
		x = id / GridSize_H;
		y = id % GridSize_H;
		SpawnNumbers[y]++;

		for (int i = x + 1; i < GridSize_W; i++)
		{
			int BlockID = i * GridSize_H + y;
			if(BlockActors[BlockID]->IsValidLowLevel())
				BlockActors[BlockID]->AddMoveDown();

		}

	}
	return SpawnNumbers;
}





void AMatch3BlockGrid::SpawnReplacementBlocks()
{

	TArray<AMatch3Block*> NewlySpawned;
	auto SpawnIndices = CalculateReplacementIndices();
	{
		for (int i = 0; i < GridSize_H; i++)
		{
			int NumOfBlocks = SpawnIndices[i];
			for (int j = 0; j < NumOfBlocks; j++)
			{

				int x = (GridSize_W + j);
				int y = i;
				const float XOffset = x * BlockSpacing; // Divide by dimension
				const float YOffset = y * BlockSpacing; // Modulo gives remainder


				int NewBlockID = x*GridSize_H + y;

				const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
				AMatch3Block* NewBlock = GetWorld()->SpawnActor<AMatch3Block>(BlockLocation, FRotator(0, 0, 0));

				//Improve Random Function
				int SelectedPower = FMath::RandRange(0, 50);
				if (SelectedPower < 48)
				{
					int BFint = FMath::RandRange(0, 2);
					switch (BFint)
					{
					case 0:
						NewBlock->SetBlockFactor(EBlockFactors::PLAYER1);
						break;
					case 1:
						NewBlock->SetBlockFactor(EBlockFactors::PLAYER2);
						break;
					case 2:
						NewBlock->SetBlockFactor(EBlockFactors::PLAYER3);
						break;
					default:
						break;
					}
				}
				else {
					NewBlock->SetBlockFactor(EBlockFactors::PLAYER4);
				}

				if (NewBlock != nullptr)
				{
					NewlySpawned.Add(NewBlock);
					NewBlock->OwningGrid = this;
					NewBlock->SetBlockIndex(NewBlockID);
				}
				for (int k = 0; k < NumOfBlocks; k++)
				{
					NewBlock->AddMoveDown();
				}

			}

		}

	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMatch3Block::StaticClass(), FoundActors);
	for(auto block: FoundActors)
	{
		((AMatch3Block*)block)->UpdatePosition();
	}

	GameState = EGameState::ANIMATING;
	FTimerHandle UniqueHandle;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AMatch3BlockGrid::FinishAnimating);
	GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, animTime, false);

}



void AMatch3BlockGrid::CheckForUnSelected(int BlockIndex)
{
	if (SelectedBlocks.Num()<1)
	{
		return;
	}
	if (SelectedBlocks.Last() != BlockIndex)
	{
		int id = SelectedBlocks.Find(BlockIndex);
		if (id >= 0)
		{
			int len = SelectedBlocks.Num() - id -1;
			for (int i = 0; i < len; i++)
			{
				int RemoveID = SelectedBlocks.Pop();
				BlockActors[RemoveID]->Highlight(false);
			}
		}
	}
}




#undef LOCTEXT_NAMESPACE
