#include <LVGL_thread.h>

/* picolibc's malloc lock hooks take no argument, unlike newlib's. */

void __malloc_lock(void);
void __malloc_unlock(void);

void __malloc_lock(void)
{
    bool insideAnISR = xPortIsInsideInterrupt();
    configASSERT( !insideAnISR ); // Make damn sure no more mallocs inside ISRs!!
    vTaskSuspendAll();
}

void __malloc_unlock(void)
{
    (void)xTaskResumeAll();
}
