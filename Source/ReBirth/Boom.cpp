// Fill out your copyright notice in the Description page of Project Settings.


#include "Boom.h"
#include "Main.h"

ABoom::ABoom() {
	/* *��������ɵ��˺� */
	harm = 15.0f;
}

void ABoom::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	/* *������Boom����ʱ�� ����յ��˺�*/
	if (OtherActor) {
		AMain* mainPlayer = Cast<AMain>(OtherActor);
		if (mainPlayer) {
			mainPlayer->HpReduce(harm);
		}
	}
}

void ABoom::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
