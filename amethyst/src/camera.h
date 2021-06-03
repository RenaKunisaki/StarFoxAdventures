typedef enum {
    CAM_FLAG_PAD3     = 0x01, //use controller 3 to move
    CAM_FLAG_INVERT_X = 0x02, //invert X axis movement
    CAM_FLAG_INVERT_Y = 0x04, //invert Y axis movement
    CAM_FLAG_NO_HUD   = 0x08, //disable the HUD
} CameraFlags;

typedef enum {
    CAM_MODE_NORMAL = 0,
    CAM_MODE_STAY,
    CAM_MODE_FREE,
    NUM_CAM_MODES,
} CameraMode;

//turns out gamecube had joycon drift before joycons did
#define CAMERA_LSTICK_DEADZONE 4
#define CAMERA_RSTICK_DEADZONE 4
#define CAMERA_TRIGGER_DEADZONE 4

#define S16_TO_RADIANS (1.0 / 10430.37835)

//camera.c
extern u8 cameraFlags; //CameraFlags
extern s8 debugCameraMode; //CameraMode
void cameraUpdateHook();
int padGetCxHook(int pad);
int padGetCyHook(int pad);
int padGetStickXHook(int pad);
int padGetStickYHook(int pad);
u32 minimapButtonHeldHook(int pad);
u32 minimapButtonPressedHook(int pad);
int viewFinderZoomHook(int pad);
