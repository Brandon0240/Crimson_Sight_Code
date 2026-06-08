#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPlayer.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UAnimationAsset;
UCLASS()
class FIRSTPERSON1_API AWeaponPlayer : public AActor
{
	GENERATED_BODY()

public:
	AWeaponPlayer();
	
protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bDrawDebugLine = true;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ShootFromCamera(UCameraComponent* Camera, AController* ShooterController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimationAsset* ShootAnimation;

};