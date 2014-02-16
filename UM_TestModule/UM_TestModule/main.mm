//#import <Foundation/Foundation.h>
#include "UM_MissionController.h"
#include "UM_Trajectory.h"
#include "UM_SensorDataCollection.h"
#include "UM_Actions.h"

int main(int argc, const char * argv[])
{
    
    @autoreleasepool {
        NSError *error;
    
        NSFileHandle *file = [NSFileHandle fileHandleForReadingAtPath:@"/Users/ifoundit/Desktop/input.csv"];
        NSString *inputData = [[NSString alloc] initWithData:[file readDataToEndOfFile] encoding:NSASCIIStringEncoding];
        
        if (error || inputData == nil)
        {
            NSLog(@"%@", error);
        }
        
        NSArray *lines = [inputData componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
        NSArray *dataArray;
//        UM_MissionController *missionController = new UM_MissionController();
        UM_SensorDataCollection *sensorData = new UM_SensorDataCollection();
        NSString *result = [[NSString alloc] init];
        
        for (int i = 0; i < ([lines count]); i++) {
            
            dataArray = [[lines objectAtIndex:i] componentsSeparatedByString:@","];
            
            sensorData->T1 = [dataArray[0] floatValue];
            sensorData->T2 = [dataArray[1] floatValue];
            sensorData->T3 = [dataArray[2] floatValue];
            
            sensorData->Ax = [dataArray[3] floatValue];
            sensorData->Ay = [dataArray[4] floatValue];
            sensorData->Az = [dataArray[5] floatValue];
            
            sensorData->Gx = [dataArray[6] floatValue];
            sensorData->Gy = [dataArray[7] floatValue];
            sensorData->Gz = [dataArray[8] floatValue];
            
            sensorData->Z = [dataArray[9] floatValue];
            sensorData->Vel = [dataArray[10] floatValue];
            
            UM_Trajectory newTrajectory = UM_Trajectory();
            UM_Actions *actions = new UM_Actions();
            actions = actions->actionsForTrajectory(newTrajectory.trajectoryFromSensorData(sensorData, NO, YES));
            
            /*
            NSLog(@"Trajectory Calculated. %f, %f, %f", newTrajectory.currentTrajectory->x, newTrajectory.currentTrajectory->y, newTrajectory.currentTrajectory->z);
            
            result = [result stringByAppendingString:@"New Trajectory Data,"];
            [result stringByAppendingString:[[NSNumber numberWithFloat:newTrajectory.currentTrajectory->x] stringValue]];
            NSLog(@"%f", newTrajectory.currentTrajectory->x);
            [result stringByAppendingString:@","];
            [result stringByAppendingString:[[NSNumber numberWithFloat:newTrajectory.currentTrajectory->y] stringValue]];
            NSLog(@"%f", newTrajectory.currentTrajectory->y);
            [result stringByAppendingString:@","];
            [result stringByAppendingString:[[NSNumber numberWithFloat:newTrajectory.currentTrajectory->z] stringValue]];
            NSLog(@"%f", newTrajectory.currentTrajectory->z);
            [result stringByAppendingString:@","];
            [result stringByAppendingString:[[NSNumber numberWithFloat:newTrajectory.currentTrajectory->magnitude] stringValue]];
            [result stringByAppendingString:@","];
            */
            for (int i = 0; i < actions->getLength(); i++)
            {
                NSLog(@"%d", (int)*(actions->getActionAtIndex(i)));
            }
        }
        NSFileHandle *oFile = [NSFileHandle fileHandleForUpdatingAtPath:@"/Users/ifoundit/Desktop/output.csv"];
        if (!oFile) {
            [[NSFileManager defaultManager] createFileAtPath:@"/Users/ifoundit/Desktop/output.csv" contents:[result dataUsingEncoding:NSUTF8StringEncoding] attributes:nil];
        }
        else {
            [oFile writeData:[NSKeyedArchiver archivedDataWithRootObject:result]];
        }
        
        [oFile closeFile];
        [file closeFile];
        NSLog(@"Data written to file.");
            
        result = nil;
        dataArray = nil;

            
        NSLog(@"Finished.");
    }
}