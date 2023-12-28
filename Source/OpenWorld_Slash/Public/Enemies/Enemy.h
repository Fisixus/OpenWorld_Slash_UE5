

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters//CharacterTypes.h"
#include "Enemy.generated.h"

class UAttributeComponent;
class UHealthBarComponent;

UCLASS()
class OPENWORLD_SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	void Die();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	
	virtual void BeginPlay() override;
	/**
	* Montage Functions
	*/
	void PlayReactMontage(const FName& SectionName) const;
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributesComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> ReactMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Particles")
	TObjectPtr<UParticleSystem> HitParticle;

};
