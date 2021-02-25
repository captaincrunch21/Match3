// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class  SDummyWidget :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SDummyWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class UGameUI>, OwingHUD)
	SLATE_END_ARGS()


	void Construcrt(const FArguments& inArgs);
	TWeakObjectPtr<class UGameUI> OwingHUD;
	virtual bool SupportsKeyboardFocus() const override { return true; }

	TAttribute<FText> Score;
	TAttribute<FText> Power;

	/*FText GetScore() const;
	FText GetPower() const;*/

};
