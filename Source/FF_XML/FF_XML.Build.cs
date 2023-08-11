// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_XML : ModuleRules
{
	public FF_XML(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            string Location_TinyXML = "../Source/FF_XML/ThirdParty/tinyxml/Windows/include";
            PrivateIncludePaths.Add(Location_TinyXML);

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "tinyxml", "Windows", "lib", "tinyxml2.lib"));
        }

        if (UnrealTargetPlatform.Android == Target.Platform)
        {
            string Location_open62541 = "../Source/OPC_Client/ThirdParty/open62541/Android";
            PrivateIncludePaths.Add(Location_open62541);
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
