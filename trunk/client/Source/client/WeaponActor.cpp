#include "client.h"
#include "WeaponActor.h"

UStaticMeshComponent* AWeaponActor::GetStaticMeshComponent()
{ return FindComponentByClass<UStaticMeshComponent>(); }
USkeletalMeshComponent* AWeaponActor::GetSkeletalMeshComponent()
{ return FindComponentByClass<USkeletalMeshComponent>(); }