#include "board_init.h"

#include "lvgl.h"
#include "port/lv_port_indev.h"
#include "common_data.h"

#include "LVGL_thread.h"
#include "touch_GT911.h"

static void glcdc_flush_finish_event(lv_event_t * event);

static void touch_init(void)
{
    fsp_err_t err;

    /* Need to initialise the Touch Controller before the LCD, as only a Single Reset line shared between them */
    err = R_IIC_MASTER_Open(&g_i2c_master1_ctrl, &g_i2c_master1_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0); //TODO: Better error handling
    }

    err = R_ICU_ExternalIrqOpen(&g_external_irq3_ctrl, &g_external_irq3_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0); //TODO: Better error handling
    }

    err = init_ts(&g_i2c_master1_ctrl);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0); //TODO: Better error handling
    }

    err = enable_ts(&g_i2c_master1_ctrl, &g_external_irq3_ctrl);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0); //TODO: Better error handling
    }
}

void board_init(void)
{
    /* Need to initialise the Touch Controller before the LCD, as only a Single Reset line shared between them */
    touch_init();

    fsp_err_t err;
    err = RM_LVGL_PORT_Open(&g_lvgl_port_ctrl, &g_lvgl_port_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    lv_display_add_event_cb(g_lvgl_port_ctrl.p_lv_display, glcdc_flush_finish_event, LV_EVENT_FLUSH_FINISH, NULL);

    lv_display_set_default(g_lvgl_port_ctrl.p_lv_display);

    lv_port_indev_init();
}

static void glcdc_flush_finish_event(lv_event_t * event)
{
    FSP_PARAMETER_NOT_USED(event);
    lv_display_t * disp;

    if (LV_EVENT_FLUSH_FINISH == lv_event_get_code(event))
    {
        /* Enable Backlight */
        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_BLEN, BSP_IO_LEVEL_HIGH);

        disp = lv_event_get_target(event);

        /* now the backlight in enabled, remove the event callback */
        lv_display_remove_event_cb_with_user_data(disp, glcdc_flush_finish_event, NULL);
    }
}
