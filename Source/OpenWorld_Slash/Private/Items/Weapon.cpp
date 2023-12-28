// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"

#include "Characters/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Box"));
	WeaponBox->SetupAttachment(RootComponent);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	WeaponStartTrace = CreateDefaultSubobject<USceneComponent>(FName("Weapon Start Trace"));
	WeaponStartTrace->SetupAttachment(RootComponent);
	WeaponEndTrace = CreateDefaultSubobject<USceneComponent>(FName("Weapon End Trace"));
	WeaponEndTrace->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlapBegin);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const
{
	ItemMeshComponent->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
}

void AWeapon::Equipped(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if(EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
			);
	}
	if(SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = WeaponStartTrace->GetComponentLocation();
	const FVector End = WeaponEndTrace->GetComponentLocation();
	
	IgnoreActors.AddUnique(this);

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(2.5f,2.5f,2.5f),
		WeaponStartTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if(BoxHit.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()	
		);
		
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if(HitInterface)
		{
			//HitInterface->GetHit(BoxHit.ImpactPoint);
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
	}
}
