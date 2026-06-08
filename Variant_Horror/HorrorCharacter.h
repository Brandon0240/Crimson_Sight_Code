// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FirstPerson1Character.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UInputAction;
class AWeaponPlayer;
class USceneComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);

/**
 *  Simple first person horror character
 *  Provides stamina-based sprinting
 */
UCLASS(abstract)
class FIRSTPERSON1_API AHorrorCharacter : public AFirstPerson1Character
{
	GENERATED_BODY()

	/** Player light source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent *SpotLight;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponPlayer> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponPlayer *CurrentWeapon;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *SprintAction;

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *FireAction;

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, Category = "Walk")
	float WalkSpeed = 250.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 3.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float SprintSpeed = 600.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	USceneComponent *LeftEyePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	USceneComponent *RightEyePoint;

	

public:
	/** Player score */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score = 0;

	/** Add points to the player score */
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Points);

	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChangedDelegate OnSprintStateChanged;

protected:
	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;

protected:
	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEndSprint();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();
};
