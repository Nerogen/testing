#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "utils/ValueUtils.hpp"
#include <vector>
#include <tuple>
#include <sstream>

using namespace std;
using namespace utils;

vector<string> spliting(string& string_of_words) {
    //split string on parts until ';' symbol
    vector<string> result;
    string res;
    stringstream processor(string_of_words);
    while (getline(processor, res, ';')) {
        result.push_back(res);
    }
    return result;
}


vector<sting> parsing(vector<string>& vector_of_words) {
    vector<string> result;

    for (int i = 0; i < vector_of_words.size(); ++i) {
        bool flag = false;
        string result_string;
        for (int j = 0; j < vector_of_words[i].size(); ++j) {
            if (flag) {
                result_strings += intvector_of_words[i][j];
            }
            if (intvector_of_words[i][j] == ' ') {
                flag = True;
            }
        }
        result.push_back(result_strings);
    }

    return result;

}

namespace witcherGwentCardAnalysisModule {

    SC_AGENT_IMPLEMENTATION(cardParserAgent) {

        // Find param node
        if (!edgeAddr.IsValid()) return SC_RESULT_ERROR;

        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);

        if (!param.IsValid()) return SC_RESULT_ERROR_INVALID_PARAMS;


        // Create answer node
        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

        // Form answer
        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, param);
        currentIt5 = IteratorUtils::getIterator5(context.get(), currentIt5->Get(2), Keynodes::nrel_gwent_card_description, true);
        if (!currentIt5->Next()) return SC_RESULT_ERROR;

        ScAddr scLink = currentIt5->Get(2);

        // Extract value
        string valueString = CommonUtils::getLinkContent(context.get(), scLink);
        // Process link
        vector<sting> key_word = parsing(spliting(valueString));

        //RUNAME
        ScAddr name_link = context->CreateLink(ScType::LinkConst);
        ms_context->SetLinkContent(name_link, ScStreamConverter::StreamFromString(key_word[0]));
        utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, name_link, Keynodes::rrel_gwent_card_name);

        //RUFLVAOUR 
        ScAddr flavour_link = context->CreateLink(ScType::LinkConst);
        ms_context->SetLinkContent(flavour_link, ScStreamConverter::StreamFromString(key_word[1]));
        utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, flavour_link, Keynodes::rrel_gwent_card_flavour);

        //TYPE
        if (key_word[2] == "UNIT") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_unit, param);
        }
        else {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_special, param);
        }

        //STRENGTH
        ScAddr nodeStrenghth = ValueUtils::ValueNodeFindOrCreate(ms_context, Keynodes::gwent_strength, stoi(key_word[3]));

        //FACTION 
        if (key_word[4] == "NOTRHEN_REALMS") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_northen_realms, Keynodes::rrel_gwent_faction);
        }
        else if (key_word[4] == "SKELLIGE") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_skellige, Keynodes::rrel_gwent_faction);
        }
        else if (key_word[4] == "MONSTERS") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_monsters, Keynodes::rrel_gwent_faction);
        }
        else if (key_word[4] == "SYNDICATE") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_syndicate, Keynodes::rrel_gwent_faction);
        }
        else if (key_word[4] == "NILFGRAARD") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_nilfgaard, Keynodes::rrel_gwent_faction);
        }
        else if (key_word[4] == "SCOIATAEL") {
            utils::GenerationUtils::generateRelationBetween(ms_context.get(), param, gwent_faction_scoiatael, Keynodes::rrel_gwent_faction);
        }

        //ROW
        if (key_word[5] == "RANGED") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_ranged, param);
        }
        else {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_close, param);
        }

        //TYPE
        if (key_word[6] == "UNIT") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_unit, param);
        }
        else {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_special, param);
        }

        //SUBTYPE
        if (key_word[7] == "UNIT") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_unit, param);
        }
        else if (key_word[7] == "HERO") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_hero, param);
        }
        else if (key_word[7] == "SPECIAL") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_special, param);
        }
        else if (key_word[7] == "WETHER") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_card_weather, param);
        }


        //ABILITY
        if (key_word[8] == "MORALE_BOOST") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_morale_boost, param);
        }
        else if (key_word[8] == "MEDIC") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_medic, param);
        }
        else if (key_word[8] == "AGILE") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_agile, param);
        }
        else if (key_word[8] == "BERSERKER") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_berserker, param);
        }
        else if (key_word[8] == "MARDROEME_UNIT") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_mardroeme_unit, param);
        }
        else if (key_word[8] == "MUSTER") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_muster, param);
        }
        else if (key_word[8] == "SPY") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_spy, param);
        }
        else if (key_word[8] == "TIGHT_BOND") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_tight_bond, param);
        }
        else if (key_word[8] == "SCROTH_UNIT") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_scorch_unit, param);
        }
        else if (key_word[8] == "COMANDERS_HORN") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_comanders_horn, param);
        }
        else if (key_word[8] == "SCORCH_SPECIAL") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_scorch_special, param);
        }
        else if (key_word[8] == "MARDROEME_SPECIAL") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_mardroeme_special, param);
        }
        else if (key_word[8] == "DECOY") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_decoy, param);
        }
        else if (key_word[8] == "BITING_FROST") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_biting_frost, param);
        }
        else if (key_word[8] == "IMPENETRABLE_FOG") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_impenetrable_fog, param);
        }
        else if (key_word[8] == "TORRENTIAL_RAIN") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_torrential_rain, param);
        }
        else if (key_word[8] == "CLEAR_WEATHER") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_clear_weather, param);
        }
        else if (key_word[8] == "SKELLIGE_STORM") {
            ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::gwent_ability_skellige_storm, param);
        }

        // Finish and return
        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;

    }
}
