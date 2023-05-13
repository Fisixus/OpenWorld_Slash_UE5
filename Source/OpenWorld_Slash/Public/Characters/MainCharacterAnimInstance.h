// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
class AMainCharacter;
class UCharacterMovementComponent;

UCLASS()
class OPENWORLD_SLASH_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<AMainCharacter> MainCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta = (AllowPrivateAccess = true))
	bool IsFalling;
};
