#ifndef GYRO_ANGLE_H
#define GYRO_ANGLE_H

bool GYRO_init(void);
void GYRO_get_rotation(int16_t* xyz_buffer);

#endif