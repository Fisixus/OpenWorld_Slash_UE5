// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "../DebugMacros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapon.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 30.f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> RotationRate = FRotator(0.f,400.f,0.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	
	HairComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("HairComponent"));
	HairComponent->SetupAttachment(GetMesh());
	HairComponent->AttachmentName = FString("head");

	EyebrowsComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyebrowsComponent"));
	EyebrowsComponent->SetupAttachment(GetMesh());
	EyebrowsComponent->AttachmentName = FString("head");
	
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		TObjectPtr<UEnhancedInputLocalPlayerSubsystem> SubSystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(SubSystem)
		{
			SubSystem->AddMappingContext(MainCharMappingContext,0);
		}
	}
}

void AMainCharacter::MoveCharacter(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	/*
	const FVector ForwardVec = GetActorForwardVector();
	AddMovementInput(ForwardVec, MovementVector.Y);
	const FVector RightVec = GetActorRightVector();
	AddMovementInput(RightVec, MovementVector.X);
	*/
	const FRotator Rotation = Controller -> GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDir, MovementVector.Y);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDir, MovementVector.X);
}

void AMainCharacter::LookCharacter(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(Controller)
	{
		AddControllerYawInput(LookAxisValue.X * MouseSensitivity);
		AddControllerPitchInput(LookAxisValue.Y * MouseSensitivity);
	}
}

void AMainCharacter::JumpCharacter(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	if(bCanJump)
	{
		Jump();
		SetCanJump(false);
	}
}

void AMainCharacter::EquipItem(const FInputActionValue& Value)
{
	AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);
	if(Weapon) //&& !EquippedWeapon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AA!"));
		Weapon->Equipped(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		EquippedWeapon = Weapon;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("BB!"));
		if(CharacterState != ECharacterState::ECS_Unequipped
			&& ActionState == EActionState::EAS_Unoccupied)
		{
			//UE_LOG(LogTemp, Warning, TEXT("CC!"));
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquppingWeapon;
		}
		else if(CharacterState == ECharacterState::ECS_Unequipped
			&& ActionState == EActionState::EAS_Unoccupied
			&& EquippedWeapon)
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquppingWeapon;
		}
	}
}

void AMainCharacter::PlayEquipMontage(FName SectionName) const
{
	const TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance -> Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool AMainCharacter::CanAttack() const
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}

void AMainCharacter::Disarm()
{
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
}

void AMainCharacter::Arm()
{
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
}

void AMainCharacter::Attack(const FInputActionValue& Value)
{
	if(CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}
}

void AMainCharacter::PlayAttackMontage() const
{
	const TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 AnimSelection = FMath::RandRange(0,1);
		FName SectionName;
		switch (AnimSelection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			SectionName = FName("Attack1");
			break;
		}
		AnimInstance -> Montage_JumpToSection(SectionName, AttackMontage);
	}
}


void AMainCharacter::SetCanJump(bool bCan)
{
	bCanJump = bCan;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddControllerPitchInput(0.5f * MouseSensitivity);
	//AddControllerYawInput(0.5f * MouseSensitivity);
	//AddControllerRollInput(0.5f * MouseSensitivity);


}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MainCharMoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::MoveCharacter);
		EnhancedInputComponent->BindAction(MainCharLookAction, ETriggerEvent::Triggered, this, &AMainCharacter::LookCharacter);
		EnhancedInputComponent->BindAction(MainCharJumpAction, ETriggerEvent::Started, this, &AMainCharacter::JumpCharacter);
		EnhancedInputComponent->BindAction(MainCharEquipAction, ETriggerEvent::Started, this, &AMainCharacter::EquipItem);
		EnhancedInputComponent->BindAction(MainCharAttackAction, ETriggerEvent::Started, this, &AMainCharacter::Attack);
	}
}



