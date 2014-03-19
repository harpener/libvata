/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *  Header file for a symbolically represented FA (facade).
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_HH_

#include <vata/vata.hh>
#include <vata/parsing/abstr_parser.hh>
#include <vata/serialization/abstr_serializer.hh>
#include <vata/util/aut_description.hh>
#include <vata/util/two_way_dict.hh>
#include <vata/util/transl_weak.hh>
#include <vata/util/transl_strict.hh>

/// @brief VATA library namespace.
namespace VATA
{
  class SymbolicFiniteAut;
  template <typename> class SymbolicLoadableAut;
  class SymbolicFiniteAutCore;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief Facade class for a symbolically represented finite automaton.
class VATA::SymbolicFiniteAut
{

GCC_DIAG_ON(effc++)

public: // data types

  /// @brief An automaton description structure.
  using AutDescription = VATA::Util::AutDescription;

  /// @brief An internal state.
  using StateType = size_t;

  /// @brief An internal symbol.
  using SymbolType = size_t;

  /// @brief A state dictionary.
  using StateDict = VATA::Util::TwoWayDict<std::string, StateType>;

  /// @brief A symbol dictionary.
  using SymbolDict = VATA::Util::TwoWayDict<std::string, SymbolType>;

  /// @brief A weak translation of states (addition allowed).
  using StringToStateTranslWeak = VATA::Util::TranslatorWeak<StateDict>;

  /// @brief A weak translation of symbols (addition allowed).
  using StringToSymbolTranslWeak = VATA::Util::TranslatorWeak<SymbolDict>;

  /// @brief A strict reverse translation of states (addition forbidden).
  using StateBackTranslStrict = VATA::Util::TranslatorStrict<StateDict::MapBwdType>;

  /// @brief A strict reverse translation of symbols (addition forbidden).
  using SymbolBackTranslStrict = VATA::Util::TranslatorStrict<SymbolDict::MapBwdType>;

private: // data types

  /// @brief A loadable automaton core.
  using SymAutCore = VATA::SymbolicLoadableAut<VATA::SymbolicFiniteAutCore>;

  /// @brief SymAutCore pointer.
  using SymAutCorePtr = std::unique_ptr<SymAutCore>;

private: // data members

  /// @brief A loadable automaton core.
  SymAutCorePtr core_;

public: // instantiation

  /// @brief Default constructor.
  SymbolicFiniteAut();

  /**
   * @brief Copy constructor.
   * @param[in] aut An automaton to copy.
   */
  SymbolicFiniteAut(
    const SymbolicFiniteAut & aut
  );

  /**
   * @brief Move constructor.
   * @param[in] aut An automaton to move.
   */
  SymbolicFiniteAut(
    SymbolicFiniteAut && aut
  );

  /// @brief Default destructor.
  ~SymbolicFiniteAut();

  /**
   * @brief Copy assignment operator.
   * @param[in] rhs An automaton to copy.
   * @return An automaton.
   */
  SymbolicFiniteAut & operator=(
    const SymbolicFiniteAut & rhs
  );

  /**
   * @brief Move assignment operator.
   * @param[in] rhs An automaton to move.
   * @return An automaton.
   */
  SymbolicFiniteAut & operator=(
    SymbolicFiniteAut && rhs
  );

public: // public methods

  /**
   * @brief Loading an automaton from a string.
   * @param[in] parser A procedure for parsing a string.
   * @param[in] str A string describing an automaton.
   * @param[in] params A method of loading.
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    const std::string &          params
  );

  /**
   * @brief Loading an automaton from a string.
   * @param[in] parser A procedure for parsing a string.
   * @param[in] str A string describing an automaton.
   * @param[in] stateDict A dictionary for translation of states.
   * @param[in] symbolDict A dictionary for translation of symbol.
   * @param[in] params A method of loading.
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    StateDict &                  stateDict,
    SymbolDict &                 symbolDict,
    const std::string &          params
  );

  /**
   * @brief Loading an automaton from a string.
   * @param[in] parser A procedure for parsing a string.
   * @param[in] str A string describing an automaton.
   * @param[in] stateTransl A functor for translation of states.
   * @param[in] symbolTransl A functor for translation of symbol.
   * @param[in] params A method of loading.
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    StringToStateTranslWeak &    stateTransl,
    StringToSymbolTranslWeak &   symbolTransl,
    const std::string &          params
  );

  /**
   * @brief Loading an automaton from aut_description.
   * @param[in] desc A structure describing and automaton.
   * @param[in] params A method of loading.
   */
  void LoadFromAutDesc(
    const AutDescription & desc,
    const std::string &    params
  );

  /**
   * @brief Loading an automaton from aut_description.
   * @param[in] desc A structure describing and automaton.
   * @param[in] stateDict A dictionary for translation of states.
   * @param[in] symbolDict A dictionary for translation of symbol.
   * @param[in] params A method of loading.
   */
  void LoadFromAutDesc(
    const AutDescription & desc,
    StateDict &            stateDict,
    SymbolDict &           symbolDict,
    const std::string &    params
  );

  /**
   * @brief Loading an automaton from aut_description.
   * @param[in] desc A structure describing and automaton.
   * @param[in] stateTransl A functor for translation of states.
   * @param[in] symbolTransl A functor for translation of symbol.
   * @param[in] params A method of loading.
   */
  void LoadFromAutDesc(
    const AutDescription &     desc,
    StringToStateTranslWeak &  stateTransl,
    StringToSymbolTranslWeak & symbolTransl,
    const std::string &        params
  );

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    const std::string & params
  ) const;

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] serializer A procedure for serializing an aut_description.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const std::string &                    params
  ) const;

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] serializer A procedure for serializing an aut_description.
   * @param[in] stateDict A dictionary for state reverse translation.
   * @param[in] symbolDict A dictionary for symbol reverse translation.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const StateDict &                      stateDict,
    const SymbolDict &                     symbolDict,
    const std::string &                    params
  ) const;

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] serializer A procedure for serializing an aut_description.
   * @param[in] stateBackTransl A functor for reverse translation of states.
   * @param[in] symbolBackTransl A functor for reverse translation of symbols.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    StateBackTranslStrict &                stateBackTransl,
    SymbolBackTranslStrict &               symbolBackTransl,
    const std::string &                    params
  ) const;

  /**
   * @brief Dumping an automaton to aut_description.
   * @param[in] params A method of dumping.
   * @return An aut_description structure describing an automaton.
   */
  AutDescription DumpToAutDesc(
    const std::string & params
  ) const;

  /**
   * @brief Dumping an automaton to aut_description.
   * @param[in] stateDict A dictionary for state reverse translation.
   * @param[in] symbolDict A dictionary for symbol reverse translation.
   * @param[in] params A method of dumping.
   * @return An aut_description structure describing an automaton.
   */
  AutDescription DumpToAutDesc(
    const StateDict &   stateDict,
    const SymbolDict &  symbolDict,
    const std::string & params
  ) const;

  /**
   * @brief Dumping an automaton to aut_description.
   * @param[in] stateBackTransl A functor for reverse translation of states.
   * @param[in] symbolBackTransl A functor for reverse translation of symbols.
   * @param[in] params A method of dumping.
   * @return An aut_description structure describing an automaton.
   */
  AutDescription DumpToAutDesc(
    StateBackTranslStrict &  stateBackTransl,
    SymbolBackTranslStrict & symbolBackTransl,
    const std::string &      params
  ) const;

  /**
   * @brief Adding a symbolic transition.
   * @param[in] lstate A string representation of a left-side state.
   * @param[in] symbol A string representation of a symbol.
   * @param[in] rstate A string representation of a right-side state.
   */
  void AddTransition(
    const std::string & lstate,
    const std::string & symbol,
    const std::string & rstate
  );

  /**
   * @brief Adding a symbolic initial state.
   * @param[in] state A string representation of a initial state.
   */
  void AddInitialState(
    const std::string & state
  );

  /**
   * @brief Adding a symbolic final state.
   * @param[in] state A string representation of a final state.
   */
  void AddFinalState(
    const std::string & state
  );
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_HH_