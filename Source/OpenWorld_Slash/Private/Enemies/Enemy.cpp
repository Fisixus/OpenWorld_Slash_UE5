

#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ImpactVector = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const float CosTheta = FVector::DotProduct(ForwardVector, ImpactVector);
	float Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ImpactVector);
	if(CrossProduct.Z < 0)
	{
		Theta = Theta * -1;
	}
	FName SectionName("ReactBack");
	if(Theta < 45.f && Theta >= -45.f)
	{
		SectionName = FName("ReactFront");
	}
	else if(Theta < -45.f && Theta >= -135.f)
	{
		SectionName = FName("ReactLeft");
	}
	else if(Theta < 135.f && Theta >= 45.f)
	{
		SectionName = FName("ReactRight");
	}
	PlayReactMontage(SectionName);
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	//DEBUG_DRAW_SPHERE(ImpactPoint, FColor::Blue);
	DirectionalHitReact(ImpactPoint);
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	if(HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint
		);
	}
}
