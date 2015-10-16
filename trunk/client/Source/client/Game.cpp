// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "Game.h"
#include "GameCharacter.h"
#include "MySaveGame.h"

UGame::UGame()
{
}


void UGame::Init()
{
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGame::OnPreLoadMap);
	//FCoreUObjectDelegates::PostLoadMap.AddUObject(this, &UGame::OnPostLoadMap);
}
void UGame::Shutdown()
{

}
void UGame::OnPreLoadMap()
{
	//在切换地图之前，临时存档
	AutoSaveGameCharacter();
}
void UGame::OnPostLoadMap()
{
	
}

void UGame::AutoSaveGameCharacter()
{
	if (LocalPlayers.Num() > 0 && LocalPlayers[0]->PlayerController!=NULL)
	{
		AGameCharacter* character = Cast<AGameCharacter>(LocalPlayers[0]->PlayerController->GetPawn());
		if (character != NULL)
		{
			UMySaveGame* SaveObject = NewObject<UMySaveGame>();
			FMemoryWriter ar(SaveObject->character);
			FGameObjectProxyArchive arObject(ar);
			character->SerializeProperty(arObject);
			SaveObject->map_name = character->GetWorld()->GetCurrentLevel()->GetPathName();
			SaveObject->time = FDateTime::Now().ToString();
			TempSavedGame = SaveObject;
		}
	}
}

void UGame::SaveSlot(int32 slotIndex)
{
	if (LocalPlayers.Num() > 0 && LocalPlayers[0]->PlayerController != NULL)
	{
		AGameCharacter* character = Cast<AGameCharacter>(LocalPlayers[0]->PlayerController->GetPawn());
		if (character != NULL)
		{
			UMySaveGame* SaveObject = NewObject<UMySaveGame>();
			FMemoryWriter ar(SaveObject->character);
			FGameObjectProxyArchive arObject(ar);
			character->SerializeProperty(arObject);
			SaveObject->map_name = character->GetWorld()->GetCurrentLevel()->GetPathName();
			SaveObject->time = FDateTime::Now().ToString();
			UGameplayStatics::SaveGameToSlot(SaveObject, FString::Printf(TEXT("SLOT_%d"), slotIndex), 0);
		}
	}
}

void UGame::LoadSlot(int32 slotIndex)
{
	UMySaveGame* SaveObject = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), slotIndex), 0));
	if (SaveObject != NULL && GetWorld()!=NULL)
	{
		//序列化玩家数据
		if (LocalPlayers.Num() > 0 && LocalPlayers[0]->PlayerController != NULL)
		{
			AGameCharacter* character = Cast<AGameCharacter>(LocalPlayers[0]->PlayerController->GetPawn());
			if (character != NULL)
			{
				FMemoryReader ar(SaveObject->character);
				FGameObjectProxyArchive arObject(ar);
				character->SerializeProperty(arObject);
			}
		}

		//如果地图不同，切换地图
		FString current_map_name = GetWorld()->GetCurrentLevel()->GetPathName();
		if (SaveObject->map_name != current_map_name)
		{
			TempSavedGame = SaveObject;
			UGameplayStatics::OpenLevel(this, *SaveObject->map_name);
			//GetWorld()->ServerTravel(SaveObject->map_name);
		}
	}
}


void UGame::CapetureScreenShot()
{
	GIsHighResScreenshot = true;
	GScreenshotResolutionX = 100;
	GScreenshotResolutionY = 100;
	FScreenshotRequest::RequestScreenshot(TEXT("SLOT_0"), false,false);
}


TArray<UMySaveGame*> UGame::LoadAllSaved()
{
	TArray<UMySaveGame*> slots;
	for (int32 i = 0; i < 3; i++)
	{
		slots.Add(Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), i), 0)));
	}
	return slots;
}


APawn* UGame::LoadOrCreateCharacter(TSubclassOf<class APawn> pwanClass,AController* NewPlayer, class AActor* StartSpot)
{
	AGameCharacter* character = GetWorld()->SpawnActor<AGameCharacter>(pwanClass, StartSpot->GetTransform());
	if (character == NULL)
	{
		return character;
	}

	if (TempSavedGame != NULL)
	{
		FVector TempLocation = character->GetActorLocation();

		FMemoryReader ar(TempSavedGame->character);
		FGameObjectProxyArchive arObject(ar);
		character->SerializeProperty(arObject);

		character->SetActorLocation(TempLocation);
	}
	else
	{
		character->Recover();
	}

	return character;
}