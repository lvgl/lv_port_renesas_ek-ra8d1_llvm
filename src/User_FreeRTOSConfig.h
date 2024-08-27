#ifndef USER_FREERTOSCONFIG_H_
#define USER_FREERTOSCONFIG_H_

#define configUSE_TRACE_FACILITY 1

#include "lvgl/src/lv_conf_internal.h"
#if LV_USE_OS == LV_OS_FREERTOS
    void lv_freertos_task_switch_in(const char * name);
    void lv_freertos_task_switch_out(void);

    #define traceTASK_SWITCHED_IN()   lv_freertos_task_switch_in(pxCurrentTCB->pcTaskName);
    #define traceTASK_SWITCHED_OUT()  lv_freertos_task_switch_out();
#endif


#define configUSE_RECURSIVE_MUTEXES (1)

#endif /* USER_FREERTOSCONFIG_H_ */
