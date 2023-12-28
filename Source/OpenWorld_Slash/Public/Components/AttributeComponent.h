// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLD_SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health;
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth;

public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent() const;
	bool IsAlive() const;
};
