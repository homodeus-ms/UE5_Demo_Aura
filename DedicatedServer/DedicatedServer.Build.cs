using UnrealBuildTool;

public class DedicatedServer : ModuleRules
{
	public DedicatedServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"GameLiftServerSDK",
			"GameplayTags",
            "HTTP",
            "NetCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate", 
			"SlateCore",
            "UMG",
            "Json",
            "JsonUtilities"
        });

        RuntimeDependencies.Add("$(BinaryOutputDir)/libcrypto-3-x64.dll", "C:/OpenSSL_3_5/libcrypto-3-x64.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/libssl-3-x64.dll", "C:/OpenSSL_3_5/libssl-3-x64.dll");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}



