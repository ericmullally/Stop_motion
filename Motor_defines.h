#ifndef __MOTOR_DEFINES_H
#define __MOTOR_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"

// ABS_POS Register
typedef union {
    uint32_t absolute_position;
    struct {
        uint32_t POS_0_21 : 22;
        uint32_t reserved : 10;
    };
} abs_pos_t;

// EL_POS Register
typedef union {
    uint16_t electrical_position;
    struct {
        uint16_t MICROSTEP : 7;
        uint16_t STEP      : 2;
        uint16_t reserved  : 7;
    };
} el_pos_t;

// MARK Register
typedef union {
    uint32_t mark_position;
    struct {
        uint32_t MARK_0_21 : 22;
        uint32_t reserved  : 10;
    };
} mark_t;

// SPEED Register
typedef union{
	uint32_t speed_register;
	struct{
		uint32_t speed : 20;
		uint32_t reserved : 12;
	};
}speed_t;

// ACC Register
typedef union{
	uint16_t acceleration_regester;
	struct{
		uint16_t acceleration : 12;
		uint16_t reserved : 4;
	};
}acceleration_t;

// DEC Register
typedef union{
	uint16_t decleratioon_regester;
	struct{
		uint16_t deceleration : 12;
		uint16_t reserved : 4;
	};
}deceleration_t;

// MAX_SPEED  Register
typedef union{
	uint16_t max_speed_register;
	struct{
		uint16_t max_speed : 10;
		uint16_t reserved  : 6;
	};
}max_speed_t;

// MIN_SPEED  Register
typedef union{
	uint16_t min_speed_register;
	struct{
		uint16_t min_speed : 12;
		uint16_t lspd_opt  : 1;
		uint16_t reserved  : 3;
	};
}min_speed_t;

// ADC_OUT Register
typedef union {
    uint8_t adc_value;
    struct {
        uint8_t adc_out     : 5;
        uint8_t reserved    : 3;
    };
} adc_out_t;

// OCD_TH Register
typedef union {
    uint8_t ocd_th;
    struct {
        uint8_t ocd_threshold : 5;
        uint8_t reserved   : 3;
    };
} ocd_th_t;

// FS_SPD Register
typedef union {
    uint16_t fs_spd;
    struct {
        uint16_t fs_speed : 10;
        uint16_t BOOST_MODE : 1;
        uint16_t reserved   : 5;
    };
} fs_spd_t;

// STEP_MODE Register
typedef union {
    uint8_t step_mode;
    struct {
        uint8_t STEP_SEL : 3;  // Bits 0-2
        uint8_t CM_VM    : 1;  // Bit 3
        uint8_t SYNC_SEL : 3;  // Bits 4-6
        uint8_t SYNC_EN  : 1;  // Bit 7
    };
} step_mode_t;

// ALARM_EN Register
typedef union {
    uint8_t alarm_enable;
    struct {
        uint8_t ALARM_OVERCURRENT   : 1;
        uint8_t ALARM_THERMAL_SD    : 1;
        uint8_t ALARM_THERMAL_WRN   : 1;
        uint8_t ALARM_UVLO          : 1;
        uint8_t ALARM_ADC_UVLO      : 1;
        uint8_t ALARM_STALL_A       : 1;
        uint8_t ALARM_STALL_B       : 1;
        uint8_t ALARM_CMD_ERR       : 1;
    };
} alarm_en_t;

// GATECFG1
typedef union{
	uint16_t gatecfg1;
	struct{
		uint16_t tcc      :5;
		uint16_t igate    :3;
		uint16_t tboost   :3;
		uint16_t wd_en    :1;
		uint16_t reserved :4;
	};
}gatecfg1_t;

// GATECFG2
typedef union{
	uint8_t gatecfg2;
	struct{
		uint8_t tdt      :5;
		uint8_t tblank   :3;
	};
}gatecfg2_t;

// STATUS Register
typedef union {
    uint16_t response;
    struct {
        uint16_t HIZ          : 1;
        uint16_t BUSY         : 1;
        uint16_t SW_F         : 1;
        uint16_t SW_EVN       : 1;
        uint16_t DIR          : 1;
        uint16_t MOT_STATUS   : 2;
        uint16_t CMD_ERROR    : 1;
        uint16_t STCK_MOD     : 1;
        uint16_t UVLO         : 1;
        uint16_t UVLO_ADC     : 1;
        uint16_t TH_STATUS    : 2;
        uint16_t OCD          : 1;
        uint16_t STALL_B      : 1;
        uint16_t STALL_A      : 1;
    };
} status_t;

// CONFIG Register
typedef union {
    uint16_t configuration;
    struct {
        uint16_t OSC_SEL      : 3;  // Bits 0-2
        uint16_t EXT_CLK      : 1;  // Bit 3
        uint16_t SW_MODE      : 1;  // Bit 4
        uint16_t EN_TQREG     : 1;  // Bit 5 (current mode) / EN_VSCOMP (voltage mode)
        uint16_t PRED_EN      : 1;  // Bit 6 (current mode) / F_PWM_DEC (voltage mode)
        uint16_t OC_SD        : 1;  // Bit 7
        uint16_t VCCVAL       : 1;  // Bit 8
        uint16_t UVLOVAL      : 1;  // Bit 9
        uint16_t F_PWM_INT    : 3;  // Bits 10-12 (voltage mode) / TSW (5 bits) (current mode)
    };
} config_t;

// TVAL Registers ( used for HOLD, RUN, ACC, DEC)
typedef union {
    uint8_t target_current;
    struct {
        uint8_t TVAL_0_6 : 7;
        uint8_t reserved : 1;
    };
} t_val_t;

// KVAL Registers ( used for HOLD, RUN, ACC, DEC)
typedef union {
    uint8_t kval;
} k_val_t;

// TFAST Register
typedef union {
    uint8_t decay_time;
    struct {
        uint8_t TOFF_FAST : 4;
        uint8_t FALL_STEP : 4;
    };
} t_fast_t;

// TON_MIN Register
typedef union {
    uint8_t min_on_time;
    struct {
        uint8_t TON_MIN_0_6 : 7;
        uint8_t reserved    : 1;
    };
} ton_min_t;

// TOFF_MIN Register
typedef union {
    uint8_t min_off_time;
    struct {
        uint8_t TOFF_MIN_0_6 : 7;
        uint8_t reserved     : 1;
    };
} toff_min_t;

// INT_SPEED Register
typedef union{
	uint16_t intersect_speed;
	struct{
		uint16_t int_speed : 14;
		uint16_t reserved : 2;
	};
};

// ST_SLP Register
typedef uint8_t st_slope_t;

// FN_SLP_ACC Register
typedef uint8_t final_slope_acc_t;

// FN_SLP_DEC Register
typedef uint8_t final_slope_dec_t;

// K_THERM Register
typedef union{
	uint8_t k_thermal;
	struct{
		uint8_t comp_factor :4;
		uint8_t reserved    :4;
	};
}k_thermal_t;

// STALL_TH Register
typedef union{
	uint8_t stall_th;
	struct{
		uint8_t stall_val : 5;
		uint8_t reserved  : 3;
	};
};

// TFAST_ Register
typedef union{
	uint8_t t_fast;
	struct{
		uint8_t fast_step : 4;
		uint8_t toff_fast : 4;
	};
}t_fast_t;

// TON_MIN
typedef union{
	uint8_t ton_min;
	struct{
		uint8_t min_on_time : 7;
		uint8_t reserved	: 1;
	};
}ton_min_t;

// TOFF_MIN
typedef union{
	uint8_t ton_min;
	struct{
		uint8_t min_off_time : 7;
		uint8_t reserved	: 1;
	};
}toff_min_t;


#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CONTROL_H */
