/*
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Code by Andrew Tridgell and Siddharth Bharat Purohit
 */
#pragma once

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_ChibiOS_Namespace.h"
#include "AP_HAL_ChibiOS.h"

class ChibiOS::Util : public AP_HAL::Util {
public:
    static Util *from(AP_HAL::Util *util) {
        return static_cast<Util*>(util);
    }

    bool run_debug_shell(AP_HAL::BetterStream *stream) override { return false; }
    uint32_t available_memory() override;

    // Special Allocation Routines
    void *malloc_type(size_t size, AP_HAL::Util::Memory_Type mem_type) override;
    void free_type(void *ptr, size_t size, AP_HAL::Util::Memory_Type mem_type) override;

    /*
      return state of safety switch, if applicable
     */
    enum safety_state safety_switch_state(void) override;

    // IMU temperature control
    void set_imu_temp(float current) override;
    void set_imu_target_temp(int8_t *target) override;

    // get system ID as a string
    bool get_system_id(char buf[40]) override;
    
#ifdef HAL_PWM_ALARM
    bool toneAlarm_init() override;
    void toneAlarm_set_buzzer_tone(float frequency, float volume, uint32_t duration_ms) override;
#endif

private:
#ifdef HAL_PWM_ALARM
    struct ToneAlarmPwmGroup {
        pwmchannel_t chan;
        PWMConfig pwm_cfg;
        PWMDriver* pwm_drv;
    };

    static ToneAlarmPwmGroup _toneAlarm_pwm_group;
#endif
    void* try_alloc_from_ccm_ram(size_t size);
    uint32_t available_memory_in_ccm_ram(void);

#if HAL_WITH_IO_MCU && HAL_HAVE_IMU_HEATER
    struct {
        int8_t *target;
        float integrator;
        uint16_t count;
        float sum;
        uint32_t last_update_ms;
    } heater;
#endif

    /*
      set HW RTC in UTC microseconds
     */
    void set_hw_rtc(uint64_t time_utc_usec) override;

    /*
      get system clock in UTC microseconds
     */
    uint64_t get_hw_rtc() const override;
#ifndef HAL_NO_FLASH_SUPPORT
    bool flash_bootloader() override;
#endif
};
