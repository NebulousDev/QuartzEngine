#pragma once

#include "util/Array.h"
#include "VGFXContext.h"
#include "util/String.h"
#include "../object/Image.h"

namespace Quartz
{
	class ManagedResource
	{
	private:
		UInt64 mHash;

	public:
		virtual UInt64 RegenerateHash() = 0;

		FORCE_INLINE UInt64 GetHash() const { return mHash; }
	};

	enum AttachmentType
	{
		ATTACHMENT_SWAPCHAIN,
		ATTACHMENT_COLOR,
		ATTACHMENT_DEPTH,
		ATTACHMENT_STENCIL,
		ATTACHMENT_DEPTH_STENCIL
	};

	class QUARTZ_API Attachment
	{
	private:
		String mName;
		AttachmentType mType;
		ImageFormat mFormat;

	public:
		Attachment(const String& name, AttachmentType type, ImageFormat format);

		FORCE_INLINE String GetName() const { return mName; }
		FORCE_INLINE AttachmentType GetType() const { return mType; }
		FORCE_INLINE ImageFormat GetFormat() const { return mFormat; }
	};

	class QUARTZ_API Subpass
	{
	private:
		String mName;
		Array<UInt32> mAttachments;

	public:
		Subpass(const String& name, const Array<UInt32>& attachmentIndices);

		FORCE_INLINE String GetName() const { return mName; }
		FORCE_INLINE const Array<UInt32>& GetAttachments() const { return mAttachments; }
	};

	class QUARTZ_API Renderpass : public ManagedResource
	{
	private:
		String mName;
		Array<Attachment> mAttachments;
		Array<Subpass> mSubpasses;

	public:
		Renderpass(const String& name, const Array<Attachment>& attachments, const Array<Subpass>& subpasses);

		UInt64 RegenerateHash() override;

		FORCE_INLINE String GetName() const { return mName; }
		FORCE_INLINE const Array<Attachment>& GetAttachments() const { return mAttachments; }
		FORCE_INLINE const Array<Subpass>& GetSubpasses() const { return mSubpasses; }
	};
}