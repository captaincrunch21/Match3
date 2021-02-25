// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum EBlockFactors
{
	NONE,
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4
};

UENUM()
enum EGameState
{
	IDLE,
	SELECTING,
	ANIMATING,
};