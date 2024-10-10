#include "syntax.hpp"
#include "common.hpp"
#include "exceptions.hpp"

// Used to mark the level to which sequence belongs.
int sequence_nesting = 0;

Sequence generate_syntaxic(std::vector<Lexem>::iterator begin,
                           std::vector<Lexem>::iterator end)
{
  // Create top level sequence.
  Sequence syntaxic(std::to_string(sequence_nesting));
  // Create iterator for the next lexem.
  auto current = begin;
  // Used to negate the whole sequence.
  bool negate_first = false;

  // Main analysis loop.
  while (current != end)
  {
    switch ((*current).type)
    {
    case LexemType::Value:
    { // Parse if lexem is a simple value.
      auto value = std::make_shared<Symbol>(Symbol(SymbolType::Value));

      value->value = (*current).symbol;

      if (negate_first && syntaxic.size() == 0)
      {
        value->is_negated = true;
      }

      if (syntaxic.size() >= 1 &&
          syntaxic.last()->type == SymbolType::Operator)
      {
        syntaxic.last()->right_relative = value;
      }
      else if (syntaxic.size() != 0)
      { // Fail if doesn't have right relative and isn't last.
        throw analysis_failed(
            std::format("Syntax analysis failed, operator missing at: {}",
                        std::distance(begin, current)));
      }
      // Increment to next lexem.
      current++;
      syntaxic.add(value);
      break;
    }
    case LexemType::Operator:
    { // Process if lexem is an operator.
      auto oper = std::make_shared<Symbol>(Symbol(SymbolType::Operator));
      oper->value = (*current).symbol;
      // Negate sequence if minus is first.
      if (syntaxic.size() == 0 && oper->value == "-")
      {
        negate_first = true;
        current++;
        break;
      }
      if (syntaxic.last()->type == SymbolType::Value ||
          syntaxic.last()->type == SymbolType::Sequence)
      {
        oper->left_relative = syntaxic.last();
      }
      else
      { // Fail if operator misses an operand to the left.
        throw analysis_failed(
            std::format("Syntax analysis failed, operand missing at: {}",
                        std::distance(begin, current)));
      }
      // Increment to next lexem.
      current++;
      syntaxic.add(oper);
      break;
    }
    case LexemType::PhraseStart:
    { // Parse sequence start.
      auto end_pos = current + 1;
      int nesting = 0;

      while (end_pos != end)
      {
        if ((*end_pos).type == LexemType::PhraseStart)
          nesting += 1;
        if ((*end_pos).type == LexemType::PhraseEnd)
        {
          if (nesting == 0)
            break;
          else
            nesting -= 1;
        }
        end_pos++;
      }
      if (end_pos == end)
      { // Fail if corresponding closing braces weren't found.
        throw analysis_failed(std::format(
            "Syntax analysis failed, closing braces missing for: {}",
            std::distance(begin, current)));
      }
      // Recursively parse subsequence. 
      sequence_nesting++;
      auto subsequence =
          std::make_shared<Sequence>(generate_syntaxic(current + 1, end_pos));
      // Negate the whole subsequence if needed.
      if (negate_first)
        subsequence->is_negated = true;

      if (syntaxic.size() >= 1 &&
          syntaxic.last()->type == SymbolType::Operator)
      {
        syntaxic.last()->right_relative = subsequence;
      }
      else if (syntaxic.size() != 0)
      { // Fail if operator misses an operand to the right and isn't last.
        throw analysis_failed(
            std::format("Syntax analysis failed, operator missing at: {}",
                        std::distance(begin, current)));
      }

      current = end_pos + 1;
      syntaxic.add(subsequence);
      break;
    }
    case LexemType::PhraseEnd:
      // Fail if found unclaimed closing braces.
      throw analysis_failed(
          std::format("Syntax analysis failed, missing opening braces: {}",
                      std::distance(begin, current)));
      break;

    default:
      // An interesting question indeed.
      throw analysis_failed("How did it even get here?");
      break;
    }
  }
  // Return reconstructed composite.
  return syntaxic;
}