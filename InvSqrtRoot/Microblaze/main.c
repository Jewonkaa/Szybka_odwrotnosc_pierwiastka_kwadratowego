#include "xparameters.h"
#include "xgpio.h"
#include "inv_sqrt.h"

#define CHANNEL 1
#define INV_SQRT_BASE_ADDR XPAR_INV_SQRT_0_S00_AXI_BASEADDR

#define CONTROL_REG_OFFSET INV_SQRT_S00_AXI_SLV_REG0_OFFSET
#define INPUT_REG_OFFSET INV_SQRT_S00_AXI_SLV_REG1_OFFSET
#define STATUS_REG_OFFSET INV_SQRT_S00_AXI_SLV_REG2_OFFSET
#define OUTPUT_REG_OFFSET INV_SQRT_S00_AXI_SLV_REG3_OFFSET

#define RESULT_REG_OUTPUT(param) ((u32)param & (u32)(0x7FFFFFF))

int main()
{
	int status;
	XGpio inGpio, outGpio, readyGpio;
	u32 data;
	u32 result, out;

	status = XGpio_Initialize(&inGpio, XPAR_AXI_IN_DEVICE_ID);
	if (status != XST_SUCCESS)
		goto FAILURE;

	XGpio_SetDataDirection(&inGpio, CHANNEL, 0x7FFFFFFF);

	status = XGpio_Initialize(&readyGpio, XPAR_AXI_READY_OUT_DEVICE_ID);
	if (status != XST_SUCCESS)
		goto FAILURE;

	status = XGpio_Initialize(&outGpio, XPAR_AXI_OUT_DEVICE_ID);
	if (status != XST_SUCCESS)
		goto FAILURE;

	XGpio_SetDataDirection(&readyGpio, CHANNEL, 0x0);
	XGpio_SetDataDirection(&outGpio, CHANNEL, 0x0000000);

	data = XGpio_DiscreteRead(&inGpio, CHANNEL);

	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 2);
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 0);

	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, INPUT_REG_OFFSET, data);

	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 1);
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 0);

	while( (INV_SQRT_mReadReg(INV_SQRT_BASE_ADDR, STATUS_REG_OFFSET) & 0x01) == 0) {}

	XGpio_DiscreteWrite(&outGpio, CHANNEL, data);

	result = INV_SQRT_mReadReg(INV_SQRT_BASE_ADDR, OUTPUT_REG_OFFSET);
	out = RESULT_REG_OUTPUT( result );

	XGpio_DiscreteWrite(&outGpio, CHANNEL, out);
	XGpio_DiscreteWrite(&readyGpio, CHANNEL, 1);

	FAILURE:
		while(1);
}
