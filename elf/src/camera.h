typedef enum {
    CAM_FLAG_PAD3     = 0x01, //use controller 3 to move
    CAM_FLAG_INVERT_X = 0x02, //invert X axis movement
    CAM_FLAG_INVERT_Y = 0x04, //invert Y axis movement
    CAM_FLAG_NO_HUD   = 0x08, //disable the HUD
} CameraFlags;

//camera.c
extern u8 cameraFlags; //CameraFlags
float cameraUpdateHook();
int padGetCxHook(int pad);
int padGetCyHook(int pad);
int padGetStickXHook(int pad);
int padGetStickYHook(int pad);
u32 minimapButtonHeldHook(int pad);
u32 minimapButtonPressedHook(int pad);
int viewFinderZoomHook(int pad);
