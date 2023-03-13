// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CosmicPlayer.generated.h"


UCLASS()
class COSMICTRIP_API ACosmicPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACosmicPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�ʿ�Ӽ�: �̵��ӵ�, �Է¾׼�, �Է¸��� ���ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_VRInput;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
		
	// �ѽ��
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_Fire;

	//��ǲ ������
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ThrowGun;

	//��ǲ �⺻��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grenade;

	//�����Ѽ����ϱ�
	//void ���ϰ��� ���� �Լ����鶧 �ڷ�����
	//void ThrowGun();

	//����� ���������� ���� �Լ�
	void ChooseGun(bool bGrenade);

	//����� �����ߴ��� �����ϴ� ����
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool BeChooseGrenade;

	//�⺻���� �����Ұ��̴�
	void OnActionGrenade();

	//�������� �����Ұ��̴�
	void OnActionThrowGun();


	//����� �̵�ó���Լ�
	void Move(const FInputActionValue& Values);

	//���콺 �Է�ó��

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;

	//ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& Values);


	//�ѽ�� ���
	void OnActionFirePressed();
	void OnActionFireReleased();

	void DoFire();

	//������ ����ƽ�޽� �߰�
	UPROPERTY(VisibleAnywhere, Category = ThrowGunMesh)
	class UStaticMeshComponent* ThrowGuncomp;

	//���콺 ���ʹ�ư�� ������ �Ѿ˰��忡�� �Ѿ��� ���� �ѱ���ġ�� ��ġ�ϰ�ʹ�.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	class ABulletActor* bullet;


protected:
	
	//ī�޶�
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* VRCamera;

	//��Ʈ�ѷ� 
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class UMotionControllerComponent* RightHand;

	//����� �� ��
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* RightHandMesh;

	//���� �����Ѵ�.
	UPROPERTY(VisibleAnyWhere, Category = "MotionController")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere)
	float fireInterval = 0.5f;
	FTimerHandle fireTimerHandle;

	//���Ȱ���
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* bulletImpactFactory;


	//����ư�� ������ ��ü�� ��� �ʹ�.
	//�ʿ�Ӽ�: �Է¾׼�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grab;
 
	//�ʿ�Ӽ�: ��������
 	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float GrabRange =100;

	//������ü�� �������
	//�޸𸮰����� ���ؼ� UPROPERTY��� ����Ʈ �����͸� 
	//������ִ°� ����. 
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	//�����༮�� �ִ��� ���� ����Һ���
	bool IsGrabbed = false;

	//������ ���ϴ� �������� ���ư����� �ϰ�ʹ�.
	//���� ����
	FVector ThrowDirection;
	//���� ��
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPower = 1000;

	// ���� ��ġ
	FVector PrevPos;

	//���õ� ���
	void TryGrab();
	//����
	void UnTryGrab();
	//����ִ���
	void Grabbing();

};
