// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "OpenWorld_Slash/DebugMacros.h"



AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMeshComponent;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
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

