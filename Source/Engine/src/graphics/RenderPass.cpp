#include "RenderPass.h"

namespace Quartz
{
	Attachment::Attachment(const String& name, AttachmentType type, ImageFormat format)
		: mName(name), mType(type), mFormat(format)
	{
		// Nothing
	}

	Subpass::Subpass(const String& name, const Array<UInt32>& attachmentIndices)
		: mName(name), mAttachments(attachmentIndices)
	{
		// Nothing
	}

	Renderpass::Renderpass(const String& name, const Array<Attachment>& attachments, const Array<Subpass>& subpasses)
		: mName(name), mAttachments(attachments), mSubpasses(subpasses)
	{
		RegenerateHash();
	}

	UInt64 Renderpass::RegenerateHash()
	{
		UInt64 hash = mName.Hash();
		return hash;
	}
}