
#include "JSONLoader.h"

namespace Quartz
{
	Bool8 ReadString(const Char** pStr, String* pRetStr)
	{
		Array<Char> strArr;

		while (**pStr != 0)
		{
			Char nextChar = **pStr;

			// Escape character
			if (**pStr == '\\')
			{
				(*pStr)++;

				switch (**pStr)
				{
				case L'"':  nextChar = '"';  break;
				case L'\\': nextChar = '\\'; break;
				case L'/':  nextChar = '/';  break;
				case L'b':  nextChar = '\b'; break;
				case L'f':  nextChar = '\f'; break;
				case L'n':  nextChar = '\n'; break;
				case L'r':  nextChar = '\r'; break;
				case L't':  nextChar = '\t'; break;

					// For now, ignore \u

				default: return false;
				}
			}

			// End of string
			else if (**pStr == '"')
			{
				(*pStr)++;
				*pRetStr = String(strArr.Data(), strArr.Size());
				return true;
			}

			strArr.PushBack(nextChar);
			(*pStr)++;
		}

		return false;
	}

	Double64 ParseInt(const Char** pStr)
	{
		Double64 value = 0;

		while (**pStr != 0 && **pStr >= '0' && **pStr <= '9')
		{
			UInt8 digit = **pStr - '0';
			value = value * 10 + digit;
			(*pStr)++;
		}

		return value;
	}

	Double64 ParseDecimal(const Char** pStr)
	{
		Double64 value = 0.0;
		Double64 factor = 0.1;

		while (**pStr != 0 && **pStr >= '0' && **pStr <= '9')
		{
			UInt8 digit = **pStr - '0';
			value = value + (digit * factor);
			factor *= 0.1;
			(*pStr)++;
		}

		return value;
	}

	Bool8 SkipWhitespace(const Char** pStr)
	{
		while (**pStr != 0 && (**pStr == ' ' || **pStr == '\t' || **pStr == '\r' || **pStr == '\n'))
			(*pStr)++;

		return **pStr != 0;
	}

	JSONValue* ParseJSON(const Char** pStr)
	{
		// String
		if (**pStr == '"')
		{
			(*pStr)++;

			String str;
			if (!ReadString(pStr, &str))
			{
				return NULL;
			}

			return new JSONValue(str);
		}

		// Null
		if (**pStr == 'n' && *((*pStr) + 1) == 'u' && *((*pStr) + 2) == 'l' && *((*pStr) + 3) == 'l')
		{
			(*pStr) += 4;
			return new JSONValue();
		}

		// Bool - true
		if (**pStr == 't' && *((*pStr) + 1) == 'r' && *((*pStr) + 2) == 'u' && *((*pStr) + 3) == 'e')
		{
			(*pStr) += 4;
			return new JSONValue(true);
		}

		// Bool - false
		if (**pStr == 'f' && *((*pStr) + 1) == 'a' && *((*pStr) + 2) == 'l' && *((*pStr) + 3) == 's' && *((*pStr) + 4) == 'e')
		{
			(*pStr) += 5;
			return new JSONValue(false);
		}

		// Number
		if (**pStr == '-' || (**pStr >= '0' && **pStr <= '9'))
		{
			Double64 number = 0;

			// Negative
			Bool8 negative = **pStr == '-';
			if (negative) (*pStr)++;

			// Zero
			if (**pStr == '0')
				(*pStr)++;

			else if (**pStr >= '0' && **pStr <= '9')
				number = ParseInt(pStr);

			else
				return NULL;

			// Decimal
			if (**pStr == '.')
			{
				(*pStr)++;

				if (!(**pStr >= '0' && **pStr <= '9'))
					return NULL;

				number += ParseDecimal(pStr);
			}

			// Exponent
			if (**pStr == 'E' || **pStr == 'e')
			{
				(*pStr)++;

				// Negative exponent
				Bool8 negativeExp = **pStr == '-';
				if (negativeExp) (*pStr)++;

				// Posative exponent
				if (**pStr == '+') (*pStr)++;

				Double64 exponent = ParseInt(pStr);

				if (negativeExp)
				{
					for (Double64 i = 0.0; i < exponent; i++)
						number /= 10.0;
				}

				else
				{
					for (Double64 i = 0.0; i < exponent; i++)
						number *= 10.0;
				}
			}

			if (negative)
				number *= -1;

			return new JSONValue(number);
		}

		// Object
		if (**pStr == '{')
		{
			JSONObject object;

			(*pStr)++; // move

			while (**pStr != 0)
			{
				String name;

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				// Empty object {}
				if (object.Size() == 0 && **pStr == '}')
				{
					(*pStr)++;
					return new JSONValue(object);
				}

				(*pStr)++;

				// Read key
				if (!ReadString(pStr, &name))
				{
					return NULL;
				}

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				// Check delimiter
				if (**pStr != ':')
				{
					return NULL;
				}

				(*pStr)++;

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				// Parse value
				JSONValue* pValue = ParseJSON(pStr);
				if (pValue == NULL)
				{
					return NULL;
				}

				// Place key : value in map
				object.Put(name, *pValue);

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				// Check end of object
				if (**pStr == '}')
				{
					(*pStr)++;
					return new JSONValue(object);
				}

				// Check new entry
				if (**pStr != ',')
				{
					return NULL;
				}

				(*pStr)++;
			}

			return NULL;
		}

		// Array
		if (**pStr == '[')
		{
			JSONArray array;

			while (**pStr != 0)
			{
				(*pStr)++;

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				JSONValue* pValue = ParseJSON(pStr);

				if (pValue == NULL)
				{
					return NULL;
				}

				array.PushBack(*pValue);

				// Skip whitespace
				if (!SkipWhitespace(pStr))
				{
					return NULL;
				}

				// Check end of array
				if (**pStr == ']')
				{
					(*pStr)++;
					return new JSONValue(array);
				}

				// Check new entry
				if (**pStr != ',')
				{
					return NULL;
				}
			}
		}

		return NULL;
	}

	JSONValue* ParseJSON(const Char* str)
	{
		if (!SkipWhitespace(&str))
			return NULL;

		JSONValue* pValue = ParseJSON(&str);

		if (SkipWhitespace(&str))
		{
			delete pValue;
			return NULL;
		}

		return pValue;
	}

	Bool8 LoadJSON(const Char* jsonString, JSON& json)
	{
		JSONValue* pValue = ParseJSON(jsonString);

		if (!pValue)
		{
			return false;
		}

		json = JSON(pValue);

		return true;
	}
}