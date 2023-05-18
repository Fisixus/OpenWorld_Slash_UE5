// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimInstance.h"

#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MainCharacter =  Cast<AMainCharacter>(TryGetPawnOwner());
	if(MainCharacter)
	{
		MovementComponent = MainCharacter->GetCharacterMovement();
	}
	
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(MainCharacter && MovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		IsFalling = MovementComponent -> IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
	}
}
