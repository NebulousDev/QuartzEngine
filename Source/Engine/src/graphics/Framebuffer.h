#pragma once

#include "RenderPass.h"

namespace Quartz
{
	class QUARTZ_API ImageView : public ManagedResource
	{
	private:
		UInt32 width;
		UInt32 height;
		UInt32 depth;

	public:

		UInt64 RegenerateHash() override;

	};

	class QUARTZ_API Framebuffer : public ManagedResource
	{
	private:


	public:
		UInt64 RegenerateHash() override;
	};

	class QUARTZ_API FramebufferCollection
	{
	private:
		Array<ImageView> mImages;
		Array<HGFXFramebuffer> mFramebuffers;

	public:
		FramebufferCollection(UInt32 width, UInt32 height, const Renderpass& renderpass, UInt32 bufferCount);

		FORCE_INLINE const Array<ImageView>& GetImageViews() const { return mImages; }
		FORCE_INLINE const Array<HGFXFramebuffer>& GetFramebuffers() const { return mFramebuffers; }
	};
}