#define LEFT_BTN 2
#define RIGHT_BTN 3
#define JUMP_BTN 4

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
