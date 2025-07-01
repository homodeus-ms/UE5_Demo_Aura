#pragma once
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace DedicatedServerTags
{
	namespace GameSessionAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListFleets);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FindOrCreateGameSession);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(CreatePlayerSession);
	}
	namespace PortalAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConfirmSignUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignIn);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignOut);
	}
	namespace SaveDataAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SaveData);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LoadData);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LoadInitInfo);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetNotANewPlayer);
	}
}