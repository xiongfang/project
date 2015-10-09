// Fill out your copyright notice in the Description page of Project Settings.

#include "client.h"
#include "MySaveGame.h"


FArchive& FGameObjectProxyArchive::operator<<(class UObject*& Obj)
{
	FArchive& interAr = this->InnerArchive;
	if (this->IsLoading())
	{
		int32 Index = 0;
		interAr << Index;
		// An index of 0 indicates that the value of the object was NULL
		if (Index == 0)
		{
			Obj = NULL;
		}
		//else if (Index < 0)
		//{
		//	// An index less than zero indicates an object for which we only stored the object pointer
		//	Obj = ReferencedObjects[-Index - 1];
		//}
		else if (Index > 0)
		{
			if (Index>CompleteObjects.Num())
			{
				FString className;
				*this << className;
				UClass* objClass = StaticLoadClass(UObject::StaticClass(), NULL, *className);
				if (objClass != NULL)
				{
					Obj = NewObject<UObject>(GetTransientPackage(), objClass);
				}
			}
			else
			{
				Obj = CompleteObjects[Index - 1];
			}

			check(Obj);

			// Ensure we don't load it more than once.
			if (!LoadedObjects.Contains(Obj))
			{
				LoadedObjects.Add(Obj);
				CompleteObjects.Add(Obj);

				//// find the location for this UObject's data in the memory archive
				//int32* ObjectOffset = ObjectMap.Find(Obj);
				//checkf(ObjectOffset, TEXT("%s wasn't not found in ObjectMap for %s - double-check that %s (and objects it references) saves the same amount of data it loads if using custom serialization"),
				//	*Obj->GetFullName(), *GetArchiveName(), *RootObject->GetFullName());

				//// move the reader to that position
				//Reader.Seek(*ObjectOffset);

				//// make sure object instancing is disabled before calling ReinitializeProperties; otherwise new copies of objects will be
				//// created for any instanced object properties owned by this object and then immediately overwritten when its serialized
				//InstanceGraph->EnableSubobjectInstancing(false);

				//// Call ReinitializeProperties to propagate base change to 'within' objects (like Components).
				//Obj->ReinitializeProperties(NULL, InstanceGraph);

				// read in the data for this UObject
				Obj->Serialize(*this);

				// we should never have RootObject serialized as an object contained by the root object
				//checkSlow(Obj != RootObject);

				// serializing the stored data for this object should have replaced all of its original instanced object references
				// but there might have been new subobjects added to the object's archetype in the meantime (in the case of updating 
				// an prefab from a prefab instance, for example), so enable subobject instancing and instance those now.
				//InstanceGraph->EnableSubobjectInstancing(true);

				//if (bInstanceSubobjectsOnLoad)
				//{
				//	// components which were identical to their archetypes weren't stored into this archive's object data, so re-instance those components now
				//	Obj->InstanceSubobjectTemplates(InstanceGraph);
				//}

				// normally we'd never have CDOs in the list of CompleteObjects (CDOs can't be contained within other objects)
				// but in some cases this operator is invoked directly (prefabs)
				if (!Obj->HasAnyFlags(RF_ClassDefaultObject))
				{
					Obj->PostLoad();
				}

			}
		}
	}
	else if (IsSaving())
	{
		// Don't save references to transient or deleted objects.
		if (Obj == NULL || (Obj->HasAnyFlags(RF_Transient)) || Obj->IsPendingKill())
		{
			// null objects are stored as 0 indexes
			int32 Index = 0;
			*this << Index;
			return *this;
		}
		if (RootObject == NULL)
		{
			RootObject = Obj;
		}
			

		UE_LOG(client, Log, TEXT("%s,%s,%s"), *Obj->GetName(), *Obj->GetPathName(), *Obj->GetClass()->GetPathName());

		// See if we have already written this object out.
		int32 CompleteIndex = CompleteObjects.Find(Obj);
		//int32 ReferencedIndex = ReferencedObjects.Find(Obj);

		// The same object can't be in both arrays.
		//check(!(CompleteIndex != INDEX_NONE && ReferencedIndex != INDEX_NONE));

		if (CompleteIndex != INDEX_NONE)
		{
			int32 Index = CompleteIndex + 1;
			*this << Index;
		}
		//else if (ReferencedIndex != INDEX_NONE)
		//{
		//	int32 Index = -ReferencedIndex - 1;
		//	interAr << Index;
		//}
		// New object - was not already saved.
		// if the object is in the SavedObjects array, it means that the object was serialized into this memory archive as a root object
		// in this case, just serialize the object as a simple reference
		else if (/*Obj->IsIn(RootObject) && */ !SavedObjects.Contains(Obj))
		{
			SavedObjects.Add(Obj);

			// we need to store the UObject data for this object
			//check(ObjectMap.Find(Obj) == NULL);

			// only the location of the UObject in the CompleteObjects
			// array is stored in the memory archive, using FPackageIndex-style
			// notation
			int32 Index = CompleteObjects.AddUnique(Obj) + 1;
			interAr << Index;
			FString ClassPathName = Obj->GetClass()->GetPathName();
			*this << ClassPathName;

			// remember the location of the beginning of this UObject's data
			//ObjectMap.Add(Obj, Writer.Tell());

			Obj->Serialize(*this);
		}
		else
		{
			// Referenced objects will be indicated by negative indexes
			int32 Index = 0;// -ReferencedObjects.AddUnique(Obj) - 1;
			interAr << Index;
		}
	}


	return interAr;
}