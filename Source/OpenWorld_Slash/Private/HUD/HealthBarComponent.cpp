// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if(HealthBarComponent == nullptr)
		HealthBarComponent = Cast<UHealthBar>(GetUserWidgetObject());
	if(HealthBarComponent && HealthBarComponent->HealthBar)
	{
		HealthBarComponent->HealthBar->SetPercent(Percent);
	}
}
