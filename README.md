# What is a laser harp?
A laser harp is a harp that has laser beams emulating the strings of a regular harp. Whenever you intercept a laser beam, a note is played to that correspondent intercepted beam.

# Cool features in our laser harp
- The notes are played by an Android app;
- It's wireless! It communicates with the app via Bluetooth;
- There are ultrasonic range finders that measure the height of your interception;
  - That makes it possible to play 3 notes in a single laser beam.
- There are two modes of operation:
  - Free Play: let your inner Jean Michel Jarre shine (:
  - Learn To Play: learn to play a song in the harp
- RGB LED strips positioned in the front of each laser beam
  - They lit to the note you play when you intercept a laser beam on Free Play mode
  - They lit to the correspondent string and height to be intercepted in the current music

# Pics or it didn't happen
Yes, there's a demo video! Too bad we didn't had the time to learn how to play the harp (:

https://www.youtube.com/watch?v=IKeanOgFvdw

# Bill of materials
- Tiva C Launchpad
- JY-MCU Bluetooth module
- 8 HC-SR04 ultrasonic range finders
- 8 5mW laser diodes
- 8 interception sensors (circuit schematic found inside the repo)
  - comparator IC
  - potentiometer
  - LDR (Light dependent resistor)
  - 0.25W resistors
  - Materials to build a PCB to each interception sensor
- 8 12V RGB LED strips
- 3 high current drivers IC
- 3 shift registers

# Makers
- José Augustinho
  - Firmware programming, PCB confection and project manager
- Rinaldi Segecin
  - Mobile app development, firmware programming
- Diogo Freitas (myself)
  - Hardware development, PCB confection, firmware debugging
