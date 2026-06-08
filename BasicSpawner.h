#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicSpawner.generated.h"

UCLASS()
class FIRSTPERSON1_API ABasicSpawner : public AActor
{
	GENERATED_BODY()

public:
	ABasicSpawner();
		
protected:
	virtual void BeginPlay() override;
	//Header variables for spawning that exponential increase every 3 enemies spawn
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	bool bLoopSpawning = true;
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnActor();

private:
	FTimerHandle SpawnTimerHandle;
};