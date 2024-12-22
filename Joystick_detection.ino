const int SW_pin = 2;  // Push button pin
const int X_pin = A0;  // X-axis analog pin
const int Y_pin = A1;  // Y-axis analog pin

// Thresholds for joystick movement
const int THRESHOLD_HIGH = 800;
const int THRESHOLD_LOW = 200;

// States for tracking joystick movements
enum JoystickState { CENTER, RIGHT, UP, LEFT, DOWN };
JoystickState currentState = CENTER;

// Flags for sequence detection
bool clockwiseCompleted = false;
bool anticlockwiseCompleted = false;

// Sequences for detection
JoystickState clockwiseSequence[4] = { RIGHT, UP, LEFT, DOWN };
JoystickState anticlockwiseSequence[4] = { DOWN, LEFT, UP, RIGHT };

// Sequence trackers
int clockwiseIndex = 0;
int anticlockwiseIndex = 0;

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);  // Configure push button pin
  Serial.begin(9600);             // Begin serial communication
  Serial.println("Joystick Authentication System Ready");
}

void loop() {
  int xValue = analogRead(X_pin); // Read X-axis value
  int yValue = analogRead(Y_pin); // Read Y-axis value

  JoystickState newState = getJoystickState(xValue, yValue);

  // Detect clockwise sequence
  if (newState != currentState && newState != CENTER) {
    if (newState == clockwiseSequence[clockwiseIndex]) {
      clockwiseIndex++;
      if (clockwiseIndex >= 4) {
        clockwiseCompleted = true;
        Serial.println("Clockwise sequence detected");
        clockwiseIndex = 0; // Reset clockwise index
      }
    } else {
      clockwiseIndex = 0; // Reset on mismatch
    }

    // Detect anti-clockwise sequence
    if (newState == anticlockwiseSequence[anticlockwiseIndex]) {
      anticlockwiseIndex++;
      if (anticlockwiseIndex >= 4) {
        anticlockwiseCompleted = true;
        Serial.println("Anti-clockwise sequence detected");
        sendAuthSignal(); // Trigger authentication
        resetStates();    // Reset all states
      }
    } else {
      anticlockwiseIndex = 0; // Reset on mismatch
    }

    currentState = newState; // Update current state
  }

  delay(50); // Add a small delay to debounce joystick movements
}

// Function to determine the joystick's current state based on X and Y values
JoystickState getJoystickState(int xValue, int yValue) {
  if (xValue > THRESHOLD_HIGH) return RIGHT;
  if (xValue < THRESHOLD_LOW) return LEFT;
  if (yValue > THRESHOLD_HIGH) return UP;
  if (yValue < THRESHOLD_LOW) return DOWN;
  return CENTER; // Joystick is in the center position
}

// Function to send authentication signal to the PC
void sendAuthSignal() {
  Serial.println("AUTH_SUCCESS"); // Send signal to PC
}

// Function to reset all sequence states
void resetStates() {
  clockwiseCompleted = false;
  anticlockwiseCompleted = false;
  clockwiseIndex = 0;
  anticlockwiseIndex = 0;
}
