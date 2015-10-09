// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "Game.h"
#include "GameCharacter.h"
#include "MySaveGame.h"


class FObjectSaveLoad :public FReloadObjectArc
{
public:
	void GetData(TArray<uint8>& data){ data = Bytes; }
	void SetData(TArray<uint8>& data){
		Bytes = data;
		Reset();
	}
};

void UGame::Save(AGameCharacter* character, TArray<uint8>& data)
{
	FMemoryWriter ar(data);
	FGameObjectProxyArchive arObject(ar);
	character->SerializeProperty(arObject);
	//arTemp.ActivateWriter();
	//arTemp.SerializeObject(character);
	//ar.GetData(data);
	//ar << character;
	//FObjectWriter writer(character, data, true, true);
}
void UGame::Load(AGameCharacter* character,TArray<uint8>& data)
{
	FMemoryReader ar(data);
	FGameObjectProxyArchive arObject(ar);
	character->SerializeProperty(arObject);
	//FObjectReader reader(character, data, true, true);
	//arTemp.ActivateReader();
	//arTemp.SerializeObject(character);
}

void UGame::AutoSaveGameCharacter(AGameCharacter* character)
{
	if (TempSavedGame == NULL)
	{
		TempSavedGame = NewObject<UMySaveGame>();
	}
	Save(character, TempSavedGame->character);
}

void UGame::AutoLoadGameCharacter(AGameCharacter* character)
{
	if (TempSavedGame == NULL)
		return;
	Load(character, TempSavedGame->character);
}