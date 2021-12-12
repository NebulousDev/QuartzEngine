#pragma once

#include "../Resource.h"

#include "util/Array.h"
#include "util/Map.h"
#include "util/String.h"

namespace Quartz
{
	enum QUARTZ_API JSONType
	{
		JSON_TYPE_NULL,
		JSON_TYPE_STRING,
		JSON_TYPE_NUMBER,
		JSON_TYPE_OBJECT,
		JSON_TYPE_ARRAY,
		JSON_TYPE_BOOL
	};

	struct QUARTZ_API JSONValue;

	typedef Array<JSONValue> JSONArray;
	typedef Map<String, JSONValue> JSONObject;

	struct QUARTZ_API JSONValue
	{
	private:
		JSONType mType;
		
		union
		{
			Double64 mNumber;
			Bool8 mBool;
			String mString;
			JSONArray mArray;
			JSONObject mObject;
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

		operator Int64()		const { return AsInt(); }
		operator Double64()		const { return AsDouble(); }
		operator Float32()		const { return AsFloat(); }
		operator Bool8()		const { return AsBool(); }
		operator String()		const { return AsString(); }
		operator JSONArray()	const { return AsArray(); }
		operator JSONObject()	const { return AsObject(); }	

		template<typename Type> JSONValue Get(const Type& key) = delete;
		template<typename Type> JSONValue Get(const Type* key) = delete;

		template<> JSONValue Get<Int32>(const Int32& index)
		{
			return index < mArray.Size() ? mArray[index] : JSONValue();
		}

		template<> JSONValue Get<UInt32>(const UInt32& index)
		{
			return index < mArray.Size() ? mArray[index] : JSONValue();
		}

		template<> JSONValue Get<Int64>(const Int64& index)
		{ 
			return index < mArray.Size() ? mArray[index] : JSONValue();
		}

		template<> JSONValue Get<UInt64>(const UInt64& index)
		{
			return index < mArray.Size() ? mArray[index] : JSONValue();
		}

		template<> JSONValue Get<String>(const String& key)
		{ 
			JSONValue* pValue = mObject.Get(key);
			return pValue ? *pValue : JSONValue();
		}

		template<> JSONValue Get<Char>(const Char* key)
		{
			JSONValue* pValue = mObject.Get(key);
			return pValue ? *pValue : JSONValue();
		}

		template<typename Type> JSONValue operator[](const Type& key) { return Get(key); }
		template<typename Type> JSONValue operator[](const Type* key) { return Get(key); }

		JSONValue& operator=(const JSONValue& value);

		FORCE_INLINE Int64			AsInt() const { return (Int64)mNumber; }
		FORCE_INLINE Double64		AsDouble() const { return mNumber; }
		FORCE_INLINE Float32		AsFloat() const { return mNumber; }
		FORCE_INLINE Bool8			AsBool() const { return mBool; }
		FORCE_INLINE String			AsString() const { return mString; }
		FORCE_INLINE JSONArray		AsArray() const { return mArray; }
		FORCE_INLINE JSONObject		AsObject() const { return mObject; }

		FORCE_INLINE Bool8 IsNull()   const { return mType == JSON_TYPE_NULL; };
		FORCE_INLINE Bool8 IsString() const { return mType == JSON_TYPE_STRING; };
		FORCE_INLINE Bool8 IsBool()   const { return mType == JSON_TYPE_BOOL; };
		FORCE_INLINE Bool8 IsNumber() const { return mType == JSON_TYPE_NUMBER; };
		FORCE_INLINE Bool8 IsArray()  const { return mType == JSON_TYPE_ARRAY; };
		FORCE_INLINE Bool8 IsObject() const { return mType == JSON_TYPE_OBJECT; };

		USize Size() const;
	};

	struct QUARTZ_API JSON : public Resource<JSON>
	{
	private:
		JSONValue*	mpRoot;

	public:
		JSON();
		JSON(JSONValue* pRoot);

		FORCE_INLINE JSONValue*	GetRoot() const { return mpRoot; }
	};
}