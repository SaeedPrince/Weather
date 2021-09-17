// Fill out your copyright notice in the Description page of Project Settings.


#include "ActrPickupable.h"
//Public
AActrPickupable::AActrPickupable()
{
	Core = GetStaticMeshComponent();
}
// Pickup Interface
void AActrPickupable::Drop_Implementation()
{
	if (IsValid(Core))
	{
		Core->SetSimulatePhysics(true);
		FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentTransformRules);
	}
}
void AActrPickupable::Pickup_Implementation(USceneComponent* inAttachTo)
{
	if (IsValid(Core) && IsValid(inAttachTo))
	{
		Core->SetSimulatePhysics(false);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
		Core->AttachToComponent(inAttachTo, AttachmentTransformRules);
	}
}