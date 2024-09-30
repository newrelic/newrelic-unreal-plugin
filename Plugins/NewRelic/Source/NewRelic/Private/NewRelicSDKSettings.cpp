// Copyright (c) 2024 NewRelic. All Rights Reserved.


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
    , backgroundReportingEnabled(false)
    , newEventSystemEnabled(true)

{
}