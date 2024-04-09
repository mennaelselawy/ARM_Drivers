/*
 * LCD.c
 *
 *  Created on: Apr 2, 2024
 *      Author: Menna Ashraf
 */

#include "../../../include/LIB/STD_TYPES.h"
#include "../../../include/MCAL/GPIO/GPIO.h"

#include "../../../include/HAL/LCD/LCD_Config.h"
#include "../../../include/HAL/LCD/LCD_Private.h"
#include "../../../include/HAL/LCD/LCD.h"

#define CONCAT_HELP(D7,D6,D5,D4,D3,D2,D1,D0)            0b##D7##D6##D5##D4##D3##D2##D1##D0
#define CONCAT(D7,D6,D5,D4,D3,D2,D1,D0)                 CONCAT_HELP(D7,D6,D5,D4,D3,D2,D1,D0)

#define LCD_TOTAL_PIN_NUMBER     (LCD_DATA_PINS+3)
#define LCD_MAX_BUFFER_SIZE       5
#define LATCH_TARGET_COUNT        2         /* Number of times latch needs to be called */

#define COMMAND_STATE_BUSY        0
#define COMMAND_STATE_READY       1

#define WRITE_COMMAND_STATE       0
#define WRITE_DATA_STATE          1

#define GPIO_STATE_RESET          0
#define GPIO_STATE_SET            1

                                 /* Commands */
#define FunctionSet                 CONCAT(0,0,1,1, LCD_NUMBER_OF_LINES, LCD_FONT_SIZE, 0,0)
#define DisplayControl              CONCAT(0,0,0,0,1, LCD_Display_Mode, LCD_CURSOR_MODE, LCD_BLINK_MODE)
#define DisplayClear                0x01
#define EntryMode                   CONCAT(0,0,0,0,0,1, LCD_INCREMENT_DECREMENT, LCD_SHIFTING)
#define LCD_SET_CURSOR_LINE1        0x80      // DDRAM
#define LCD_SET_CURSOR_LINE2        0xC0      // DDRAM
#define LCD_SET_CGRAM_ADDRESS       0x40      // CGRAM  , Second Line Offset

                                              /* DATA TYPES */
typedef enum{
	Busy,
	Ready
}UserRequestState_t;

typedef enum{
	WRITE_REQUEST,
	CLEAR_REQUEST,
	SETPOSITION_REQUEST,
	NONE_REQUEST
}UserRequestType_t;

typedef struct
{
	const u8 * USER_String;
	u8 User_Length;
	UserRequestState_t User_State;
	UserRequestType_t User_Type;
	u8 CurrentPosition;
	u8 ROW;
	u8 COLUMN;
	CallBack_t CallBack;
}UserRequest_t;

typedef enum
{
	 LCD_InitState,
	 LCD_Operational,
	 LCD_OFF
}LcdState_t;

/*
typedef struct{
	u8 * Data;
	u8 Length;
	u8 CurrentPosition;
	CallBack_t CallBack;
}WriteRequest_t;

typedef struct{
	u8 ROW;
	u8 COLUMN;
	CallBack_t CallBack;
}SetPositionRequest_t;

typedef struct{
	CallBack_t CallBack;
}ClearRequest_t;
*/

// for 8 bit mode
typedef enum
{
	POWER_ON_STATE,
	FUNCTIONAL_SET_STATE,
	DISPLAY_CONTROL_STATE,
	CLEAR_DISPLAY_STATE,
	ENTRY_MODE_STATE,
	END_STATE,
}INIT_STATES_t;


                                    /* STATIC GLOBAL VARIABLE */
extern LCD_Pins_Configurations_t LCD_Pins;

static LcdState_t        GLOBAL_LcdState = LCD_OFF;
static UserRequest_t         GLOBAL_UserRequest[LCD_MAX_BUFFER_SIZE];
/*static WriteRequest_t        GLOBAL_WriteRequest;
static SetPositionRequest_t  GLOBAL_SetPositionRequest;
static ClearRequest_t        GLOBAL_ClearRequest;
*/

//static u8 CurrentIndex = 0;
static u8 USER_CurrentRequest = 0;
static GLOBAL_OperationalLatchCount;

                                       /*STATIC FUNCTIONS*/
static void Init_State_Function(void);
static void WriteCommand(u8 Command, u8 State);
static static void OperationalState_WRITEprocess();
static void OperationalState_CLEARprocess();
static void OperationalState_SETPOSITIONprocess();

//static ();

                                     /* API IMPLEMENTATION*/
COM_ErrorStatus_t LCD_Init()
{
	COM_ErrorStatus_t RET_ErrorStatue = LCD_OK;
	u8 LOC_Index;
	GPIO_strConfig_t LOC_LcdPin;
	LOC_LcdPin.MODE = GPIO_MODE_OUT_PP;
	LOC_LcdPin.SPEED = GPIO_SPEED_MEDIUM;
	LOC_LcdPin.AF = GPIO_AF_SYSTEM;

	for(LOC_Index = 0; LOC_Index < LCD_DATA_PINS; LOC_Index ++)
	{
		LOC_LcdPin.PIN = LCD_Pins.DataPins[LOC_Index].PIN;
		LOC_LcdPin.PORT = LCD_Pins.DataPins[LOC_Index].PORT;
		GPIO_Init(&LOC_LcdPin);
	}
	LOC_LcdPin.PIN = LCD_Pins.R_S_Pin.PIN;
	LOC_LcdPin.PORT = LCD_Pins.R_S_Pin.PORT;
	GPIO_Init(&LOC_LcdPin);
	LOC_LcdPin.PIN = LCD_Pins.R_S_Pin.PIN;
	LOC_LcdPin.PORT = LCD_Pins.R_S_Pin.PORT;
	GPIO_Init(&LOC_LcdPin);
	LOC_LcdPin.PIN = LCD_Pins.R_S_Pin.PIN;
	LOC_LcdPin.PORT = LCD_Pins.R_S_Pin.PORT;
	GPIO_Init(&LOC_LcdPin);

	GLOBAL_LcdState = LCD_InitState;
	return RET_ErrorStatue;
}

COM_ErrorStatus_t LCD_WriteStringAsynchronous(const u8 * String, u8 Length, CallBack_t CB)
{
	COM_ErrorStatus_t RET_ErrorStatue = LCD_NOK;
	u8 LOCAL_Index;
	if(String == NULL)
	{
		RET_ErrorStatue = Error_NULLPointer;
	}
	else if(LOCAL_Index == LCD_MAX_BUFFER_SIZE)
	{
		RET_ErrorStatue = LCD_NOK;
	}
	else
	{
		for(LOCAL_Index =0; LOCAL_Index < LCD_MAX_BUFFER_SIZE; LOCAL_Index++)
			{
				if( (GLOBAL_LcdState == LCD_Operational) && (GLOBAL_UserRequest[LOCAL_Index].User_State == Ready) )
				{
					GLOBAL_UserRequest[LOCAL_Index].USER_String = String;
					GLOBAL_UserRequest[LOCAL_Index].User_Length = Length;
					GLOBAL_UserRequest[LOCAL_Index].CallBack = CB;
					GLOBAL_UserRequest[LOCAL_Index].CurrentPosition = 0;
					GLOBAL_UserRequest[LOCAL_Index].User_Type = WRITE_REQUEST;
					GLOBAL_UserRequest[LOCAL_Index].User_State = Busy;
					break;
				}
				RET_ErrorStatue = LCD_OK;
			}
	}
	return RET_ErrorStatue;
}

COM_ErrorStatus_t LCD_SetCurosrPositionAsynchronous(u8 Position_ROW, u8 Position_COLUMN, CallBack_t CB)
{
	COM_ErrorStatus_t RET_ErrorStatue = LCD_NOK;
	u8 LOCAL_Index;
	for(LOCAL_Index =0; LOCAL_Index < LCD_MAX_BUFFER_SIZE; LOCAL_Index++)
	{
		if( GLOBAL_UserRequest[LOCAL_Index].User_State == Ready )
		{
			GLOBAL_UserRequest[LOCAL_Index].ROW = Position_ROW;
			GLOBAL_UserRequest[LOCAL_Index].COLUMN = Position_COLUMN;
			GLOBAL_UserRequest[LOCAL_Index].CallBack = CB;
			GLOBAL_UserRequest[LOCAL_Index].User_Type = SETPOSITION_REQUEST;
			GLOBAL_UserRequest[LOCAL_Index].User_State = Busy;
			break;
		}
		RET_ErrorStatue = LCD_OK;
	}
	if(LOCAL_Index == LCD_MAX_BUFFER_SIZE)
	{
		RET_ErrorStatue = LCD_NOK;
	}
	else
	{
		RET_ErrorStatue = LCD_OK;
	}
	return RET_ErrorStatue;
}

COM_ErrorStatus_t LCD_ClearScreenAsynchronous(CallBack_t CB)
{
	COM_ErrorStatus_t RET_ErrorStatue = LCD_NOK;
	u8 LOCAL_Index;
	for(LOCAL_Index =0; LOCAL_Index < LCD_MAX_BUFFER_SIZE; LOCAL_Index++)
	{
		if( GLOBAL_UserRequest[LOCAL_Index].User_State == Ready )
		{
			GLOBAL_UserRequest[LOCAL_Index].CallBack = CB;
			GLOBAL_UserRequest[LOCAL_Index].User_Type = CLEAR_REQUEST;
			GLOBAL_UserRequest[LOCAL_Index].User_State = Busy;
			break;
		}
		RET_ErrorStatue = LCD_OK;
	}
	if(LOCAL_Index == LCD_MAX_BUFFER_SIZE)
	{
		RET_ErrorStatue = LCD_NOK;
	}
	else
	{
		RET_ErrorStatue = LCD_OK;
	}
	return RET_ErrorStatue;
}

COM_ErrorStatus_t LCD_GetState(u8 * State)
{
	COM_ErrorStatus_t  RET_ErrorStatue = LCD_NOK;
	if(State == NULL)
	{
		RET_ErrorStatue = Error_NULLPointer;
	}
	else
	{
		*State = GLOBAL_LcdState;
	}
	return RET_ErrorStatue;
}



                     /* //////////////////  STATIC FUNCTIONS IMPLEMENTATION  ///////////////// */
//for 8 bit mode
static void Init_State_Function(void)
{
	static INIT_STATES_t InitState = POWER_ON_STATE;
	static u8 InitCallCount =0;
	switch(InitState)
	{
	case POWER_ON_STATE:
		/*Delay For 30 millisecond */
		InitCallCount += LCD_UPDATE_PERIODICITY_MS;
		if(InitCallCount >= 30)
		{
			InitCallCount = 0;
			InitState = FUNCTIONAL_SET_STATE;
		}
		break;
	case FUNCTIONAL_SET_STATE:
		InitCallCount ++;
		if(InitCallCount == LATCH_TARGET_COUNT)
		{
			InitCallCount = 0;
			InitState = DISPLAY_CONTROL_STATE;
		}
		WriteCommand(FunctionSet, WRITE_COMMAND_STATE);
		break;
	case DISPLAY_CONTROL_STATE:
		InitCallCount ++;
		if(InitCallCount == LATCH_TARGET_COUNT)
		{
			InitCallCount = 0;
			InitState = CLEAR_DISPLAY_STATE;
		}
		WriteCommand(DisplayControl, WRITE_COMMAND_STATE);
		break;
	case CLEAR_DISPLAY_STATE:
		InitCallCount ++;
		if(InitCallCount == LATCH_TARGET_COUNT)
		{
			InitCallCount = 0;
			InitState = ENTRY_MODE_STATE;
		}
		WriteCommand(DisplayClear, WRITE_COMMAND_STATE);
		break;
	case ENTRY_MODE_STATE:
		InitCallCount ++;
		if(InitCallCount == LATCH_TARGET_COUNT)
		{
			InitCallCount = 0;
			InitState = END_STATE;
		}
		WriteCommand(EntryMode, WRITE_COMMAND_STATE);
		break;
	case END_STATE:
		GLOBAL_LcdState = LCD_Operational;
		break;
	default:
	}
}

static void WriteCommand(u8 Command, u8 State)   //if state = 0 means write command , if state = 1 means write data
{
	static u8 LOCAL_CommandState = COMMAND_STATE_READY;
	u8 LOCAL_Index;
	switch(LOCAL_CommandState)
	{
	case COMMAND_STATE_READY:
		GPIO_SetPinValue(LCD_Pins.R_W_Pin.PORT, LCD_Pins.R_W_Pin.PIN , GPIO_STATE_RESET);
		GPIO_SetPinValue(LCD_Pins.R_S_Pin.PORT , LCD_Pins.R_S_Pin.PIN , State);
		for(LOCAL_Index = 0; LOCAL_Index < 8; LOCAL_Index++)
		{
			GPIO_SetPinValue(LCD_Pins.DataPins[LOCAL_Index].PORT , LCD_Pins.DataPins[LOCAL_Index].PIN , (Command >> LOCAL_Index)&1 );
		}
		GPIO_SetPinValue(LCD_Pins.E_Pin.PORT , LCD_Pins.E_Pin.PIN , GPIO_STATE_SET);
		LOCAL_CommandState = COMMAND_STATE_BUSY;
		break;
	case COMMAND_STATE_BUSY:
		GPIO_SetPinValue(LCD_Pins.E_Pin.PORT , LCD_Pins.E_Pin.PIN , GPIO_STATE_RESET);
		LOCAL_CommandState = COMMAND_STATE_READY;
		break;
	default:
		break;
	}
}

static void OperationalState_WRITEprocess()
{
	if(GLOBAL_UserRequest[USER_CurrentRequest].CurrentPosition < GLOBAL_UserRequest[USER_CurrentRequest].User_Length)
	{
		WriteCommand( *(GLOBAL_UserRequest[USER_CurrentRequest].USER_String + GLOBAL_UserRequest[USER_CurrentRequest].CurrentPosition) , WRITE_DATA_STATE);
		GLOBAL_UserRequest[USER_CurrentRequest].CurrentPosition ++;
	}
	else
	{
		GLOBAL_UserRequest[USER_CurrentRequest]. User_State = Ready;
		GLOBAL_UserRequest[USER_CurrentRequest].CurrentPosition = 0;
		USER_CurrentRequest ++;
		 if (GLOBAL_UserRequest[USER_CurrentRequest].User_State == Ready || USER_CurrentRequest == LCD_MAX_BUFFER_SIZE)
		 {
			 USER_CurrentRequest = 0;
		 }
		 if(USER_CurrentRequest && (GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack ) )
		 {
		 	GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack();
		 }
	}
	/*if(GLOBAL_WriteRequest.CurrentPosition < GLOBAL_WriteRequest.Length)
	{
		WriteCommand(GLOBAL_WriteRequest.Data[GLOBAL_WriteRequest.CurrentPosition] , WRITE_DATA_STATE);
		GLOBAL_OperationalLatchCount++;
		if(GLOBAL_OperationalLatchCount == LATCH_TARGET_COUNT)
		{
			GLOBAL_WriteRequest.CurrentPosition++;
			GLOBAL_OperationalLatchCount = 0;
		}
	}
	else
	{
		GLOBAL_UserRequest[GLOBAL_WriteRequest.CurrentPosition].User_State = Ready;
		GLOBAL_UserRequest.User_Type = NONE_REQUEST;
	}*/
}

static void OperationalState_CLEARprocess()
{
	if(GLOBAL_OperationalLatchCount == LATCH_TARGET_COUNT)
	{
		GLOBAL_OperationalLatchCount = 0;
		GLOBAL_UserRequest[USER_CurrentRequest]. User_State = Ready;
		GLOBAL_UserRequest.User_Type = NONE_REQUEST;
	}
	else
	{
		WriteCommand(DisplayClear , WRITE_DATA_STATE);
		GLOBAL_OperationalLatchCount ++;
		if (GLOBAL_UserRequest[USER_CurrentRequest].User_State == Ready || USER_CurrentRequest == LCD_MAX_BUFFER_SIZE)
		{
			USER_CurrentRequest = 0;
		}
		if(USER_CurrentRequest && (GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack ) )
		{
			GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack();
		}
	}
}

static void OperationalState_SETPOSITIONprocess()
{
	u8 LOCAL_Location = LCD_SET_CURSOR_LINE1;
	if(GLOBAL_OperationalLatchCount == LATCH_TARGET_COUNT)
	{
		GLOBAL_OperationalLatchCount = 0;
		GLOBAL_UserRequest[USER_CurrentRequest]. User_State = Ready;
		GLOBAL_UserRequest.User_Type = NONE_REQUEST;
	}
	else
	{
		if(GLOBAL_UserRequest[USER_CurrentRequest].ROW == 0)
		{
			LOCAL_Location += GLOBAL_UserRequest[USER_CurrentRequest].COLUMN;
		}
		else
		{
			LOCAL_Location += ( GLOBAL_UserRequest[USER_CurrentRequest].COLUMN + LCD_SET_CGRAM_ADDRESS );
		}
		WriteCommand( LOCAL_Location , WRITE_DATA_STATE);
		GLOBAL_OperationalLatchCount ++;
		if (GLOBAL_UserRequest[USER_CurrentRequest].User_State == Ready || USER_CurrentRequest == LCD_MAX_BUFFER_SIZE)
		{
			USER_CurrentRequest = 0;
		}
		if(USER_CurrentRequest && (GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack ) )
		{
			GLOBAL_UserRequest[USER_CurrentRequest-1].CallBack();
		}
	}
}

void LCD_TaskRunnable(void)
{
	if(GLOBAL_LcdState == LCD_InitState)
	{
		Init_State_Function();
	}
	else if(GLOBAL_LcdState == LCD_Operational)
	{
		if(GLOBAL_UserRequest[USER_CurrentRequest].User_State == Busy)
		{
			switch(GLOBAL_UserRequest[USER_CurrentRequest].User_Type)
				{
				case WRITE_REQUEST:
					OperationalState_WRITEprocess();
					break;
				case CLEAR_REQUEST:
					OperationalState_CLEARprocess();
					break;
				case SETPOSITION_REQUEST:
					OperationalState_SETPOSITIONprocess();
					break;
				default:
					break;
				}
		}
	}
	else
	{
		/* */
	}
}
