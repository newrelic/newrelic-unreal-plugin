<a href="https://opensource.newrelic.com/oss-category/#community-project"><picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/newrelic/opensource-website/raw/main/src/images/categories/dark/Community_Project.png"><source media="(prefers-color-scheme: light)" srcset="https://github.com/newrelic/opensource-website/raw/main/src/images/categories/Community_Project.png"><img alt="New Relic Open Source community project banner." src="https://github.com/newrelic/opensource-website/raw/main/src/images/categories/Community_Project.png"></picture></a>


# New Relic Unreal Engine Agent

This agent utilizes the native New Relic Android and iOS agents to instrument Unreal Engine Mobile apps. The New Relic SDKs gather data such as crashes and other relevant information to help monitor and assess the performance of Unreal Engine Mobile apps.

## Features
* Record and Capture errors
* Handled Exception
* Capture interactions and the sequence in which they were created
* Pass user information to New Relic to track user sessions
* offline monitoring of events and exceptions
* C++ Crashes (iOS Only)
* Capture Background Events when app is in background

## Current Support:
- Android API 24+ (AGP 7 and Higher)
- iOS 10
- Depends on New Relic iOS/XCFramework and Android agents

## Installation

1. In your game’s root directory, create a folder named Plugins
   Copy the NewRelic folder From Plugins into it. You should have something like

```
  MyProject
  ├── Plugins
          └── NewRelic
              └── NewRelic.uplugin
```

<!--- **From the Unreal Engine Marketplace**
1. Install the NewRelic plugin from the Unreal Engine Marketplace.
2. Enable the plugin from the Plugins window in Unreal Editor, available in the main menu under Edit → Plugins.
3. Select NewRelic under “Installed” plugins, and check the “Enabled” checkbox. --->
![NewRelic SDK Installation](images/newrelic_sdk_installation.png)
2. Click “Restart Now” to continue configuration.

3. Add NewRelic to the list of dependencies in your game module’s .Build.cs file in order to allow interfacing with NewRelic from C++ code:

**PrivateDependencyModuleNames.Add("NewRelic");**

## Basic configuration#
Configure your API key through Unreal Editor’s Project Settings window, under Plugins → NewRelic.

![NewRelic SDK Configuration](images/newrelic_sdk_configuration.png)

Or in **Config/DefaultEngine.ini**:\
**[/Script/NewRelic.NewRelicSDKSettings]**\
newrelicAppToken=application-token\
newrelicAppTokenIOS=ios-application-token

## Usage

See the examples below, and for more detail, see [New Relic IOS SDK doc](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-ios/ios-sdk-api) or [Android SDK](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api).

### [startInterAction](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/start-interaction)(FString name): &lt;InteractionId&gt;;
> Track a method as an interaction.

`InteractionId` is string.


### [endInterAction](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/end-interaction)(FString interactionIdentifier): void;
> End an interaction
> (Required). This uses the string ID for the interaction you want to end.
> This string is returned when you use startInteraction().

  ```C++

    #include "NewRelicBPLibrary.h"

    FString id =  UNewRelicBPLibrary::startInterAction("test Unreal InterAction");

    FPlatformProcess::Sleep(6.0);   

    UNewRelicBPLibrary::endInterAction(id);
  ```

Using Blueprint :

![Interaction Example](images/newrelic_sdk_interaction_example.png)

### [setBooleanAttribute](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-attribute)(Boolean value): void;

> Creates a session-level boolean attribute shared by multiple mobile event types. Overwrites its previous value and type each time it is called.
  ```C++

     #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::setBooleanAttribute("From Unreal",true);
  ```

### [setDoubleAttribute](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-attribute)(double value): void;

> Creates a session-level double attribute shared by multiple mobile event types. Overwrites its previous value and type each time it is called.
  ```C++

     #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::setDoubleAttribute("Unreal Double", 15.0);;
  ```

  ### [setStringAttribute](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-attribute)(FString value): void;

> Creates a session-level double attribute shared by multiple mobile event types. Overwrites its previous value and type each time it is called.
  ```C++

     #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::setStringAttribute("Unreal String","From UnReal");

  ```
  
  Using BluePrint:
  ![NewRelic Attribute Example](images/newrelic_attribute_example.png)
  

### [removeAttribute](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/remove-attribute)(FString name): void;
> This method removes the attribute specified by the name string..
  ```C++
       #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::removeAttribute("Unreal String");
  ```

### [removeAllAttributes](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/remove-all-attributes)(): void;
> Removes all attributes from the session
```C#
    #include "NewRelicBPLibrary.h"
  
    UNewRelicBPLibrary::removeAllAttributes();
```
Using Blueprint:

![NewRelic SDK Remove Attribute](images/newrelic_sdk_remove_attribute.png)


### recordError(FString error,TMap <FString, FString> errorAttributes ): void;
> Records errors for Unreal with Map Parameters .
```C++
      #include "NewRelicBPLibrary.h"

      TMap<FString, FString> errorsMap;
      errorsMap.Add("place", TEXT("Robots"));
      errorsMap.Add("user", TEXT("Nisarg"));
       UNewRelicBPLibrary::recordError(TEXT("Error Message"), errorsMap);
```
Using Blueprint:
![Newrelic Record Error](images/newrelic_sdk_record_error.png)

### [setMaxEventBufferTime](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-max-event-buffer-time)(int64 seconds): void;
> Sets the event harvest cycle length. Default is 600 seconds (10 minutes). Minimum value can not be less than 60 seconds. Maximum value should not be greater than 600 seconds.
```C++
     #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::setMaxEventBufferTime(60);
```

### [setMaxEventPoolSize](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-max-event-pool-size)(int64 size): void;
> Sets the maximum size of the event pool stored in memory until the next harvest cycle. Default is a maximum of 1000 events per event harvest cycle. When the pool size limit is reached, the agent will start sampling events, discarding some new and old, until the pool of events is sent in the next harvest cycle.
```C++
     #include "NewRelicBPLibrary.h"

     UNewRelicBPLibrary::setMaxEventPoolSize(2000);
```

### [incrementAttribute](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/increment-attribute)(FString name, double amount): void;
> Increments the count of an attribute with a specified name. Overwrites its previous value and type each time it is called. If the attribute does not exists, it creates a new attribute. If no value is given, it increments the value by 1.
```C++
    #include "NewRelicBPLibrary.h"

    UNewRelicBPLibrary::incrementAttribute("Unreal Double",5.0)
```
### [setUserId](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/set-user-id)(FString userId): void;
> Set a custom user identifier value to associate user sessions with analytics events and attributes.
  ```C++
      #include "NewRelicBPLibrary.h"

      UNewRelicBPLibrary::setUserId("Unreal12934");
  ```

 ### [recordBreadcrumb](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/recordbreadcrumb)(FString breadcrumbName): void;
> Track app activity/screen that may be helpful for troubleshooting crashes.

  ```C++
      #include "NewRelicBPLibrary.h"

      UNewRelicBPLibrary::.recordBreadCrumb("Unreal Breadcrumb Without Attribute");
  ```

   ### [recordBreadCrumbWithAttributes](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/recordbreadcrumb)(FString breadcrumbName,TMap <FString, FString> customData): void;
> Track app activity/screen that may be helpful for troubleshooting crashes with Custom Attributes.

  ```C++
      #include "NewRelicBPLibrary.h"

      TMap<FString, FString> breadCrumbMap;
      breadCrumbMap.Add("place", TEXT("Robots"));
      breadCrumbMap.Add("user", TEXT("Nisarg"));

      UNewRelicBPLibrary::recordBreadCrumbWithAttributes("Unreal Breadcrumb",breadCrumbMap);
  ```

  Using BluePrint:
  ![Record Breadcrumb exam](images/newrelic_sdk_record_breadcrumb.png)


### [recordCustomEvent](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/recordcustomevent-android-sdk-api)(FString eventType, TMap <FString, FString> eventAttributes): void;
> Creates and records a custom event for use in New Relic Insights.

  ```C++
        #include "NewRelicBPLibrary.h"

         TMap<FString, FString> customEventMap;
         customEventMap.Add("place", TEXT("Robots"));
         customEventMap.Add("user", TEXT("user1"));

        UNewRelicBPLibrary::recordCustomEvent("Unreal Custom Event Example", customEventMap);
  ```

  ### [recordCustomEventWithEventType](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/recordcustomevent-android-sdk-api)(FString eventType,FString eventName, TMap <FString, FString> eventAttributes): void;
> Creates and records a custom event for use in New Relic Insights.

  ```C++
        #include "NewRelicBPLibrary.h"
        
         TMap<FString, FString> customEventMap;
         customEventMap.Add("place", TEXT("Robots"));
         customEventMap.Add("user", TEXT("user1"));

        UNewRelicBPLibrary::recordCustomEvent("Unreal Custom Event Type","Unreal Custom Event Example", customEventMap);
  ```

  Using BluePrint:
  ![Record Custom Event](images/newrelic_sdk_record_custom_event.png)

### [currentSessionId](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/currentsessionid-android-sdk-api)(): FString;
> Returns the current session ID. This method is useful for consolidating monitoring of app data (not just New Relic data) based on a single session definition and identifier.
```C++
    #include "NewRelicBPLibrary.h"
   
    FString id =  UNewRelicBPLibrary::currentSessionId();

```
### [shutdown](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/shut-down/)() : void;
> Shut down the agent within the current application lifecycle during runtime.
```C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::shutdown();
```

### [crashNow](https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-android/android-sdk-api/crashnow-android-sdk-api)(FString message): void;
> Throws a demo run-time exception to test New Relic crash reporting.

```C++
     #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::crashNow("This is test");
```

### logInfo(FString message) : void

> Logs an informational message to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::logInfo("This is an informational message");
```


### logError(FString message) : void
> Logs an error message to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::logError("This is an error message");
```
### logVerbose(FString message) : void
> Logs a verbose message to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::logVerbose("This is a verbose message");
```

### logWarning(FString message) : void
> Logs a warning message to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::logWarning("This is a warning message");
```

### logDebug(FString message) : void
> Logs a debug message to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::logDebug("This is a debug message");
```

### log(AgentLogLevel level, FString message) : void
> Logs a message to the New Relic log with a specified log level.
``` C++
    #include "NewRelicBPLibrary.h"      

    UNewRelicBPLibrary::log(AgentLogLevel::Debug, "This is a debug message");
```

### logAttributes(TMap <FString, FString> attributes) : void
> Logs a message with attributes to the New Relic log.
``` C++
    #include "NewRelicBPLibrary.h"      

    TMap<FString, FString> attributes;
    attributes.Add("place", TEXT("Robots"));
    attributes.Add("user", TEXT("user1"));
    attributes.Add("level", TEXT("DEBUG"));
    attributes.Add("message", TEXT("This is a debug message"));

    UNewRelicBPLibrary::logAttributes(attributes);
```



## Contribute

We encourage your contributions to improve `newrelic-unreal-plugin`! Keep in mind that when you submit your pull request, you'll need to sign the CLA via the click-through using CLA-Assistant. You only have to sign the CLA one time per project.

If you have any questions, or to execute our corporate CLA (which is required if your contribution is on behalf of a company), drop us an email at opensource@newrelic.com.

**A note about vulnerabilities**

As noted in our [security policy](https://github.com/newrelic/newrelic-unreal-plugin/security/policy), New Relic is committed to the privacy and security of our customers and their data. We believe that providing coordinated disclosure by security researchers and engaging with the security community are important means to achieve our security goals.

If you believe you have found a security vulnerability in this project or any of New Relic's products or websites, we welcome and greatly appreciate you reporting it to New Relic through [our bug bounty program](https://docs.newrelic.com/docs/security/security-privacy/information-security/report-security-vulnerabilities/).

If you would like to contribute to this project, review [these guidelines](./CONTRIBUTING.md).

## Support

New Relic hosts and moderates an online forum where customers, users, maintainers, contributors, and New Relic employees can discuss and collaborate:

[forum.newrelic.com](https://forum.newrelic.com/).

## License

This project is distributed under the [Apache 2 license](LICENSE).
>newrelic-unreal-plugin also uses source code from third-party libraries. You can find full details on which libraries are used and the terms under which they are licensed in the third-party notices document.


