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
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
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
	const FVector CurrentValue = Value.Get<FVector>();
	if(CurrentValue.Z > 0)
	{
		MoveUpBird();
		UE_LOG(LogTemp, Warning, TEXT("UP!"));
	}
	else if(CurrentValue.Z < 0)
	{
		MoveDownBird();
		UE_LOG(LogTemp, Warning, TEXT("DOWN!"));
	}
	else if(CurrentValue.Y > 0)
	{
		MoveRightBird();
		UE_LOG(LogTemp, Warning, TEXT("RIGHT!"));
	}
	else if(CurrentValue.Y < 0)
	{
		MoveLeftBird();
		UE_LOG(LogTemp, Warning, TEXT("LEFT!"));
	}
}

void ABird::MoveUpBird()
{
	const FVector Up = GetActorUpVector();
	AddMovementInput(Up, 1);
}

void ABird::MoveDownBird()
{
	const FVector Down = -GetActorUpVector();
	AddMovementInput(Down, 1);
}

void ABird::MoveRightBird()
{
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, 1);
}

void ABird::MoveLeftBird()
{
	const FVector Left = -GetActorRightVector();
	AddMovementInput(Left, 1);
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//const FVector Forward = GetActorForwardVector();
	//AddMovementInput(Forward, 1);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(BirdMoveAction, ETriggerEvent::Triggered, this, &ABird::MoveBird);
	}

}

