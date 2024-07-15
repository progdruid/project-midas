// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Body = FindComponentByClass<UPrimitiveComponent>();
	ensureMsgf(Body, TEXT("%s: no PrimitiveComponent on this Item"), *GetName());
}

void AItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	//rewrite PrimitiveComponent to optimize this screaming sh*t
	for (UPrimitiveComponent* Collider : TouchingComponents)
	{
		FCollisionQueryParams Params(FName("Test"), false, this);
		const bool Overlap = Collider->ComponentOverlapComponent(Body, Body->GetComponentLocation(), Body->GetComponentQuat(), Params);
		if (!Overlap)
			TouchingComponents.Remove(Collider);
	}

	// GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,
	// 	FString("Overlap: ") + FString::FromInt(TouchingComponents.Num()));
}

void AItem::ApplyAcceleration(FVector Acc)
{
	Body->AddForce(Acc, NAME_None, true);
}

bool AItem::IsTouchingGround() const
{
	return TouchingComponents.Num() > 0;
}

void AItem::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
		TouchingComponents.AddUnique(OtherComp);
}
