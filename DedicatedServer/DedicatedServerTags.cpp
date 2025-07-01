#include "DedicatedServerTags.h"

namespace DedicatedServerTags
{
	namespace GameSessionAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServerTags.GameSessionAPI.ListFleets", "For Get ListFleets");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, "DedicatedServerTags.GameSessionAPI.FindOrCreateGameSession", "For FindOrCreateGameSession");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession, "DedicatedServerTags.GameSessionAPI.CreatePlayerSession", "For CreatePlayerSession");
	}
	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUp, "DedicatedServerTags.PortalAPI.SignUp", "For SignUp");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmSignUp, "DedicatedServerTags.PortalAPI.ConfirmSignUp", "For Confirm SignUp");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignIn, "DedicatedServerTags.PortalAPI.SignIn", "For SignIn");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignOut, "DedicatedServerTags.PortalAPI.SignOut", "For SignOut");
	}
	namespace SaveDataAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SaveData, "DedicatedServerTags.SaveDataAPI.SaveData", "For Save Player Data");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(LoadData, "DedicatedServerTags.SaveDataAPI.LoadData", "For Load Player Data");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(LoadInitInfo, "DedicatedServerTags.SaveDataAPI.LoadInitInfo", "For Load Player Init Info");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetNotANewPlayer, "DedicatedServerTags.SaveDataAPI.SetNotANewPlayer", "After Saved Data First Time");
	}
}