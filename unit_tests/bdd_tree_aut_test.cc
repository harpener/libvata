/*****************************************************************************
 *  Vojnar's Army Tree Automata Library
 *
 *  Copyright (c) 2011  Ondra Lengal <ilengal@fit.vutbr.cz>
 *
 *  Description:
 *    Test suite for BDD based tree automaton
 *
 *****************************************************************************/

// VATA headers
#include <vata/vata.hh>
#include <vata/aut_base.hh>
#include <vata/bdd_tree_aut.hh>
#include <vata/bdd_tree_aut_op.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/util.hh>

using VATA::AutBase;
using VATA::BDDTreeAut;
using VATA::MTBDDPkg::VarAsgn;
using VATA::Parsing::TimbukParser;
using VATA::Serialization::TimbukSerializer;
using VATA::Util::AutDescription;
using VATA::Util::Convert;

// Boost headers
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BDDTreeAut
#include <boost/test/unit_test.hpp>

// testing headers
#include "log_fixture.hh"
#include "aut_db.hh"


/******************************************************************************
 *                                    Types                                   *
 ******************************************************************************/

typedef AutBase::StringToStateDict StringToStateDict;

typedef AutBase::StateToStateMap StateToStateMap;


/******************************************************************************
 *                                  Constants                                 *
 ******************************************************************************/

const fs::path UNREACHABLE_TIMBUK_FILE =
	AUT_DIR / "unreachable_removal_timbuk.txt";

const fs::path INCLUSION_TIMBUK_FILE =
	AUT_DIR / "inclusion_timbuk.txt";

const fs::path INTERSECTION_TIMBUK_FILE =
	AUT_DIR / "intersection_timbuk.txt";


/******************************************************************************
 *                                  Fixtures                                  *
 ******************************************************************************/

/**
 * @brief  BDDTreeAut testing fixture
 *
 * Fixture for test of BDDTreeAut
 */
class BDDTreeAutFixture : public LogFixture
{ };

/******************************************************************************
 *                              Start of testing                              *
 ******************************************************************************/


BOOST_FIXTURE_TEST_SUITE(suite, BDDTreeAutFixture)

BOOST_AUTO_TEST_CASE(timbuk_import_export)
{
	TimbukParser parser;
	TimbukSerializer serializer;

	auto filenames = GetTimbukAutFilenames();
	for (const std::string& filename : filenames)
	{
		BOOST_MESSAGE("Loading automaton " + filename + "...");
		std::string autStr = VATA::Util::ReadFile(filename);

		BDDTreeAut aut;

		StringToStateDict stateDict;
		aut.LoadFromString(parser, autStr, &stateDict);

		std::string autOut = aut.DumpToString(serializer, &stateDict);

		AutDescription descOrig = parser.ParseString(autStr);
		AutDescription descOut = parser.ParseString(autOut);

		BOOST_CHECK_MESSAGE(descOrig == descOut,
			"\n\nExpecting:\n===========\n" +
			std::string(autStr) +
			"===========\n\nGot:\n===========\n" + autOut + "\n===========");
	}
}

BOOST_AUTO_TEST_CASE(adding_transitions)
{
	TimbukParser parser;
	TimbukSerializer serializer;

	BDDTreeAut aut;

	StringToStateDict stateDict;
	aut.LoadFromString(parser, AUT_TIMBUK_A4, &stateDict);
	AutDescription descCorrect = parser.ParseString(AUT_TIMBUK_A4);

	// get state "q"
	//BDDTreeAut::StateType stateQ = stateDict.TranslateFwd("q");
	// get state "p"
	BDDTreeAut::StateType stateP = stateDict.TranslateFwd("p");
	// insert state "qa"
	BDDTreeAut::StateType stateQA = aut.AddState();
	stateDict.Insert(std::make_pair("qa", stateQA));

	// add the following transition: a -> qa, to the description ...
	AutDescription::Transition newTransition(std::vector<std::string>(), "a", "qa");
	descCorrect.transitions.insert(newTransition);
	// ... and to the automaton
	aut.AddSimplyTransition(BDDTreeAut::StateTuple(),
		BDDTreeAut::TranslateStringToSymbol("a"), stateQA);

	// add the following transition: a(qa, qa) -> p, to the description ...
	std::vector<std::string> childrenStr;
	childrenStr.push_back("qa");
	childrenStr.push_back("qa");
	newTransition = AutDescription::Transition(childrenStr, "a", "p");
	descCorrect.transitions.insert(newTransition);
	// ... and to the automaton
	BDDTreeAut::StateTuple children;
	children.push_back(stateQA);
	children.push_back(stateQA);
	aut.AddSimplyTransition(children, BDDTreeAut::TranslateStringToSymbol("a"),
		stateP);

	std::string autOut = aut.DumpToString(serializer, &stateDict);
	AutDescription descOut = parser.ParseString(autOut);

	BOOST_CHECK_MESSAGE(descCorrect == descOut,
		"\n\nExpecting:\n===========\n" +
		serializer.Serialize(descCorrect) +
		"===========\n\nGot:\n===========\n" + autOut + "\n===========");
}


BOOST_AUTO_TEST_CASE(aut_union_simple)
{
	TimbukParser parser;
	TimbukSerializer serializer;

	BDDTreeAut autU1;
	StringToStateDict autU1StateDict;
	autU1.LoadFromString(parser, AUT_TIMBUK_UNION_1, &autU1StateDict);
	AutDescription autU1Desc = parser.ParseString(AUT_TIMBUK_UNION_1);

	BDDTreeAut autU2(autU1.GetTransTable());
	StringToStateDict autU2StateDict;
	autU2.LoadFromString(parser, AUT_TIMBUK_UNION_2, &autU2StateDict);
	AutDescription autU2Desc = parser.ParseString(AUT_TIMBUK_UNION_2);

	BDDTreeAut autUnion12 = VATA::Union(autU1, autU2);
	StringToStateDict autUnion12StateDict =
		autU1StateDict.Union(autU2StateDict);

	std::string autUnion12Str = autUnion12.DumpToString(serializer,
		&autUnion12StateDict);
	AutDescription descOutU12 = parser.ParseString(autUnion12Str);

	AutDescription descCorrectU12 = parser.ParseString(AUT_TIMBUK_UNION_12_RESULT);

	BOOST_CHECK_MESSAGE(descCorrectU12 == descOutU12,
		"\n\nExpecting:\n===========\n" +
		serializer.Serialize(descCorrectU12) +
		"===========\n\nGot:\n===========\n" + autUnion12Str + "\n===========");
}


BOOST_AUTO_TEST_CASE(aut_union_trans_table_copy)
{
	TimbukParser parser;
	TimbukSerializer serializer;

	BDDTreeAut autU1;
	StringToStateDict autU1StateDict;
	autU1.LoadFromString(parser, AUT_TIMBUK_UNION_1, &autU1StateDict);
	AutDescription autU1Desc = parser.ParseString(AUT_TIMBUK_UNION_1);

	BDDTreeAut autU3;
	StringToStateDict autU3StateDict;
	autU3.LoadFromString(parser, AUT_TIMBUK_UNION_3, &autU3StateDict);
	AutDescription autU3Desc = parser.ParseString(AUT_TIMBUK_UNION_3);

	BDDTreeAut autUnion13 = VATA::Union(autU1, autU3);

	std::string autUnion13Str = autUnion13.DumpToString(serializer);
	AutDescription descOutU13 = parser.ParseString(autUnion13Str);

	AutDescription descCorrectU13 = parser.ParseString(AUT_TIMBUK_UNION_13_RESULT);

	BOOST_CHECK_MESSAGE(descCorrectU13 == descOutU13,
		"\n\nExpecting:\n===========\n" +
		serializer.Serialize(descCorrectU13) +
		"===========\n\nGot:\n===========\n" + autUnion13Str + "\n===========");
}

BOOST_AUTO_TEST_CASE(aut_intersection)
{
	auto testfileContent = ParseTestFile(INTERSECTION_TIMBUK_FILE.string());

	TimbukParser parser;
	TimbukSerializer serializer;

	for (auto testcase : testfileContent)
	{
		BOOST_REQUIRE_MESSAGE(testcase.size() == 3, "Invalid format of a testcase: " +
			Convert::ToString(testcase));

		std::string inputLhsFile = (AUT_DIR / testcase[0]).string();
		std::string inputRhsFile = (AUT_DIR / testcase[1]).string();
		std::string resultFile = (AUT_DIR / testcase[2]).string();

		BOOST_MESSAGE("Performing intersection of " + inputLhsFile + " and " + inputRhsFile + "...");

		std::string autLhsStr = VATA::Util::ReadFile(inputLhsFile);
		std::string autRhsStr = VATA::Util::ReadFile(inputRhsFile);
		std::string autCorrectStr = VATA::Util::ReadFile(resultFile);

		BDDTreeAut autLhs;
		StringToStateDict stateDictLhs;
		autLhs.LoadFromString(parser, autLhsStr, &stateDictLhs);
		AutDescription autLhsDesc = parser.ParseString(autLhsStr);

		BDDTreeAut autRhs;
		StringToStateDict stateDictRhs;
		autRhs.LoadFromString(parser, autRhsStr, &stateDictRhs);
		AutDescription autRhsDesc = parser.ParseString(autRhsStr);

		AutBase::ProductTranslMap translMap;
		BDDTreeAut autIntersect = VATA::Intersection(autLhs, autRhs, &translMap);

		StringToStateDict stateDictIsect = VATA::Util::CreateProductStringToStateMap(
			stateDictLhs, stateDictRhs, translMap);

		std::string autIntersectStr = autIntersect.DumpToString(serializer,
			&stateDictIsect);

		AutDescription descOut = parser.ParseString(autIntersectStr);
		AutDescription descCorrect = parser.ParseString(autCorrectStr);

		BOOST_CHECK_MESSAGE(descOut == descCorrect,
			"\n\nExpecting:\n===========\n" + autCorrectStr +
			"===========\n\nGot:\n===========\n" + autIntersectStr + "\n===========");
	}
}

BOOST_AUTO_TEST_CASE(aut_remove_unreachable)
{
	auto testfileContent = ParseTestFile(UNREACHABLE_TIMBUK_FILE.string());

	TimbukParser parser;
	TimbukSerializer serializer;

	for (auto testcase : testfileContent)
	{
		BOOST_REQUIRE_MESSAGE(testcase.size() == 2, "Invalid format of a testcase: " +
			Convert::ToString(testcase));

		std::string inputFile = (AUT_DIR / testcase[0]).string();
		std::string resultFile = (AUT_DIR / testcase[1]).string();

		BOOST_MESSAGE("Removing unreachable states from " + inputFile + "...");

		std::string autStr = VATA::Util::ReadFile(inputFile);
		std::string autCorrectStr = VATA::Util::ReadFile(resultFile);

		BDDTreeAut aut;
		StringToStateDict stateDict;
		aut.LoadFromString(parser, autStr, &stateDict);
		AutDescription autDesc = parser.ParseString(autStr);

		StateToStateMap translMap;
		BDDTreeAut autNoUnreach = VATA::RemoveUnreachableStates(aut, &translMap);

		stateDict = VATA::Util::RebindMap(stateDict, translMap);
		std::string autNoUnreachStr =
			autNoUnreach.DumpToString(serializer, &stateDict);

		AutDescription descOutNoUnreach = parser.ParseString(autNoUnreachStr);
		AutDescription descCorrectNoUnreach = parser.ParseString(autCorrectStr);

		BOOST_CHECK_MESSAGE(descCorrectNoUnreach == descOutNoUnreach,
			"\n\nExpecting:\n===========\n" +
			serializer.Serialize(descCorrectNoUnreach) +
			"===========\n\nGot:\n===========\n" + autNoUnreachStr + "\n===========");
	}
}

BOOST_AUTO_TEST_CASE(aut_down_inclusion)
{
	auto testfileContent = ParseTestFile(INCLUSION_TIMBUK_FILE.string());

	TimbukParser parser;
	TimbukSerializer serializer;

	for (auto testcase : testfileContent)
	{
		BOOST_REQUIRE_MESSAGE(testcase.size() == 3, "Invalid format of a testcase: " +
			Convert::ToString(testcase));

		std::string inputSmallerFile = (AUT_DIR / testcase[0]).string();
		std::string inputBiggerFile = (AUT_DIR / testcase[1]).string();
		unsigned expectedResult = static_cast<bool>(
			Convert::FromString<unsigned>(testcase[2]));

		BOOST_MESSAGE("Testing inclusion " + inputSmallerFile + " <= " +
			inputBiggerFile  + "...");

		std::string autSmallerStr = VATA::Util::ReadFile(inputSmallerFile);
		std::string autBiggerStr = VATA::Util::ReadFile(inputBiggerFile);

		BDDTreeAut autSmaller;
		autSmaller.LoadFromString(parser, autSmallerStr);

		BDDTreeAut autBigger;
		autBigger.LoadFromString(parser, autBiggerStr);

		bool doesInclusionHold = VATA::CheckInclusion(autSmaller, autBigger);

		BOOST_CHECK_MESSAGE(expectedResult == doesInclusionHold,
			"\n\nError checking inclusion " + inputSmallerFile + " <= " +
			inputBiggerFile + ": expected " + Convert::ToString(expectedResult) +
			", got " + Convert::ToString(doesInclusionHold));
	}
}

BOOST_AUTO_TEST_SUITE_END()

