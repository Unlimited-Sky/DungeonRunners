// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionTargetActorCollider.h"


// Sets default values
ACollisionTargetActorCollider::ACollisionTargetActorCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACollisionTargetActorCollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollisionTargetActorCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

