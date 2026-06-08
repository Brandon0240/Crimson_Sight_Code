#include "BasicSpawner.h"
#include "TimerManager.h"
#include "Engine/World.h"

ABasicSpawner::ABasicSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABasicSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bLoopSpawning && ActorToSpawn)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&ABasicSpawner::SpawnActor,
			SpawnInterval,
			true
		);
	}
}

void ABasicSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//Method for spawning that exponential increase every 3 enemies spawn
void ABasicSpawner::SpawnActor()
{
	if (!ActorToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("BasicSpawner: ActorToSpawn is not set."));
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	World->SpawnActor<AActor>(
		ActorToSpawn,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);
}