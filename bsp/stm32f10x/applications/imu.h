#ifndef __IMU_H__
#define __IMU_H__

unsigned char IMU_Update(float ax,float ay,float az,float gx,float gy,float gz);
void get_euler(float *roll,float *pitch,float *yaw);

#endif

