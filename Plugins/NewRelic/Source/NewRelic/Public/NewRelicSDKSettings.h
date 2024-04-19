// Fill out your copyright notice in the Description page of Project Settings.
//
//

#pragma once

#include "NewRelicSDKSettings.generated.h"

UCLASS(config = Engine, DefaultConfig)
class UNewRelicSDKSettings: public UObject
{
    GENERATED_BODY()

public:
    UNewRelicSDKSettings(const FObjectInitializer& ObjectInitializer);

    // NewRelic Application Token
    UPROPERTY(Config, EditAnywhere, Category = "New Relic", meta = (DisplayName = "New Relic Android AppToken"))
    FString newrelicAppToken;

	// NewRelic Application Token for IOS. Leave blank if you want to use the same key from "NewRelic Application Token" setting.
	UPROPERTY(Config, EditAnywhere, Category = "New Relic", meta = (DisplayName = "New Relic iOS AppToken (Leave blank if same as above)"))
	FString newrelicAppTokenIOS;
    
    // Enable or disable collection of event data.
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable/Disable Analytics Events)"))
    bool analyticsEventEnabled;

    // Enable or disable crash reporting.
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable or disable crash reporting."))
    bool crashReportingEnabled;

    // Enable or disable interaction tracing
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable or disable interaction tracing."))
    bool interactionTracingEnabled;

    //Enable or disable agent logging.
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable or disable agent logging."))
    bool loggingEnabled;

    // Enable/Disable automatic instrumentation of WebViews
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable/Disable automatic instrumentation of WebViews"))
    bool webViewInstrumentation;

   // Enable/Disable Offline Monitoring
    UPROPERTY(Config, EditAnywhere, config, Category = "New Relic", meta = (DisplayName = "Enable/Disable Offline Monitoring"))
    bool offlineMonitoringEnabled;
};