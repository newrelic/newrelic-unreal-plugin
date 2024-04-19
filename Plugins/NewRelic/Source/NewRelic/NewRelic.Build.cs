// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

namespace UnrealBuildTool.Rules
{
	public class NewRelic : ModuleRules
	{
		public NewRelic(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicIncludePaths.AddRange(new string[] { });
			PrivateIncludePaths.AddRange(new string[] { "NewRelic/Private" });
			PublicDependencyModuleNames.AddRange(new string[] { "Core", });
			PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "HTTP" });
			DynamicallyLoadedModuleNames.AddRange(new string[] { });
			PrivateIncludePathModuleNames.AddRange(new string[] { "Settings" });


			if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				PublicAdditionalFrameworks.Add(
					   new Framework(
						"NewRelic",
						"../NewRelic/ThirdParty/IOS/NewRelic.embeddedframework.zip"
					)
				);

				PublicFrameworks.AddRange(
					new string[]
				{
				"UIKit",
				"SystemConfiguration",
				"CoreTelephony"
				}
				);

				//UPL
				string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "NewRelic_UPL_IOS.xml"));
			}
			else if (Target.Platform == UnrealTargetPlatform.Android)
			{
				// Unreal Plugin Language
				string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "NewRelic_UPL_Android.xml"));
				// JNI
				PublicIncludePathModuleNames.Add("Launch");
			}


		}
	}
}
