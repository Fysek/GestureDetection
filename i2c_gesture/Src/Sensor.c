#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "Sensor.h"


gesture_data_type gesture_data_;
int gesture_motion_;
int gesture_ud_delta_;
int gesture_lr_delta_;
int gesture_ud_count_;
int gesture_lr_count_;
int gesture_near_count_;
int gesture_far_count_;
int gesture_state_;

void enablePower(I2C_HandleTypeDef hi2c){
	uint8_t temp;
	
	HAL_I2C_Mem_Read(&hi2c, APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&temp,1,10);
	
	temp |= 1;		//set first bit - power on	
	HAL_I2C_Mem_Write(&hi2c, APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&temp,1,10);
}

void disableAll(I2C_HandleTypeDef hi2c){
	uint8_t temp;
	
	temp = 0;
	
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&temp,1,10);
}

void enableGesture(I2C_HandleTypeDef hi2c){
	uint8_t temp;
	
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&temp,1,10);	
	
	temp |= (64 | 4);		//enable gesture engine and proximity engine
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&temp,1,10);
	
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF4,1,&temp,1,10);
	
	temp |= 1; 	//set Gesture Mode bit
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF4,1,&temp,1,10);	
}

void apdsInit(I2C_HandleTypeDef hi2c){
	uint8_t temp;
	
	disableAll(hi2c);
	
	/* Set default values for ambient light and proximity registers */
	temp = DEFAULT_ATIME;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_ATIME, 1,&temp,1,10);
	
	temp = DEFAULT_WTIME;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_WTIME, 1,&temp,1,10);
	
	temp = DEFAULT_PROX_PPULSE;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_PPULSE, 1,&temp,1,10);
	
	temp = DEFAULT_POFFSET_UR;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_POFFSET_UR, 1,&temp,1,10);
	
	temp = DEFAULT_POFFSET_DL;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_POFFSET_DL, 1,&temp,1,10); 
	
	temp = DEFAULT_CONFIG1;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_CONFIG1, 1,&temp,1,10);

	//set default ledDrive
	setLEDdrive(hi2c, DEFAULT_LDRIVE);
	//set default proximity gain
	setProximityGain(hi2c,DEFAULT_PGAIN);
	//set default ambient light gain
	setAmbientLightGain(hi2c, DEFAULT_AGAIN);
	
	temp = DEFAULT_PILT;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_PILT,1,&temp,1,10);

	temp = DEFAULT_PIHT;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_PIHT,1,&temp,1,10);
	
	/*Sets threshold for ambient light interrupts*/
	setLightIntLowThreshold(hi2c,DEFAULT_AILT);
	setLightIntHighThreshold(hi2c,DEFAULT_AIHT);

	temp = DEFAULT_PERS;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_PERS, 1,&temp,1,10);
	
	temp = DEFAULT_CONFIG2;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_CONFIG2, 1,&temp,1,10);
	
	temp = DEFAULT_CONFIG3;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_CONFIG3, 1,&temp,1,10);
    
  /* Set default values for gesture sense registers */	
	temp = DEFAULT_GPENTH;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GPENTH,1,&temp,1,10);

	temp = DEFAULT_GEXTH;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GEXTH,1,&temp,1,10);

	temp = DEFAULT_GCONF1;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF1,1,&temp,1,10);

	setGestureGain(hi2c, DEFAULT_GGAIN);
	setGestureLEDDrive(hi2c,DEFAULT_GLDRIVE);
	setGestureWaitTime(hi2c,DEFAULT_GWTIME);

	temp = DEFAULT_GOFFSET;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GOFFSET_U, 1,&temp,1,10);
	
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GOFFSET_D, 1,&temp,1,10);
	
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GOFFSET_L, 1,&temp,1,10);
	
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GOFFSET_R, 1,&temp,1,10);
	
	temp = DEFAULT_GPULSE;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GPULSE, 1,&temp,1,10);
	
	temp = DEFAULT_GCONF3;
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS, APDS9960_GCONF3, 1,&temp,1,10);
	
	setGestureIntEnable(hi2c,DEFAULT_GIEN);
}

void setLEDdrive(I2C_HandleTypeDef hi2c, uint8_t drive){
	uint8_t val;
	
	/* Read value from CONTROL register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
	
	/* Set bits in register to given value */
	drive &= 3;
	drive = drive << 6;
	val &= 63;
	val |= drive;
	
	/* Write register value back into CONTROL register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
}

void setProximityGain(I2C_HandleTypeDef hi2c,uint8_t drive){
	uint8_t val;
	
	/* Read value from CONTROL register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
	
	/* Set bits in register to given value */
	drive &= 3;
	drive = drive << 2;
	val &= 243;
	val |= drive;
	
	/* Write register value back into CONTROL register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
}

void setAmbientLightGain(I2C_HandleTypeDef hi2c, uint8_t drive){
	uint8_t val;
	
	/* Read value from CONTROL register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
	
  /* Set bits in register to given value */
  drive &= 3;
  val &= 252;
  val |= drive;
	
	/* Write register value back into CONTROL register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_CONTROL,1,&val,1,10);
}

void setLightIntLowThreshold(I2C_HandleTypeDef hi2c, uint16_t threshold){
	uint8_t val_low;
	uint8_t val_high;
	
	/* Break 16-bit threshold into 2 8-bit values */
	val_low = threshold & 0x00FF;
	val_high = (threshold & 0xFF00) >> 8;
	
	/* Write low byte */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_AILTL,1,&val_low,1,10);
	
	/* Write high byte */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_AILTH,1,&val_high,1,10);	
}

void setLightIntHighThreshold(I2C_HandleTypeDef hi2c, uint16_t threshold){
	uint8_t val_low;
	uint8_t val_high;
	
	/* Break 16-bit threshold into 2 8-bit values */
	val_low = threshold & 0x00FF;
	val_high = (threshold & 0xFF00) >> 8;
	
	/* Write low byte */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_AIHTL,1,&val_low,1,10);
	
	/* Write high byte */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_AIHTH,1,&val_high,1,10);	
}

void setGestureGain(I2C_HandleTypeDef hi2c, uint8_t gain){
	uint8_t val;
	
	/* Read value from GCONF2 register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
	
	/* Set bits in register to given value */
	gain &= 3;
	gain = gain << 5;
	val &= 159;
	val |= gain;
	
	/* Write register value back into GCONF2 register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
}

void setGestureLEDDrive(I2C_HandleTypeDef hi2c, uint8_t gain){
	uint8_t val;
	
	/* Read value from GCONF2 register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
	
	/* Set bits in register to given value */
	gain &= 3;
	gain = gain << 3;
	val &= 231;
	val |= gain;
	
	/* Write register value back into GCONF2 register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
}

void setGestureWaitTime(I2C_HandleTypeDef hi2c, uint8_t time){
	uint8_t val;
	
	/* Read value from GCONF2 register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
	
    /* Set bits in register to given value */
    time &= 7;
    val &= 248;
    val |= time;
	
	/* Write register value back into GCONF2 register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF2,1,&val,1,10);
}

void setGestureIntEnable(I2C_HandleTypeDef hi2c, uint8_t enable){
	uint8_t val;
	
	/* Read value from GCONF4 register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF4,1,&val,1,10);
	
	/* Set bits in register to given value */
	enable &= 1;
	enable = enable << 1;
	val &= 253;
	val |= enable;
	
	/* Write register value back into GCONF4 register */
	HAL_I2C_Mem_Write(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GCONF4,1,&val,1,10);
}

uint8_t isGestureAvailable(I2C_HandleTypeDef hi2c){
	uint8_t val;
	
	/* Read value from GSTATUS register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GSTATUS,1,&val,1,10);
	
	/* Shift and mask out GVALID bit */
	val &= APDS9960_GVALID;
	
	/* Return true/false based on GVALID bit */
	if( val == 1) {
			return 1;
	} else {
			return 0;
	}
}

uint8_t getMode(I2C_HandleTypeDef hi2c){
	uint8_t enable_value;
	
	/* Read current ENABLE register */
	HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_ENABLE,1,&enable_value,1,10);
	
	return enable_value;
}

int readGesture(I2C_HandleTypeDef hi2c)		//FIXME:
{
    uint8_t fifo_level = 0;
    uint8_t bytes_read = 0;
    uint8_t fifo_data[128];
    uint8_t gstatus;
    int motion;
    int i;
	
	
    /* Make sure that power and gesture is on and data is valid */
    if( !isGestureAvailable(hi2c) || !(getMode(hi2c) & 65) ) {
        return DIR_NONE;
    }
    
    /* Keep looping as long as gesture data is valid */
    while(1) {
    
        /* Wait some time to collect next batch of FIFO data */
        HAL_Delay(FIFO_PAUSE_TIME);
        
        /* Get the contents of the STATUS register. Is data still valid? */
				HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GSTATUS,1,&gstatus,1,10);
        
        /* If we have valid data, read in FIFO */
        if( (gstatus & APDS9960_GVALID) == APDS9960_GVALID ) {
        
            /* Read the current FIFO level */
						HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GFLVL,1,&fifo_level,1,10);

//#if DEBUG
//            Serial.print("FIFO Level: ");
//            Serial.println(fifo_level);
//#endif

            /* If there's stuff in the FIFO, read it into our data block */
            if( fifo_level > 0) {
							if(HAL_I2C_Mem_Read(&hi2c,APDS9960_I2C_ADDRESS,APDS9960_GFIFO_U,1,fifo_data,(fifo_level*4),10) != HAL_OK)
								return ERROR;
                
//#if DEBUG
//                Serial.print("FIFO Dump: ");
//                for ( i = 0; i < bytes_read; i++ ) {
//                    Serial.print(fifo_data[i]);
//                    Serial.print(" ");
//                }
//                Serial.println();
//#endif

                /* If at least 1 set of data, sort the data into U/D/L/R */
                //if( bytes_read >= 4 ) {
                    for( i = 0; i < (fifo_level*4); i += 4 ) {
                        gesture_data_.u_data[gesture_data_.index] = \
                                                            fifo_data[i + 0];
                        gesture_data_.d_data[gesture_data_.index] = \
                                                            fifo_data[i + 1];
                        gesture_data_.l_data[gesture_data_.index] = \
                                                            fifo_data[i + 2];
                        gesture_data_.r_data[gesture_data_.index] = \
                                                            fifo_data[i + 3];
                        gesture_data_.index++;
                        gesture_data_.total_gestures++;
                    }
                    
//#if DEBUG
//                Serial.print("Up Data: ");
//                for ( i = 0; i < gesture_data_.total_gestures; i++ ) {
//                    Serial.print(gesture_data_.u_data[i]);
//                    Serial.print(" ");
//                }
//                Serial.println();
//#endif

                    /* Filter and process gesture data. Decode near/far state */
                    if( processGestureData() ) {
                        if( decodeGesture() ) {
                            //***TODO: U-Turn Gestures
//#if DEBUG
//                            //Serial.println(gesture_motion_);
//#endif
                        }
                    }
                    
                    /* Reset data */
                    gesture_data_.index = 0;
                    gesture_data_.total_gestures = 0;
                //}
            }
        } else {
    
            /* Determine best guessed gesture and clean up */
            HAL_Delay(FIFO_PAUSE_TIME);
            decodeGesture();
            motion = gesture_motion_;
//#if DEBUG
//            Serial.print("END: ");
//            Serial.println(gesture_motion_);
//#endif
            resetGestureParameters();
            return motion;
        }
    }
}

uint8_t processGestureData(void){
    uint8_t u_first = 0;
    uint8_t d_first = 0;
    uint8_t l_first = 0;
    uint8_t r_first = 0;
    uint8_t u_last = 0;
    uint8_t d_last = 0;
    uint8_t l_last = 0;
    uint8_t r_last = 0;
    int ud_ratio_first;
    int lr_ratio_first;
    int ud_ratio_last;
    int lr_ratio_last;
    int ud_delta;
    int lr_delta;
    int i;

    /* If we have less than 4 total gestures, that's not enough */
    if( gesture_data_.total_gestures <= 4 ) {
        return 0;
    }
    
    /* Check to make sure our data isn't out of bounds */
    if( (gesture_data_.total_gestures <= 32) && \
        (gesture_data_.total_gestures > 0) ) {
        
        /* Find the first value in U/D/L/R above the threshold */
        for( i = 0; i < gesture_data_.total_gestures; i++ ) {
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_first = gesture_data_.u_data[i];
                d_first = gesture_data_.d_data[i];
                l_first = gesture_data_.l_data[i];
                r_first = gesture_data_.r_data[i];
                break;
            }
        }
        
        /* If one of the _first values is 0, then there is no good data */
        if( (u_first == 0) || (d_first == 0) || \
            (l_first == 0) || (r_first == 0) ) {
            
            return 0;
        }
        /* Find the last value in U/D/L/R above the threshold */
        for( i = gesture_data_.total_gestures - 1; i >= 0; i-- ) {
//#if DEBUG
//            Serial.print(F("Finding last: "));
//            Serial.print(F("U:"));
//            Serial.print(gesture_data_.u_data[i]);
//            Serial.print(F(" D:"));
//            Serial.print(gesture_data_.d_data[i]);
//            Serial.print(F(" L:"));
//            Serial.print(gesture_data_.l_data[i]);
//            Serial.print(F(" R:"));
//            Serial.println(gesture_data_.r_data[i]);
//#endif
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_last = gesture_data_.u_data[i];
                d_last = gesture_data_.d_data[i];
                l_last = gesture_data_.l_data[i];
                r_last = gesture_data_.r_data[i];
                break;
            }
        }
    }
    
    /* Calculate the first vs. last ratio of up/down and left/right */
    ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
    lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
    ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
    lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);
       
//#if DEBUG
//    Serial.print(F("Last Values: "));
//    Serial.print(F("U:"));
//    Serial.print(u_last);
//    Serial.print(F(" D:"));
//    Serial.print(d_last);
//    Serial.print(F(" L:"));
//    Serial.print(l_last);
//    Serial.print(F(" R:"));
//    Serial.println(r_last);

//    Serial.print(F("Ratios: "));
//    Serial.print(F("UD Fi: "));
//    Serial.print(ud_ratio_first);
//    Serial.print(F(" UD La: "));
//    Serial.print(ud_ratio_last);
//    Serial.print(F(" LR Fi: "));
//    Serial.print(lr_ratio_first);
//    Serial.print(F(" LR La: "));
//    Serial.println(lr_ratio_last);
//#endif
       
    /* Determine the difference between the first and last ratios */
    ud_delta = ud_ratio_last - ud_ratio_first;
    lr_delta = lr_ratio_last - lr_ratio_first;
    
//#if DEBUG
//    Serial.print("Deltas: ");
//    Serial.print("UD: ");
//    Serial.print(ud_delta);
//    Serial.print(" LR: ");
//    Serial.println(lr_delta);
//#endif

    /* Accumulate the UD and LR delta values */
    gesture_ud_delta_ += ud_delta;
    gesture_lr_delta_ += lr_delta;
    
//#if DEBUG
//    Serial.print("Accumulations: ");
//    Serial.print("UD: ");
//    Serial.print(gesture_ud_delta_);
//    Serial.print(" LR: ");
//    Serial.println(gesture_lr_delta_);
//#endif
    
    /* Determine U/D gesture */
    if( gesture_ud_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = 1;
    } else if( gesture_ud_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = -1;
    } else {
        gesture_ud_count_ = 0;
    }
    
    /* Determine L/R gesture */
    if( gesture_lr_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = 1;
    } else if( gesture_lr_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = -1;
    } else {
        gesture_lr_count_ = 0;
    }
    
    /* Determine Near/Far gesture */
    if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 0) ) {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
            
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            } else if( (ud_delta != 0) || (lr_delta != 0) ) {
                gesture_far_count_++;
            }
            
            if( (gesture_near_count_ >= 10) && (gesture_far_count_ >= 2) ) {
                if( (ud_delta == 0) && (lr_delta == 0) ) {
                    gesture_state_ = NEAR_STATE;
                } else if( (ud_delta != 0) && (lr_delta != 0) ) {
                    gesture_state_ = FAR_STATE;
                }
                return 1;
            }
        }
    } else {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
                
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            }
            
            if( gesture_near_count_ >= 10 ) {
                gesture_ud_count_ = 0;
                gesture_lr_count_ = 0;
                gesture_ud_delta_ = 0;
                gesture_lr_delta_ = 0;
            }
        }
    }
    
//#if DEBUG
//    Serial.print("UD_CT: ");
//    Serial.print(gesture_ud_count_);
//    Serial.print(" LR_CT: ");
//    Serial.print(gesture_lr_count_);
//    Serial.print(" NEAR_CT: ");
//    Serial.print(gesture_near_count_);
//    Serial.print(" FAR_CT: ");
//    Serial.println(gesture_far_count_);
//    Serial.println("----------");
//#endif
    
    return 0;
}

uint8_t decodeGesture()
{
    /* Return if near or far event is detected */
    if( gesture_state_ == NEAR_STATE ) {
        gesture_motion_ = DIR_NEAR;
        return 1;
    } else if ( gesture_state_ == FAR_STATE ) {
        gesture_motion_ = DIR_FAR;
        return 1;
    }
    
    /* Determine swipe direction */
    if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_UP;
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_DOWN;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 1) ) {
        gesture_motion_ = DIR_RIGHT;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == -1) ) {
        gesture_motion_ = DIR_LEFT;
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else {
        return 0;
    }
    
    return 1;
}

void resetGestureParameters(void)
{
    gesture_data_.index = 0;
    gesture_data_.total_gestures = 0;
    
    gesture_ud_delta_ = 0;
    gesture_lr_delta_ = 0;
    
    gesture_ud_count_ = 0;
    gesture_lr_count_ = 0;
    
    gesture_near_count_ = 0;
    gesture_far_count_ = 0;
    
    gesture_state_ = 0;
    gesture_motion_ = DIR_NONE;
}

void handleGesture(I2C_HandleTypeDef hi2c) {
    if ( isGestureAvailable(hi2c) ) {
			BSP_LCD_Clear(LCD_COLOR_WHITE);
			switch ( readGesture(hi2c) ) {
				case DIR_UP:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"UP");
					break;
				case DIR_DOWN:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"DOWN");
					break;
				case DIR_LEFT:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"LEFT");
					break;
				case DIR_RIGHT:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"RIGHT");
					break;
				case DIR_NEAR:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"NEAR");
					break;
				case DIR_FAR:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"FAR");
					break;
				default:
					BSP_LCD_DisplayStringAtLine(3, (uint8_t*)"NONE");
    }
  }
}
