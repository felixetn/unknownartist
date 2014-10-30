/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file barcode_reader.c
 * @author pnotz
 * @brief Barcode reader is able to detect line barcodes on white paper on a floor that is darker
 * 	      than the paper. It will switch from WallFollow to LineFollow as soon as a barcode is expected
 * 	      (white paper), it will read the barcode and then interpret it as defined in the set
 * 	      barcode handling funktion (setBarcodeHandlingFunction(.))
 **/

#include "barcode_reader.h"

//freq of thread

const uint8_t BARCODES[4] = { 0x2A, 0x06, 0x30, 0x1C }; // {0x15, 0x18, 0x03, 0x0E};
uint8_t BARCODE_BIT_LENGTH = 6;
xTaskHandle xHandleBarcodeReader;


/* Barcode Status
 * 0: had no line found
 * 1: counting to detect edge 0->1
 * 2: had line found
 * 3: counting to detect edge 1->0
 */
int bcStatus = 0;

//counting len of barcode part
int barCounter = 0;
//len of detected parts
int edgeBarCounter = 0;

//defined values of bit length
uint8_t bcBitLength[2] = { 10 }; //dummy init value for bit length

//mode for calibration / reading a barcode
int onBarcode = 0;

//counter for safe detection of edges (clean from artifacts)
int flankenLenCounter = 0;

//detected barcode
uint8_t barcode = 0;
//current barcode bit
int barcodeBitID = 0;

//switch for searching or reading barcode
int expectingBarcode = 0;

// pointer to the function that handles a detected barcode
void (*bcHandlingFunction)(uint8_t) = &BarcodeHandlingLeftStraightRightBack;

/*
 * initializes barcode reader functionality
 */
void initBarcodeReader(void) {
	xHandleBarcodeReader = 0;
}

/*
 * starts barcode reader thread
 */
void startBarcodeReader(void) {
#ifdef BARCODE_tASK
	if (xHandleBarcodeReader == 0) //not registered yet
	{
		xHandleBarcodeReader = os_registerProcessStack(BarcodeReaderThread, "BarcodeReader", 315);
	}
	else //task already registered but suspended
	{
		os_resumeTask(xHandleBarcodeReader);
	}
#endif
}

/*
 * suspends barcode reader thread
 */
void stopBarcodeReader(void) {
	if(xHandleBarcodeReader)
	{
		os_suspendTask(xHandleBarcodeReader);
	}
}

/*
 * sets the function pointer to a function that handles the meaning of the barcodes
 */
void setBarcodeHandlingFunction(void(*func)(uint8_t)) {
	bcHandlingFunction = func;
}

/*
 * function that handles detected barcodes
 */
void BarcodeHandlingLeftStraightRightBack(uint8_t barcode) {

	//stop car
	Drive_SetMotor(0);
	Drive_SetServo(0);
	stopLineFollow();

	showOnSegmentDisplay(barcode, 1000);

#ifdef BARCODE_READER_DEBUG
	printf("Detected barcode: %d\r\n", barcode);
	return;
#endif


	if (barcode == BARCODES[0])
	{   // turn right
		driveLen(100);
		turn90(RIGHT, FORWARDS);
		driveLen(100);
	}
	else if (barcode == BARCODES[1])
	{   // straight on
		driveLen(50);
	}
	else if (barcode == BARCODES[2])
	{   // turn left
		driveLen(100);
		turn90(LEFT, FORWARDS);
		driveLen(100);
	}
	else if (barcode == BARCODES[3])
	{   // turn 180
		driveLen(100);
		turn180(LEFT, BACKWARDS);
		driveLen(100);
	}
	else
	{   // barcode not detected correctly
		showOnSegmentDisplay(505, 1000); // SOS
	}
}

/*
 * switches to barcode reading, called by if white paper / barcode detected
 */
void startBCreading(void) {

	expectingBarcode = 1;

	//reset segment display
	Seg_Init();

	//to read a barcode we need to follow the line
	startLineFollow();

	//set motor speed low
	Drive_SetMotor(2);
}

/*
 * switches back to search mode, sets the car ready to detect a new barcode
 */
void stopBCreading(void) {
	barcodeBitID = 0; //reset barcode storage
	onBarcode = -1; //ready to find new barcode

	expectingBarcode = 0; //switch to search mode

	//no more line to follow
	stopLineFollow();

	Seg_Dec(0);
}

/*
 * determine width of barcode part by bitlength constants
 * @param counter tick counter value to be converted
 * @param zeroOneEdge 1: 0->1 edge, 0: 1->0 edge
 * @return int number of bits for barcode
 */
int estimateWidth(int counter, int zeroOneEdge) {
	// partlen = FLOOR( (len + 0.5 * bitlen) / bitlen )
	int retVal = counter + (bcBitLength[zeroOneEdge] >> 1);
	return retVal / bcBitLength[zeroOneEdge];
}

/*
 * adding detected barcode bits to barcode
 * @param bit the bit value (0 or 1)
 * @param len number of bits to be set to 'bit'
 */
void addToBarcode(char bit, int len) {

	//barcode parts can only be added if not too long (max. 8 Bit)
	if ((barcodeBitID + len) < 8) {
		int i;
		for (i = barcodeBitID; i < (barcodeBitID + len); i++) {
			//barcode[i]=bit;
			barcode = (barcode & ~(1 << i)) | (bit << i); //set bit in barcode
		}
	}

	//set current bit ID in barcode
	barcodeBitID += len;

	//if we have detected a complete barcode
	if (barcodeBitID >= BARCODE_BIT_LENGTH) {

		// compare barcode bitwise
		int barcodeID;

		for (barcodeID = 0; barcodeID < BARCODE_NUMBER; barcodeID++) {
			int biterrors=0;
			int k;
			uint32_t xored;

			xored = BARCODES[barcodeID] ^ barcode; //look for differences to defined barcode

			//count different bits: bit errors
			for (k = 0; k < BARCODE_BIT_LENGTH; k++) {
				biterrors += ((xored & (1 << k)) != 0);
			}

			if (biterrors <= BARCODE_MAX_BIT_ERRORS) { // valid barcode detected!
				//set error corrected barcode
				barcode = BARCODES[barcodeID];

#ifdef BARCODE_READER_DEBUG
				printf("valid barcode number: %d, bit-errs: %d\r\n", barcodeID, biterrors);
#endif
				//quit barcode checking loop
				break;
			}
		}

		// call the method for handling the meaning of the barcodes
		bcHandlingFunction(barcode);
	}
}

/*
 * method to handle detected edges in barcode
 * @param zeroOneEdge 1: 0->1 edge, 0: 1->0 edge
 */
void foundEdge(int zeroOneEdge) {

	//onBarcode < 0 means calibration mode
	//onBarcode >= 0 means reading a barcode

	if (zeroOneEdge == 1) //0->1
	{
		if (onBarcode == -3) //calibration line found, looking for space
		{
			onBarcode++;
		}
		else if (onBarcode == -2) //now we have read the calibration bit
		{
			//found init bit: len=barCounter
			bcBitLength[0] = edgeBarCounter; // barCounter;
			bcBitLength[1] = bcBitLength[0];

			//now we are ready to read a barcode
			onBarcode = -1;

		}
		else if (onBarcode == -1) //calibrated, ready to read a barcode
		{
			onBarcode++; //waiting for 1->0 edge which is the starting point of the barcode
		}
		else if (onBarcode == 1) //we are reading a barcode
		{
			//get len of barcode part that was just read
			int partLen = estimateWidth(edgeBarCounter, zeroOneEdge);

#ifdef BARCODE_READER_DEBUG
			printf("0 Bit: BarCounter: %d - PartLen: %d\r\n", edgeBarCounter, partLen);
#endif

			//add detected bits to barcode
			addToBarcode(0, partLen);
		}

	}
	else //1->0
	{
		if (onBarcode == 0) //this 1->0 edge is the starting point of the barcode
		{
			onBarcode++;
		}
		else if (onBarcode == 1) //reading a bit
		{
			//get len of barcode part that was just read
			int partLen = estimateWidth(edgeBarCounter, zeroOneEdge);

#ifdef BARCODE_READER_DEBUG
			printf("1 Bit: BarCounter: %d - PartLen: %d\r\n", edgeBarCounter, partLen);
#endif
			//add detected bits to barcode
			addToBarcode(1, partLen);
		}
	}
}

/*
 * in barcode reader mode - looks for edges in barcode line
 */
void checkLineStatus(void) {
	switch (bcStatus) {
		case 0: // had no line found
			if (bcLineFound == 1)
			{ //now we have a line: starting to detect 0->1 edge
				bcStatus++; //switch to edge detection
				flankenLenCounter = 0; //counter to safely detect an edge
				edgeBarCounter = barCounter; //save current barcounter value
			}
			break;
		case 1: // counting to detect edge 0->1
			if (bcLineFound == 1)
			{ //still having a line
				flankenLenCounter++; //count cycles with line
			}
			else //lost line: edge probably was an artifact
			{
				bcStatus--; //go back to: had no line found
			}

			if (flankenLenCounter > 2) //enough, we have an edge: 0->1
			{
				barCounter -= edgeBarCounter; //next bit has already begun
				foundEdge(1); //we have an edge, len of previous line: edgeBarCounter
				bcStatus++; //switch to look for next edge
			}
			break;
		case 2: // had line found
			if (bcLineFound == 0)
			{ // now we don't have a line any more: starting to detect 1->0 edge
				bcStatus++; //switch to edge detection
				flankenLenCounter = 0; //counter to safely detect an edge
				edgeBarCounter = barCounter; //save current barcounter value
			}
			break;
		case 3: // counting to detect edge 1->0
			if (bcLineFound == 0)
			{ //still having no line
				flankenLenCounter++; //count cycles without line
			}
			else //found line: edge probably was an artifact
			{
				bcStatus--; //go back to had line found
			}

			if (flankenLenCounter > 2) //enough, we have an edge: 1->0
			{
				barCounter -= edgeBarCounter; //next bit has already begun
				foundEdge(0); //we have an edge, len of previous line: edgeBarCounter
				bcStatus = 0; //switch to look for next edge
			}
			break;
	}
}

/*
 * in search mode - checks for white paper / barcode that can be detected
 */
void checkExpectBarcode(void) {

	//delay task so that searching for a barcode is run in lower frequency
	os_wait(200);

	Seg_Hex(0);

#ifdef BARCODE_READER_DEBUG
	printf("calibrated sensor sum: %d \r\n", (cal_sensor[0] + cal_sensor[1] + cal_sensor[2] + cal_sensor[3] + cal_sensor[4]));
#endif

	//if sensor-data below threshold -> white paper detected
	if ((cal_sensor[0] + cal_sensor[1] + cal_sensor[2] + cal_sensor[3]
			+ cal_sensor[4]) < 5 * 7)
	{
		//found barcode -> switch to barcode reading mode
		stopWallFollow();
		startBCreading();

#ifdef BARCODE_READER_DEBUG
		printf("Switching to BC Mode!\r\n");
#endif
	}
}

/*
 * Thread checking for barcodes
 */
void BarcodeReaderThread(void) {

	barCounter = 0;

	bcStatus = 0;

	stopBCreading(); //reset to be ready to read a barcode (init done by SD Card)

	//barcode reader main loop
	for (;;) {

		if (expectingBarcode == 1)
		{ //reading barcode
			checkLineStatus(); //looking for edges
		}
		else //searching for barcode
		{
			checkExpectBarcode();
		}

		os_wait(10);

		//counter for barcode length
		barCounter++;
	}
}
