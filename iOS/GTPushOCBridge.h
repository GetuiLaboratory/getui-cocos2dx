//
//  GTPushOCBridge.h
//  HelloWorld
//
//  Created by  joy on 2022/12/15.
//

#import <Foundation/Foundation.h>
#import <GTSDK/GeTuiSdk.h>

@interface GTPushOCBridge: NSObject <GeTuiSdkDelegate>

+ (instancetype)sharedInstance;

@end
