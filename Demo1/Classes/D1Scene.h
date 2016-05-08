#pragma once


class D1Scene : public AEScene {

public:

    D1Scene(AERO_SCENE_DESC desc);

    /* ---------- Inheriting ----------------- */
    VOID update();
    VOID render(INT renderMode = RENDER_ALL);
    VOID processInput();
    VOID processCollision();
    /* ---------- End of Inheriting ---------- */

    VOID initialize();
    VOID reset();
    VOID setPlayer(D1PlayerFighter* _player) { player = _player; }
    D1PlayerFighter* getPlayer() { return player; }
    INT getLevel() { return level; }
    INT getScore() { return score; }
    BOOL isGameOver() { return (gameOverTimer <= 0); }
    BOOL isPlayerKilled() { return (player->getAction() > D1PlayerFighter::ACTION_KILLED); }
    BOOL isGameStarted() { return isStarted; }

    INT getEnemyCount();
    VOID addEnemy();

protected:

    D1PlayerFighter* player;
    INT dik_left, dik_right, dik_up, dik_down, dik_attack, dik_bomb;

    INT level, levelUpTimer;
    INT score;
    INT addEnemyTimer;
    INT gameOverTimer;
    BOOL isStarted;

};