#include <LVGL_thread.h>
#include "board_init.h"
#include "lvgl/demos/lv_demos.h"


static uint32_t idle_time_sum;
static uint32_t non_idle_time_sum;
static uint32_t task_switch_timestamp;
static bool idle_task_running;

//char *strdup(const char *src)
//{
//    char *str;
//    char *p;
//    int len = 0;
//
//    while (src[len])
//        len++;
//    str = malloc(len + 1);
//    p = str;
//    while (*src)
//        *p++ = *src++;
//    *p = '\0';
//    return str;
//}
//int toLower(int chr)//touches only one character per call
//{
//    return (chr >='A' && chr<='Z') ? (chr + 32) : (chr);
//}
//
//int strcasecmp (const char *s1, const char *s2)
//{
//  const unsigned char *p1 = (const unsigned char *) s1;
//  const unsigned char *p2 = (const unsigned char *) s2;
//  int result;
//  if (p1 == p2)
//    return 0;
//  while ((result = toLower (*p1) - toLower (*p2++)) == 0)
//    if (*p1++ == '\0')
//      break;
//  return result;
//}

void lv_freertos_task_switch_in(const char * name)
{
    if(strcmp(name, "IDLE")) idle_task_running = false;
    else idle_task_running = true;

    task_switch_timestamp = lv_tick_get();
}

void lv_freertos_task_switch_out(void)
{
    uint32_t elaps = lv_tick_elaps(task_switch_timestamp);
    if(idle_task_running) idle_time_sum += elaps;
    else non_idle_time_sum += elaps;
}

uint32_t lv_os_get_idle_percent(void)
{
    if(non_idle_time_sum + idle_time_sum == 0) {
        LV_LOG_WARN("Not enough time elapsed to provide idle percentage");
        return 0;
    }

    uint32_t pct = (idle_time_sum * 100) / (idle_time_sum + non_idle_time_sum);

    non_idle_time_sum = 0;
    idle_time_sum = 0;

    return pct;
}


void timer_tick_callback(timer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    lv_tick_inc(1);
}

void vApplicationMallocFailedHook( void )
{
    __BKPT(0);
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


    err = R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    err = R_GPT_Start(&g_timer0_ctrl);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    /* TODO: add your own code here */
    while (1)
    {
        lv_timer_handler();
        vTaskDelay (1);
    }
}
