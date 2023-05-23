

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class OPENWORLD_SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit(const FVector& ImpactPoint) override;
	
protected:
	
	virtual void BeginPlay() override;
	/**
	* Montage Functions
	*/
	void PlayReactMontage(const FName& SectionName) const;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> ReactMontage;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Particles")
	TObjectPtr<UParticleSystem> HitParticle;
};
