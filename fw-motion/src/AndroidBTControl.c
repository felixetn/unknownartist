#include "FreeRTOS.h"
#include "task.h"
#include "api/api.h"
#include "api/battery.h"
#include "api/wireless/wireless.h"
#include "api/wireless/bluetooth_package_types.h"
#include "api/drive.h"
#include "api/us.h"
#include "AndroidBTControl.h"
#include "api/linesensor.h"
#include "api/printf_stdarg.h"

void motorControlTest_bt(wireless_interface_t wiif, wirelessMessage_t* msg) {
	Drive_SetMotor(msg->data[0] - 43);
	Drive_SetServo((msg->data[1] - 10) * 10);
}

void calibrate_us_bt(wireless_interface_t wiif, wirelessMessage_t* msg) {
	static uint8_t val = 0;

	Us_IO_DDR(2, 0, 1);
	while (Us_Cmd_Wait(1) || Us_Cmd_Wait(2))
		__wait_nop();
	Us_IO_DDR(2, 1, 1);
	while (Us_Cmd_Wait(1) || Us_Cmd_Wait(2))
		__wait_nop();
	Us_IO_Value(2, 0, val);
	while (Us_Cmd_Wait(1) || Us_Cmd_Wait(2))
		__wait_nop();
	val ^= 0x01;
	Us_IO_Value(2, 1, val);
	while (Us_Cmd_Wait(1) || Us_Cmd_Wait(2))
		__wait_nop();

    Us_Calibrate(2);
}

void calibrate_ls_bt(wireless_interface_t wiif, wirelessMessage_t* msg) {
	Linesensor_Calibrate();
}

void bt_send_sensor_data(void) {
	portTickType lastWakeTime;
	wirelessMessage_t msg;
	int16_t bat;

	msg.data[0] = 0;
	msg.dataLength = 16;
	msg.destinationId = 0;
	msg.messageType = PT_ANDROID_SENSORDATA;
	msg.priority = 0;

	lastWakeTime = os_getTime();

	for (;;) {
		os_frequency(&lastWakeTime, 100);

		bat = Battery_GetVoltage();

		msg.data[1] = Us_Data.Front_Distance & 0xff;
		msg.data[2] = (Us_Data.Front_Distance >> 8) & 0xff;
		msg.data[3] = Us_Data.Rear_Distance & 0xff;
		msg.data[4] = (Us_Data.Rear_Distance >> 8) & 0xff;
		msg.data[5] = Us_Data.Left_Distance & 0xff;
		msg.data[6] = (Us_Data.Left_Distance >> 8) & 0xff;
		msg.data[7] = Us_Data.Right_Distance & 0xff;
		msg.data[8] = (Us_Data.Right_Distance >> 8) & 0xff;
		msg.data[9] = linefound;
		msg.data[10] = averagePos & 0xff;
		msg.data[11] = (averagePos >> 8) & 0xff;
		msg.data[12] = (averagePos >> 16) & 0xff;
		msg.data[13] = (averagePos >> 24) & 0xff;
		msg.data[14] = bat & 0xff;
		msg.data[15] = (bat >> 8) & 0xff;
		wirelessSend(WI_IF_BLUETOOTH, &msg);
	}
}

void Init_AndroidBTControl(void) {
	wirelessSubscribe(calibrate_us_bt, PT_CALIBRATE_SENSORS);
	wirelessSubscribe(calibrate_ls_bt, PT_CALIBRATE_LINESENSORS);
	wirelessSubscribe(motorControlTest_bt, PT_ANDROID_CONTROL);
	os_registerProcessStack(bt_send_sensor_data, "Send sensor data via BT", 680);
	printf("Android BT motor control started.\r\n");
}
