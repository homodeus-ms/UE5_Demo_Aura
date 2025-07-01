using UnrealBuildTool;
using System.Collections.Generic;

public class AuraClientTarget : TargetRules
{
    public AuraClientTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Client;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.Add("Aura");
        ExtraModuleNames.Add("DedicatedServer");
    }
}
