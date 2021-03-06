#include "game/ICharacter.h"
#include "game/WarriorDummy.h"
#include "game/MeleeBattle.h"
#include "game/BattleProducer.h"
#include "game/BattleConsumer.h"
#include "managers/JsonManager.h"
#define JSONPATH "./assets/main.json"
#include "utils/OsUtils.h"
#include "utils/json.hpp"
#include "utils/string-utils.hpp"
#include "utils/log/loguru.cpp"
#include "utils/filesystem.hpp"
#include "utils/pch.h"

#include <vector>

// namespaces
using json = nlohmann::json;
namespace fs = ghc::filesystem;

// EK: function to execute a simple battle
// TODO: move as a part of GameManager class that will decide on which function to run
void BattleStarter(std::shared_ptr<IBattle> pBattle)
{
    if(pBattle.get())
    {
        pBattle->BeginFight();
        pBattle->LogResults();
    }
    else
    {
        LOG_F(ERROR, "Error incorrect battle pointer has passed.");
    }
}

int main(int argc, char* argv[])
{
	// YM: Initializing log library
	loguru::init(argc, argv);
	loguru::add_file("simulate.io.txt", loguru::Append, loguru::Verbosity_MAX);
	LOG_F(INFO, "Starting simulate.io");

	// TODO: move as a part of SimulationManager
	OsUtils osUtils = OsUtils();

    // YM: simple json manager implementation with no ownership on the data
    // TODO: move JsonManager as part of SimulationManager
    JsonManager p_jsonManager;

    // YM: Importing main.json
    std::error_code error;
    bool bExists = fs::exists(JSONPATH, error);
    fs::exists(JSONPATH, error);
	assert( bExists && "./assets/main.json not found!");
    // YM: the return type is shared_ptr use .get() or .release();
    p_jsonManager.Init(JSONPATH);

    Battles_vec_ptr jsonData = p_jsonManager.Get();


    // TODO: move as a part of GameManager class that will decide on which characters to send
    WarriorDummy attacker;
    WarriorDummy defender;
    MeleeBattle battle(std::move(attacker), std::move(defender));
    FightersPair_t fighterPair(attacker, defender);
    std::vector<FightersPair_t> fighterPairsVect;
    fighterPairsVect.emplace_back(fighterPair);

    // KE: Tasks Producer to work on the characters battles to be run
	BattleProducer producer(fighterPairsVect, &BattleStarter);
    producer.CreateWork();
	BattlePackageTaskVector battlesToRun = producer.GetBattleQueue();

	// KE: Create consumer and run all the battles
    BattleConsumer consumer(battlesToRun);
    consumer.RunBattles(osUtils.GetAvailableThreads());
	

	return 0;
}
