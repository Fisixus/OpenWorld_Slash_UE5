// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BirdCapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMeshComponent"));
	SKMeshComponent->SetupAttachment(GetRootComponent());

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
	const bool CurrentValue = Value.Get<bool>();
	if(CurrentValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move Working!"));
	}
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
	}

}

