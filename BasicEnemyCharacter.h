
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"





class UAnimMontage;



UCLASS()

class FIRSTPERSON1_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABasicEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	// Max Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	// Current Enemy health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	// Called when the enemy takes damage
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const &DamageEvent,
		AController *EventInstigator,
		AActor *DamageCauser) override;

	// Called when health reaches 0/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

	// Preventive variable to make sure Die() only happens once
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	// A later implementation utilizing the death animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage *DeathMontage;

	// Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float DeathDestroyDelay = 3.0f;
};
