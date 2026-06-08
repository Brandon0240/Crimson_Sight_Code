
#include "BasicEnemyCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Variant_Horror/HorrorCharacter.h"

ABasicEnemyCharacter::ABasicEnemyCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
}

//Set Health to full when the enemy spawn in
void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void ABasicEnemyCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}
//Provides ability for enemy to take damage by using parameters and returns the damage amount
float ABasicEnemyCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const &DamageEvent,
	AController *EventInstigator,
	AActor *DamageCauser)
{
	if (bIsDead)
	{
		return 0.0f;
	}
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHealth -= ActualDamage;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Red,
			FString::Printf(TEXT("Zombie Health: %.1f"), CurrentHealth));
	}

	if (CurrentHealth <= 0.0f)
	{
		CurrentHealth = 0.0f;
		Die();
	}

	return ActualDamage;
}
// Upon defeating an enemy, disables movement, collision and last for .5 secs in scene
void ABasicEnemyCharacter::Die()
{

	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	AHorrorCharacter *Player = Cast<AHorrorCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		Player->AddScore(100);
	}
	if (AAIController *AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	float DestroyDelay = DeathDestroyDelay;
	if (UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f);
	}

	if (DeathMontage)
	{
		float MontageLength = PlayAnimMontage(DeathMontage);
		if (MontageLength > 0.0f)
		{
			DestroyDelay = MontageLength;
		}
	}

	SetLifeSpan(.5f);
}
void ABasicEnemyCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
