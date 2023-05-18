// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Characters/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "OpenWorld_Slash/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(48.f);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
	//UE_LOG(LogTemp, Warning, TEXT("HOLA!"));
	//const FVector Location = GetActorLocation();
	//DEBUG_DRAW_SPHERE(Location, FColor::Blue)
	//DEBUG_DRAW_VECTOR(Location, Location + GetActorForwardVector()*100.f, FColor::Blue)
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//const FString Message = FString::Printf(TEXT("DELTATIME: %f"), DeltaTime);
	//DEBUG_PRINT_SCREEN(2, Message)
	
}

float AItem::TransformedSin(float DeltaTime)
{
	RunningTime += DeltaTime;
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos(float DeltaTime)
{
	RunningTime += DeltaTime;
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);
	if(MainChar)
	{
		MainChar->SetOverlappingItem(this);
	}
	// const FString ActName = OtherActor->GetName();
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(1,30.f,FColor::Blue, ActName);
	// }
	
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);
	if(MainChar)
	{
		MainChar->SetOverlappingItem(nullptr);
	}
	// const FString ActName = OtherActor->GetName();
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(1,30.f,FColor::Red, ActName);
	// }
}

