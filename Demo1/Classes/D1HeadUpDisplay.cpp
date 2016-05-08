#include "AEroEngine.h"
#include "D1Sprite.h";
#include "D1Scene.h"
#include "D1HeadUpDisplay.h"
#include "SpriteFont.h"

extern SpriteBatch*                         xtk_SpriteBatch;
extern SpriteFont*                          xtk_SpriteFont_Arial_10;


D1HeadUpDisplay::D1HeadUpDisplay(INT spriteTableSize) : AEHeadUpDisplay(spriteTableSize) {

}

VOID D1HeadUpDisplay::render() {

    D1Scene* d1scene = dynamic_cast<D1Scene*>(scene);
    D1PlayerFighter* player = d1scene->getPlayer();

    LPTSTR str = new TCHAR[1024];

    if (d1scene->isGameStarted()) {
        wsprintf(str, L"Score: %d", d1scene->getScore());
        xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, str, XMFLOAT2(-143.0f, -195.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
        wsprintf(str, L"Level: %d", d1scene->getLevel());
        xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, str, XMFLOAT2(93.0f, -195.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
        wsprintf(str, L"Armor: %d", player->getHP());
        xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, str, XMFLOAT2(-30.0f, 180.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
        if (d1scene->isGameOver()) {
            xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, L"GAME OVER", XMFLOAT2(-42.0f, -20.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
            wsprintf(str, L"Your Score: %d", d1scene->getScore());
            xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, str, XMFLOAT2(-50.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
            xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, L"Press Z to Replay", XMFLOAT2(-54.0f, 20.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }
    else {
        xtk_SpriteFont_Arial_10->DrawString(xtk_SpriteBatch, L"Press Z to Start", XMFLOAT2(-45.0f, -5.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    }
    
    AEHeadUpDisplay::render();
}