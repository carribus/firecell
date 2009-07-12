#include <sstream>
#include "fccutils.h"

std::string formatCurrencyValue(std::string currSymbol, FCULONG value)
{
  std::stringstream ss;
  std::string out;

  ss << currSymbol << value;
  out = ss.str();
  int edge = currSymbol.length(), idx = out.length()-3;
  while ( idx > edge )
  {
    out.insert(idx, ",");
    idx -= 3;
  }

  return out;
}