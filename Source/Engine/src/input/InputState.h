#pragma once

#include "Common.h"
#include "util/Map.h"

namespace Quartz
{
	typedef UInt64 InputId;

	enum InputAction : UInt8
	{
		INPUT_NO_ACTION = 0x0,
		INPUT_HELD = 0x1,
		INPUT_PRESSED = 0x2,
		INPUT_RELEASED = 0x4,
		INPUT_REPEATED = 0x8
	};

	typedef Flags8 InputActionFlags;

	struct InputBinding
	{
		InputId	inputId;
		Float32	scale;
		Bool8	needsReset;

		FORCE_INLINE Bool8 operator==(const InputBinding& binding)
		{
			return inputId == binding.inputId && scale == binding.scale;
		}
	};

	class InputContext;

	class InputState
	{
	public:
		friend class InputContext;
		friend class InputHandler;

		struct InputBindingState
		{
			InputActionFlags	actions;
			Float32				value;
			Bool8				isCurrent;
		};

	private:
		Map<InputBinding, InputBindingState> mInputState;

	public:
		InputState();

		void GenerateStates(InputContext& context);

		Bool8 HasState(InputBinding binding);
		void UpdateState(InputBinding binding, InputActionFlags newActions, Float32 newValue);

		InputActionFlags GetStateActionFlags(InputBinding binding);
		Float32 GetStateValue(InputBinding binding);
	};

	template<>
	FORCE_INLINE UInt32 Hash<InputBinding>(const InputBinding& value)
	{
		return value.inputId * (value.scale + 1);
	}
}