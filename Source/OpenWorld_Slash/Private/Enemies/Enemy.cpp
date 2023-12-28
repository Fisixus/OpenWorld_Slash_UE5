

#include "Enemies/Enemy.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"
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

	AttributesComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributesComponent"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
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

void AEnemy::Die()
{
	const TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 AnimSelection = FMath::RandRange(0,4);
		FName SectionName;
		switch (AnimSelection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Dead1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Dead2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Dead3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Dead4;
			break;
		default:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Dead1;
			break;
		}
		AnimInstance -> Montage_JumpToSection(SectionName, DeathMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(7.5f);
	HealthBarWidget->SetVisibility(false);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DEBUG_DRAW_SPHERE(ImpactPoint, FColor::Blue);
	if (AttributesComponent && AttributesComponent->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(AttributesComponent && HealthBarWidget)
	{
		AttributesComponent->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(AttributesComponent->GetHealthPercent());
	}
	return DamageAmount;
}

