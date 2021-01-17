#pragma once

#include "platform/VPInput.h"

#include "Win32.h"

#include "util/Map.h"

namespace Quartz
{
	struct Win32InputDeviceInfo
	{
		StringW id;
		StringW parentId;
		StringW name;
		StringW vendor;
		StringW type;
		StringW fullPath;
		UInt32 usagePage;
		UInt32 usage;
	};

	struct Win32InputMouse : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	struct Win32InputKeyboard : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	struct Win32InputController : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	typedef StringW DeviceId;

	class QUARTZ_API Win32Input : public VPInput
	{
	private:
		VPMouseMoveCallbackFunc mVPMouseMoveCallbackFunc;
		VPMouseButtonCallbackFunc mVPMouseButtonCallbackFunc;
		VPKeyboardKeyCallbackFunc mVPKeyboardKeyCallbackFunc;

		Map<Handle64, DeviceId> mDeviceIds;

		Map<DeviceId, Win32InputMouse*> mMice;
		Map<DeviceId, Win32InputKeyboard*> mKeyboards;
		Map<DeviceId, Win32InputController*> mControllers;

	public:
		void PreInitialize() override;
		void Initialize() override;

		void SetMouseMoveCallback(VPMouseMoveCallbackFunc callback) override;
		void SetMouseButtonCallback(VPMouseButtonCallbackFunc callback) override;
		void SetKeyboardKeyCallback(VPKeyboardKeyCallbackFunc callback) override;

		void PollConnections() override;
		void PollInput() override;
	};
}