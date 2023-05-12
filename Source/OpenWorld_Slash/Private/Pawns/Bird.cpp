// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BirdCapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMeshComponent"));
	SKMeshComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		TObjectPtr<UEnhancedInputLocalPlayerSubsystem> SubSystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(SubSystem)
		{
			SubSystem->AddMappingContext(BirdMappingContext,0);
		}
	}
	
}

void ABird::MoveBird(const FInputActionValue& Value)
{
	const float ForwardValue = Value.Get<float>();
	if(Controller && ForwardValue > 0)
	{
		MoveForwardBird();
		//UE_LOG(LogTemp, Warning, TEXT("Forward!"));
	}
	else if(Controller && ForwardValue < 0)
	{
		MoveBackwardBird();
		//UE_LOG(LogTemp, Warning, TEXT("Backward!"));
	}
}

void ABird::RotateBird(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(Controller)
	{
		AddControllerYawInput(LookAxisValue.X * MouseSensitivity);
		AddControllerPitchInput(LookAxisValue.Y * MouseSensitivity);
	}
}

void ABird::MoveForwardBird()
{
	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, 1);
}

void ABird::MoveBackwardBird()
{
	const FVector Backward = -GetActorForwardVector();
	AddMovementInput(Backward, 1);
}


void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(BirdMoveAction, ETriggerEvent::Triggered, this, &ABird::MoveBird);
		EnhancedInputComponent->BindAction(BirdLookAction, ETriggerEvent::Triggered, this, &ABird::RotateBird);
	}

}

