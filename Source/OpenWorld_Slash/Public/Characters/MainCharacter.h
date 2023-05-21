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
class AWeapon;

UCLASS()
class OPENWORLD_SLASH_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);
	
	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool bCan);
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Arm();
	
protected:
	virtual void BeginPlay() override;
	bool CanAttack() const;
	/**
	* Callbacks For Input
	*/
	void MoveCharacter(const FInputActionValue& Value);
	void LookCharacter(const FInputActionValue& Value);
	void JumpCharacter(const FInputActionValue& Value);
	void EquipItem(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	/**
	* Montage Functions
	*/
	void PlayAttackMontage() const;
	void PlayEquipMontage(FName SectionName) const;
	
private:
	/**
	* Input Actions
	*/
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input ,meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MainCharAttackAction;
	
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
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> EquipMontage;
public:
	FORCEINLINE void SetOverlappingItem(AItem* a){OverlappingItem = a;};
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;};
};
