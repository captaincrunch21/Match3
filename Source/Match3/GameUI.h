// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class MATCH3_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;


	UPROPERTY()
	UButton* ResetButton;

	void OnButtonClicked(){}
	
};
