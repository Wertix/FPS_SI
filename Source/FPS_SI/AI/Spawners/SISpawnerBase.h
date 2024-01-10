// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SISpawnerBase.generated.h"

class ASIEnemyCharacterBase;


USTRUCT(BlueprintType)
struct FSISpawnerSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinEnemiesSpawned = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxEnemiesSpawned = 8;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DelayInSecToSpawnEnemiesBeyondMin = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<ASIEnemyCharacterBase>> EnemiesToSpawn;
};

UCLASS()
class FPS_SI_API ASISpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:

	ASISpawnerBase();

	void IncrementSpawnerCounter() { SpawnedEnemiesCounter++; }

protected:

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner Settings")
	FSISpawnerSettings SpawnerSettings;

private:

	int32 SpawnedEnemiesCounter;

	FTimerHandle SpawnTimer;

	void StartSpawning();
	void SpawnNextEnemy();

	UFUNCTION()
	void EnemyDied();

	/** Check spawner conditions if SpawnedEnemiesCounter < MinEnemiesSpawned */
	void FastRespawnCheck();
	/** Check spawner conditions after DelayInSecToSpawnEnemiesBeyondMin */
	void LongRespawnCheck();	
};
