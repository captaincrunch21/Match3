// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Match3BlockGrid.h"
#include "Match3Pawn.generated.h"

UCLASS(config=Game)
class AMatch3Pawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void SetGameGrid(AMatch3BlockGrid *Grid);

protected:
	void OnResetVR();
	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);
	void MouseClick();
	void MouseRelease();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AMatch3Block* CurrentBlockFocus;

	


private:
	AMatch3BlockGrid *GameGrid;
};
