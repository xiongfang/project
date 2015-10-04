// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "GameNPC.h"

// Sets default values
AGameNPC::AGameNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static FName CollisionProfileName(TEXT("IgnoreOnlyPawn"));

	Body = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Body"));
	if (Body)
	{
		Body->AlwaysLoadOnClient = true;
		Body->AlwaysLoadOnServer = true;
		Body->bOwnerNoSee = false;
		Body->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Body->bCastDynamicShadow = true;
		Body->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Body->bChartDistanceFactor = true;
		Body->SetCollisionProfileName(CollisionProfileName);
		Body->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Body->AttachParent = GetMesh();
		Body->SetMasterPoseComponent(GetMesh());
	}

	Head = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Head"));
	if (Head)
	{
		Head->AlwaysLoadOnClient = true;
		Head->AlwaysLoadOnServer = true;
		Head->bOwnerNoSee = false;
		Head->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Head->bCastDynamicShadow = true;
		Head->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Head->bChartDistanceFactor = true;
		Head->SetCollisionProfileName(CollisionProfileName);
		Head->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Head->AttachParent = GetMesh();
		Head->SetMasterPoseComponent(GetMesh());

	}
	Hand = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Hand"));
	if (Hand)
	{
		Hand->AlwaysLoadOnClient = true;
		Hand->AlwaysLoadOnServer = true;
		Hand->bOwnerNoSee = false;
		Hand->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Hand->bCastDynamicShadow = true;
		Hand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Hand->bChartDistanceFactor = true;
		Hand->SetCollisionProfileName(CollisionProfileName);
		Hand->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Hand->AttachParent = GetMesh();
		Hand->SetMasterPoseComponent(GetMesh());

	}
	Feet = CreateOptionalDefaultSubobject< USkeletalMeshComponent>(TEXT("Feet"));
	if (Feet)
	{
		Feet->AlwaysLoadOnClient = true;
		Feet->AlwaysLoadOnServer = true;
		Feet->bOwnerNoSee = false;
		Feet->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Feet->bCastDynamicShadow = true;
		Feet->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Feet->bChartDistanceFactor = true;
		Feet->SetCollisionProfileName(CollisionProfileName);
		Feet->bGenerateOverlapEvents = false;

		// Mesh acts as the head, as well as the parent for both animation and attachment.
		Feet->AttachParent = GetMesh();
		Feet->SetMasterPoseComponent(GetMesh());
	}
}