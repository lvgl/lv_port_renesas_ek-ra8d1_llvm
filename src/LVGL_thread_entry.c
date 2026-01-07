#include <LVGL_thread.h>
#include "board_init.h"
#include "demos/lv_demos.h"

static uint32_t tick_us100;


void vApplicationMallocFailedHook( void )
{
    __BKPT(0);
}

uint32_t get_idle_percent(void)
{
#if LV_USE_OS == LV_OS_FREERTOS
    return lv_os_get_idle_percent();
#else
    return lv_timer_get_idle();
#endif
}

/* New Thread entry function */
/* pvParameters contains TaskHandle_t */
void LVGL_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t err;

    lv_init();

    board_init();

#if (1 == LV_USE_DEMO_BENCHMARK)
    lv_demo_benchmark();
#endif

#if (1 == LV_USE_DEMO_MUSIC)
    lv_demo_music();
#endif

#if (1 == LV_USE_DEMO_KEYPAD_AND_ENCODER)
    lv_demo_keypad_encoder();

#endif

#if (1 == LV_USE_DEMO_STRESS)
    lv_demo_stress();
#endif

#if (1 == LV_USE_DEMO_WIDGETS && 0 == LV_USE_DEMO_BENCHMARK)
    lv_demo_widgets();
#endif


    /* TODO: add your own code here */
    while (1)
    {
        lv_timer_handler();
        vTaskDelay (1);
    }
}
