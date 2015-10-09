// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<uint8> character;
	
};


class FGameObjectProxyArchive :public FNameAsStringProxyArchive
{
public:
	FGameObjectProxyArchive(FArchive& InInnerArchive)
		: FNameAsStringProxyArchive(InInnerArchive)
	{
		RootObject = NULL;
	}

	FArchive& operator<<(class UObject*& Obj);

	UObject* RootObject;

	TArray<UObject*> CompleteObjects;

	/**
	* List of top-level objects that have saved into the memory archive.  Used to prevent objects
	* from being serialized into storage multiple times.
	*/
	TSet<UObject*>	SavedObjects;

	/**
	* List of top-level objects that have been loaded using the memory archive.  Used to prevent
	* objects from being serialized multiple times from the same memory archive.
	*/
	TSet<UObject*>	LoadedObjects;
};