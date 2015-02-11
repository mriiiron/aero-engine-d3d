#include <d3d11_1.h>
#include "AEMath.h"
#include "AECamera.h"

AECamera::AECamera() {
    mode = 0;
    focus = { 0.0f, 0.0f };
    adjustedFocus = { 0.0f, 0.0f };
    vx = vy = 0.0f;
    target = nullptr;
    shakeTimer = shakeAmp = 0;
}

VOID AECamera::update() {
    if (shakeTimer > 0) {
        adjustedFocus.x = focus.x;
        adjustedFocus.y = focus.y + shakeAmp * (2.0f * (shakeTimer % 6) / 3 - 1.0f);
        shakeTimer--;
    }
    else {
        adjustedFocus = focus;
    }
}

VOID AECamera::shake(INT time, INT amplitude) {
    shakeTimer = time;
    shakeAmp = amplitude;
}