
#include "eyebot++.h"
#include <string>

#define SPEED 300
#define DIST2WALL 0.15 * 1e3 //[mm]

void aligning2Wall();
void moveForward2Wall();
void followWall(std::string sideLR);
void cornerTurning();

int main()
{
  LCDPrintf("Welcome to the Lawnmover demonstration \n");
  LCDMenu("RUN", "STOP", "MOVE ON", "END");
  KEYWait(KEY1);

  // move robot to starting position in the corner
  moveForward2Wall();
  aligning2Wall();
  followWall("left");
  cornerTurning();

  // start lawn-mowing-pattern
  LCDPrintf("Start lawn moving pattern \n");
  followWall("left");
  cornerTurning();
  VWSetSpeed(SPEED, 0);
  int dt = 1 * 1e3; // move 1 sec
  OSWait(dt);
  VWTurn(-90, 90); // turn right / clock-wise
  VWWait();
  moveForward2Wall();

  VWTurn(90, 90); // turn left / counter-clock-wise
  VWSetSpeed(SPEED, 0);
  OSWait(dt);
  VWTurn(90, 90); // turn left
  VWWait();
  moveForward2Wall();

  KEYWait(KEY4);
  return 0;
}

//***************************************************************************//

void moveForward2Wall()
{
  // move straight in front direction until wall within 15 cm distance
  VWSetSpeed(SPEED, 0); // drive straight 300mm/s
  LCDPrintf("--- moving forward---\n");
  while (PSDGet(PSD_FRONT) > DIST2WALL)
  {
    OSWait(100);
  }
  VWSetSpeed(0, 0);
}

void aligning2Wall()
{
  int distL = PSDGet(PSD_LEFT);
  int distR = PSDGet(PSD_RIGHT);
  int distF = PSDGet(PSD_FRONT);
  LCDPrintf("Distances: L = %3d, R = %3d, F = %3d\n", distL, distR, distF);

  // calculate angle between robot's path and wall
  if (distL > distR)
  {
    LCDPrintf("L > R \n");
    float alpha = atan2(distR, distF) * 180 / M_PI;
    LCDPrintf("alpha = %2f\n", alpha);
    // rotate to be parallel to wall
    float beta = 90 - alpha;
    VWTurn(-(90 + beta), 90);
    VWWait();
  }
  else if (distR > distL)
  {
    LCDPrintf("R > L \n");
    float alpha = atan2(distL, distF) * 180 / M_PI;
    LCDPrintf("alpha = %2f\n", alpha);
    // rotate to be parallel to wall
    VWTurn(-alpha, 90);
    VWWait();
  }
}

void followWall(std::string sideLR)
{
  int sideDist;
  if (sideLR == "left")
  {
    sideDist = PSDGet(PSD_LEFT);
  }
  else if (sideLR == "right")
  {
    sideDist = PSDGet(PSD_RIGHT);
  }
  // move straight in front direction until wall within 15 cm distance
  VWSetSpeed(SPEED, 0); // drive straight 300mm/s
  LCDPrintf("--- move along wall---\n");
  while (PSDGet(PSD_FRONT) > DIST2WALL)
  {
    OSWait(10); // OSWait(10)

    if (sideDist > 1 * DIST2WALL) // too far away
    {
      LCDPrintf("too far away\n");
      VWTurn(10, 90); // turn counter-clock-wise  (10, 90)
      VWWait();
      VWSetSpeed(SPEED, 0);
    }
    else if (sideDist < 0.75 * DIST2WALL) // too close
    {
      LCDPrintf("too close\n");
      VWTurn(-1, 90); // turn clock-wise  (1, 90)
      VWWait();
      VWSetSpeed(SPEED, 0);
    }
  }
  VWSetSpeed(0, 0);
}

void cornerTurning()
{
  LCDPrintf("--- Turning in corner ---\n");
  int distF;
  VWTurn(-80, 90);
  VWWait();
  do
  {
    distF = PSDGet(PSD_FRONT);
    VWTurn(-0.1, 90);
    VWWait();
    LCDPrintf("Distances: F = %3d, dF = %3d\n", PSDGet(PSD_FRONT), abs(distF - PSDGet(PSD_FRONT)));

  } while (abs(distF - PSDGet(PSD_FRONT)) > 2);
}
