// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_TinyXml2 : ModuleRules
{
	public FF_TinyXml2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        UndefinedIdentifierWarningLevel = WarningLevel.Off;
        bEnableExceptions = true;

        if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            bUseRTTI = true;
        }

        if (UnrealTargetPlatform.Android == Target.Platform || UnrealTargetPlatform.IOS == Target.Platform)
        {
            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "tinyxml"));
        }

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "TinyXML2"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
