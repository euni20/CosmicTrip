// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseAttackEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "CosmicPlayer.h"
#include "CloseAttackEnemy.h"
#include <NavigationSystem.h>
#include "AIController.h"
#include "CloseAttackEnemyAnim.h"
#include "RazerRobot.h"

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
	ai = Cast<AAIController>(me->GetController());
	hp = maxHP;
	
	//��������ġ�� ���� ��	
	UpdateRandomLocation(randLocationRadius, randomLocation);

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
	case EEnemyState::MOVETOROBOT:
		TickMoveToRobot();
		break;
	case EEnemyState::ATTACK:
		TickAttack();
		break;	
	case EEnemyState::ATTACKROBOT:
		TickAttackRobot();
		break;
	case EEnemyState::DAMAGE:
		TickDamage();
		break;
	case EEnemyState::DIE:
		TickDie();
		break;	
	}	
}

void UCloseAttackEnemyFSM::SetState(EEnemyState next)
{
	state = next;
	me->caEnemyAnim->state = next;
	currentTime = 0;
}

void UCloseAttackEnemyFSM::TickIdle()
{	
	mainTarget = Cast<ACosmicPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!mainTarget) return;
	

	SetState(EEnemyState::MOVE);
}

void UCloseAttackEnemyFSM::TickMove()
{	
	//�÷��̾��� ����, ��ġ ã�´�
	//�÷��̾ ���� �����Ÿ� �ȿ� �ִٸ� �÷��̾ ���� �̵��Ѵ�
	
	FVector targetDir = mainTarget->GetActorLocation() - me->GetActorLocation();
	targetDist = mainTarget->GetDistanceTo(me);
	me->caEnemyAnim->bChooseWalk = true;
	if (targetDist <= trackingRange)
	{
		ai->MoveToLocation(mainTarget->GetActorLocation());
		wantedLocation = mainTarget->GetActorLocation();
	}

	//�÷��̾ ������ ���� �ȿ� ���Դٸ� �÷��̾� ���� ���·� ��ȯ
	if (attackRange >= targetDist)
	{
		SetState(EEnemyState::ATTACK);
	}

	//�κ��� ã�´�
	//�κ��� ���� �������� �ȿ� ������ �κ����� ���ϴ� state�� �����
	razerTarget = Cast<ARazerRobot>(UGameplayStatics::GetActorOfClass(GetWorld(), ARazerRobot::StaticClass()));
	
	if (trackingRange <= trackingRobotRange)
	{
		SetState(EEnemyState::MOVETOROBOT);
	}

}

//�κ��� ���� �����Ÿ� �ȿ� �ִٸ� �κ��� ���� �̵��Ѵ�
void UCloseAttackEnemyFSM::TickMoveToRobot()
{
	//�κ��� ����� ��ġ�� ã�´�
	FVector robotDir = razerTarget->GetActorLocation() - me->GetActorLocation();
	razerTargetDist = razerTarget->GetDistanceTo(me);

	//�κ��� ���� �̵��Ѵ�
	ai->MoveToLocation(razerTarget->GetActorLocation());

	//�κ��� ������ ���� �ȿ� ���Դٸ� �κ� ���� ���·� ��ȯ
	if (razerTargetDist <= attackRange)
	{
		SetState(EEnemyState::ATTACKROBOT);
	}
	
}

//�÷��̾ ����
void UCloseAttackEnemyFSM::TickAttack()
{
	//���� �ð��� ������
	currentTime += GetWorld()->GetDeltaSeconds();

	//���� �ð��� �ѹ��� �����ϱ�
	if (currentTime >= attackDelayTime)
	{	
		//���� montage ȣ��
		me->caEnemyAnim->AnimAttack(TEXT("Attack"));
		
		currentTime = 0;
// 		bAttackPlay = false;
// 		else
// 		{
// 
// 			me->caEnemyAnim->bAttackPlay = true;
// 		}

	}
	//���ݹ����� ����� move�� ��ȯ
	targetDist = mainTarget->GetDistanceTo(me);

	if (targetDist > attackRange)
	{
		SetState(EEnemyState::MOVE);
	}

}

//�κ��� ����
void UCloseAttackEnemyFSM::TickAttackRobot()
{
	//���� �ð��� ������
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime >= attackDelayTime) 
	{
		//���� �ִϸ��̼�
		me->caEnemyAnim->AnimAttack(TEXT("Attack"));
		currentTime = 0;

	}	
	//�κ��� ���� �Ÿ�
	razerTargetDist = razerTarget->GetDistanceTo(me);
	//���� ������ ����� move�� ��ȯ
	if (razerTargetDist > attackRange)
	{
		SetState(EEnemyState::MOVE);
	}
		
}

void UCloseAttackEnemyFSM::TickDamage()
{
	currentTime += GetWorld()->GetDeltaSeconds();

	//���� �ִϸ��̼� ȣ��
	me->caEnemyAnim->AnimDamage(TEXT("Damage"));

	if (currentTime > 1)
	{
		SetState(EEnemyState::MOVE);
		currentTime = 0;
	}	
	
}

void UCloseAttackEnemyFSM::TickDie()
{
	//bug - delete
	me->caEnemyAnim->bcaEnemyDieEnd = true;

	if (false == me->caEnemyAnim->bcaEnemyDieEnd)
	{
		return;
	}
	//���� �ִϸ��̼� ȣ��
	me->caEnemyAnim->AnimDie(TEXT("Die"));
	//�Ҹ�
	me->Destroy();

}

void UCloseAttackEnemyFSM::OnTakeDamage(float damage)
{	
	maxHP -= damage;
	SetState(EEnemyState::DAMAGE);

	if (maxHP <= 0)
	{
		SetState(EEnemyState::DIE);
		UE_LOG(LogTemp, Warning, TEXT("UCloseAttackEnemyFSM::OnTakeDamage________________________"))
		//me->caEnemyAnim->bcaEnemyDieEnd = false;
	}
	else
	{
		SetState(EEnemyState::MOVE);
	}
}

void UCloseAttackEnemyFSM::OnDamageProcess(int damageValue)
{
	me->PlayAnimMontage(me->caEnemyAnim->enemyMontageFactory, 1, FName("Damage"));
	/*
	UE_LOG(LogTemp, Error, TEXT("Damaged"));
	if (ai)
	{
		ai->StopMovement();
	}
	maxHP -= damageValue;
	if (maxHP <= 0)
	{
		maxHP = 0;
		SetState(EEnemyState::DIE);
		me->caEnemyAnim->AnimDie(TEXT("Die"));
	}
	else
	{
		SetState(EEnemyState::DAMAGE);
		me->caEnemyAnim->AnimDie(TEXT("Damage"));
	}
	*/
}

//�÷��̾��� ü���� ���� ���̴�
void UCloseAttackEnemyFSM::OnHitEvent()
{
	me->caEnemyFSM->bAttackPlay = false;

}

bool UCloseAttackEnemyFSM::UpdateRandomLocation(float radius, FVector& outLocation)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation navLoc;

	bool result = ns->GetRandomPointInNavigableRadius(me->GetActorLocation(), radius, navLoc);

	if (result)
	{
		outLocation = navLoc.Location;
	}

	return result;
}


