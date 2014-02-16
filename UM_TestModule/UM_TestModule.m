#import <Foundation/Foundation.h>

#include "UM_MissionController.h"
#include "UM_Trajectory.h"
#include "UM_SensorDataCollection.h"

int main(int argc, const char * argv[])
{
    
    @autoreleasepool {
        
        NSString *path = @"input.dat";
        NSString *data = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
        
        while ([data ]) {
            <#statements#>
        }
        
        
    
    }
}