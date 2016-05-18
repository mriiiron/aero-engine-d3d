#pragma once


class D2Keys {

public:

    static const UINT KEY_LEFT = 0x1;
    static const UINT KEY_RIGHT = 0x2;
    static const UINT KEY_JUMP = 0x4;

};


class D2Scene : public AEScene {

public:

    static const FLOAT GRAVITY;
    static const FLOAT FRACTION;

    D2Scene(AERO_SCENE_DESC desc);

    /* ---------- Inheriting ----------------- */
    VOID update();
    VOID render(INT renderMode = RENDER_ALL);
    VOID processInput();
    VOID processCollision();
    /* ---------- End of Inheriting ---------- */

    VOID initialize();

protected:

    D2Sprite* player = nullptr;
    INT dik_left, dik_right, dik_jump;

};