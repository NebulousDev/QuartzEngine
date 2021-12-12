#include "JSON.h"

namespace Quartz
{
	JSONValue::JSONValue()
		: mType(JSON_TYPE_NULL), mNumber(0)
	{
		// Nothing
	}

	JSONValue::JSONValue(Int64 value)
		: mType(JSON_TYPE_NUMBER), mNumber(value)
	{ }

	JSONValue::JSONValue(Double64 value)
		: mType(JSON_TYPE_NUMBER), mNumber(value)
	{ }

	JSONValue::JSONValue(Bool8 value)
		: mType(JSON_TYPE_BOOL), mBool(value)
	{ }

	JSONValue::JSONValue(const Char* value)
		: mType(JSON_TYPE_STRING), mString(value)
	{ }

	JSONValue::JSONValue(const String& value)
		: mType(JSON_TYPE_STRING), mString(value)
	{ }

	JSONValue::JSONValue(const JSONArray& value)
		: mType(JSON_TYPE_ARRAY), mArray(value)
	{ }

	JSONValue::JSONValue(const JSONObject& value)
		: mType(JSON_TYPE_OBJECT), mObject(value)
	{ }

	JSONValue::JSONValue(const JSONValue& value)
		: mType(value.mType)
	{
		switch (mType)
		{
			case Quartz::JSON_TYPE_NUMBER:	new (&mNumber)	Double64(value.mNumber);	break;
			case Quartz::JSON_TYPE_BOOL:	new (&mBool)	Bool8(value.mBool);			break;
			case Quartz::JSON_TYPE_STRING:	new (&mString)	String(value.mString);		break;
			case Quartz::JSON_TYPE_OBJECT:	new (&mObject)	JSONObject(value.mObject);	break;
			case Quartz::JSON_TYPE_ARRAY:	new (&mArray)	JSONArray(value.mArray);	break;
			default: break;
		}
	}

	JSONValue::~JSONValue()
	{
		switch (mType)
		{
			case Quartz::JSON_TYPE_STRING:	(&mString)->~String();		break;
			case Quartz::JSON_TYPE_OBJECT:	(&mObject)->~JSONObject();	break;
			case Quartz::JSON_TYPE_ARRAY:	(&mArray)->~JSONArray();	break;
			default: break;
		}
	}

	JSONValue& JSONValue::operator=(const JSONValue& value)
	{
		this->~JSONValue();
		new (this) JSONValue(value);
		return *this;
	}

	USize JSONValue::Size() const
	{
		if (mType == JSON_TYPE_OBJECT)
		{
			return mObject.Size();
		}
		else if (mType == JSON_TYPE_ARRAY)
		{
			return mArray.Size();
		}
		else
		{
			return 0;
		}
	}

	JSON::JSON()
		: mpRoot(nullptr)
	{
		// Nothing
	}

	JSON::JSON(JSONValue* pRoot)
		: mpRoot(pRoot)
	{
		// Nothing
	}
}