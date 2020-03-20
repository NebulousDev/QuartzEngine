#pragma once

#include "../util/Array.h"
#include "../util/Map.h"
#include "../util/String.h"

namespace Quartz
{
	enum JSONType
	{
		JSON_TYPE_STRING,
		JSON_TYPE_NUMBER,
		JSON_TYPE_OBJECT,
		JSON_TYPE_ARRAY,
		JSON_TYPE_BOOL,
		JSON_TYPE_NULL,
	};

	struct JSONValue;

	typedef Array<JSONValue> JSONArray;
	typedef Map<String, JSONValue> JSONObject;

	struct JSONValue
	{
	private:
		JSONType mType;
		
		union
		{
			Double64 mNumber;
			Bool8 mBool;
			String* mpString;
			JSONArray* mpArray;
			JSONObject* mpObject;
		};

	public:
		JSONValue();
		JSONValue(Int64 value);
		JSONValue(Double64 value);
		JSONValue(Bool8 value);
		JSONValue(const Char* value);
		JSONValue(const String& value);
		JSONValue(const JSONArray& value);
		JSONValue(const JSONObject& value);
		JSONValue(const JSONValue& value);

		~JSONValue();

		operator Int64() const { return (Int64)mNumber; }
		operator Double64() const { return mNumber; }
		operator Bool8() const { return mBool; }
		operator String&() const { return *mpString; }
		operator JSONArray&() const { return *mpArray; }
		operator JSONObject&() const { return *mpObject; }

		JSONValue& operator[](const Char* name) { return *mpObject->Get(name); }
		JSONValue& operator[](const String& name) { return *mpObject->Get(name); }

		FORCE_INLINE Int64 AsInt() const { return (Int64)mNumber; }
		FORCE_INLINE Double64 AsNumber() const { return mNumber; }
		FORCE_INLINE Bool8 AsBool() const { return mBool; }
		FORCE_INLINE String& AsString() const { return *mpString; }
		FORCE_INLINE JSONArray& AsArray() const { return *mpArray; }
		FORCE_INLINE JSONObject& AsObject() const { return *mpObject; }

		FORCE_INLINE Bool8 IsNull()   const { return mType == JSON_TYPE_NULL; };
		FORCE_INLINE Bool8 IsString() const { return mType == JSON_TYPE_STRING; };
		FORCE_INLINE Bool8 IsBool()   const { return mType == JSON_TYPE_BOOL; };
		FORCE_INLINE Bool8 IsNumber() const { return mType == JSON_TYPE_NUMBER; };
		FORCE_INLINE Bool8 IsArray()  const { return mType == JSON_TYPE_ARRAY; };
		FORCE_INLINE Bool8 IsObject() const { return mType == JSON_TYPE_OBJECT; };
	};

	JSONValue* ParseJSON(const Char* str);
}