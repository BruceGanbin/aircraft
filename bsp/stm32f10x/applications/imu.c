#include "imu.h"
#include "math.h"

float q0=1,q1=0,q2=0,q3=0;
float exInt=0,eyInt=0,ezInt=0;

float imu_pitch=0,imu_roll=0,imu_yaw=0;

float B1_quad_Kp=0;
float B1_quad_Ki=0;
float B1_quad_halfT=0;
float B1_gyro_scale=0;

unsigned char IMU_Update(float ax,float ay,float az,float gx,float gy,float gz)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    //normalise the measurements
    norm = sqrt(ax*ax + ay*ay + az*az);
    if(norm==0)
    {
        return 0;
    }
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    //estimated directon of gravity
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    //error is sum of cross product between reference direction of field and direction measured by sensor
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    //integral error scaled integral gain
    exInt = exInt + ex * B1_quad_Ki;
    eyInt = eyInt + ey * B1_quad_Ki;
    ezInt = ezInt + ez * B1_quad_Ki;

    //adjusted groscope measurements
    gx = gx + B1_quad_Kp * ex + exInt;
    gy = gy + B1_quad_Kp * ey + eyInt;
    gz = gz + B1_quad_Kp * ez + ezInt;

    //integrate quaternion rate and noralise
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)* B1_quad_halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy) * B1_quad_halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx) * B1_quad_halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx) * B1_quad_halfT;

    //noralise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    if(norm==0)
    {
        return 0;
    }
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    //Euler angles
    imu_roll = asin(-2*q1q3 + 2*q0q2)*57.30f;
    imu_pitch = atan2(2*q2q3 + 2*q0q1, -2*q1q1-2*q2q2 + 1)*57.30f;
    imu_yaw = imu_yaw - gz * B1_gyro_scale;

    return 1;
}
