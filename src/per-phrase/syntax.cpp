#include "syntax.hpp"
#include "analysis-failed.hpp"
#include "common.hpp"

int sequence_nesting = 0;

Sequence generate_syntaxic(std::vector<Lexem>::iterator begin,
                           std::vector<Lexem>::iterator end)
{
  Sequence syntaxic(std::to_string(sequence_nesting));
  auto current = begin;
  bool negate_first = false;

  while (current != end)
  {
    switch ((*current).type)
    {
    case LexemType::Value:
    {
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
      {
        throw analysis_failed(
            std::format("Syntax analysis failed, operator missing at: {}",
                        std::distance(begin, current)));
      }

      current++;
      syntaxic.add(value);
      break;
    }

    case LexemType::Operator:
    {
      auto oper = std::make_shared<Symbol>(Symbol(SymbolType::Operator));
      oper->value = (*current).symbol;

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
      {
        throw analysis_failed(
            std::format("Syntax analysis failed, operand missing at: {}",
                        std::distance(begin, current)));
      }

      current++;
      syntaxic.add(oper);
      break;
    }
    case LexemType::PhraseStart:
    {
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
      {
        throw analysis_failed(std::format(
            "Syntax analysis failed, closing braces missing for: {}",
            std::distance(begin, current)));
      }

      sequence_nesting++;
      auto subsequence =
          std::make_shared<Sequence>(generate_syntaxic(current + 1, end_pos));

      if (negate_first)
        subsequence->is_negated = true;

      if (syntaxic.size() >= 1 &&
          syntaxic.last()->type == SymbolType::Operator)
      {
        syntaxic.last()->right_relative = subsequence;
      }
      else if (syntaxic.size() != 0)
      {
        throw analysis_failed(
            std::format("Syntax analysis failed, operator missing at: {}",
                        std::distance(begin, current)));
      }

      current = end_pos + 1;
      syntaxic.add(subsequence);
      break;
    }
    case LexemType::PhraseEnd:
      throw analysis_failed(
          std::format("Syntax analysis failed, missing opening braces: {}",
                      std::distance(begin, current)));
      break;

    default:
      throw analysis_failed("How did it even get here?");
      break;
    }
  }

  return syntaxic;
}