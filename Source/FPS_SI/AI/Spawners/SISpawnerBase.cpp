// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SISpawnerBase.h"

#include "FPS_SI/Characters/Enemies/SIEnemyCharacterBase.h"


ASISpawnerBase::ASISpawnerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}


void ASISpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnerSettings.EnemiesToSpawn.Num() > 0)
	{
		StartSpawning();
	}
}


void ASISpawnerBase::StartSpawning()
{
	int32 RandomEnemyIndex = FMath::RandRange(0, SpawnerSettings.EnemiesToSpawn.Num() - 1);
	TSubclassOf<ASIEnemyCharacterBase> CurrentEnemyToSpawn = SpawnerSettings.EnemiesToSpawn[RandomEnemyIndex];

	if (ensure(CurrentEnemyToSpawn))
	{
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = GetActorLocation();;

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		TObjectPtr<ASIEnemyCharacterBase> SpawnedEnemy = GetWorld()->SpawnActor<ASIEnemyCharacterBase>(CurrentEnemyToSpawn, SpawnLocation, SpawnRotation, ActorSpawnParams);

		if (ensure(SpawnedEnemy))
		{
			SpawnedEnemy->SetSpawnerRef(this);

			SpawnedEnemiesCounter++;

			SpawnedEnemy->EnemyDiedDelegate.AddUniqueDynamic(this, &ASISpawnerBase::EnemyDied);

			SpawnNextEnemy();
		}
	}
}


void ASISpawnerBase::EnemyDied()
{
	SpawnedEnemiesCounter--;
	SpawnNextEnemy();
}


void ASISpawnerBase::SpawnNextEnemy()
{
	if (SpawnedEnemiesCounter >= SpawnerSettings.MinEnemiesSpawned)
	{
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASISpawnerBase::LongRespawnCheck, SpawnerSettings.DelayInSecToSpawnEnemiesBeyondMin, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASISpawnerBase::FastRespawnCheck, 1.f, false);
	}
}


void ASISpawnerBase::FastRespawnCheck()
{
	if (SpawnedEnemiesCounter < SpawnerSettings.MinEnemiesSpawned)
	{
		StartSpawning();
	}
}


void ASISpawnerBase::LongRespawnCheck()
{
	if (SpawnedEnemiesCounter < SpawnerSettings.MaxEnemiesSpawned && SpawnedEnemiesCounter >= SpawnerSettings.MinEnemiesSpawned)
	{
		StartSpawning();
	}
}
