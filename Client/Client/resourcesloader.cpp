#include "resourcesloader.h"

namespace NeonHockey
{    
    ResourcesLoader::ResourcesLoader(std::shared_ptr<ResourceManager> rm)
        : _rm(rm)
    {
        //REVIEW: надо ассоциировать с типом сразу
        _gfxResources.emplace_back(SpriteInfo(0, 0, 800, 600, "../resources/Field.png"));
        _gfxResources.emplace_back(SpriteInfo(0, 0, 64, 64, "../resources/Puck.png"));
        _gfxResources.emplace_back(SpriteInfo(0, 0, 64, 64, "../resources/Paddle.png"));

        _gfxResources.emplace_back(SpriteInfo(0, 0, 160, 32, "../resources/InputField.png"));
        _gfxResources.emplace_back(SpriteInfo(0, 0, 100, 32, "../resources/Button.png"));
    }

    void ResourcesLoader::loadEverything()
    {
        loadGraphics();
#ifndef PLATFORM_UNIX
        loadSound();
#endif
        loadFonts();
    }

    void ResourcesLoader::loadGraphics()
    {
        const int backgroundId = 0;
        const int puckId = 1;
        const int paddleId = 2;
        const int inputFieldId = 3;
        const int buttonId = 4;

        _rm->addTexture(GfxType::BACKGROUND, _gfxResources[backgroundId].texturePath());
        _rm->addTexture(GfxType::PUCK, _gfxResources[puckId].texturePath());
        _rm->addTexture(GfxType::PADDLE, _gfxResources[paddleId].texturePath());
        _rm->addTexture(GfxType::INPUTFIELD, _gfxResources[inputFieldId].texturePath());
        _rm->addTexture(GfxType::BUTTON, _gfxResources[buttonId].texturePath());


        _rm->addSprite(GfxType::BACKGROUND, _gfxResources[backgroundId])->SetHotSpot(0, 0);
        _rm->addSprite(GfxType::PUCK, _gfxResources[puckId])->SetHotSpot(_gfxResources[puckId].width() / 2,
                                                                        _gfxResources[puckId].height() / 2);

        _rm->addSprite(GfxType::INPUTFIELD, _gfxResources[inputFieldId]);

        _rm->addSprite(GfxType::PADDLE, _gfxResources[paddleId])->SetHotSpot(_gfxResources[paddleId].width() / 2,
                                                                            _gfxResources[paddleId].height() / 2);

    }

    void ResourcesLoader::loadFonts()
    {
        _rm->addFont(FontType::SCORE, "../resources/Digital.fnt");
        _rm->addFont(FontType::GOAL, "../resources/Digital.fnt");
        _rm->addFont(FontType::MENU, "../resources/Digital.fnt");
        _rm->addFont(FontType::STATUSBAR, "../resources/Digital.fnt");
        _rm->addFont(FontType::UI, "../resources/Digital.fnt");
    }

    void ResourcesLoader::loadSound()
    {
        _rm->addSound(SoundType::COLLISION, "../resources/hit.ogg");
        _rm->addSound(SoundType::GOAL, "../resources/goal.ogg");
    }

}
