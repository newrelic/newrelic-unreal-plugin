// Copyright (c) 2024 NewRelic. All Rights Reserved.

#include "NewRelicBPLibrary.h"
#include "NewRelic.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Engine/Engine.h"
#include "NewRelicSDKSettings.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#import <mutex>
#include <dlfcn.h>
#include <jni.h>
#include <cstdio>
#elif PLATFORM_IOS
#import <NewRelic/NewRelic.h>
#include "IOSAppDelegate.h"
#import <objc/message.h>
#endif

UNewRelicBPLibrary::UNewRelicBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UNewRelicBPLibrary::NewRelicSampleFunction(float Param) {

    return -1;
}


void UNewRelicBPLibrary::start(FString applicationtoken) {

const UNewRelicSDKSettings *defaultSettings = GetDefault<UNewRelicSDKSettings>();
 
#if PLATFORM_ANDROID
    
    JNIEnv* env = FAndroidApplication::GetJavaEnv();

    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
     
     if(!defaultSettings->analyticsEventEnabled) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF("analyticsEventEnabled"), false);
     }

     if(!defaultSettings->crashReportingEnabled) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF("crashReportingEnabled"), false);
     }

     if(!defaultSettings->interactionTracingEnabled) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF("interactionTracingEnabled"), false);
     }

     if(!defaultSettings->offlineMonitoringEnabled) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF("offlineMonitoringEnabled"), false);
     }

    if(!defaultSettings->backgroundReportingEnabled) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF("backgroundReportingEnabled"), false);
    }

     jmethodID disableFeatures = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_DisableFeatures",
                                    "(Ljava/util/Map;)V", false);

    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, disableFeatures, map);


    jmethodID startAgent = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_StartAgent",
                                  "(Ljava/lang/String;)V", false);
    jstring jApplicationToken = env->NewStringUTF(TCHAR_TO_UTF8(*defaultSettings->newrelicAppToken));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, startAgent, jApplicationToken);
#elif PLATFORM_IOS

  NSString *token = defaultSettings->newrelicAppTokenIOS.IsEmpty() ? defaultSettings->newrelicAppToken.GetNSString() : defaultSettings->newrelicAppTokenIOS.GetNSString();


  dispatch_async(dispatch_get_main_queue(), ^ {
    
    if(!defaultSettings->crashReportingEnabled) {
     [NewRelic disableFeatures:NRFeatureFlag_CrashReporting];   
      } 

    if(!defaultSettings->interactionTracingEnabled) {
      [NewRelic disableFeatures:NRFeatureFlag_InteractionTracing];
    }

    if(defaultSettings->loggingEnabled) {
       [NRLogger setLogLevels: 65535];
    }

    if(!defaultSettings->webViewInstrumentation) {
      [NewRelic disableFeatures:NRFeatureFlag_WebViewInstrumentation];
    }

    if(!defaultSettings->offlineMonitoringEnabled) {
      [NewRelic disableFeatures:NRFeatureFlag_OfflineStorage];
    }
    if(!defaultSettings->backgroundReportingEnabled) {
      [NewRelic disableFeatures:NRFeatureFlag_BackgroundReporting];
    }
    if(!defaultSettings->newEventSystemEnabled) {
      [NewRelic disableFeatures:NRFeatureFlag_NewEventSystem];
    }
    [NewRelic setPlatform:(NRMAApplicationPlatform)NRMAPlatform_Unreal];
      SEL selector = NSSelectorFromString(@"setPlatformVersion:");
      [NewRelic performSelector:selector withObject:@"1.3.0"];
      [NewRelic startWithApplicationToken: token];
  });
#endif
}

void UNewRelicBPLibrary::recordBreadCrumbWithAttributes(FString breadcrumbName,TMap <FString, FString> customData) {

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID recordBreadCrumb = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_RecordBreadCrumb",
                                    "(Ljava/lang/String;Ljava/util/Map;)V", false);
    jstring jBreadcrumbName = env->NewStringUTF(TCHAR_TO_UTF8(*breadcrumbName));
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (const TPair<FString, FString>& pair : customData) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF(TCHAR_TO_UTF8(*pair.Key)), env->NewStringUTF(TCHAR_TO_UTF8(*pair.Value)));
    }
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, recordBreadCrumb, jBreadcrumbName, map);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {

        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
        for (const TPair<FString, FString>& pair : customData) {
            [dictionary setValue:pair.Value.GetNSString() forKey:pair.Key.GetNSString()];
        }

        [NewRelic recordBreadcrumb:breadcrumbName.GetNSString() attributes:dictionary];
    });
#else
    return;
#endif
}

void UNewRelicBPLibrary::recordBreadCrumb(FString breadcrumbName) {

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID recordBreadCrumb = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_RecordBreadCrumb",
                                    "(Ljava/lang/String;)V", false);
    jstring jBreadcrumbName = env->NewStringUTF(TCHAR_TO_UTF8(*breadcrumbName));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, recordBreadCrumb, jBreadcrumbName);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic recordBreadcrumb:breadcrumbName.GetNSString() attributes:nil];
    });
#else
    return;
#endif
}
    

	 void UNewRelicBPLibrary::setUserId(FString userId){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID setUserID = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetUserId",
                                  "(Ljava/lang/String;)V", false);
    jstring jUserId = env->NewStringUTF(TCHAR_TO_UTF8(*userId));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, setUserID, jUserId);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setUserId:userId.GetNSString()];
    });
#endif

     }

	 void UNewRelicBPLibrary::setStringAttribute(FString name,FString value){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID setStringAttribute = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetStringAttribute",
                                  "(Ljava/lang/String;Ljava/lang/String;)V", false);
    jstring jAttributeName = env->NewStringUTF(TCHAR_TO_UTF8(*name));
    jstring jAttributeValue = env->NewStringUTF(TCHAR_TO_UTF8(*value));

    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, setStringAttribute, jAttributeName,jAttributeValue);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setAttribute:name.GetNSString() value:value.GetNSString()];
    });
#endif

     }

	 void UNewRelicBPLibrary::setDoubleAttribute(FString name,double value){


#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID setDoubleAttribute = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetDoubleAttribute",
                                  "(Ljava/lang/String;D)V", false);
    jstring jAttributeName = env->NewStringUTF(TCHAR_TO_UTF8(*name));

    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, setDoubleAttribute, jAttributeName,value);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setAttribute:name.GetNSString() value:[NSNumber numberWithDouble:value]];
    });
#endif

    }

	 void UNewRelicBPLibrary::setBooleanAttribute(FString name ,bool value){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID setBooleanAttribute = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetBooleanAttribute",
                                  "(Ljava/lang/String;Z)V", false);
    jstring jAttributeName = env->NewStringUTF(TCHAR_TO_UTF8(*name));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, setBooleanAttribute, jAttributeName,value);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setAttribute:name.GetNSString() value:[NSNumber numberWithDouble:value]];
    });
#endif

    }

	 void UNewRelicBPLibrary::removeAttribute(FString name){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID removeAttribute = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_RemoveAttribute",
                                  "(Ljava/lang/String;)V", false);
    jstring jAttributeName = env->NewStringUTF(TCHAR_TO_UTF8(*name));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, removeAttribute, jAttributeName);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic removeAttribute:name.GetNSString()];
    });
#endif

    }

	 void UNewRelicBPLibrary::removeAllAttributes(){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID removeAllAttributes = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  " AndroidThunk_RemoveAllAttributes",
                                  "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, removeAllAttributes);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic removeAllAttributes];
    });
#endif

     }

	 void UNewRelicBPLibrary::incrementAttribute(FString name,double value){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID incrementAttribute = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_IncrementAttribute",
                                  "(Ljava/lang/String;D)V", false);
    jstring jAttributeName = env->NewStringUTF(TCHAR_TO_UTF8(*name));

    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, incrementAttribute, jAttributeName,value);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic incrementAttribute:name.GetNSString() value:[NSNumber numberWithDouble:value]];
    });
#endif

     }


	 void UNewRelicBPLibrary::setMaxEventBufferTime(int64 maxBufferTimeInSec){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID maxEventBufferTime = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetMaxEventBufferTime","(I)V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, maxEventBufferTime,(jint)maxBufferTimeInSec);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setMaxEventBufferTime:maxBufferTimeInSec];
    });
#endif

     }

	 void UNewRelicBPLibrary::setMaxEventPoolSize(int64 maxSize){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID maxEventPoolSize = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_SetMaxEventPoolSize","(I)V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, maxEventPoolSize,(jint)maxSize);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic setMaxEventPoolSize:maxSize];
    });
#endif
       
    
     }

	 void UNewRelicBPLibrary::shutdown(){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID shutDown = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_Shutdown","()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, shutDown);
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
				 [NewRelic shutdown];
    });
#endif

     }

	 void UNewRelicBPLibrary::crashNow(FString message){

        FPlatformProcess::Sleep(3.0);   
        
        UE_LOG(LogTemp, Fatal, TEXT("NEWRELIC Crash!"));

     }

 void UNewRelicBPLibrary::recordCustomEventWithEventType(FString eventType,FString eventName, TMap <FString, FString> eventAttributes) {

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID recordCustomEvent = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_RecordCustomEvent",
                                    "(Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;)V", false);
    jstring jEventType = env->NewStringUTF(TCHAR_TO_UTF8(*eventType));
    jstring jEventName = env->NewStringUTF(TCHAR_TO_UTF8(*eventName));
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (const TPair<FString, FString>& pair : eventAttributes) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF(TCHAR_TO_UTF8(*pair.Key)), env->NewStringUTF(TCHAR_TO_UTF8(*pair.Value)));
    }
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, recordCustomEvent, jEventType,jEventName, map);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {

        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
        for (const TPair<FString, FString>& pair : eventAttributes) {
            [dictionary setValue:pair.Value.GetNSString() forKey:pair.Key.GetNSString()];
        }

        [NewRelic recordCustomEvent:eventType.GetNSString() name:eventName.GetNSString() attributes:dictionary];

    });
#else
    return;
#endif
}

 void UNewRelicBPLibrary::recordCustomEvent(FString eventType, TMap <FString, FString> eventAttributes) {

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID recordCustomEvent = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_RecordCustomEvent",
                                    "(Ljava/lang/String;Ljava/util/Map;)V", false);
    jstring jEventType = env->NewStringUTF(TCHAR_TO_UTF8(*eventType));
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (const TPair<FString, FString>& pair : eventAttributes) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF(TCHAR_TO_UTF8(*pair.Key)), env->NewStringUTF(TCHAR_TO_UTF8(*pair.Value)));
    }
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, recordCustomEvent,jEventType, map);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {

        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
        for (const TPair<FString, FString>& pair : eventAttributes) {
            [dictionary setValue:pair.Value.GetNSString() forKey:pair.Key.GetNSString()];
        }

        [NewRelic recordCustomEvent:eventType.GetNSString() attributes:dictionary];

    });
#else
    return;
#endif
}

	FString UNewRelicBPLibrary::startInterAction(FString interActionName) {


#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID startInterAction = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_StartInterAction",
                                    "(Ljava/lang/String;)Ljava/lang/String;", false);
    jstring jInterActionName = env->NewStringUTF(TCHAR_TO_UTF8(*interActionName));
    jstring jInterActionId = (jstring)FJavaWrapper::CallObjectMethod(env, FJavaWrapper::GameActivityThis, startInterAction, jInterActionName);
    const char* cInterActionId = env->GetStringUTFChars(jInterActionId, 0);
    FString interActionId = FString(UTF8_TO_TCHAR(cInterActionId));
    env->ReleaseStringUTFChars(jInterActionId, cInterActionId);
    return interActionId;      
#elif PLATFORM_IOS
    __block NSString *interActionId;    
    dispatch_sync(dispatch_get_main_queue(), ^ {
        interActionId = [NewRelic startInteractionWithName:interActionName.GetNSString()];
    });
    return interActionId;   
#else
    return "";       
#endif                   
    
    }

FString UNewRelicBPLibrary::currentSessionId() {


#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID currentSessionId = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_CurrentSessionId",
                                    "Ljava/lang/String;", false);
    jstring jCurrentsessionId = (jstring)FJavaWrapper::CallObjectMethod(env, FJavaWrapper::GameActivityThis, currentSessionId);
    const char* cCurrentsessionId = env->GetStringUTFChars(jCurrentsessionId, 0);
    FString currentsessionId = FString(UTF8_TO_TCHAR(cCurrentsessionId));
    env->ReleaseStringUTFChars(jCurrentsessionId, cCurrentsessionId);
    return currentsessionId;      
#elif PLATFORM_IOS
    __block NSString *currentsessionId;    
    dispatch_sync(dispatch_get_main_queue(), ^ {
        currentsessionId = [NewRelic currentSessionId];
    });
    return currentsessionId;   
#else
    return "";       
#endif                   
    
    }

	void UNewRelicBPLibrary::endInterAction(FString interActionId){

#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID endInterAction = FJavaWrapper::FindMethod(env,
                                  FJavaWrapper::GameActivityClassID,
                                  "AndroidThunk_EndInterAction",
                                  "(Ljava/lang/String;)V", false);
    jstring jInterActionId = env->NewStringUTF(TCHAR_TO_UTF8(*interActionId));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, endInterAction, jInterActionId);
#elif PLATFORM_IOS

    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic stopCurrentInteraction:interActionId.GetNSString()];
    });     
#endif

    }

void UNewRelicBPLibrary::recordError(FString errorMessage,TMap <FString, FString> errorAttributes) {

#if PLATFORM_IOS

        dispatch_async(dispatch_get_main_queue(), ^ {

        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
        for (const TPair<FString, FString>& pair : errorAttributes) {
            [dictionary setValue:pair.Value.GetNSString() forKey:pair.Key.GetNSString()];
        }

        NSError* const error = [[NSError alloc]
		initWithDomain:errorMessage.GetNSString()
		code : 200
		userInfo : @{ NSLocalizedDescriptionKey: errorMessage.GetNSString() }];
        [NewRelic recordError:error attributes:dictionary];
	    [error release] ;
    });

#elif PLATFORM_ANDROID

    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID recordError = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_RecordError",
                                    "(Ljava/lang/String;Ljava/util/Map;)V", false);
    jstring jErrorMessage = env->NewStringUTF(TCHAR_TO_UTF8(*errorMessage));
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (const TPair<FString, FString>& pair : errorAttributes) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF(TCHAR_TO_UTF8(*pair.Key)), env->NewStringUTF(TCHAR_TO_UTF8(*pair.Value)));
    }
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, recordError, jErrorMessage,map);
    return;

#endif
}

void UNewRelicBPLibrary::logInfo(FString message)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logInfo = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogInfo",
                                    "(Ljava/lang/String;)V", false);
    jstring jMessage = env->NewStringUTF(TCHAR_TO_UTF8(*message));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logInfo, jMessage);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic logInfo:message.GetNSString()];
    });
#else
    return;
#endif
    
}

void UNewRelicBPLibrary::logVerbose(FString message)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logVerbose = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogVerbose",
                                    "(Ljava/lang/String;)V", false);
    jstring jMessage = env->NewStringUTF(TCHAR_TO_UTF8(*message));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logVerbose, jMessage);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic logVerbose:message.GetNSString()];
    });
#else
    return;
#endif
}

void UNewRelicBPLibrary::logError(FString message)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logError = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogError",
                                    "(Ljava/lang/String;)V", false);
    jstring jMessage = env->NewStringUTF(TCHAR_TO_UTF8(*message));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logError, jMessage);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic logError:message.GetNSString()];
    });
#else
    return;
#endif
}

void UNewRelicBPLibrary::logWarning(FString message)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logWarning = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogWarning",
                                    "(Ljava/lang/String;)V", false);
    jstring jMessage = env->NewStringUTF(TCHAR_TO_UTF8(*message));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logWarning, jMessage);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic logWarning:message.GetNSString()];
    });
#else
    return;
#endif
}

void UNewRelicBPLibrary::logDebug(FString message)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logDebug = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogDebug",
                                    "(Ljava/lang/String;)V", false);
    jstring jMessage = env->NewStringUTF(TCHAR_TO_UTF8(*message));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logDebug, jMessage);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        [NewRelic logDebug:message.GetNSString()];
    });
#else
    return;
#endif
}

void UNewRelicBPLibrary::log(AgentLogLevel level,FString message)
{

    if (level == AgentLogLevel::INFO) {
        logInfo(message);
    } else if (level == AgentLogLevel::VERBOSE) {
        logVerbose(message);
    } else if (level == AgentLogLevel::ERROR) {
        logError(message);
    } else if (level == AgentLogLevel::WARNING) {
        logWarning(message);
    } else if (level == AgentLogLevel::DEBUG) {
        logDebug(message);
    } else
    {
        logError(message);
    }
}

void UNewRelicBPLibrary::logAttributes(TMap <FString, FString> attributes)
{
#if PLATFORM_ANDROID
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID logAttributes = FJavaWrapper::FindMethod(env,
                                    FJavaWrapper::GameActivityClassID,
                                    "AndroidThunk_LogAttributes",
                                    "(Ljava/util/Map;)V", false);
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject map = env->NewObject(mapClass, mapConstructor);
    jmethodID putMethod = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (const TPair<FString, FString>& pair : attributes) {
        env->CallObjectMethod(map, putMethod, env->NewStringUTF(TCHAR_TO_UTF8(*pair.Key)), env->NewStringUTF(TCHAR_TO_UTF8(*pair.Value)));
    }
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, logAttributes, map);
    return;
#elif PLATFORM_IOS
    dispatch_async(dispatch_get_main_queue(), ^ {
        
        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
        for (const TPair<FString, FString>& pair : attributes) {
            [dictionary setValue:pair.Value.GetNSString() forKey:pair.Key.GetNSString()];
        }
        
        [NewRelic logAttributes:dictionary];
        
    });
#else
    return;
#endif
}







