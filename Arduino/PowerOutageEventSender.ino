int powerCheckPin = 9;
unsigned char powerOnMsg = 49;
unsigned char powerOffMsg = 48;
unsigned char startCode = 50;
bool started = false;
int powerState = 0;

void setup() {
    pinMode(powerCheckPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    if (!started) {
        serialScanner();
        return;
    }
    powerState = digitalRead(powerCheckPin);
    if (powerState == HIGH) {
        Serial.write(powerOnMsg);
    } else {
        Serial.write(powerOffMsg);
    }
    delay(100);
}

void serialScanner() {
    if (Serial.available() <= 0) {
        return;
    }
    int receivedByte = Serial.peek();
    if (receivedByte == startCode) {
        started = true;
    }
}
