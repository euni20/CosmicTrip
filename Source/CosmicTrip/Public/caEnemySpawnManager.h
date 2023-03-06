// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "caEnemySpawnManager.generated.h"

UCLASS()
class COSMICTRIP_API AcaEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcaEnemySpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//������ �ð�����
	UPROPERTY(EditDefaultsOnly)
	float minTime = 1;

	UPROPERTY(EditDefaultsOnly)
	float maxTime = 6;

	//������ ��ҿ���
	UPROPERTY(EditDefaultsOnly)
	TArray<class AActor*> spawnList;

	int prevRandIndex = -1;

	//������ �ð�
	float spawningTime;
	FTimerHandle timerHandlespawncaEnemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ACloseAttackEnemy> caEnemyFactory;

	void SpawnEnemy();
	

};
