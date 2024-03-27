// Copyright Epic Games, Inc. All Rights Reserved.

#include "NewRelicBPLibrary.h"
#include "NewRelic.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Engine/Engine.h"
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
int b = 0;
float UNewRelicBPLibrary::NewRelicSampleFunction(float Param)
{

    b++;
    if(b < 100) {

   
     UE_LOG(LogTemp, Warning, TEXT("NEWRELIC!"));
    }

if (b == 3) {

    FString id =  startInterAction("test Unreal InterAction");

    FPlatformProcess::Sleep(6.0);   

    endInterAction(id);
}


if(b > 100 && b < 105) {

if(b == 102) {
      TMap<FString, FString> errorsMap;
      errorsMap.Add("place", TEXT("Robots"));
      errorsMap.Add("user", TEXT("Nisarg"));
      recordError(TEXT("Error Message"), errorsMap);
}



    // int x = 2147483647;
    //
    // x= x+1k
    // check(0); 
    // FString nis = "Test";
    // nis.RemoveAt(10);
    // UE_LOG(LogTemp, Fatal, TEXT("NEWRELIC Crash!"));


// setUserId("Unreal Engine");

// setBooleanAttribute("From Unreal",true);

// setDoubleAttribute("Unreal Double", 15.0);

// setStringAttribute("Unreal String","Nisarg Desai");

// removeAttribute("Unreal String");

// incrementAttribute("Unreal Double",5.0);

TMap<FString, FString> FruitMap;

FruitMap.Add("5", TEXT("Banana"));
FruitMap.Add("2", TEXT("Grapefruit"));
FruitMap.Add("7", TEXT("Pineapple"));


// 
recordCustomEvent("Unreal Event Name", FruitMap);

recordBreadCrumb("Unreal Breadcrumb Without Attribute");


FString uriBase = "https://reactnative.dev";
    FString uriLogin = uriBase + TEXT("/movies.json");


    FHttpModule& httpModule = FHttpModule::Get();

    // Create an http request
    // The request will execute asynchronously, and call us back on the Lambda below
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule.CreateRequest();

    // This is where we set the HTTP method (GET, POST, etc)
    // The Space-Track.org REST API exposes a "POST" method we can use to get
    // general perturbations data about objects orbiting Earth
    pRequest->SetVerb(TEXT("GET"));

    // We'll need to tell the server what type of content to expect in the POST data

    // Set the POST content, which contains:
    // * Identity/password credentials for authentication
    // * A REST API query
    //   This allows us to login and get the results is a single call
    //   Otherwise, we'd need to manage session cookies across multiple calls.

    // Set the http URL
    pRequest->SetURL(uriLogin);

    // Set the callback, which will execute when the HTTP call is complete
    pRequest->OnProcessRequestComplete().BindLambda(
        // Here, we "capture" the 'this' pointer (the "&"), so our lambda can call this
        // class's methods in the callback.
        [&](
            FHttpRequestPtr pRequest,
            FHttpResponsePtr pResponse,
            bool connectedSuccessfully) mutable {

        if (connectedSuccessfully) {

            UE_LOG(LogTemp,Warning,TEXT("connected Successfully"));
        }
        else {
            switch (pRequest->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
                UE_LOG(LogTemp, Error, TEXT("Connection failed."));
            default:
                UE_LOG(LogTemp, Error, TEXT("Request failed."));
            }
        }
    });

    // Finally, submit the request for processing
    pRequest->ProcessRequest();

}

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
        currentsessionId = [NewRelic currentsessionId];
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








