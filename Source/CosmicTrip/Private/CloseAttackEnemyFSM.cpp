// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "CosmicPlayer.h"
#include "CloseAttackEnemy.h"
#include <NavigationSystem.h>
#include "AIController.h"

// Sets default values for this component's properties
UCloseAttackEnemyFSM::UCloseAttackEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UCloseAttackEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	state = EEnemyState::IDLE;
	me = Cast<ACloseAttackEnemy>(GetOwner());
	hp = maxHP;
	
}


// Called every frame
void UCloseAttackEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::IDLE:
		TickIdle();
		break;
	case EEnemyState::MOVE:
		TickMove();
		break;
	case EEnemyState::ATTACK:
		TickAttack();
		break;
	case EEnemyState::DAMAGE:
		TickDamage();
		break;
	case EEnemyState::DIE:
		TickDie();
		break;	
	}
	
}

void UCloseAttackEnemyFSM::TickIdle()
{
	mainTarget = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!mainTarget) return;
	state = EEnemyState::MOVE;

}

void UCloseAttackEnemyFSM::TickMove()
{
	//�÷��̾ ���� �޷��´�
	FVector targetDir = mainTarget->GetActorLocation() - me->GetActorLocation();

	//���� �� �� �ִ� �� ���� ���� ����� �ִ°�
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query; //�� ã�� �� �ִ��� ����
	FAIMoveRequest request;
	request.SetAcceptanceRadius(acceptanceRadius);
	request.SetGoalLocation(mainTarget->GetActorLocation());

	ai->BuildPathfindingQuery(request, query);
	ns->FindPathSync(query);
	FPathFindingResult result = ns->FindPathSync(query);

	if (result.Result == ENavigationQueryResult::Success)
	{
		//me->AddMovementInput(targetDir.GetSafeNormal());
		ai->MoveToLocation(mainTarget->GetActorLocation());
	}
// 	else
// 	{
// 		//Ÿ���� �׺���̼� ���� ���ٸ� ������ ��ġ�� ���ƴٴϵ���
// 		//������ ��ġ
// 		//auto randLoc
// 	}


	float targetDist = mainTarget->GetDistanceTo(me);

	//���� ������ ������ ���� ���·� ��ȯ
	if (targetDist < attackRange)
	{
		state = EEnemyState::ATTACK;
	}
}


void UCloseAttackEnemyFSM::TickAttack()
{
	//�÷��̾ ���� �Ѿ��� ������
	//Shoot();
	
	
}

void UCloseAttackEnemyFSM::TickDamage()
{
	//�÷��̾��� ������ �޾Ҵٸ� ���ݴ��� �ִϸ��̼� ���


// 	hp�� ���δ�
// 		if (hp = 0)
// 		{
// 			//hp�� �� ���̸� ���� ���·� ��ȯ�Ѵ�
// 			state = EEnemyState::DIE;
// 		}
}

void UCloseAttackEnemyFSM::TickDie()
{
	//�״� �ִϸ��̼��� �����Ų��

}

void UCloseAttackEnemyFSM::OnTakeDamage()
{
}