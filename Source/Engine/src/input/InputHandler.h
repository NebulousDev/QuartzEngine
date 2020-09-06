#pragma once

#include "../entity/System.h"
#include "PlatformInput.h"
#include "util\RefPtr.h"

#include "InputContext.h"
#include "InputState.h"

namespace Quartz
{
	template<typename Scope>
	using ScopedInputCallbackFunc = void(Scope::*)(InputActionFlags actions, Float32 value);
	using UnscopedInputCallbackFunc = void(*)(InputActionFlags actions, Float32 value);

	class QUARTZ_API InputHandler
	{
	private:
		struct InputDispatch
		{
			virtual void DispatchInputCallback(InputActionFlags actions, Float32 value) = 0;
		};

		template<typename Scope>
		struct ScopedInputDispatch : public InputDispatch
		{
			Scope*							pScope;
			ScopedInputCallbackFunc<Scope>	func;

			void DispatchInputCallback(InputActionFlags actions, Float32 value) override
			{
				(pScope->*func)(actions, value);
			}
		};

		struct UnscopedInputDispatch : public InputDispatch
		{
			UnscopedInputCallbackFunc func;

			void DispatchInputCallback(InputActionFlags actions, Float32 value) override
			{
				(*func)(actions, value);
			}
		};

		struct DispatchEntry
		{
			InputId inputId;
			UInt32 dispatchIndex;
		};

		friend void MouseMoveInputCallback(InputDeviceId deviceId, Int64 relX, Int64 relY);
		friend void MouseButtonInputCallback(InputDeviceId deviceId, UInt32 button, InputButtonState state);
		friend void MouseWheelInputCallback(InputDeviceId deviceId, Float32 value);
		friend void KeyboardInputCallback(InputDeviceId deviceId, UInt32 scancode, InputButtonState state);
		friend void ButtonInputCallback(InputDeviceId deviceId, UInt32 button, InputButtonState state);

	private:
		InputContext						mDefaultInputContext;
		InputContext*						mpInputContext;
		Map<InputId, Array<InputDispatch*>> mDispatches;
		Map<Handle64, DispatchEntry>		mScopeEntries;
		InputState							mInputState;

		Bool8		mMouseCaptured;
		Bool8		mMouseCapturePaused;
		Window*		mpCapturedWindow;
		Bounds2i	mCapturedBounds;
		Point2i		mMouseCapturePoint;

	public:
		InputHandler();

		void Init();
		void Update();
		void Destroy();

		void SetInputContext(InputContext* pContext);

		template<typename Scope>
		void BindInputCallback(String inputName, ScopedInputCallbackFunc<Scope> func, Scope* pScope)
		{
			ScopedInputDispatch<Scope>* pInputDispatch = new ScopedInputDispatch<Scope>;
			pInputDispatch->pScope = pScope;
			pInputDispatch->func = func;

			InputId inputId = inputName.Hash();
			Array<InputDispatch*>* pDispatchArray = mDispatches.Get(inputId);

			if (pDispatchArray == nullptr)
			{
				pDispatchArray = &mDispatches.Put(inputId, Array<InputDispatch*>());
			}

			UInt32 dispatchIndex = pDispatchArray->Size();
			pDispatchArray->PushBack(pInputDispatch);

			DispatchEntry dispatchEntry;
			dispatchEntry.inputId = inputId;
			dispatchEntry.dispatchIndex = dispatchIndex;

			mScopeEntries.Put(reinterpret_cast<Handle64>(pScope), dispatchEntry);
		}

		void BindInputCallback(String inputName, UnscopedInputCallbackFunc func)
		{
			UnscopedInputDispatch* pInputDispatch = new UnscopedInputDispatch;
			pInputDispatch->func = func;

			InputId inputId = inputName.Hash();
			Array<InputDispatch*>* pDispatchArray = mDispatches.Get(inputId);

			if (pDispatchArray == nullptr)
			{
				pDispatchArray = &mDispatches.Put(inputId, Array<InputDispatch*>());
			}

			UInt32 dispatchIndex = pDispatchArray->Size();
			pDispatchArray->PushBack(pInputDispatch);
		}

		//TODO: Allow unbinding of unscoped functions?

		template<typename Scope>
		void UnbindInputCallbacks(Scope* pScope)
		{
			Handle64 scopeHandle = reinterpret_cast<Handle64>(pScope);

			DispatchEntry* pDispatchEntry = mScopeEntries.Get(scopeHandle);
			if (pDispatchEntry != nullptr)
			{
				Array<InputDispatch*>* pDispatchArray = mDispatches.Get(pDispatchEntry->inputId);

				delete (*pDispatchArray)[pDispatchEntry->dispatchIndex];
				pDispatchArray->Remove(pDispatchEntry->dispatchIndex);

				mScopeEntries.Remove(scopeHandle);
			}
		}

		Point2i GetMousePosition() const;

		void CaptureMouse(Window& window, Bounds2i bounds);
		void PauseCaptureMouse();
		void ResumeCaptureMouse();
		void ReleaseMouse();

		Bool8 IsMouseCaptured() const;
		Bool8 IsMouseCapturePaused() const;
	};
}