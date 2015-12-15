#ifndef bool
typedef unsigned char bool;
#define true 1
#define false 0
#endif

#define NUMBER_OF_LASERS 8

#define SPEED_OF_SOUND 	0.03434 //In cm/us
#define ECHO_INDEX 		1
#define TRIGGER_INDEX 	0

#define SR_CLOCK 15 //PB_3
#define RGB_DATA 14 //PE_0

#define GREEN	0
#define RED		1
#define BLUE	2
#define LIGHTOFF	3

const int LDR_PINS[ NUMBER_OF_LASERS ] = {30, 9, 27, 4, 11, 34, 38, 17}; //from sensor 0 to 7

const int DISTANCE_SENSOR_PINS[ NUMBER_OF_LASERS ][ 2 ] = {
	{28, 29},
	{7, 8},
	{25, 26},
	{2, 3},
	{13, 12},
	{35, 34},
	{40, 39},
	{19, 18}
};

unsigned int ldrInterruptFlags[NUMBER_OF_LASERS];
bool ldrInterruptBuffer[NUMBER_OF_LASERS];
unsigned int distance;
unsigned int colors[NUMBER_OF_LASERS];

void changeLaser0() {
  ldrInterruptFlags[0] = digitalRead(LDR_PINS[0]);  
}

void changeLaser1() {
  ldrInterruptFlags[1] = digitalRead(LDR_PINS[1]);  
}

void changeLaser2() {
  ldrInterruptFlags[2] = digitalRead(LDR_PINS[2]);  
}

void changeLaser3() {
  ldrInterruptFlags[3] = digitalRead(LDR_PINS[3]);  
}

void changeLaser4() {
  ldrInterruptFlags[4] = digitalRead(LDR_PINS[4]);  
}

void changeLaser5() {
  ldrInterruptFlags[5] = digitalRead(LDR_PINS[5]);  
}

void changeLaser6() {
  ldrInterruptFlags[6] = digitalRead(LDR_PINS[6]);  
}

void changeLaser7() {
  ldrInterruptFlags[7] = digitalRead(LDR_PINS[7]);  
}

void setup() {
	Serial.begin(9600);
	Serial5.begin(9600);

	for (int i; i < NUMBER_OF_LASERS; i++) {
		pinMode(LDR_PINS[i], INPUT);
		pinMode(DISTANCE_SENSOR_PINS[i][ECHO_INDEX], INPUT);
		pinMode(DISTANCE_SENSOR_PINS[i][TRIGGER_INDEX], OUTPUT);
                ldrInterruptBuffer[i] = LIGHTOFF;
	}
	
	attachInterrupt(LDR_PINS[0], changeLaser0, CHANGE);
	attachInterrupt(LDR_PINS[1], changeLaser1, CHANGE);
	attachInterrupt(LDR_PINS[2], changeLaser2, CHANGE);
	attachInterrupt(LDR_PINS[3], changeLaser3, CHANGE);
	attachInterrupt(LDR_PINS[4], changeLaser4, CHANGE);
	attachInterrupt(LDR_PINS[5], changeLaser5, CHANGE);
	attachInterrupt(LDR_PINS[6], changeLaser6, CHANGE);
	attachInterrupt(LDR_PINS[7], changeLaser7, CHANGE);

        pinMode( RGB_DATA, OUTPUT );
        pinMode( SR_CLOCK, OUTPUT );
        digitalWrite( SR_CLOCK, LOW );
        digitalWrite( RGB_DATA, LOW );
        
        lightsLedTape();
}

void SendToBluetooth(int laser, unsigned int height, boolean ioParam) {
	char str [6];
	str[0] = ((char) (laser + 'A'));

	if (ioParam) {
		str[1] = 'i';
	}
	else {
		str[1] = 'o';
	}

	if (height < 10) {
		str[2] = ((char) height + '0');
		str[3] = '\r';
		str[4] = '\n';
	}
	else if((10 <= height) && (height <= 99)) {
		int tmp = height / 10;
		str[2] = ((char) tmp + '0');
		tmp = height % 10;
		str[3] = ((char) tmp + '0');
		str[4] = '\r';
		str[5] = '\n';
	}
	Serial5.write(str);
}

unsigned int getDistance(int laserIntercepted) {
	int TRIGGER_PIN = DISTANCE_SENSOR_PINS[laserIntercepted][TRIGGER_INDEX];
	int ECHO_PIN = DISTANCE_SENSOR_PINS[laserIntercepted][ECHO_INDEX];
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN, LOW);
	unsigned int duration = pulseIn(ECHO_PIN, HIGH, 4100);
	unsigned int distance_cm = (duration / 2) * SPEED_OF_SOUND;

	return distance_cm;
}

void loop() {
	
	for (int i; i < NUMBER_OF_LASERS; i++) {
		if (ldrInterruptFlags[i] != ldrInterruptBuffer[i]) {
			ldrInterruptBuffer[i] = ldrInterruptFlags[i];
				
			Serial.print("Color ");
			Serial.print(i);
			Serial.print(" is ");
				
			if (ldrInterruptBuffer[i] == HIGH) {
				distance = getDistance(i);
				SendToBluetooth(i, distance, true);
				
				if ((0 <= distance) && (distance < 22)) {
					colors[i] = GREEN;
					Serial.println(" GREEN");
				}
				else if ((22 <= distance) && ( distance < 44)) {
					colors[i] = RED;
					Serial.println(" RED");
				}
				else if ((44 <= distance) && (distance <= 68)) {
					colors[i] = BLUE;
					Serial.println(" BLUE");
				}
				else {
					colors[i] = LIGHTOFF;
					Serial.println(" OFF");
				}
			}
			else {
				SendToBluetooth(i, 0, false);
				colors[i] = LIGHTOFF;
				Serial.println(" OFF");
			}



			lightsLedTape();
		}
	}
			
	if (Serial5.available() > 0) {
                char incomingByteBluetooth = Serial5.read();
                char comTmp = incomingByteBluetooth - 'A';

		if ((0 <= comTmp) && (comTmp < 24)) {
			colors[comTmp / 3] = comTmp % 3;
			lightsLedTape();
		}
		
		comTmp = incomingByteBluetooth - '0';
		
		if ((0 <= comTmp) && (comTmp < 8)) {
			colors[comTmp] = LIGHTOFF;
			lightsLedTape();
		}

		Serial.println(incomingByteBluetooth);
	}

}

void pulse( int numberOfPulses ) {
	for( int pulse = 0; pulse < numberOfPulses; pulse++ ) {
		digitalWrite( SR_CLOCK, HIGH );
		delayMicroseconds( 15 ); //Possível fonte de erro
		digitalWrite( SR_CLOCK, LOW );
	}
}

void lightsGreen( ) {
	pulse( 2 );
	digitalWrite( RGB_DATA, HIGH );
	pulse( 1 );
	digitalWrite( RGB_DATA, LOW );
}

void lightsBlue( ) {
	digitalWrite( RGB_DATA, HIGH );
	pulse( 1 );
	digitalWrite( RGB_DATA, LOW );
	pulse( 2 );
}

void lightsRed( ) {
	pulse( 1 );
	digitalWrite( RGB_DATA, HIGH );
	pulse( 1 );
	digitalWrite( RGB_DATA, LOW );
	pulse( 1 );
}

void lightsOff( ) {
	pulse( 3 );
}

void lightsLedTape() {

  	for (int i = 0; i < NUMBER_OF_LASERS; i++) {
		switch( colors[i] ) {
			case GREEN:
				lightsGreen();
				break;
			case RED:
				lightsRed();
				break;
			case BLUE:
				lightsBlue();
				break;
			default:
				lightsOff();
				break;
		}
	}
	//According to Texas Instruments 74595 IC datasheet,
	//we need to send an extra pulse as the RCLR and RSCLK pins are short-circuited
	pulse( 1 );
	
}




