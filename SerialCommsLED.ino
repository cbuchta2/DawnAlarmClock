/*Pin Constants*/
const int RED_PIN    = 11;
const int GREEN_PIN  = 10;
const int BLUE_PIN   = 9;

/*Index constants*/
const int RED_VAL   = 0;
const int GREEN_VAL = 1;
const int BLUE_VAL  = 2;
const int NUM_PINS = 3;

/*Offset constant, RGB PWM values are sent as value +4 to prevent collisions with STX and ETX*/
const int VALUE_OFFSET = 4;

/*Local Vars*/
int rgbValues[NUM_PINS];
bool newData = false;

void setup() {
  /*Setup Pins to be outputs*/
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
 
  /*Setup Serial comms*/
  Serial.begin(9600);
}

void loop() {
  /*Check Serial for RGB Led commands*/
  recvWithStartEndMarkers();
  if(newData){
    newData = 0;
    setColor(rgbValues[RED_VAL], rgbValues[GREEN_VAL], rgbValues[BLUE_VAL]);
  }
}

/*Function to read data from serial buffer
 * Expected Message format: <STX>RRGGBB<ETX>
 * RR = 16 bit HEX value for Red LED duty cycle
 * GG = 16 bit HEX value for Green LED duty cycle
 * BB = 16 bit HEX value for Blue LED duty cycle
 * Valid Duty cycle range = 0 - 255
 * NOTE: values are shifted by +4 in serial message to prevent conflicts with STX and ETX (0x02 and 0x03)
 * IE: Duty cycle of 0 is recieved as 0x04 
 */
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = 0x02; //<STX>
    char endMarker = 0x03;  //<ETX>
    char rc;

    while (Serial.available() > 0 && !newData) {
        rc = Serial.read();
        /*Wait for STX then read in mesg until ETX*/
        if (recvInProgress == true) {
            if (rc == endMarker) {
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
            else {
                rgbValues[ndx] = rc - VALUE_OFFSET;
                ndx++;
                if (ndx >= NUM_PINS) { //Prevent array out of bounds
                    ndx = NUM_PINS - 1;
                }
            }
        }else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void setColor(int red, int green, int blue){
   /*Write PWM value between 0..255 to pins*/
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
