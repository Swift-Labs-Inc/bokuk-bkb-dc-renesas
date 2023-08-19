#include <function.h>


typedef enum
{
  SWITCH_ACITVE_LOW = 0,
  SWITCH_ACTIVE_HIGH
}switch_active_level;

typedef enum
{
  SWITCH_RELEASED = 0,
  SWITCH_PUSHED
}switch_operation_status;

typedef enum
{
  SWITCH_INPUT_DEREPEAT_DISABLED = 0,
  SWITCH_INPUT_DEREPEAT_ENABLED
}switch_derepeat_function_select;

typedef enum
{
  SWITCH_INPUT_DETECTION_DISABLED = 0,
  SWITCH_INPUT_DETECTION_ENABLED
}switch_derepeat_status;

typedef enum
{
  SWITCH_LONG_INPUT_DISABLED = 0,
  SWITCH_LONG_INPUT_ENABLED
}switch_long_input_status;


#define SWITCH_PWR_INPUT_READ                (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_PWR, BSP_IO_LEVEL_HIGH);
#define SWITCH_SELECT_INPUT_READ             (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_SELECT, BSP_IO_LEVEL_HIGH);
#define SWITCH_TEMPUP_READ                   (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_TEMPUP, BSP_IO_LEVEL_HIGH);
#define SWITCH_TEMPDN_READ                   (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_TEMPDN, BSP_IO_LEVEL_HIGH);
#define SWITCH_TIME1_READ                    (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_TEMPUP, BSP_IO_LEVEL_HIGH);
#define SWITCH_TIME2_READ                    (switch_operation_status)R_IOPORT_PinRead(&g_ioport_ctrl, SW_TEMPDN, BSP_IO_LEVEL_HIGH);


struct struct_switch{
  switch_active_level Flag_Switch_Active_Level;                         // SWITCH_ACTIVE_LOW : Pull-Up | SWITCH_ACITVE_HIGH : Pull-Down
  uint8_t Val_Switch_Push_Dechattering_Time;                            // Unit : ms
  uint8_t Val_Switch_Release_Dechattering_Time;                         // Unit : ms
  uint8_t Val_Switch_Dechattering_Base_Time;                            // Unit : ms
  switch_derepeat_function_select Flag_Switch_Derepeat_Function_Select; // SWITCH_INPUT_DEREPEAT_DISABLED : Disable unintentional repetitive input | SWITCH_INPUT_DEREPEAT_ENABLED : Enable unintentional repetitive input
  switch_derepeat_status Flag_Switch_Derepeat;                          // SWITCH_INPUT_DETECTION_DISABLED : Switch Input Non-Detection | SWITCH_INPUT_DETECTION_ENABLED : Switch Input Detection
  switch_long_input_status Flag_Switch_Long_Input_Enable;               // SWITCH_LONG_INPUT_DISABLED : Long Key Press Disable | SWITCH_LONG_INPUT_ENABLED : Long key Press Enable
  uint16_t Val_Switch_Long_Input_Delay_Time;                            // Unit : ms
  uint8_t Val_Switch_Long_Input_Repeat_Time;                            // Unit : ms
  switch_operation_status Flag_Switch_Push_Value;                       // SWITCH_RELEASED : Un-Push | SWITCH_PUSHED : Push
};

struct struct_switch Struct_User_Switch;


struct struct_switch Switch_Input_Control(switch_operation_status Flag_Switch_Status, struct struct_switch Struct_User_Switch)
{
    struct struct_switch Struct_Switch_Temp;
    switch_operation_status Flag_Switch_Status_Temp = SWITCH_RELEASED;

     Struct_Switch_Temp = Struct_User_Switch;

     // Floating Protection -> Pull-Up
     if(Struct_Switch_Temp.Flag_Switch_Active_Level == SWITCH_ACITVE_LOW)
     {
       Flag_Switch_Status_Temp = (switch_operation_status)(Flag_Switch_Status ^ SWITCH_PUSHED);
     }
     // Active Input -> Pull-Down
     else if(Struct_Switch_Temp.Flag_Switch_Active_Level == SWITCH_ACTIVE_HIGH)
     {
       Flag_Switch_Status_Temp = Flag_Switch_Status;
     }

     // Switch Push -> Active
     if(Flag_Switch_Status_Temp == SWITCH_PUSHED)
     {
       Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time = 0;
       Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time++;

       if(Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time)
       {
         Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time = Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time;
       }
     }
     // Switch Un-Push -> Non-Active
     else if(Flag_Switch_Status_Temp == SWITCH_RELEASED)
     {
       Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time = 0;
       Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time++;

       if(Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time)
       {
         Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time = Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time;
       }
     }

     // Switch Push -> Chattering Protection & DEREPEAT_DISABLED
     if((Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time) && (Struct_Switch_Temp.Flag_Switch_Derepeat == SWITCH_INPUT_DETECTION_ENABLED))
     {
       Struct_Switch_Temp.Flag_Switch_Derepeat = SWITCH_INPUT_DETECTION_DISABLED;

       Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_PUSHED;
     }
     // Switch Un-Push -> Chattering Protection & DEREPEAT_DISABLED
     else if((Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time) && (Struct_Switch_Temp.Flag_Switch_Derepeat == SWITCH_INPUT_DETECTION_DISABLED))
     {
       Struct_Switch_Temp.Flag_Switch_Derepeat = SWITCH_INPUT_DETECTION_ENABLED;

       Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_RELEASED;
     }
     else
     {
       if(Struct_Switch_Temp.Flag_Switch_Derepeat_Function_Select == SWITCH_INPUT_DEREPEAT_ENABLED)
       {
         Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_RELEASED;
       }
     }

     // Switch Long Key Press DEREPEAT_DISABLED
     if(Struct_Switch_Temp.Flag_Switch_Long_Input_Enable == SWITCH_LONG_INPUT_ENABLED)
     {
       if((Flag_Switch_Status_Temp == SWITCH_PUSHED) && (Struct_Switch_Temp.Val_Switch_Push_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time))
       {
         if(Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time == 0)
         {
           Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_PUSHED;
         }
         else if(Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time == 400)
         {
           Struct_Switch_Temp.Val_Switch_Long_Input_Repeat_Time++;

           if(Struct_Switch_Temp.Val_Switch_Long_Input_Repeat_Time >= 100)
           {
             Struct_Switch_Temp.Val_Switch_Long_Input_Repeat_Time = 0;

             Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_PUSHED;
           }
           else
           {
             Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_RELEASED;
           }
         }
         else
         {
           Struct_Switch_Temp.Flag_Switch_Push_Value = SWITCH_RELEASED;
         }

         Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time++;

         if(Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time >= 400)
         {
           Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time = 400;
         }
       }
       else if((Flag_Switch_Status_Temp == SWITCH_RELEASED) && (Struct_Switch_Temp.Val_Switch_Release_Dechattering_Time >= Struct_Switch_Temp.Val_Switch_Dechattering_Base_Time))
       {
         Struct_Switch_Temp.Val_Switch_Long_Input_Delay_Time = 0;
         Struct_Switch_Temp.Val_Switch_Long_Input_Repeat_Time = 0;
       }
     }

     return Struct_Switch_Temp;
}
