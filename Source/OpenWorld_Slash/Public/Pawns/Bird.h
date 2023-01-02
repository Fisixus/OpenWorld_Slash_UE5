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
class USpringArmComponent;
class UCameraComponent;

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
	void RotateBird(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SKMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> BirdMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BirdMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BirdLookAction;
	UPROPERTY(EditAnywhere, Category=Input, meta = (ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float MouseSensitivity = 0.5f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	void MoveForwardBird();
	void MoveBackwardBird();

};
