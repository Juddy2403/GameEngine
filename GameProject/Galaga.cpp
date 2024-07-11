#include "Galaga.h"

#include <SDL_rect.h>

#include "BulletTracker.h"
#include "DataStructs.h"
#include "GameCommands.h"
#include "Initializers.h"
#include "Minigin.h"
#include "Scene.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Game components/BackgroundComponent.h"
#include "Game components/FormationComponent.h"
#include "Game components/ModeSelectionComp.h"
#include "Game components/NameSelectionComp.h"
#include "Game components/ScoreComponent.h"
#include "Game observers/BulletObserver.h"
#include "Game observers/EnemyAIManager.h"
#include "Game observers/EnemyAttacksObserver.h"
#include "Game observers/EnemyObserver.h"
#include "Game observers/ExplosionObserver.h"
#include "Game observers/FighterObserver.h"
#include "Game observers/FormationObserver.h"
#include "Game observers/ScoreManager.h"
#include "Managers/InputManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Sound/DerivedSoundSystems.h"
#include "Sound/ServiceLocator.h"
#include "Subjects/GameObject.h"
#include "Trajectory Logic/Parsers.h"

#ifndef NDEBUG
#include "Game components/FPSComponent.h"
#endif

int Galaga::volume = baseVolume;

void Galaga::LoadStartScene()
{
    //----------SOUND----------------
#if NDEBUG
    GameEngine::ServiceLocator::RegisterSoundSystem(std::make_unique<GameEngine::SdlSoundSystem>());
#else
    GameEngine::ServiceLocator::RegisterSoundSystem(std::make_unique<GameEngine::LoggingSoundSystem>(std::make_unique<GameEngine::SdlSoundSystem>()));
#endif

    GameEngine::ServiceLocator::GetSoundSystem().FillSoundPaths("../Data/Audio/SoundPaths.txt");
    GameEngine::ServiceLocator::GetSoundSystem().PlaySound(static_cast<GameEngine::SoundId>(SoundId::start), Galaga::volume);

    GameEngine::SceneManager::GetInstance().AddScene(static_cast<int>(SceneId::startMenu), LoadStartScreen());
    m_CurrentScene = SceneId::startMenu;
    GameEngine::SceneManager::GetInstance().SetCurrentScene(static_cast<int>(SceneId::startMenu));

    // GameEngine::SceneManager::GetInstance().AddScene(static_cast<int>(SceneId::gameOver), LoadGameOverScene());
    // m_CurrentScene = SceneId::gameOver;
    // GameEngine::SceneManager::GetInstance().SetCurrentScene(static_cast<int>(SceneId::gameOver));
}
void Galaga::LevelCleared()
{
    switch(m_CurrentScene)
    {
    case SceneId::levelOne:
        ChangeScene(SceneId::levelTwo, LoadLevel("../Data/Formations/EnemyInfo2.json", "../Data/Formations/FormationTrajectories2.json"));
           break;
    case SceneId::levelTwo:
        ChangeScene(SceneId::levelThree, LoadLevel("../Data/Formations/EnemyInfo3.json", "../Data/Formations/FormationTrajectories3.json"));
        break;
    case SceneId::levelThree:
        ChangeScene(SceneId::gameOver, LoadGameOverScene());
        break;
    default: break;
    }
}
void Galaga::SetPlayerName(const std::string& name)
{
    m_PlayerName = name;
    auto score = ScoreManager::GetPlayerScore();
    
    std::ifstream inFile("../Data/HighestScores.txt");
    std::map<int,std::string> scores;

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string playerName;
        int playerScore;
        iss >> playerName >> playerScore;
        scores[playerScore] = playerName;
    }
    inFile.close();

    scores[score] = name;
    if(scores.size() > 10) scores.erase(scores.begin());

    std::ofstream outFile("../Data/HighestScores.txt");
    //write to the output file in descending order
    for(auto it = scores.rbegin(); it != scores.rend(); ++it)
    {
        outFile << it->second << " " << it->first << "\n";
    }

    if(scores.size() < 10)
    {
        int scoresLeft = 10 - static_cast<int>(std::ssize(scores));
        for(int i{}; i < scoresLeft; ++i)
            outFile << "AAA 0" << "\n";
    }
    outFile.close();
    
}
void Galaga::GameLost()
{
    ChangeScene(SceneId::gameOver, LoadGameOverScene());
}
void Galaga::ChooseName()
{
    ChangeScene(SceneId::chooseName, LoadChooseNameScene());
}
void Galaga::ChangeScene(SceneId sceneId, std::unique_ptr<GameEngine::Scene>&& scene)
{
    for(auto key : m_PrevKeyboardSceneKeys)
        GameEngine::InputManager::GetInstance().UnbindCommand(key);
    for(auto [key, controllerIdx] : m_PrevControllerSceneKeys)
        GameEngine::InputManager::GetInstance().UnbindCommand(key, controllerIdx);

    GameEngine::SceneManager::GetInstance().RemoveScene(static_cast<int>(m_CurrentScene));
    GameEngine::SceneManager::GetInstance().AddScene(static_cast<int>(sceneId), std::move(scene));
    GameEngine::SceneManager::GetInstance().SetCurrentScene(static_cast<int>(sceneId));
    m_CurrentScene = sceneId;
}
void Galaga::SetGameMode(GameMode mode)
{
    if(m_HasGameModeBeenSet) return;
    m_CurrentGameMode = mode;
    m_HasGameModeBeenSet = true;
    ChangeScene(SceneId::levelOne, LoadLevel("../Data/Formations/EnemyInfo1.json", "../Data/Formations/FormationTrajectories1.json"));
}
std::unique_ptr<GameEngine::Scene> Galaga::LoadLevel(const std::string& enemyInfoPath, const std::string& trajectoryInfoPath)
{
    auto scene = std::make_unique<GameEngine::Scene>();
    
    m_PrevKeyboardSceneKeys = std::move(m_KeyboardSceneKeys);
    m_KeyboardSceneKeys = { GameEngine::KeyboardInputKey::A, GameEngine::KeyboardInputKey::D, GameEngine::KeyboardInputKey::SPACE };

    //do the same for controller keys
    m_PrevControllerSceneKeys = std::move(m_ControllerSceneKeys);
    m_ControllerSceneKeys = { {GameEngine::ControllerInputKey::dpadLeft, 0}, {GameEngine::ControllerInputKey::dpadRight, 0}, {GameEngine::ControllerInputKey::X, 0} };
    
    //------BACKGROUND--------
    auto gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::texture));
    SDL_Rect& destRect = gameObject->AddComponent<GameEngine::TextureComponent>("Background.png")->m_DestRect;
    destRect.w = GameEngine::g_WindowRect.w;
    destRect.h = GameEngine::g_WindowRect.h;
    gameObject->AddComponent<BackgroundComponent>(gameObject->GetComponent<GameEngine::TextureComponent>(), 700);
    scene->AddObject(std::move(gameObject));

    auto font = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 16);
    auto smallerFont = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10);
    //------FPS--------
    #ifndef NDEBUG
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<FPSComponent>(gameObject->AddComponent<GameEngine::TextComponent>(smallerFont, "160 FPS"));
    gameObject->SetPosition(550, 20);
    scene->AddObject(std::move(gameObject));
    #endif

    //----------NR OF PLAYERS---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    std::string text{};
    if(m_CurrentGameMode == GameMode::singlePlayer) text = "1UP";
    else text = "2UP";
    gameObject->AddComponent<GameEngine::TextComponent>(font, text, SDL_Color{ 255,0,0 });
    gameObject->SetPosition(30, 10);
    scene->AddObject(std::move(gameObject));

    //----------SCORE---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<ScoreComponent>(gameObject->AddComponent<GameEngine::TextComponent>(font));
    gameObject->SetPosition(10, 30);
    scene->AddObject(std::move(gameObject));

    //----------HIGHEST SCORE---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextComponent>(font,"HIGHEST SCORE", SDL_Color{ 255,0,0 });
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->SetPosition(200, 10);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextComponent>(font,std::to_string(ScoreManager::GetHighestScore()));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->SetPosition(200, 30);
    scene->AddObject(std::move(gameObject));

    //--------FIGHTER--------
    auto explosionObserverUnique = std::make_unique<ExplosionObserver>(scene.get());
    auto explosionObserver = scene->AddObserver(-1, std::move(explosionObserverUnique), nullptr);

    gameObject = InitFighter();
    auto fighterObserver = std::make_unique<FighterObserver>();
    auto bulletObserver = std::make_unique<BulletObserver>(scene.get());
    scene->AddObserver(static_cast<int>(ObserverIdentifier::bullet), std::move(bulletObserver), gameObject.get());
    scene->AddObserver(-1, std::move(fighterObserver), gameObject.get());
    gameObject->AddObserver(-1, explosionObserver);
    auto playerComp = gameObject->GetComponent<PlayerComponent>();
    m_pPlayer = scene->AddObject(std::move(gameObject));
    m_pPlayer->GetComponent<PlayerComponent>()->BindCommands();

    //----------------ENEMIES--------------------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto enemyAIObserver = gameObject->AddComponent<EnemyAIManager>();
    scene->AddObject(std::move(gameObject));

    auto enemyObserverUnique = std::make_unique<EnemyObserver>();
    auto enemyObserver = scene->AddObserver(-1, std::move(enemyObserverUnique), nullptr);
    auto enemyAttackObserverUnique = std::make_unique<EnemyAttacksObserver>(scene.get());
    auto enemyAttackObserver = scene->AddObserver(static_cast<int>(ObserverIdentifier::enemyAttack), std::move(enemyAttackObserverUnique), nullptr);

    //--------- Enemy formation------------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    gameObject->AddComponent<FormationComponent>();
    scene->AddObject(std::move(gameObject));

    auto formationObserverUnique = std::make_unique<FormationObserver>();
    auto formationObserver = scene->AddObserver(static_cast<int>(ObserverIdentifier::formation),
        std::move(formationObserverUnique), nullptr);

    //--------- Enemy creation------------
    auto enemyVec = Parser::ParseEnemyInfoByStage(enemyInfoPath,
        trajectoryInfoPath, playerComp);
    for (auto& enemy : enemyVec)
    {
        enemy->AddObserver(-1, enemyObserver);
        enemy->AddObserver(static_cast<int>(ObserverIdentifier::formation), formationObserver);
        enemy->AddObserver(static_cast<int>(ObserverIdentifier::enemyAttack), enemyAttackObserver);
        enemy->AddObserver(-1, enemyAIObserver);
        enemy->AddObserver(-1, explosionObserver);
        scene->AddObject(std::move(enemy));
    }

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto& input = GameEngine::InputManager::GetInstance();
    if(m_CurrentGameMode == GameMode::versus)
    {
        input.BindCommand(GameEngine::ControllerInputKey::X,
            std::make_unique<BombingRunCommand>(gameObject.get()),0);
        m_ControllerSceneKeys.push_back({ GameEngine::ControllerInputKey::X, 0 });
        input.BindCommand(GameEngine::ControllerInputKey::Y,
            std::make_unique<ShootBeamCommand>(gameObject.get()),0);
        m_ControllerSceneKeys.push_back({ GameEngine::ControllerInputKey::Y, 0 });
    }
    input.BindCommand(GameEngine::KeyboardInputKey::M,
        std::make_unique<MuteCommand>(gameObject.get()));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::M);
    
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    input.BindCommand(GameEngine::KeyboardInputKey::F1,
        std::make_unique<SkipLevelCommand>(gameObject.get()));
    scene->AddObject(std::move(gameObject));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::F1);
    
    //erase everything from the previous keyboard scene keys that match the current keyboard scene keys
    for(auto key : m_KeyboardSceneKeys)
        std::erase(m_PrevKeyboardSceneKeys, key);
    for(auto [key, controllerIdx] : m_ControllerSceneKeys)
        std::erase(m_PrevControllerSceneKeys, std::pair{key, controllerIdx});
    
    return scene;
}
std::unique_ptr<GameEngine::Scene> Galaga::LoadStartScreen()
{
    auto scene = std::make_unique<GameEngine::Scene>();

    //------BACKGROUND--------
    auto gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::texture));
    SDL_Rect& destRect = gameObject->AddComponent<GameEngine::TextureComponent>("Background.png")->m_DestRect;
    destRect.w = GameEngine::g_WindowRect.w;
    destRect.h = GameEngine::g_WindowRect.h;
    gameObject->AddComponent<BackgroundComponent>(gameObject->GetComponent<GameEngine::TextureComponent>(), 700);
    scene->AddObject(std::move(gameObject));

    //------TITLE--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::texture));
    gameObject->AddComponent<GameEngine::TextureComponent>("Title.png");
    gameObject->SetPosition(200, 50);
    scene->AddObject(std::move(gameObject));

    auto font = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 20);

    //------SWITCH MODES OBJ--------
    auto switchModesObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto switchModeComp = switchModesObject->AddComponent<ModeSelectionComp>();

    //----------Play modes---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    auto textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "Single player");
    switchModeComp->AddTextComponent(textComp, GameMode::singlePlayer);
    gameObject->SetPosition(200, 250);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "Coop");
    switchModeComp->AddTextComponent(textComp, GameMode::coop);
    gameObject->SetPosition(200, 300);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "Versus");
    switchModeComp->AddTextComponent(textComp, GameMode::versus);
    gameObject->SetPosition(200, 350);
    scene->AddObject(std::move(gameObject));

    auto& input = GameEngine::InputManager::GetInstance();
    input.BindCommand(GameEngine::KeyboardInputKey::UP,
        std::make_unique<SwitchModesCommand>(switchModesObject.get(), true));
    input.BindCommand(GameEngine::KeyboardInputKey::DOWN,
        std::make_unique<SwitchModesCommand>(switchModesObject.get(), false));
    input.BindCommand(GameEngine::KeyboardInputKey::ENTER,
        std::make_unique<SelectModeCommand>(switchModesObject.get()));

    input.BindCommand(GameEngine::ControllerInputKey::dpadUp,
        std::make_unique<SwitchModesCommand>(switchModesObject.get(), true),0);
    input.BindCommand(GameEngine::ControllerInputKey::dpadDown,
        std::make_unique<SwitchModesCommand>(switchModesObject.get(), false),0);
    input.BindCommand(GameEngine::ControllerInputKey::A,
        std::make_unique<SelectModeCommand>(switchModesObject.get()),0);
    scene->AddObject(std::move(switchModesObject));
    
    m_KeyboardSceneKeys = { GameEngine::KeyboardInputKey::UP, GameEngine::KeyboardInputKey::DOWN, GameEngine::KeyboardInputKey::ENTER };
    m_ControllerSceneKeys = { {GameEngine::ControllerInputKey::dpadUp, 0}, {GameEngine::ControllerInputKey::dpadDown, 0}, {GameEngine::ControllerInputKey::A, 0} };

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    input.BindCommand(GameEngine::KeyboardInputKey::M,
        std::make_unique<MuteCommand>(gameObject.get()));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::M);
    scene->AddObject(std::move(gameObject));
    
    return scene;
}
std::unique_ptr<GameEngine::Scene> Galaga::LoadGameOverScene()
{
    auto scene = std::make_unique<GameEngine::Scene>();
    m_PrevKeyboardSceneKeys = std::move(m_KeyboardSceneKeys);
    m_PrevControllerSceneKeys = std::move(m_ControllerSceneKeys);

    //------BACKGROUND--------
    auto gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::texture));
    SDL_Rect& destRect = gameObject->AddComponent<GameEngine::TextureComponent>("Background.png")->m_DestRect;
    destRect.w = GameEngine::g_WindowRect.w;
    destRect.h = GameEngine::g_WindowRect.h;
    gameObject->AddComponent<BackgroundComponent>(gameObject->GetComponent<GameEngine::TextureComponent>(), 700);
    scene->AddObject(std::move(gameObject));

    auto font = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 20);
    auto smallerFont = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 16);

    //------TITLE--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<GameEngine::TextComponent>(font, "- RESULT -")->SetColor({255, 0, 0,255});
    gameObject->SetPosition(200, 150);
    scene->AddObject(std::move(gameObject));

    auto bulletsFired = BulletTracker::GetBulletsFired();
    auto bulletsHit = BulletTracker::GetBulletsHit();
    auto hitMissRatio = static_cast<float>(bulletsHit) / bulletsFired * 100.f;
    if(bulletsFired == 0) hitMissRatio = 0;
    std::stringstream hitMissStr{};
    hitMissStr << std::fixed << std::setprecision(1) << hitMissRatio<<"%";
    //------SHOTS FIRED--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<GameEngine::TextComponent>(font, "SHOTS FIRED       "+ std::to_string(bulletsFired))->SetColor({0, 0, 255,255});
    gameObject->SetPosition(50, 200);
    scene->AddObject(std::move(gameObject));

    //------NUMBER OF HITS--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<GameEngine::TextComponent>(font, "NUMBER OF HITS    "+ std::to_string(bulletsHit));
    gameObject->SetPosition(50, 250);
    scene->AddObject(std::move(gameObject));

    //------HIT-MISS RATIO--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<GameEngine::TextComponent>(font, "HIT-MISS RATIO    "+ hitMissStr.str())->SetColor({255, 255, 0,255});
    gameObject->SetPosition(50, 300);
    scene->AddObject(std::move(gameObject));

    //----------NR OF PLAYERS---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    std::string text{};
    if(m_CurrentGameMode == GameMode::singlePlayer) text = "1UP";
    else text = "2UP";
    gameObject->AddComponent<GameEngine::TextComponent>(smallerFont, text, SDL_Color{ 255,0,0 });
    gameObject->SetPosition(30, 10);
    scene->AddObject(std::move(gameObject));

    //----------SCORE---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<ScoreComponent>(gameObject->AddComponent<GameEngine::TextComponent>(smallerFont));
    gameObject->SetPosition(10, 30);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto& input = GameEngine::InputManager::GetInstance();
    input.BindCommand(GameEngine::KeyboardInputKey::M,
        std::make_unique<MuteCommand>(gameObject.get()));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::M);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    input.BindCommand(GameEngine::KeyboardInputKey::ENTER,
        std::make_unique<LoadChooseNameCommand>(gameObject.get()));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::ENTER);
    scene->AddObject(std::move(gameObject));

    //erase everything from the previous keyboard scene keys that match the current keyboard scene keys
    for(auto key : m_KeyboardSceneKeys)
        std::erase(m_PrevKeyboardSceneKeys, key);

    //do the same for controller keys
    for(auto [key, controllerIdx] : m_ControllerSceneKeys)
        std::erase(m_PrevControllerSceneKeys, std::pair{key, controllerIdx});
    
    return scene;
}

std::unique_ptr<GameEngine::Scene> Galaga::LoadChooseNameScene()
{
    auto scene = std::make_unique<GameEngine::Scene>();

    //------BACKGROUND--------
    auto gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::texture));
    SDL_Rect& destRect = gameObject->AddComponent<GameEngine::TextureComponent>("Background.png")->m_DestRect;
    destRect.w = GameEngine::g_WindowRect.w;
    destRect.h = GameEngine::g_WindowRect.h;
    gameObject->AddComponent<BackgroundComponent>(gameObject->GetComponent<GameEngine::TextureComponent>(), 700);
    scene->AddObject(std::move(gameObject));

    auto font = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 20);
    auto smallerFont = GameEngine::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 16);

    //------TITLE--------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<GameEngine::TextComponent>(font, "HIGHEST SCORES");
    gameObject->SetPosition(200, 50);
    scene->AddObject(std::move(gameObject));
    

    //----------SCORE---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    gameObject->AddComponent<ScoreComponent>(gameObject->AddComponent<GameEngine::TextComponent>(smallerFont));
    gameObject->SetPosition(10, 30);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto& input = GameEngine::InputManager::GetInstance();
    input.BindCommand(GameEngine::KeyboardInputKey::M,
        std::make_unique<MuteCommand>(gameObject.get()));
    m_KeyboardSceneKeys.push_back(GameEngine::KeyboardInputKey::M);
    scene->AddObject(std::move(gameObject));

    //-----------------NAME SELECTION----------------
    auto nameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::misc));
    auto nameSelectionComp = nameObject->AddComponent<NameSelectionComp>();
    
    //----------Play modes---------
    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    auto textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "A");
    nameSelectionComp->AddTextComponent(textComp);
    gameObject->SetPosition(200, 500);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "A");
    nameSelectionComp->AddTextComponent(textComp);
    gameObject->SetPosition(220, 500);
    scene->AddObject(std::move(gameObject));

    gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
    gameObject->AddComponent<GameEngine::TextureComponent>();
    textComp = gameObject->AddComponent<GameEngine::TextComponent>(font, "A");
    nameSelectionComp->AddTextComponent(textComp);
    gameObject->SetPosition(240, 500);
    scene->AddObject(std::move(gameObject));

    std::ifstream inFile("../Data/HighestScores.txt");
    std::string line;
    float yPosition = 100.f; // starting y position for the scores
    int index = 0; // index for the scores

    while (std::getline(inFile, line) && index < 10)
    {
        std::istringstream iss(line);
        std::string playerName;
        int playerScore;
        iss >> playerName >> playerScore;

        gameObject = std::make_unique<GameEngine::GameObject>(static_cast<int>(GameId::text));
        gameObject->AddComponent<GameEngine::TextureComponent>();
        gameObject->AddComponent<GameEngine::TextComponent>(font, playerName + " " + std::to_string(playerScore));
        gameObject->SetPosition(200, yPosition);
        scene->AddObject(std::move(gameObject));

        yPosition += 30; // increment y position for the next score
        ++index; // increment the index
    }
    inFile.close();

    input.BindCommand(GameEngine::KeyboardInputKey::UP,
        std::make_unique<SwitchNameCommand>(nameObject.get(), true));
    input.BindCommand(GameEngine::KeyboardInputKey::DOWN,
        std::make_unique<SwitchNameCommand>(nameObject.get(), false));
    input.BindCommand(GameEngine::KeyboardInputKey::ENTER,
        std::make_unique<SelectNameCommand>(nameObject.get()));

    input.BindCommand(GameEngine::ControllerInputKey::dpadUp,
        std::make_unique<SwitchNameCommand>(nameObject.get(), true),0);
    input.BindCommand(GameEngine::ControllerInputKey::dpadDown,
        std::make_unique<SwitchNameCommand>(nameObject.get(), false),0);
    input.BindCommand(GameEngine::ControllerInputKey::A,
        std::make_unique<SelectNameCommand>(nameObject.get()),0);
    scene->AddObject(std::move(nameObject));
    
    return scene;
}
