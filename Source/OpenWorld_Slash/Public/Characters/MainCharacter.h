// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class AItem;

UCLASS()
class OPENWORLD_SLASH_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool bCan);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveCharacter(const FInputActionValue& Value);
	void LookCharacter(const FInputActionValue& Value);
	void JumpCharacter(const FInputActionValue& Value);
	void EquipItem(const FInputActionValue& Value);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> MainCharMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MainCharMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MainCharLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MainCharJumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MainCharEquipAction;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, Category=Input, meta = (ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta = (AllowPrivateAccess = true))
	float bCanJump = true;

	UPROPERTY(EditAnywhere, Category=Accessories)
	TObjectPtr<UGroomComponent> HairComponent;
	
	UPROPERTY(EditAnywhere, Category=Accessories)
	TObjectPtr<UGroomComponent> EyebrowsComponent;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;
public:
	FORCEINLINE void SetOverlappingItem(AItem* a){OverlappingItem = a;};
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;};
};
