#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>

#define ButtonPin 2
#define DiscoPin 3
#define SpeakerPin 9

TMRpcm tmrpcm;
File root;

void setup() {
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(DiscoPin, OUTPUT);

  // Use Serial for debugging.
  Serial.begin(9600);
  Serial.println("Initialising SD.");
  
  if (!SD.begin(4)) {
    Serial.println("SD failed.");
    while(true); // Stay here if the SD fails to initialise.
  }
  Serial.println("SD success.");
  
  tmrpcm.speakerPin = 9;
  
  root = SD.open("/");
  tmrpcm.setVolume(6);
  tmrpcm.quality(0);
}

void loop() {
  if (!tmrpcm.isPlaying() ) {
    Serial.println("No music");
    // no file being played.
    if (digitalRead(ButtonPin) == LOW) {
      Serial.println("Wait for button.");
      while(debounce());
      Serial.println("Playing");
      tmrpcm.play("rickroll.wav");
      digitalWrite(DiscoPin, HIGH);
    }
  } else {
    if (digitalRead(ButtonPin) == LOW) {
      while(debounce());
      // If the button is pressed again, stop playing music.
      tmrpcm.stopPlayback();
      // Write both pins low to turn of the lights and stop the speaker from making a terrible whining noise.
      digitalWrite(SpeakerPin, LOW);
      digitalWrite(DiscoPin, LOW);
      return;
    }
  }
  
}

bool debounce() {
  // Give time for a button to be pressed and released by the user- limiting the rate at which the function is executed.
  byte count = 0;
  for (byte i = 0; i < 5; i++) {
    if (digitalRead(ButtonPin) == LOW) count++;
    delay(10);
  }
  if (count > 2) return 1;
  return 0;
}
