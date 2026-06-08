#include "WeaponPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
AWeaponPlayer::AWeaponPlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void AWeaponPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Allows player to shoot a hit scan by showing a laser
void AWeaponPlayer::ShootFromCamera(UCameraComponent *Camera, AController *ShooterController)
{
	USkeletalMeshComponent *MeshToAnimate = nullptr;

	TArray<USkeletalMeshComponent *> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);

	for (USkeletalMeshComponent *MeshComp : MeshComponents)
	{
		if (MeshComp && MeshComp->GetSkeletalMeshAsset())
		{
			MeshToAnimate = MeshComp;
			break;
		}
	}

	if (MeshToAnimate && ShootAnimation)
	{
		MeshToAnimate->PlayAnimation(ShootAnimation, false);
	}
	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponPlayer: Camera is missing."));
		return;
	}

	UWorld *World = GetWorld();

	if (!World)
	{
		return;
	}

	FVector Start = Camera->GetComponentLocation();
	FVector Forward = Camera->GetForwardVector();
	FVector End = Start + Forward * Range;

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (AActor *OwnerActor = GetOwner())
	{
		CollisionParams.AddIgnoredActor(OwnerActor);
	}

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams);

	if (bDrawDebugLine)
	{
		DrawDebugLine(
			World,
			Start,
			End,
			FColor::Red,
			false,
			1.0f,
			0,
			2.0f);
	}

	if (bHit)
	{
		AActor *HitActor = HitResult.GetActor();

		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(
				HitActor,
				Damage,
				ShooterController,
				this,
				nullptr);

			UE_LOG(LogTemp, Warning, TEXT("Weapon hit: %s"), *HitActor->GetName());
		}
	}
}