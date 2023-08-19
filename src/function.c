/*
 * function.c
 *
 *  Created on: Aug 7, 2023
 *      Author: alex
 */

#include "function.h"


/*


// local variables
volatile bool b_ready_to_read = false;
static bool g_window_comp_event = false;
static uint16_t g_prev_adc_data;
static uint16_t g_adc_data;
char volt_str[5] = {RESET_VALUE};
float adc_volt = {RESET_VALUE};




volatile uint8_t uart_rec_buffer[];
volatile uint8_t uart_rec_buffer_index = 0;
volatile uint8_t uart_byte_rec_flag = 0;
volatile uint8_t uart_packet_rec_flag = 0;
volatile uint8_t uart_tx_complete_flag = 0;


//------------------------------------------------------------------------------------------------
//      handle error
//------------------------------------------------------------------------------------------------

// ADC handle error
const char* adc_open_err =          "R_ADC_Open API failed\r\n";
const char* adc_close_err =         "R_ADC_Close API failed\r\n";
const char* adc_start_err =         "R_ADC_Start calibration function failed\r\n";
const char* adc_stop_err =          "R_ADC_Stop API failed\r\n";
const char* adc_scan_cfg_err =      "R_ADC_ScanCfg API failed\r\n";
const char* adc_scan_start_err =    "R_ADC_Scan Start API failed\r\n";
const char* adc_scan_stop_err =     "R_ADC_ScanStop API failed\r\n";
const char* adc_read_err =          "R_ADC_Read API failed\r\n";
const char* adc_calibration_err =   "R_ADC Calibration failed\r\n";
const char* adc_status_get_err =    "R_ADC_StatusGet API failed\r\n";

// TIMER handle error
const char* gpt_open_err =          "R_GPT_Open API failed\r\n";
const char* gpt_close_err =         "R_GPT_Close API failed\r\n";
const char* gpt_start_err =         "R_GPT_Start API failed\r\n";
const char* gpt_stop_err =          "R_GPT_Stop API failed\r\n";

// UART handle error







//------------------------------------------------------------------------------------------------
//      UART9(Debug) -> handle error message
//------------------------------------------------------------------------------------------------


void debug_uart_callback(uart_callback_args_t *p_args);



void debug_uart_callback(uart_callback_args_t *p_args)
{



}




//------------------------------------------------------------------------------------------------
//      ADC
//------------------------------------------------------------------------------------------------
static fsp_err_t adc_scan_start(void);
static fsp_err_t adc_scan_stop(void);
static fsp_err_t adc_start_calibration(void);


fsp_err_t adc_read_data(void);
fsp_err_t deinit_adc_module(void);
fsp_err_t adc_callback(adc_callback_args_t * p_args);




static fsp_err_t adc_scan_start(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status
    g_window_comp_event = false;

    if (false == b_ready_to_read)
    {
        // Open/Initialize ADC module
        err = R_ADC_Open (&g_adc_ctrl, &g_adc_cfg);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message //
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_open_err,  strlen(adc_open_err));
            return err;
        }


        //Configures the ADC scan parameters
        err = R_ADC_ScanCfg (&g_adc_ctrl, &g_adc_channel_cfg);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_scan_cfg_err,  strlen(adc_scan_cfg_err));
            return err;
        }

        // Start the ADC scan
        err = R_ADC_ScanStart (&g_adc_ctrl);
        // handle error //
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_scan_start_err,  strlen(adc_scan_start_err));
            return err;
        }

        // Disable interrupts
        R_BSP_IrqDisable((IRQn_Type)ADC_EVENT_SCAN_COMPLETE);

        // Indication to start reading the adc data
        b_ready_to_read = true;
    }
    else
    {
        NOP();
    }

    return err;
}


static fsp_err_t adc_scan_stop(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    // Stop the scan if adc scan is started in continuous scan mode else ignore
    if((ADC_MODE_SINGLE_SCAN != g_adc_cfg.mode) && (true == b_ready_to_read ))
    {
        // Stop ADC scan
        err = R_ADC_ScanStop (&g_adc_ctrl);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_scan_stop_err,  strlen(adc_scan_stop_err));
            return err;
        }


        // reset to indicate stop reading the adc data
        b_ready_to_read = false;

        // Close the ADC module
        err = R_ADC_Close (&g_adc_ctrl);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_close_err,  strlen(adc_close_err));
            return err;
        }
    }
    else
    {
        NOP();
    }

    return err;
}

fsp_err_t adc_read_data(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    // Read the result
    err = R_ADC_Read (&g_adc_ctrl, ADC_CHANNEL_0, &g_adc_data);
    // handle error
    if (FSP_SUCCESS != err)
    {
        // ADC Failure message
        R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_read_err,  strlen(adc_read_err));
        return err;
    }


#ifdef adc_volt_setup
    {
        adc_volt = (float)((g_adc_data * V_ref)/ADC_16_BIT);
    }
#else
    {
        adc_volt = (float)((g_adc_data * V_ref)/ADC_12_BIT);
    }
#endif


    // Voltage Reading from ADC
    R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) g_adc_data,  strlen(g_adc_data));
    // input voltage
    R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) volt_str,  strlen(volt_str));


    // In adc single scan mode after reading the data, it stops.So reset the b_ready_to_read state to
        avoid reading unnecessarily. close the adc module as it gets opened in start scan command.
    if (ADC_MODE_SINGLE_SCAN == g_adc_cfg.mode || g_window_comp_event == true)
    {
        b_ready_to_read = false;

        // Stop ADC scan
        err = R_ADC_ScanStop (&g_adc_ctrl);
        // Handle error
        if (FSP_SUCCESS != err)
        {   // ADC ScanStop message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_scan_stop_err,  strlen(adc_scan_stop_err));
        }



        // Close the ADC module
        err = R_ADC_Close (&g_adc_ctrl);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_close_err,  strlen(adc_close_err));
            return err;
        }
    }


    // 1 Seconds Wait time between successive readings
    R_BSP_SoftwareDelay (ADC_READ_DELAY, BSP_DELAY_UNITS_SECONDS);
    return err;
}


void deinit_adc_module(void)
{
    fsp_err_t err = FSP_SUCCESS;

    // close the ADC driver
    err = R_ADC_Close (&g_adc_ctrl);

    // handle error
    if (FSP_SUCCESS != err)
    {
        // adc Close failure message
        R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_close_err,  strlen(adc_close_err));
    }
}


static fsp_err_t adc_start_calibration(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status
    adc_status_t adc_status;         // to get adc status

    // Initiate ADC calibration
    err = R_ADC_Calibrate (&g_adc_ctrl, NULL);
    // handle error
    if (FSP_SUCCESS != err)
    {
        // ADC Failure message
        R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_calibration_err,  strlen(adc_calibration_err));
        return err;
    }

    do
    {
        // To get the adc status
        err = R_ADC_StatusGet (&g_adc_ctrl, &adc_status);
        // handle error
        if (FSP_SUCCESS != err)
        {
            // ADC status Failure message
            R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_status_get_err,  strlen(adc_status_get_err));
            return err;
        }
    } while (ADC_STATE_IDLE != adc_status.state); //wait here till the calibration.It takes 24msec to 780msec based on clock

    NOP();

    return err;
}

#ifdef board_ra2a1
static fsp_err_t adc_deviation_in_output(void)
{
    static uint8_t count = RESET_VALUE;
    fsp_err_t err = FSP_SUCCESS;     // Error status

    if (RESET_VALUE != g_prev_adc_data)
    {
        if (((g_prev_adc_data > g_adc_data) && (g_prev_adc_data - g_adc_data > TEMPERATURE_DEVIATION_LIMIT))
                || ((g_prev_adc_data < g_adc_data) && (g_adc_data - g_prev_adc_data > TEMPERATURE_DEVIATION_LIMIT)))
        {
            // Increment deviation count
            count++;
            if (TEMPERATURE_DEVIATION_LIMIT < count)
            {
                // Reset the error count
                count = RESET_VALUE;

                // Stop the scan to initiate calibration
                err = adc_scan_stop ();
                // handle error
                if (FSP_SUCCESS != err)
                {
                    // ADC Failure message
                    R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_scan_stop_err,  strlen(adc_scan_stop_err));
                    return err;
                }

                // Calibrate the ADC again
                err = adc_start_calibration ();
                // handle error
                if (FSP_SUCCESS != err)
                {
                    // ADC Failure message
                    R_SCI_UART_Write(g_uart9_ctrl, (uint8_t *) adc_calibration_err,  strlen(adc_calibration_err));
                    return err;
                }

                // clear the current reading
                g_adc_data = RESET_VALUE;
            }
        }
        else
        {
            count = RESET_VALUE;
        }
    }
    return err;
}
#endif



// Callback procedure for when window A compare event occurs
void adc_callback(adc_callback_args_t * p_args)
{
    if(ADC_EVENT_WINDOW_COMPARE_A == p_args->event)
    {
        g_window_comp_event = true;
        IRQn_Type irq = R_FSP_CurrentIrqGet();
        R_BSP_IrqDisable(irq);
    }

}

*/
