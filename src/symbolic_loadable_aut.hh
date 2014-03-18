/*****************************************************************************
 *  VATA Tree Automata Library
 *
 *  Copyright (c) 2014  Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Header file for the symbolic loadable automaton template.
 *
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_LOADABLE_AUT_HH_
#define _VATA_SYMBOLIC_LOADABLE_AUT_HH_

#include <vata/util/aut_description.hh>
#include <vata/util/convert.hh>
#include <vata/serialization/timbuk_serializer.hh>

/// @brief VATA library namespace.
namespace VATA
{
  template <typename> class SymbolicLoadableAut;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/**
 * @brief Loadable class for a symbolically represented finite automaton.
 * @tparam TBaseAut Base automaton class to merge.
 */
template <typename TBaseAut>
class VATA::SymbolicLoadableAut : public TBaseAut
{

GCC_DIAG_ON(effc++)

public:   // data types

  /// @brief Base automaton class.
  using ParentAut = TBaseAut;

  /// @brief An automaton description structure.
  using AutDescription = typename TBaseAut::AutDescription;

  /// @brief An internal numeric state.
  using StateType = typename TBaseAut::StateType;

  /// @brief An internal numeric symbol.
  using SymbolType = typename TBaseAut::SymbolType;

  /// @brief A state dictionary between string and numeric representation.
  using StateDict = typename TBaseAut::StateDict;

  /// @brief A symbol dictionary between string and numeric representation.
  using SymbolDict = typename TBaseAut::SymbolDict;

  /// @brief A weak translation of states (addition allowed).
  using StringToStateTranslWeak = typename TBaseAut::StringToStateTranslWeak;

  /// @brief A weak translation of symbols (addition allowed).
  using StringToSymbolTranslWeak = typename TBaseAut::StringToSymbolTranslWeak;

  /// @brief A strict reverse translation of states (addition forbidden).
  using StateBackTranslStrict = typename TBaseAut::StateBackTranslStrict;

  /// @brief A strict reverse translation of symbols (addition forbidden).
  using SymbolBackTranslStrict = typename TBaseAut::SymbolBackTranslStrict;

private:  // data types

  /// @brief Contains some string conversion methods.
  using Convert = VATA::Util::Convert;

public:   // public methods

  /// @brief Inherits all constructors.
  using TBaseAut::TBaseAut;

  /**
   * @brief Implicit conversion from @p TBaseAut
   * @param baseAut Base automaton class to copy.
   */
  SymbolicLoadableAut(
    const TBaseAut & baseAut
  ) : TBaseAut(baseAut)
  {}

  /**
   * @brief Implicit conversion from @p TBaseAut
   * @param baseAut Base automaton class to move.
   */
  SymbolicLoadableAut(
    TBaseAut && baseAut
  ) : TBaseAut(std::move(baseAut))
  {}

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
  )
  {
    this->LoadFromAutDesc(
      parser.ParseString(str),
      params
    );
  }

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
  )
  {
    this->LoadFromAutDesc(
      parser.ParseString(str),
      stateDict,
      symbolDict,
      params
    );
  }

  /**
   * @brief Loading an automaton from a string.
   * @tparam StateTranslFunc A functor for translation of states.
   * @tparam SymbolTranslFunc A functor for translation of symbols.
   * @param[in] parser A procedure for parsing a string.
   * @param[in] str A string describing an automaton.
   * @param[in] stateTransl A functor for translation of states.
   * @param[in] symbolTransl A functor for translation of symbol.
   * @param[in] params A method of loading.
   */
  template <typename StateTranslFunc, typename SymbolTranslFunc>
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    StateTranslFunc              stateTransl,
    SymbolTranslFunc             symbolTransl,
    const std::string &          params
  )
  {
    this->LoadFromAutDesc(
      parser.ParseString(str),
      stateTransl,
      symbolTransl,
      params
    );
  }

  /**
   * @brief Loading an automaton from aut_description.
   * @param[in] desc A structure describing and automaton.
   * @param[in] params A method of loading.
   */
  void LoadFromAutDesc(
    const AutDescription & desc,
    const std::string &    params
  )
  {
    if (params == "explicit")
    {
      StateDict stateDict;
      SymbolDict symbolDict;

      this->LoadFromAutDesc(
        desc,
        stateDict,
        symbolDict,
        params
      );
    }

    else if (params == "symbolic")
    {
      this->LoadFromAutDescSymbolic(desc);
    }

    else
    { // unknown params
      throw std::runtime_error("Undefined method of loading.");
    }
  }

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
  )
  {
    if (params == "explicit")
    {
      StateType state(0);
      SymbolType symbol(0);

      this->LoadFromAutDescExplicit(
        desc,
        StringToStateTranslWeak(
          stateDict,
          [&state](const std::string &){return state++;}
        ),
        StringToSymbolTranslWeak(
          symbolDict,
          [&symbol](const std::string &){return symbol++;}
        )
      );
    }

    else if (params == "symbolic")
    {
      this->LoadFromAutDescSymbolic(desc);
    }

    else
    { // unknown params
      throw std::runtime_error("Undefined method of loading.");
    }
  }

  /**
   * @brief Loading an automaton from aut_description.
   * @tparam StateTranslFunc A functor for translation of states.
   * @tparam SymbolTranslFunc A functor for translation of symbols.
   * @param[in] desc A structure describing and automaton.
   * @param[in] stateTransl A functor for translation of states.
   * @param[in] symbolTransl A functor for translation of symbol.
   * @param[in] params A method of loading.
   */
  template <typename StateTranslFunc, typename SymbolTranslFunc>
  void LoadFromAutDesc(
    const AutDescription & desc,
    StateTranslFunc        stateTransl,
    SymbolTranslFunc       symbolTransl,
    const std::string &    params
  )
  {
    if (params == "explicit")
    {
      this->LoadFromAutDescExplicit(
        desc,
        stateTransl,
        symbolTransl
      );
    }

    else if (params == "symbolic")
    {
      this->LoadFromAutDescSymbolic(desc);
    }

    else
    { // unknown params
      throw std::runtime_error("Undefined method of loading.");
    }
  }

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    const std::string & params
  ) const
  {
    VATA::Serialization::TimbukSerializer serializer;

    return serializer.Serialize(this->DumpToAutDesc(params));
  }

  /**
   * @brief Dumping an automaton to a string.
   * @param[in] serializer A procedure for serializing an aut_description.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const std::string &                    params
  ) const
  {
    return serializer.Serialize(this->DumpToAutDesc(params));
  }

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
  ) const
  {
    return serializer.Serialize(
      this->DumpToAutDesc(
        stateDict,
        symbolDict,
        params
      )
    );
  }

  /**
   * @brief Dumping an automaton to a string.
   * @tparam StateBackTranslFunc A functor for reverse translation of states.
   * @tparam SymbolBackTranslFunc A functor for reverse translation of symbols.
   * @param[in] serializer A procedure for serializing an aut_description.
   * @param[in] stateBackTransl A functor for reverse translation of states.
   * @param[in] symbolBackTransl A functor for reverse translation of symbols.
   * @param[in] params A method of dumping.
   * @return A string describing an automaton.
   */
  template <typename StateBackTranslFunc, typename SymbolBackTranslFunc>
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    StateBackTranslFunc                    stateBackTransl,
    SymbolBackTranslFunc                   symbolBackTransl,
    const std::string &                    params
  ) const
  {
    return serializer.Serialize(
      this->DumpToAutDesc(
        stateBackTransl,
        symbolBackTransl,
        params
      )
    );
  }

  /**
   * @brief Dumping an automaton to aut_description.
   * @param[in] params A method of dumping.
   * @return An aut_description structure describing an automaton.
   */
  AutDescription DumpToAutDesc(
    const std::string & params
  ) const
  {
    if (params == "explicit")
    {
      return this->DumpToAutDescExplicit(
        [](const StateType& state){return Convert::ToString(state);},
        [](const SymbolType& symbol){return Convert::ToString(symbol);}
      );
    }

    else if (params == "symbolic")
    {
      return this->DumpToAutDescSymbolic();
    }
    
    else
    { // unknown params
      throw std::runtime_error("Undefined method of dumping.");
    }
  }

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
  ) const
  {
    if (params == "explicit")
    {
      return this->DumpToAutDescExplicit(
        StateBackTranslStrict(stateDict.GetReverseMap()),
        SymbolBackTranslStrict(symbolDict.GetReverseMap())
      );
    }

    else if (params == "symbolic")
    {
      return this->DumpToAutDescSymbolic();
    }

    else
    { // unknown params
      throw std::runtime_error("Undefined method of dumping.");
    }
  }

  /**
   * @brief Dumping an automaton to aut_description.
   * @tparam StateBackTranslFunc A functor for reverse translation of states.
   * @tparam SymbolBackTranslFunc A functor for reverse translation of symbols.
   * @param[in] stateBackTransl A functor for reverse translation of states.
   * @param[in] symbolBackTransl A functor for reverse translation of symbols.
   * @param[in] params A method of dumping.
   * @return An aut_description structure describing an automaton.
   */
  template <typename StateBackTranslFunc, typename SymbolBackTranslFunc>
  AutDescription DumpToAutDesc(
    StateBackTranslFunc  stateBackTransl,
    SymbolBackTranslFunc symbolBackTransl,
    const std::string &  params
  ) const
  {
    if (params == "explicit")
    {
      return this->DumpToAutDescExplicit(
        stateBackTransl,
        symbolBackTransl
      );
    }

    else if (params == "symbolic")
    {
      return this->DumpToAutDescSymbolic();
    }

    else
    { // unknown params
      throw std::runtime_error("Undefined method of dumping.");
    }
  }
};

#endif // _VATA_SYMBOLIC_LOADABLE_AUT_HH_
