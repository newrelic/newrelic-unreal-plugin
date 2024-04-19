// Copyright Epic Games, Inc. All Rights Reserved.

#include "NewRelic.h"
#include "NewRelicBPLibrary.h"
#include "ISettingsModule.h"
#include "NewRelicSDKSettings.h"

DEFINE_LOG_CATEGORY(LogNewRelicSDK);

#define LOCTEXT_NAMESPACE "FNewRelicModule"


void FNewRelicModule::StartupModule()
{
	    // Register settings: `Settings -> Project Settings -> Plugins -> AppsFlyer SDK`
    if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        settingsModule->RegisterSettings("Project", "Plugins", "NewRelic SDK",
                                 LOCTEXT("RuntimeSettingsName", "NewRelic SDK"),
                                 LOCTEXT("RuntimeSettingsDescription", "Configure Agent(plugin)"),
                        GetMutableDefault<UNewRelicSDKSettings>());
        // UE_LOG(LOCTEXT_NAMESPACE, Display, TEXT("[StartupModule][RegisterSettings] Done"));
    }
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
		#if PLATFORM_ANDROID || PLATFORM_IOS
    UNewRelicBPLibrary::start("");
#endif
	
}

void FNewRelicModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNewRelicModule, NewRelic)