/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xparameters.h"
#include "inv_sqrt.h"

/**************************** user definitions ********************************/


#define INV_SQRT_BASE_ADDR      XPAR_INV_SQRT_0_S00_AXI_BASEADDR

#define CONTROL_REG_OFFSET    INV_SQRT_S00_AXI_SLV_REG0_OFFSET
#define INPUT_REG_OFFSET      INV_SQRT_S00_AXI_SLV_REG1_OFFSET
#define STATUS_REG_OFFSET     INV_SQRT_S00_AXI_SLV_REG2_OFFSET
#define RESULT_REG_OFFSET     INV_SQRT_S00_AXI_SLV_REG3_OFFSET

#define CONTROL_REG_RESET_MASK (u32)(0x02)
#define CONTROL_REG_OFF_MASK (u32)(0x00)
#define CONTROL_REG_START_MASK (u32)(0x01)
#define STATUS_REG_READY_MASK (u32)(0x01)

int getResult(float in, float* out)
{

u32 data  = (u32)( in * 8388608);
u32 result;

// u32 u_in, u_out;
   INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, CONTROL_REG_RESET_MASK);
   INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, CONTROL_REG_OFF_MASK);
//Send data to data register of cordic processor
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, INPUT_REG_OFFSET, data);
//Start cordic processor - pulse start bit in control register
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 1);//CONTROL_REG_START_MASK);
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 0);//CONTROL_REG_OFF_MASK);
//Wait for ready bit in status register
	u32 tmp_res = 0;//0

	while( (tmp_res && STATUS_REG_READY_MASK) == 0)
	{
		//tmp_res = INV_SQRT_mReadReg(INV_SQRT_BASE_ADDR, STATUS_REG_OFFSET);
		result = INV_SQRT_mReadReg(INV_SQRT_BASE_ADDR, STATUS_REG_OFFSET);
	}
//Get results
	//result = INV_SQRT_mReadReg(INV_SQRT_BASE_ADDR, RESULT_REG_OFFSET);
	*out = (float) (result /8388608);
	INV_SQRT_mWriteReg(INV_SQRT_BASE_ADDR, CONTROL_REG_OFFSET, 0);
	
	print("dupa");
	return 1;
}
