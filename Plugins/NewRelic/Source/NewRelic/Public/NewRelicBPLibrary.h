// Copyright (c) 2024 NewRelic. All Rights Reserved.

#pragma once

#include "AgentLogLevel.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NewRelicBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UNewRelicBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "NewRelic sample test testing"), Category = "NewRelicTesting")
	static float NewRelicSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Start")
	static void start(FString applicationtoken);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Record Bredcrumb With Custom Attributes")
	static void recordBreadCrumbWithAttributes(FString breadcrumbName, TMap <FString, FString> customData);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Set Customer User Id for Sessions")
	static void setUserId(FString userId);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Create a Custom String Attribute")
	static void setStringAttribute(FString name,FString value);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Create a Custom Double Attribute")
	static void setDoubleAttribute(FString name,double value);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Create a Custom Boolean Attribute")
	static void setBooleanAttribute(FString name ,bool value);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Remove an attribute")
	static void removeAttribute(FString name);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Remove all attribute")
	static void removeAllAttributes();

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Increment session attribute count")
	static void incrementAttribute(FString name,double value);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Set max event buffer time")
	static void setMaxEventBufferTime(int64 maxBufferTimeInSec);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Set max event pool size")
	static void setMaxEventPoolSize(int64 maxSize);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Shut down the agent")
	static void shutdown();

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "Test crash reporting")
	static void crashNow(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Record Custom Event With Attributes")
	static void recordCustomEventWithEventType(FString eventType,FString eventName, TMap <FString, FString> eventAttributes);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Record Custom Event")
	static void recordCustomEvent(FString eventType, TMap <FString, FString> eventAttributes);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Record Bredcrumb")
	static void recordBreadCrumb(FString breadcrumbName);
    
	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Record Error with Attributes")
	static void recordError(FString errorMessage,TMap <FString, FString> errorAttributes);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Start InterAction")
	static FString startInterAction(FString interActionName);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK End InterAction")
	static void endInterAction(FString interActionId);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Get Current Session ID")
	static FString currentSessionId();

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs an informational message to the New Relic log.")
	static void logInfo(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs a verbose message to the New Relic log.")
	static void logVerbose(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs an error message to the New Relic log..")
	static void logError(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs a warning message to the New Relic log.")
	static void logWarning(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs a debug message to the New Relic log.")
	static void logDebug(FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs a message with a specific log level to the New Relic log.")
	static void log(AgentLogLevel level,FString message);

	UFUNCTION(BlueprintCallable, Category = NewRelicSDK, DisplayName = "NewRelicSDK Logs a message with attributes to the New Relic log.")
	static void logAttributes(TMap <FString, FString> attributes);
	
};
