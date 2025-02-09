# Modbus Library (Master + Slave) C-Library

### Revision History

- 1.9 Added a `_MB_USE_CRC_LOOKUP`, when defined, the library will use a lookup table instead of direct calculation, faster but consumes 512bytes of ROM
- 2.0 Fixed the modbus master callback to issue MB_Ok in case of success
- 2.1 : Added `_MB_USE_MALLOC`, when defined, the library will include the `stdlib` and dynamically allocate the TX and RX buffers, as well as the 4 memory allocations (holding regs, input regs, input bits and coil bits).
Added `_MB_TICKLESS`, when defined, the system operates without a tick timer (tickless operation) then the user is required to call the `MB_Master_Update_Tick` or `MB_Slave_Update_Tick` functions to update the tick counter

### Before you start coding
Make sure you correctly configure the file `MB_Config.h` depending on your usage

### How to use --> Slave

Declare a MB_Slave_t Instance with its necessary configuration as follows:

```c
MB_Slave_t MB =
    {
        .SLA = 10, //salve address
        .TX_RX_Buffer_Size = 256, //size of the TX and RX data buffers
        .HoldingRegSize = 32, //declared in words
        .InputRegSize = 32, //declared in words
        .CoilBitsSize = 2, //declared in bytes (multiples of 8)
        .InputBitsSize = 2, //declared in bytes (multiples of 8)

        //decalre the function pointers
        .hw_interface.MB_Transmit = MB_Transmit, //to transmit data
        .hw_interface.MB_StartListening = MB_StartListening, //to enable the data reception
        .hw_interface.MB_Activate_TX = MB_Activate_TX, //to activate TX mode (RS485)
        .hw_interface.MB_Request_Recieved = MB_Request_Recieved, //to be called when a request is recieved
        .RX_Silent_Interval_MS = 1, //time between the reception of a request and the slave response
        .RX_Timeout = 200, //timeout to receive a complete message
        .TX_Automplete = 0,//set it to 1 in case of non-interrupt tranmsmit is used
};
```

Make sure to call the `void MB_Slave_Routine(MB_Slave_t *MB, uint32_t Ticks)` in your infinite loop, passing a pointer to your instance and the current ticl count, for an STM32 Device, the implementation might look like the following:

```c
MB_Slave_Routine(&MB, HAL_GetTick());
```

When a Master request is recieved, the `MB_Request_Recieved` function is called, you can then handle your data request as follows:

```c
MB_Status_t MB_Request_Recieved(void *ptr)
{
	MB_Slave_t *mb = (MB_Slave_t*) ptr;

    //put your code that handles different requests
	switch (mb->Fcn)
	{
		case MB_PRST_MUL_REGS:
        break;
		case MB_PRST_SNG_REG:
        break;
		case MB_READ_MUL_HLD_REG:
			break;
		default:
			break;
	}
	return MB_STAT_OK; //always return MB_STAT_OK
}
```

Upon execution fo this funtcion, the salve device will automatically reply to the request.

### How to use --> Master

Declare a MB_Slave_t Instance with its necessary configuration as follows:

```c
MB_Master_t MS =
    {
        .TX_RX_BufferSize = 128, //size of the TX and RX data buffers
        .RX_Timeout = 500,
        .TX_Automplete = 0,//set it to 1 in case of non-interrupt tranmsmit is used
        .hw_interface = //declare the interface functions, same as with the MB_Salve_t instance
            {.MB_Transmit = MS_Transmit,
            .MB_StartListening = MS_StartListening,
            .MB_Activate_TX = MS_Activate_TX,
            .MB_Request_Recieved = MS_Request_Recieved},
};
```

To Communicate with a certain salve use one of the below functions:

```c
	/// @brief Reads holding registers starting from \p RegAddress for \p LEN words from Modbus slave at \p SLA
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len numbert of words to be read
	/// @param Data ptr to the data array to be filled
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_ReadHoldingRegs(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint16_t *Data);

	/// @brief Reads input registers starting from \p RegAddress for \p LEN words from Modbus slave at \p SLA
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len numbert of words to be read
	/// @param Data ptr to the data array to be filled
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_ReadInputRegs(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint16_t *Data);

	/// @brief Writes a singel holding register at address \p RegAddress
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Data data word to be written to this address
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_WriteHoldingReg(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Data);

	/// @brief Writes multiple holding regsstarting at \p RegAddress for \p Len words
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len number of words to be written
	/// @param Data data array of words to be written
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_WriteHoldingRegs(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint16_t *Data);

	/// @brief reads the status of \p Len coils starting from address \p RegAddress
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len length of coil bits to be read
	/// @param Coils pointer to a byte array where the read data will be saved
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_ReadCoils(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint8_t *Coils);

	/// @brief reads the status of \p Len input bits starting from address \p RegAddress
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len length of input bits to be read
	/// @param Coils pointer to a byte array where the read data will be saved
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_ReadInputs(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint8_t *Inputs);

	/// @brief Writes the status of a singel coil
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Coil Status of the coil 1 for HIGH and 0 for LOW
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_WriteCoil(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint8_t Coil);

	/// @brief Writes the status of multiple coils starting from \p RegAddress for \p Len coils
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param Len number of coils to be written
	/// @param Coils byte array containing the coil status to be written
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_WriteCoils(MB_Master_t *MB, uint8_t SLA, uint16_t RegAddress, uint16_t Len, uint8_t *Coils);

	/// @brief Performs a combined read/write operation
	/// @param MB Ptr to the master handle
	/// @param SLA Salve Address
	/// @param RegAddress Address of the register
	/// @param ReadLen length of data to be read
	/// @param ReadData word array to hold the read data once recoeved form the slave
	/// @param WriteRegAddress starting address of the register to be written
	/// @param WriteLen number of registers to be written
	/// @param WriteData word array contining the values to be written
	/// @return MB_STAT_OK
	MB_Status_t MB_Master_ReadWriteHoldingRegs(MB_Master_t *MB,
											   uint8_t SLA,
											   uint16_t ReadRegAddress,
											   uint16_t ReadLen,
											   uint16_t *ReadData,
											   uint16_t WriteRegAddress,
											   uint16_t WriteLen,
											   uint16_t *WriteData);
```
Also make sure you call the `void MB_Master_Routine(MB_Master_t *MB, uint32_t Ticks);` function for the background worker to function properly