#include "board_init.h"

#include "lvgl.h"
#include "port/lv_port_indev.h"
#include "common_data.h"

#include "LVGL_thread.h"
#include "touch_GT911.h"

/* The FSP LVGL pack ships lv_renesas_glcdc.h without its implementation, so the
 * display comes from the rm_lvgl_port middleware instead. */

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

    fsp_err_t err = RM_LVGL_PORT_Open(&g_lvgl_port_ctrl, &g_lvgl_port_cfg);
    if(FSP_SUCCESS != err)
    {
        __BKPT(0); //TODO: Better error handling
    }

    lv_display_set_default(g_lvgl_port_ctrl.p_lv_display);

    /* Enable the backlight */
    R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_BLEN, BSP_IO_LEVEL_HIGH);

    lv_port_indev_init();
}
