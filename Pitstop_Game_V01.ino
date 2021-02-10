// =======================================================================================
//            CSE-24111 Final Project Sketch
// =======================================================================================
//            Last Revised Date: 01/26/2021
//            Revised By: Jonathon Vasilak
// =======================================================================================
// ---------------------------------------------------------------------------------------
//            Included Libraries
// ---------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// ---------------------------------------------------------------------------------------
//             Declare Screen (OLED) Items
// ---------------------------------------------------------------------------------------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Intialize the Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------------------------------------------------------------------------------
//             Declare Gamepad Button Items
// ---------------------------------------------------------------------------------------
const int up_button = 2;
const int right_button = 3;
const int down_button = 4;
const int left_button = 5;
const int start_button = 6;
const int select_button = 7;
const int joystick_button = 8;
const int joystick_x = A0;
const int joystick_y = A1;

// ---------------------------------------------------------------------------------------
//             Declare Extra Clicks Management Items
// ---------------------------------------------------------------------------------------
boolean extraClicks = false;
long extraClickMillis = millis();

// ---------------------------------------------------------------------------------------
//             Declare Sound Routine Items
// ---------------------------------------------------------------------------------------
const int buzzer = A2; //buzzer to arduino pin A2 - A2 is a global reference to the actual PIN#
long sound_millis = millis();
int sound_counter = 0;
long currentToneMillis = millis();
int currentToneNumber = 0;
int currentTone = 0;
int currentToneDuration = 0;
int currentSongLength = 0;

// Define decible values for notes
struct MusicStruct {
  int A = 550;
  int As = 582;
  int B = 617;
  int C = 654;
  int Cs = 693;
  int D = 734;
  int Ds = 777;
  int E = 824;
  int F = 873;
  int Fs = 925;
  int G = 980;
  int Gs = 1003;
  int A2 = 1100;
  int A2s = 1165;
  int B2 = 1234;
  int C3 = 1308;
  int C3s = 1385;
  int D3 = 1555;
}Music;

int Song[][2] = {{200,Music.D},{200,0},{200,Music.D},{100,0},{100,Music.E},{100,Music.F},{300,Music.C},{300,Music.C},{300,Music.A},{600,Music.B},{200,Music.D},{200,0},{200,Music.D},{100,0},{100,Music.E},{100,Music.F},{300,Music.C},{300,Music.C},{300,Music.F},{600,Music.G}};
int Tone = Music.A;

//----------------------------------------------------------------------------------------
//              Declare Boolean Variables and Coordinates
//----------------------------------------------------------------------------------------
boolean songIsPlaying = false;
boolean SongFinished = false;
boolean toneIsPlaying = false;
boolean toneComplete = false;
boolean toneStarted = false;

boolean End = false;
boolean Paused = false;

boolean RightCarCollision = false;
boolean LeftCarCollision = false;
boolean TopCarCollision = false;
boolean BottomCarCollision = false;
boolean CarCollision = false;

boolean RWallCollision = false;
boolean LWallCollision = false;
boolean TWallCollision = false;
boolean BWallCollision = false;

boolean XObjCollision = false;
boolean YObjCollision = false;
boolean RObjCollision = false;
boolean LObjCollision = false;
boolean TObjCollision = false;
boolean BObjCollision = false;
boolean ObjectCollision = false;

boolean RConflict = false;
boolean LConflict = false;
boolean TConflict = false;
boolean BConflict = false;

boolean TireHeld = false;
boolean T1_Collision = false;
boolean T1_Set = false;
boolean T2_Set = false;
boolean T3_Set = false;
boolean T4_Set = false;

// User's Coordinates
int XCoord = 10;
int YCoord = 10;
// Coordinates of the Car
int X3 = 32;
int Y3 = 16;
// Rate of change of the object that is held by the user
int dx2 = 0;
int dy2 = 0;

// Coordinates of the Upper Left Corners of the Tires
int Tire1X = 120;
int Tire1Y = 10;
int Tire2X = 43;
int Tire2Y = 12;
int Tire3X = 43;
int Tire3Y = 47;
int Tire4X = 86;
int Tire4Y = 12;

int tireState = 1; // Rotational State of the Tire
int tireWid = 7;
int tireHgt = 5;

// 'Splash Screen', 128x64px
const unsigned char SplashScreen [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x80, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfe, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x07, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x0f, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x1e, 0x07, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0x1f, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xcf, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xfc, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xcc, 0x3c, 0x0e, 0x0c, 0x0e, 0x1c, 0x1f, 0xfc, 0xce, 0x1c, 0x0e, 0x1f, 0xff, 0xff, 
  0xff, 0xfc, 0xcf, 0x3e, 0x7c, 0xfe, 0x7c, 0xcc, 0xcf, 0xfc, 0xff, 0xcc, 0xa4, 0xcf, 0xff, 0xff, 
  0xff, 0xfc, 0x1f, 0x3e, 0x7c, 0xfe, 0x7c, 0xcc, 0xcf, 0xfc, 0xff, 0xcc, 0xa4, 0xcf, 0xff, 0xff, 
  0xff, 0xfc, 0xff, 0x3e, 0x7e, 0x1e, 0x7c, 0xcc, 0xcf, 0xfc, 0x8e, 0x0c, 0xa4, 0x0f, 0xff, 0xff, 
  0xff, 0xfc, 0xff, 0x3e, 0x7f, 0xce, 0x7c, 0xcc, 0xcf, 0xfc, 0xcc, 0xcc, 0xa4, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xff, 0x3e, 0x7f, 0xce, 0x7c, 0xcc, 0xcf, 0xfc, 0xcc, 0xcc, 0xa4, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xfc, 0x0f, 0x0c, 0x1f, 0x0e, 0x1c, 0x1f, 0xfe, 0x0e, 0x0c, 0xe6, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// 'Game board', 72x32px
const unsigned char GameBoard [] PROGMEM = {
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x03, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0x00, 0x03, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x07, 0x82, 0x03, 0xff, 0xff, 
  0x80, 0x00, 0x40, 0x00, 0x07, 0xc2, 0x1f, 0xff, 0xff, 0xe1, 0x80, 0x40, 0x00, 0x07, 0xc2, 0x1f, 
  0xff, 0xff, 0xff, 0xf8, 0x40, 0x03, 0x0f, 0xc2, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x43, 0x3f, 0x0f, 
  0xc2, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x43, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 
  0xc2, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xc2, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x21, 
  0xff, 0x0f, 0xc2, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x21, 0x9f, 0x07, 0xc2, 0x1f, 0xff, 0xff, 0xff, 
  0xfc, 0x20, 0x03, 0x07, 0x82, 0x1f, 0xff, 0xff, 0xe1, 0xc0, 0x20, 0x00, 0x07, 0x82, 0x03, 0xff, 
  0xff, 0x80, 0x00, 0x20, 0x00, 0x03, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x03, 0x82, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
};

int address = 0;
int wins = EEPROM.read(address);
// ---------------------------------------------------------------------------------------
//                 MAIN SETUP FUNCTION - RAN ONCE AT START
// ---------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
  // Set pin mode for the buzzer
  pinMode(buzzer, OUTPUT); // Set buzzer - A2 as an output

  // Setup Gamepad Inputs
  pinMode(up_button, INPUT);
  pinMode(down_button, INPUT);
  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);
  pinMode(select_button, INPUT);
  pinMode(start_button, INPUT);
  pinMode(joystick_button, INPUT);

  digitalWrite(up_button, HIGH);
  digitalWrite(down_button, HIGH);
  digitalWrite(left_button, HIGH);
  digitalWrite(right_button, HIGH);
  digitalWrite(select_button, HIGH);
  digitalWrite(start_button, HIGH);
  digitalWrite(joystick_button, HIGH);

  // Initialize the Display - Show Splash Screen
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.drawBitmap(1, 1, SplashScreen, 128, 64,  WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

// ---------------------------------------------------------------------------------------
//                 MAIN PROCESSING LOOP - RAN RECURSIVELY AT CPU CLOCK SPEED
// ---------------------------------------------------------------------------------------

void loop() 
{  
  while(!End)
  {
      // Reset extraClicks after 1 second
  if ((extraClickMillis + 500 < millis()) && extraClicks) {
      extraClicks = false;
  }
   checkInputs();
   if(T1_Set && T2_Set && T3_Set && T4_Set)
   {
    End = true;
    wins++;
    EEPROM.write(address,wins);
    Serial.println(wins);
    playSong();
   }
  }
  if (songIsPlaying) {
    check_songManager();
  }

  if (toneIsPlaying) {
    check_tonePlayer();
  }
  
  if(!songIsPlaying)
  {
    ResetGame();
  }
}

//----------------------------------------------------------------------------------------
//                                FUNCTION DEFINITIONS
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
//                 Initialize Song Function
//----------------------------------------------------------------------------------------
void playSong() 
{
  songIsPlaying = true;
  currentSongLength = sizeof(Song) / 4;
}

// ---------------------------------------------------------------------------------------
//                 Main Song Manager Function
// ---------------------------------------------------------------------------------------
void check_songManager() {
  if (currentToneNumber == 0 && !toneIsPlaying) {
    currentToneDuration = Song[0][0];
    currentTone = Song[0][1];
    currentToneMillis = millis();
    toneIsPlaying = true;
    toneComplete = false;
  } else {
    if (toneComplete) {
      currentToneNumber++;
      if (currentToneNumber > (currentSongLength - 1)) {  // end of song
        songIsPlaying = false;
        SongFinished = true;
        toneIsPlaying = false;   
        currentToneNumber = 0;   
        noTone(buzzer);
      } else {
        currentTone = Song[currentToneNumber][1];
        currentToneDuration = Song[currentToneNumber][0];
        currentToneMillis = millis();
        toneIsPlaying = true;
        toneComplete = false;
      }
    }
  }
}

// ---------------------------------------------------------------------------------------
//                 Single Tone Player Function
// ---------------------------------------------------------------------------------------
void check_tonePlayer() {
  if ((currentToneMillis + currentToneDuration) > millis()) {
    if (!toneStarted) {  
      tone(buzzer, currentTone);
      toneStarted = true;
    }
  } else {
    toneComplete = true;
    toneStarted = false;
  }
}

//----------------------------------------------------------------------------------------
//               Check For User Inputs Function
//----------------------------------------------------------------------------------------
void checkInputs()
{
  int p = digitalRead(start_button); // checks if the user has paused the game
  if(p != 1)
  {
    if(Paused)
    {
      Paused = false;
    }
    else
    {
      Paused = true;
    }
  }
  int r = digitalRead(select_button); // checks if the user has reset the game
  if(r != 1)
  {
    End = true;
  }
  if(!Paused) // only allows the system to read other inputs if the user has not paused the game
  {
   checkJoystick();
   int a = digitalRead(right_button); // this button will rotate the tire held by the user to the right
   if(a != 1)
   {
    rotateRight();
   }
   else
   {
     a = digitalRead(left_button); // this button will rotate the tire held by the user to the left
     if(a != 1)
     {
      rotateLeft();
     }
   }
   int b = digitalRead(down_button); // picks up the tire if the user is close enough to it
   if(b != 1)
   {
    PickupTire();
   }
  }
}

//----------------------------------------------------------------------------------------
//                 Check Joystick Inputs Function
//----------------------------------------------------------------------------------------

void checkJoystick()
{
  setscreen();
  checkWalls();
  checkCarCollision();
  checkConflicts();
  checkTireSet();
  checkTireCollision();
  int X = analogRead(joystick_x);
  int dx = 0;
  if(X < 500 && !LConflict)
  {
    dx = -1;
    if(TireHeld)
    {
      dx2 = -1;
    }
    else
    {
      dx2 = 0;
    }
  }
  else if(X > 515 && !RConflict)
  {
    dx = 1;
    if(TireHeld)
    {
      dx2 = 1;
    }
    else
    {
      dx2 = 0;
    }
  }
  else
  {
    dx = 0;
    dx2 = 0;
  }
  int Y = analogRead(joystick_y);
  int dy = 0;
   if(Y < 500 && !BConflict)
  {
    dy = 1;
    if(TireHeld)
    {
      dy2 = 1;
    }
    else
    {
      dy2 = 0;
    }
  }
  else if(Y > 515 && !TConflict)
  {
    dy = -1;
    if(TireHeld)
    {
      dy2 = -1;
    }
    else
    {
      dy2 = 0;
    }
  }
  else
  {
    dy = 0;
    dy2 = 0;
  }
  XCoord = XCoord + dx;
  Tire1X = Tire1X + dx2;
  YCoord = YCoord + dy;
  Tire1Y = Tire1Y + dy2;
}

//----------------------------------------------------------------------------------------
//               Display Function
//----------------------------------------------------------------------------------------
void setscreen()
{
  display.clearDisplay();
  display.setCursor(XCoord,YCoord);
  display.fillCircle(XCoord, YCoord, 3, WHITE);
  display.drawBitmap(X3,Y3,GameBoard,72,32,WHITE);
  display.drawRect(Tire1X,Tire1Y,tireWid,tireHgt,WHITE);
  display.drawRect(Tire2X,Tire2Y,7,5,WHITE);
  display.drawRect(Tire3X,Tire3Y,7,5,WHITE);
  display.drawRect(Tire4X,Tire4Y,7,5,WHITE);
  display.display();
}

//----------------------------------------------------------------------------------------
//                 Checks for User Collisions With the Edges of the Display
//----------------------------------------------------------------------------------------
void checkWalls()
{
  if(XCoord <= 3)
  {
    LWallCollision = true;
  }
  else
  {
    LWallCollision = false;
  }
  if(XCoord >= 122)
  {
    RWallCollision = true;
  }
  else
  {
    RWallCollision = false;
  }
  if(YCoord <= 3)
  {
    TWallCollision = true;
  }
  else
  {
    TWallCollision = false;
  }
  if(YCoord >= 58)
  {
    BWallCollision = true;
  }
  else
  {
    BWallCollision = false;
  }
}

//----------------------------------------------------------------------------------------
//               Plays a Buzzer Tone to Inform User of Collision
//----------------------------------------------------------------------------------------
void playSound()
{
  if(ObjectCollision || CarCollision)
  {
    tone(buzzer, Tone);
  }
  else
  {
    noTone(buzzer);
  }
}

//----------------------------------------------------------------------------------------
//                 Checks for a User Collision with the Car Sprite
//----------------------------------------------------------------------------------------
void checkCarCollision()
{ 
  playSound();
  if(XCoord <= X3+75 && XCoord >= X3-3)
  {
    XObjCollision = true;
   if(YCoord == Y3-3)
    {
      TObjCollision = true;
    }
    else
    {
      TObjCollision = false;
    }
     if(YCoord == Y3+35)
    {
      BObjCollision = true;
    }
    else
    {
      BObjCollision = false;
    }
  }
  else
  {
    XObjCollision = false;
    BObjCollision = false;
    TObjCollision = false;
  }
  if(YCoord <= Y3 + 35 && YCoord >= Y3-3)
  {
    YObjCollision = true;
    if(XCoord == X3-3)
    {
      LObjCollision = true;
    }
    else
    {
      LObjCollision = false;
    }
     if(XCoord == X3+75)
    {
      RObjCollision = true;
    }
    else
    {
      RObjCollision = false;
    }
  }
  else
  {
   YObjCollision = false;
   RObjCollision = false;
   LObjCollision = false;
  }
  if(YObjCollision && XObjCollision)
  {
    ObjectCollision = true;
  }
  else
  {
    ObjectCollision = false;
  }
}

//----------------------------------------------------------------------------------------
//                   Checks if the User is Touching a Tire
//----------------------------------------------------------------------------------------
void checkTireCollision()
{
  if(XCoord <= Tire1X+10 && XCoord >= Tire1X-3 && YCoord <= Tire1Y+8 && YCoord >= Tire1Y -3)
  {
    T1_Collision = true;
  }
  else
  {
    T1_Collision = false;
  }
}

//----------------------------------------------------------------------------------------
//   Checks for Any Conflicts Which Would Prevent the User From Moving a Certain Direction
//----------------------------------------------------------------------------------------
void checkConflicts()
{
  if(ObjectCollision && RObjCollision)
  {
    LConflict = true;
  }
  else if(LWallCollision)
  {
    LConflict = true;
  }
  else if(LeftCarCollision)
  {
    LConflict = true;
  }
  else
  {
    LConflict = false;
  }
  if(ObjectCollision && LObjCollision)
  {
    RConflict = true;
  }
  else if(RWallCollision)
  {
    RConflict = true;
  }
  else if(RightCarCollision)
  {
    RConflict = true;
  }
  else
  {
    RConflict = false;
  }
  if(ObjectCollision && TObjCollision)
  {
    BConflict = true;
  }
  else if(BWallCollision)
  {
    BConflict = true;
  }
  else if(BottomCarCollision)
  {
    BConflict = true;
  }
  else
  {
    BConflict = false;
  }
  if(ObjectCollision && BObjCollision)
  {
    TConflict = true;
  }
  else if(TWallCollision)
  {
    TConflict = true;
  }
  else if(TopCarCollision)
  {
    TConflict = true;
  }
  else
  {
    TConflict = false;
  }
}

//----------------------------------------------------------------------------------------
//               Rotates tires to the right 90 degrees if a tire is held by the user
//----------------------------------------------------------------------------------------
void rotateRight()
{
  if(TireHeld)
  {
   if(tireState == 1)
   {
    Tire1X = XCoord+3;
    Tire1Y = YCoord-4;
    if(tireHgt == 5)
    {
     tireHgt = 7;
     tireWid = 5;
    }
    else
    {
     tireHgt = 5;
     tireWid = 7;
    }
    tireState = 2;
   }
  else if(tireState == 2)
  {
   Tire1X = XCoord-4;
   Tire1Y = YCoord-9;
   if(tireHgt == 5)
   {
     tireHgt = 7;
     tireWid = 5;
   }
   else
   {
      tireHgt = 5;
      tireWid = 7;
   }
   tireState = 3;
  }
  else if(tireState == 3)
  {
   Tire1X = XCoord-9;
   Tire1Y = YCoord-4;
   if(tireHgt == 5)
   {
     tireHgt = 7;
     tireWid = 5;
   }
   else
   {
      tireHgt = 5;
      tireWid = 7;
   }
   tireState = 4;
  }
  else if(tireState == 4)
  {
   Tire1X = XCoord-4;
   Tire1Y = YCoord+3;
    if(tireHgt == 5)
     {
      tireHgt = 7;
      tireWid = 5;
     }
     else
    {
        tireHgt = 5;
        tireWid = 7;
     }
    tireState = 1;
   }
  }
}

//----------------------------------------------------------------------------------------
//              Rotates tires to the left by 90 degrees if a tire is held by the user
//----------------------------------------------------------------------------------------
void rotateLeft()
{
  if(TireHeld)
  {
   if(tireState == 3)
   {
    Tire1X = XCoord+3;
    Tire1Y = YCoord-4;
    if(tireHgt == 5)
    {
     tireHgt = 7;
     tireWid = 5;
    }
    else
    {
     tireHgt = 5;
     tireWid = 7;
    }
    tireState = 2;
   }
  else if(tireState == 4)
  {
   Tire1X = XCoord-4;
   Tire1Y = YCoord-9;
   if(tireHgt == 5)
   {
     tireHgt = 7;
     tireWid = 5;
   }
   else
   {
      tireHgt = 5;
      tireWid = 7;
   }
   tireState = 3;
  }
  else if(tireState == 1)
  {
   Tire1X = XCoord-9;
   Tire1Y = YCoord-4;
   if(tireHgt == 5)
   {
     tireHgt = 7;
     tireWid = 5;
   }
   else
   {
      tireHgt = 5;
      tireWid = 7;
   }
   tireState = 4;
  }
  else if(tireState == 2)
  {
   Tire1X = XCoord-4;
   Tire1Y = YCoord+3;
    if(tireHgt == 5)
     {
      tireHgt = 7;
      tireWid = 5;
     }
     else
    {
        tireHgt = 5;
        tireWid = 7;
     }
    tireState = 1;
   }
  }
}

//----------------------------------------------------------------------------------------
//          Picks up or drops a tire depending on whether the user is already holding one
//----------------------------------------------------------------------------------------
void PickupTire()
{
  if(!TireHeld && T1_Collision)
  {
    Tire1X = XCoord - 3;
    Tire1Y = YCoord + 3;
    TireHeld = true;
  }
  else 
  {
    TireHeld = false;
  }
}

//----------------------------------------------------------------------------------------
//                   Resets the Game Board so it Can be Played Again
//----------------------------------------------------------------------------------------
void ResetGame()
{
  songIsPlaying = false;
  SongFinished = false;
  toneIsPlaying = false;
  toneComplete = false;
  toneStarted = false;
  End = false;
  Paused = false;
  RightCarCollision = false;  
  LeftCarCollision = false;
  TopCarCollision = false;
  BottomCarCollision = false;
  CarCollision = false;
  RWallCollision = false;
  LWallCollision = false;
  TWallCollision = false;
  BWallCollision = false;
  XObjCollision = false;
  YObjCollision = false;
  RObjCollision = false;
  LObjCollision = false;
  TObjCollision = false;
  BObjCollision = false;
  ObjectCollision = false;
  RConflict = false;
  LConflict = false;
  TConflict = false;
  BConflict = false;
  TireHeld = false;
  T1_Collision = false;
  T1_Set = false;
  T2_Set = false;
  T3_Set = false;
  T4_Set = false;
  XCoord = 10;
  YCoord = 10;
  X3 = 32;
  Y3 = 16;
  dx2 = 0;
  dy2 = 0;
  Tire1X = 120;
  Tire1Y = 10;
  Tire2X = 43;
  Tire2Y = 12;
  Tire3X = 43;
  Tire3Y = 47;
  Tire4X = 86;
  Tire4Y = 12;
  tireState = 1;
  tireWid = 7;
  tireHgt = 5;
}

//----------------------------------------------------------------------------------------
//            Checks if Tires are all in the correct location to win the game
//----------------------------------------------------------------------------------------
void checkTireSet()
{
  if(Tire1X == 86 && Tire1Y == 47)
  {
    T1_Set = true;
  }
  else
  {
    T1_Set = false;
  }
  if(Tire2X == 43 && Tire2Y == 12)
  {
    T2_Set = true;
  }
  else
  {
    T2_Set = false;
  }
  if(Tire3X == 43 && Tire3Y == 47)
  {
    T3_Set = true;
  }
  else
  {
    T3_Set = false;
  }
  if(Tire4X == 86 && Tire4Y == 12)
  {
    T4_Set = true;
  }
  else
  {
    T4_Set = false;
  }
}
