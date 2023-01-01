// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class OPENWORLD_SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveBird(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SKMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> BirdMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BirdMoveAction;

};
