

#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "OpenWorld_Slash/DebugMacros.h"

AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayReactMontage(const FName& SectionName) const
{
	const TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && ReactMontage)
	{
		AnimInstance->Montage_Play(ReactMontage);
		AnimInstance -> Montage_JumpToSection(SectionName, ReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DEBUG_DRAW_SPHERE(ImpactPoint, FColor::Blue);
	PlayReactMontage(FName("ReactLeft"));
}

