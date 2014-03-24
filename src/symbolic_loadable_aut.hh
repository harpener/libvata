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

/// @brief  VATA library namespace
namespace VATA
{
  template <typename> class SymbolicLoadableAut;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief  Loadable class for a symbolically represented finite automaton
template <typename TBaseAut>
class VATA::SymbolicLoadableAut : public TBaseAut
{

GCC_DIAG_ON(effc++)

public: // public data types

  /// @brief  Base automaton class
  using ParentAut = TBaseAut;

  /// @brief  Structure describing an automaton
  using AutDescription = typename TBaseAut::AutDescription;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a state
   */
  using StateDict = typename TBaseAut::StateDict;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a symbol
   */
  using SymbolDict = typename TBaseAut::SymbolDict;

  /// @brief  Translator using StateDict with addition allowed
  using StringToStateTranslWeak = typename TBaseAut::StringToStateTranslWeak;

  /// @brief  Translator using SymbolDict with addition allowed
  using StringToSymbolTranslWeak = typename TBaseAut::StringToSymbolTranslWeak;

  /// @brief  Reverse translator using StateDict with addition forbidden
  using StateBackTranslStrict = typename TBaseAut::StateBackTranslStrict;

  /// @brief  Reverse translator using SymbolDict with addition forbidden
  using SymbolBackTranslStrict = typename TBaseAut::SymbolBackTranslStrict;

private: // private data types

  /// @brief  String conversions
	using Convert = VATA::Util::Convert;

public: // public methods

  /// @brief  Inherits all constructors
  using TBaseAut::TBaseAut;

  /**
   * @brief  Implicit conversion from @p TBaseAut
   *
   * @param  baseAut  Base automaton class to copy
   */
  SymbolicLoadableAut(
    const TBaseAut & baseAut
  ) : TBaseAut(baseAut)
  {}

  /**
   * @brief  Implicit conversion from @p TBaseAut
   *
   * @param  baseAut Base automaton class to move
   */
  SymbolicLoadableAut(
    TBaseAut && baseAut
  ) : TBaseAut(std::move(baseAut))
  {}

  /**
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser  Tool parsing a string describing an automaton
   * @param[in]  str     String describing an automaton
   * @param[in]  params  Explicit or symbolic method of loading
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
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser      Tool parsing a string describing an automaton
   * @param[in]  str         String describing an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of loading
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
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser        Tool parsing a string describing an automaton
   * @param[in]  str           String describing an automaton
   * @param[in]  stateTransl   Translator between string and internal
                               representation of a state
   * @param[in]  symbolTransl  Translator between string and internal
                               representation of a symbol
   * @param[in]  params        Explicit or symbolic method of loading
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
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc    Structure describing an automaton
   * @param[in]  params  Explicit or symbolic method of loading
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
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc        Structure describing an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of loading
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
      size_t state(0);
      size_t symbol(0);

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
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc          Structure describing an automaton
   * @param[in]  stateTransl   Translator between string and internal
                               representation of a state
   * @param[in]  symbolTransl  Translator between string and internal
                               representation of a symbol
   * @param[in]  params        Explicit or symbolic method of loading
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
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  params  Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    const std::string & params
  ) const
  {
    VATA::Serialization::TimbukSerializer serializer;

    return serializer.Serialize(this->DumpToAutDesc(params));
  }

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer  A tool serializing an AutDescription structure
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const std::string &                    params
  ) const
  {
    return serializer.Serialize(this->DumpToAutDesc(params));
  }

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer  Tool serializing a structure describing
                             an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
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
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer        Tool serializing a structure describing
                                   an automaton
   * @param[in]  stateBackTransl   Reverse translator between string
                                   and internal representation of a state
   * @param[in]  symbolBackTransl  Reverse translator translating between string
                                   and internal representation of a symbol
   * @param[in]  params            Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
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
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  params  Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
   */
  AutDescription DumpToAutDesc(
    const std::string & params
  ) const
  {
    if (params == "explicit")
    {
      return this->DumpToAutDescExplicit(
        [](const size_t & state){return Convert::ToString(state);},
        [](const size_t & symbol){return Convert::ToString(symbol);}
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
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
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
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  stateBackTransl   Reverse translator between string
                                   and internal representation of a state
   * @param[in]  symbolBackTransl  Reverse translator between string
                                   and internal representation of a symbol
   * @param[in]  params            Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
   */
  template <typename StateBackTranslFunc, typename SymbolBackTranslFunc>
  AutDescription DumpToAutDesc(
    StateBackTranslFunc    stateBackTransl,
    SymbolBackTranslFunc   symbolBackTransl,
    const std::string &    params
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