// Fill out your copyright notice in the Description page of Project Settings.
//
//

#include "NewRelicSDKSettings.h"

UNewRelicSDKSettings::UNewRelicSDKSettings(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , newrelicAppToken("")
    , newrelicAppTokenIOS("")
    , analyticsEventEnabled(true)
    , crashReportingEnabled(true)
    , interactionTracingEnabled(true)
    , loggingEnabled(true)
    , webViewInstrumentation(true)
    , offlineMonitoringEnabled(true)
{
}