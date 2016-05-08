#pragma once


class TemplateScene : public AEScene {

public:

    TemplateScene(AERO_SCENE_DESC desc);

    /* ---------- Inheriting ----------------- */
    VOID update();
    VOID render(INT renderMode = RENDER_ALL);
    VOID processInput();
    VOID processCollision();
    /* ---------- End of Inheriting ---------- */

    VOID initialize();

private:



};