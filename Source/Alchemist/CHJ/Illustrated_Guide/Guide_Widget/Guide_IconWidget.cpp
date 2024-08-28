// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_IconWidget.h"

#include "Components/SizeBox.h"

void UGuide_IconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SizeBox_Root->SetWidthOverride(300);
	SizeBox_Root->SetHeightOverride(300);
}


