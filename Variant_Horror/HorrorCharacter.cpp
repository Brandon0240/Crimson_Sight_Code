
#include "Variant_Horror/HorrorCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "WeaponPlayer.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

AHorrorCharacter::AHorrorCharacter()
{
	// create the spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;
	LeftEyePoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftEyePoint"));
	LeftEyePoint->SetupAttachment(GetFirstPersonCameraComponent());
	LeftEyePoint->SetRelativeLocation(FVector(20.0f, -4.0f, -2.0f));

	RightEyePoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightEyePoint"));
	RightEyePoint->SetupAttachment(GetFirstPersonCameraComponent());
	RightEyePoint->SetRelativeLocation(FVector(20.0f, 4.0f, -2.0f));
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		CurrentWeapon = GetWorld()->SpawnActor<AWeaponPlayer>(
			WeaponClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams);

		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(
				GetFirstPersonCameraComponent(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			CurrentWeapon->SetActorRelativeLocation(FVector(30.0f, 15.0f, -15.0f));
			CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
	//implemented by Epic Games's starting First person template
	SprintMeter = SprintTime;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHorrorCharacter::SprintFixedTick, SprintFixedTickTime, true);
}
//implemented by Epic Games's starting First person template
void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}
//implemented by Epic Games's starting First person template
void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	{

		if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
	
			if (SprintAction)
			{
				EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoStartSprint);
				EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorCharacter::DoEndSprint);
			}

			if (FireAction)
			{
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AHorrorCharacter::FireWeapon);
			}
		}
	}
}
//implemented by Epic Games's starting First person template
void AHorrorCharacter::DoStartSprint()
{
	bSprinting = true;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		OnSprintStateChanged.Broadcast(true);
	}
}
//implemented by Epic Games's starting First person template
void AHorrorCharacter::DoEndSprint()
{
	bSprinting = false;

	if (!bRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		OnSprintStateChanged.Broadcast(false);
	}
}
//implemented by Epic Games's starting First person template
void AHorrorCharacter::SprintFixedTick()
{

	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{
		if (SprintMeter > 0.0f)
		{
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

			if (SprintMeter <= 0.0f)
			{
				bRecovering = true;
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
	}
	else
	{

		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			bRecovering = false;

		
			GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;


			OnSprintStateChanged.Broadcast(bSprinting);
		}
	}

	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}
// Laser weapon's debug line
void AHorrorCharacter::FireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ShootFromCamera(GetFirstPersonCameraComponent(), GetController());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentWeapon is NULL"));
	}

	if (GetWorld() && GetFirstPersonCameraComponent())
	{
		const FVector LaserEnd =
			GetFirstPersonCameraComponent()->GetComponentLocation() +
			GetFirstPersonCameraComponent()->GetForwardVector() * 5000.0f;

		if (LeftEyePoint)
		{
			DrawDebugLine(
				GetWorld(),
				LeftEyePoint->GetComponentLocation(),
				LaserEnd,
				FColor::Red,
				false,
				0.15f,
				0,
				6.0f);
		}

		if (RightEyePoint)
		{
			DrawDebugLine(
				GetWorld(),
				RightEyePoint->GetComponentLocation(),
				LaserEnd,
				FColor::Red,
				false,
				0.15f,
				0,
				6.0f);
		}
	}
}
//Add score with debug message onscreen.
void AHorrorCharacter::AddScore(int32 Points)
{
	Score += Points;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Yellow,
			FString::Printf(TEXT("Score: %d"), Score)
		);
	}
}
