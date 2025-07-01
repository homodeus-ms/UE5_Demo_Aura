using UnrealBuildTool;
using System.Collections.Generic;

public class AuraServerTarget : TargetRules
{
    public AuraServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.Add("Aura");
        ExtraModuleNames.Add("DedicatedServer");
    }
}
