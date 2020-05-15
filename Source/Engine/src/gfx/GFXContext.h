#pragma once
#include "module\Module.h"
#include "util\Array.h"

namespace Quartz
{
	/********************************************************************************************/


	class QUARTZ_API GFXQueueFamily
	{
	protected:
		UInt32	mFamilyIndex;
		UInt32	mQueueCount;
		Flags32 mQueueFlags;

		//Bool8 mPresentationSupport?

		GFXQueueFamily() = default;

	public:
		virtual ~GFXQueueFamily() = default;

		FORCE_INLINE UInt32 GetFamilyIndex() const { return mFamilyIndex; }
		FORCE_INLINE UInt32 GetQueueCount() const { return mQueueCount; }
		FORCE_INLINE Bool8	GetQueueFlags() const { return mQueueFlags; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXQueueFlags : Flags32
	{
		GFX_QUEUE_TYPE_NONE				= 0x00,
		GFX_QUEUE_TYPE_GRAPHICS_BIT		= 0x01,
		GFX_QUEUE_TYPE_COMPUTE_BIT		= 0x02,
		GFX_QUEUE_TYPE_TRANSFER_BIT		= 0x04,
		GFX_QUEUE_TYPE_BINDING_BIT		= 0x08,
		GFX_QUEUE_TYPE_GENERIC_BIT		= 0x10
	};

	struct QUARTZ_API GFXQueueInfo
	{
		const Char*		debugName;
		GFXQueueFlags	queueFlags;
		UInt32			familyIndex;
		UInt32			queueIndex;
		Float32			priority;
	};

	class QUARTZ_API GFXQueue
	{
	protected:
		const Char*		mDebugName;
		GFXQueueFlags	mQueueFlags;
		UInt32			mFamilyIndex;
		UInt32			mQueueIndex;
		Float32			mPriority;

		GFXQueue() = default;

	public:
		virtual ~GFXQueue() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE Flags32		GetQueueFlags() const { return mQueueFlags; }
		FORCE_INLINE UInt32			GetFamilyIndex() const { return mFamilyIndex; }
		FORCE_INLINE UInt32			GetQueueIndex() const { return mQueueIndex; }
		FORCE_INLINE Float32		GetPriority() const { return mPriority; }
	};

	typedef Array<GFXQueueInfo>			GFXQueueInfoList;
	typedef Array<GFXQueue*>			GFXQueueList;
	typedef Array<Array<GFXQueue*>>		GFXQueueMatrix;
	typedef Array<GFXQueueFamily*>		GFXQueueFamilyList;


	/********************************************************************************************/


	class QUARTZ_API GFXPhysicalDevice
	{
	protected:
		const Char*			mDebugName;
		const Char*			mDeviceName;
		const Char*			mVendorName;
		UInt32				mDeviceID;
		UInt32				mVendorID;
		UInt32				mApiVersion;
		UInt32				mDriverVersion;
		GFXQueueFamilyList	mQueueFamilyList;

		GFXPhysicalDevice() = default;

	public:
		virtual ~GFXPhysicalDevice() = default;

		FORCE_INLINE const Char*				GetDebugName() const { return mDebugName; }
		FORCE_INLINE const Char*				GetDeviceName() const { return mDeviceName; };
		FORCE_INLINE const Char*				GetVendorName() const { return mVendorName; };
		FORCE_INLINE UInt32						GetDeviceID() const { return mDeviceID; };
		FORCE_INLINE UInt32						GetVendorID() const { return mVendorID; };
		FORCE_INLINE UInt32						GetApiVersion() const { return mApiVersion; };
		FORCE_INLINE UInt32						GetDriverVersion() const { return mDriverVersion; };
		FORCE_INLINE const GFXQueueFamilyList&	GetQueueFamilies() const { return mQueueFamilyList; }
	};

	typedef Array<GFXPhysicalDevice*> GFXPhysicalDeviceList;


	/********************************************************************************************/


	struct QUARTZ_API GFXDeviceInfo
	{
		const Char*			debugName;
		GFXQueueInfoList*	pQueueList;
	};

	class QUARTZ_API GFXDevice
	{
	protected:
		const Char*			mDebugName;
		GFXPhysicalDevice*	mpPhysicalDevice;
		GFXQueueMatrix		mQueueMatrix;

		GFXDevice() = default;

	public:
		virtual ~GFXDevice() = default;

		GFXQueue*			GetQueue(UInt32 familyIndex, UInt32 queueIndex);
		const GFXQueueList& GetQueueList(UInt32 familyIndex);

		FORCE_INLINE const Char*		GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXPhysicalDevice* GetPhysicalDevice() const { return mpPhysicalDevice; }
	};

	typedef Array<GFXDevice*> GFXDeviceList;


	/********************************************************************************************/


	class Window;

	struct QUARTZ_API GFXSurfaceInfo
	{
		const Char*	debugName;
		Window*		pWindow;
	};

	class QUARTZ_API GFXSurface
	{
	protected:
		const Char*	mDebugName;
		UInt32		mWidth;
		UInt32		mHeight;

		// SurfaceFormats?

		GFXSurface() = default;

	public:
		virtual ~GFXSurface() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE UInt32			GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32			GetHeight() const { return mHeight; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXPresentMode
	{
		GFX_PRESENT_MODE_IMMEDIATE,
		GFX_PRESENT_MODE_VSYNC,
		GFX_PRESENT_MODE_TRIPPLE_BUFFERED
	};

	enum QUARTZ_API GFXSharingMode
	{
		GFX_SHARING_MODE_EXCLUSIVE,
		GFX_SHARING_MODE_SHARED
	};

	struct QUARTZ_API GFXSwapchainInfo
	{
		const Char*		debugName;
		GFXPresentMode	presentMode;
		GFXSharingMode	sharingMode;
		UInt32			imageLayers;
		Bool8			clipped;
	};

	class QUARTZ_API GFXSwapchain
	{
	protected:
		const Char*		mDebugName;
		GFXDevice*		mpParentDevice;
		GFXSurface*		mpSurface;
		GFXPresentMode	mPresentMode;
		GFXSharingMode	mSharingMode;
		UInt32			mImageLayers;
		Bool8			mClipped;

		GFXSwapchain() = default;

	public:
		virtual ~GFXSwapchain() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXDevice*		GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE GFXSurface*	GetSurface() const { return mpSurface; }
		FORCE_INLINE GFXPresentMode GetPresentMode() const { return mPresentMode; }
		FORCE_INLINE GFXSharingMode GetSharingMode() const { return mSharingMode; }
		FORCE_INLINE UInt32			GetImageLayers() const { return mImageLayers; }
		FORCE_INLINE Bool8			IsClipped() const { return mClipped; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXBufferMapFlagBits : Flags32
	{
		GFX_BUFFER_FLAG_NONE = 0x0
	};

	typedef Flags32 GFXBufferMapFlags;

	enum QUARTZ_API GFXBufferUsageFlagBits : Flags32
	{
		GFX_BUFFER_USAGE_NONE				= 0x00,
		GFX_BUFFER_USAGE_VERTEX_BIT			= 0x01,
		GFX_BUFFER_USAGE_INDEX_BIT			= 0x02,
		GFX_BUFFER_USAGE_UNIFORM_BIT		= 0x04,
		GFX_BUFFER_USAGE_STORAGE_BIT		= 0x08,
		GFX_BUFFER_USAGE_TRANSFER_SRC_BIT	= 0x10,
		GFX_BUFFER_USAGE_TRANSFER_DEST_BIT	= 0x20
	};

	typedef Flags32 GFXBufferUsageFlags;
	
	enum QUARTZ_API GFXBufferMemoryFlagBits : Flags32
	{
		GFX_BUFFER_MEMORY_NONE			= 0x00,
		GFX_BUFFER_MEMORY_VISIBLE_BIT	= 0x01,
		GFX_BUFFER_MEMORY_COHERENT_BIT	= 0x02
	};

	typedef Flags32 GFXBufferMemoryFlags;

	struct QUARTZ_API GFXBufferInfo
	{
		const Char*				debugName;
		USize					size;
		GFXBufferUsageFlags		usageFlags;
		GFXBufferMemoryFlags	memoryFlags;
		GFXSharingMode			sharingMode;
	};

	class QUARTZ_API GFXBuffer
	{
	protected:
		const Char*				mDebugName;
		GFXDevice*				mpParentDevice;
		USize					mSize;
		GFXBufferUsageFlags		mUsageFlags;
		GFXBufferMemoryFlags	mMemoryFlags;
		GFXSharingMode			mSharingMode;

	public:
		GFXBuffer() = default;

		virtual ~GFXBuffer() = default;

		FORCE_INLINE const Char*			GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXDevice*				GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE USize					GetSize() const { return mSize; }
		FORCE_INLINE GFXBufferUsageFlags	GetBufferUsageFlags() const { return mUsageFlags; }
		FORCE_INLINE GFXBufferMemoryFlags	GetBufferMemoryFlags() const { return mMemoryFlags; }
		FORCE_INLINE GFXSharingMode			GetSharingMode() const { return mSharingMode; }
	};

	typedef Array<GFXBuffer> GFXBufferArray;


	/********************************************************************************************/


	enum QUARTZ_API GFXFormat
	{
		GFX_FORMAT_UNKNOWN,

		GFX_FORMAT_R32G32B32_SFLOAT,
		GFX_FORMAT_R32G32B32A32_SFLOAT,

		GFX_FORMAT_R8G8B8_UNORM,
		GFX_FORMAT_R8G8B8A8_UNORM,

		GFX_FORMAT_B8G8R8_UNORM,
		GFX_FORMAT_B8G8R8A8_UNORM
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXImageType
	{
		GFX_IMAGE_TYPE_UNKNOWN,
		GFX_IMAGE_TYPE_1D,
		GFX_IMAGE_TYPE_2D,
		GFX_IMAGE_TYPE_3D,
		GFX_IMAGE_TYPE_CUBE_MAP
	};

	struct QUARTZ_API GFXImageInfo
	{
		const Char*		debugName;
		UInt32			width;
		UInt32			height;
		GFXFormat		format;
		GFXImageType	imageType;
		UInt32			mipLevels;
		UInt32			multisamples;
		GFXSharingMode	sharingMode;
	};

	class QUARTZ_API GFXImage
	{
	protected:
		const Char*		mDebugName;
		GFXDevice*		mpParentDevice;
		UInt32			mWidth;
		UInt32			mHeight;
		GFXFormat	mImageFormat;
		GFXImageType	mImageType;
		UInt32			mMipLevels;
		UInt32			mMultisamples;
		GFXSharingMode	mSharingMode;

		GFXImage() = default;

	public:
		virtual ~GFXImage() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXDevice*		GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE UInt32			GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32			GetHeight() const { return mHeight; }
		FORCE_INLINE GFXFormat GetImageFormat() const { return mImageFormat; }
		FORCE_INLINE GFXImageType	GetImageType() const { return mImageType; }
		FORCE_INLINE UInt32			GetMipLevels() const { return mMipLevels; }
		FORCE_INLINE UInt32			GetMultisamples() const { return mMultisamples; }
		FORCE_INLINE GFXSharingMode GetSharingMode() const { return mSharingMode; }
	};


	/********************************************************************************************/


	struct QUARTZ_API GFXImageViewInfo
	{
		const Char*		debugName;
		GFXImageType	imageType;
		GFXFormat	imageFormat;
		UInt32			baseMipLevel;
	};

	class QUARTZ_API GFXImageView
	{
	protected:
		const Char*		mDebugName;
		GFXImage*		mpImage;
		GFXImageType	mImageType;
		GFXFormat	mImageFormat;
		UInt32			mBaseMipLevel;

		GFXImageView() = default;

	public:
		virtual ~GFXImageView() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXImage*		GetImage() const { return mpImage; }
		FORCE_INLINE GFXImageType	GetImageType() const { return mImageType; }
		FORCE_INLINE GFXFormat GetImageFormat() const { return mImageFormat; }
		FORCE_INLINE UInt32			GetBaseMipLevle() const { return mBaseMipLevel; }
	};

	typedef Array<GFXImage*> GFXImageArray;
	typedef Array<GFXImageView*> GFXImageViewArray;


	/********************************************************************************************/


	enum QUARTZ_API GFXShaderLang
	{
		GFX_SHADER_LANG_GLSL,
		GFX_SHADER_LANG_HLSL,
		GFX_SHADER_LANG_SPIRV
	};

	enum QUARTZ_API GFXShaderStage
	{
		GFX_SHADER_STAGE_VERTEX						= 0x01,
		GFX_SHADER_STAGE_TESSELATION_CONTROL		= 0x02,
		GFX_SHADER_STAGE_TESSELATION_EVALUATION		= 0x04,
		GFX_SHADER_STAGE_GEOMETRY					= 0x08,
		GFX_SHADER_STAGE_FRAGMENT					= 0x10,
		GFX_SHADER_STAGE_COMPUTE					= 0x20
	};

	typedef Flags32 GFXShaderStageFlags;

	struct QUARTZ_API GFXShaderInfo
	{
		const Char*		debugName;
		GFXShaderLang	shaderLang;
		GFXShaderStage	shaderStage;
		const Char*		entryName;
		USize			codeSize;
		void*			pCode;
	};
	
	class QUARTZ_API GFXShader
	{
	protected:
		const Char*		mDebugName;
		GFXDevice*		mpParentDevice;
		GFXShaderStage	mShaderStage;

		GFXShader() = default;

	public:
		virtual ~GFXShader() = default;

		FORCE_INLINE const Char*	GetDebugName() const { return mDebugName; }
		FORCE_INLINE GFXDevice*		GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE GFXShaderStage	GetShaderStage() const { return mShaderStage; }
	};


	/********************************************************************************************/


	enum GFXVertexInputRate
	{
		GFX_VERTEX_INPUT_RATE_VERTEX,
		GFX_VERTEX_INPUT_RATE_INSTANCE
	};

	struct GFXVertexInputBindingInfo
	{
		UInt32				binding;
		UInt32				stride;
		GFXVertexInputRate	inputRate;
	};

	struct GFXVertexInputAttributeInfo
	{
		UInt32		location;
		UInt32		binding;
		GFXFormat	format;
		UInt32		offset;
	};

	struct QUARTZ_API GFXVertexInputStateInfo
	{
		UInt32							vertexBindingCount;
		GFXVertexInputBindingInfo*		pVertexBindingInfos;
		UInt32							vertexAttributeCount;
		GFXVertexInputAttributeInfo*	pVertexAttributeInfos;
	};

	class QUARTZ_API GFXVertexInputState
	{
	protected:
		UInt32							mVertexBindingCount;
		GFXVertexInputBindingInfo*		mpVertexBindingInfos;
		UInt32							mVertexAttributeCount;
		GFXVertexInputAttributeInfo*	mpVertexAttributeInfos;

		GFXVertexInputState() = default;

	public:
		virtual ~GFXVertexInputState() = default;

		FORCE_INLINE UInt32							GetVertexBindingCount() const { return mVertexBindingCount; }
		FORCE_INLINE GFXVertexInputBindingInfo*		GetVertexBindingInfos() const { return mpVertexBindingInfos; }
		FORCE_INLINE UInt32							GetVertexAttributeCount() const { return mVertexAttributeCount; }
		FORCE_INLINE GFXVertexInputAttributeInfo*	GetVertexAttributeInfos() const { return mpVertexAttributeInfos; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXPrimitiveTopology
	{
		GFX_PRIMITIVE_TOPOLOGY_POINTS,
		GFX_PRIMITIVE_TOPOLOGY_LINES,
		GFX_PRIMITIVE_TOPOLOGY_LINE_STRIPS,
		GFX_PRIMITIVE_TOPOLOGY_TRIANGLES,
		GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIPS
	};

	struct QUARTZ_API GFXInputAssemblyStateInfo
	{
		GFXPrimitiveTopology topology;
	};

	class QUARTZ_API GFXInputAssemblyState
	{
	protected:
		GFXPrimitiveTopology mTopology;

		GFXInputAssemblyState() = default;

	public:
		virtual ~GFXInputAssemblyState() = default;

		FORCE_INLINE GFXPrimitiveTopology GetPrimitiveTopology() const { return mTopology; }
	};


	/********************************************************************************************/


	struct QUARTZ_API GFXViewport
	{
		Float32 x;
		Float32 y;
		Float32 width;
		Float32 height;
		Float32 minDepth;
		Float32 maxDepth;
	};

	struct QUARTZ_API GFXScissor
	{
		Int32 x;
		Int32 y;
		UInt32 width;
		UInt32 height;
	};

	struct QUARTZ_API GFXViewportStateInfo
	{
		GFXViewport viewport;
		GFXScissor	scissor;
	};

	class QUARTZ_API GFXViewportState
	{
	protected:
		GFXViewport mViewport;
		GFXScissor	mScissor;

		GFXViewportState() = default;

	public:
		virtual ~GFXViewportState() = default;

		FORCE_INLINE GFXViewport	GetViewport() const { return mViewport; }
		FORCE_INLINE GFXScissor		GetScissor() const { return mScissor; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXFillMode
	{
		GFX_POLYGON_FILL_MODE_FILL,
		GFX_POLYGON_FILL_MODE_LINE,
		GFX_POLYGON_FILL_MODE_POINT
	};

	enum QUARTZ_API GFXCullFace
	{
		GFX_CULL_FACE_NONE,
		GFX_CULL_FACE_FRONT,
		GFX_CULL_FACE_BACK,
		GFX_CULL_FACE_FRONT_AND_BACK
	};

	enum QUARTZ_API GFXFrontFace
	{
		GFX_FRONT_FACE_CLOCKWISE,
		GFX_FRONT_FACE_COUNTER_CLOCKWISE
	};

	struct QUARTZ_API GFXRasterizationStateInfo
	{
		GFXFillMode		fillMode;
		Float32			lineWidth;
		GFXCullFace		cullFace;
		GFXFrontFace	frontFace;
	};

	class QUARTZ_API GFXRasterizationState
	{
	protected:
		GFXFillMode		mFillMode;
		Float32			mLineWidth;
		GFXCullFace		mCullFace;
		GFXFrontFace	mFrontFace;

		GFXRasterizationState() = default;

	public:
		virtual ~GFXRasterizationState() = default;

		FORCE_INLINE GFXFillMode	GetFillMode() const { return mFillMode; }
		FORCE_INLINE Float32		GetLineWidth() const { return mLineWidth; }
		FORCE_INLINE GFXCullFace	GetCullFace() const { return mCullFace; }
		FORCE_INLINE GFXFrontFace	GetFrontFace() const { return mFrontFace; }
	};


	/********************************************************************************************/


	struct QUARTZ_API GFXMultisampleStateInfo
	{
		Bool8	enableMultisampling;
		UInt32	multisamples;
	};

	class QUARTZ_API GFXMultisampleState
	{
	protected:
		Bool8	mMultisamplingEnabled;
		UInt32	mMultisamples;

		GFXMultisampleState() = default;

	public:
		virtual ~GFXMultisampleState() = default;

		FORCE_INLINE Bool8	IsMultisamplingEnabled() const { return mMultisamplingEnabled; }
		FORCE_INLINE UInt32	GetMultisamples() const { return mMultisamples; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXColorMaskBits : Flags32
	{
		GFX_COLOR_COMPONENT_NONE	= 0x0,
		GFX_COLOR_COMPONENT_R_BIT	= 0x1,
		GFX_COLOR_COMPONENT_G_BIT	= 0x2,
		GFX_COLOR_COMPONENT_B_BIT	= 0x4,
		GFX_COLOR_COMPONENT_A_BIT	= 0x8,
		GFX_COLOR_COMPONENT_ALL		= 0xF
	};

	typedef Flags32 GFXColorMask;

	enum QUARTZ_API GFXBlendFactor
	{
		GFX_BLEND_FACTOR_ONE,
		GFX_BLEND_FACTOR_ZERO,

		GFX_BLEND_FACTOR_SRC_COLOR,
		GFX_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
		GFX_BLEND_FACTOR_DST_COLOR,
		GFX_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
		GFX_BLEND_FACTOR_CONSTANT_COLOR,
		GFX_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,

		GFX_BLEND_FACTOR_SRC_ALPHA,
		GFX_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		GFX_BLEND_FACTOR_DST_ALPHA,
		GFX_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		GFX_BLEND_FACTOR_CONSTANT_ALPHA,
		GFX_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA
	};

	enum QUARTZ_API GFXBlendOp
	{
		GFX_BLEND_OP_ADD,
		GFX_BLEND_OP_SUBTRACT,
		GFX_BLEND_OP_REVERSE_SUBTRACT,
		GFX_BLEND_OP_MIN,
		GFX_BLEND_OP_MAX
	};

	struct QUARTZ_API GFXColorBlendAttachment
	{
		Bool8				blendEnable;
		GFXBlendFactor		srcColorBlendFactor;
		GFXBlendFactor		srcAlphaBlendFactor;
		GFXBlendOp			colorBlendOp;
		GFXBlendFactor		destColorBlendFactor;
		GFXBlendFactor		destAlphaBlendFactor;
		GFXBlendOp			alphaBlendOp;
		GFXColorMask		colorMask;
	};

	enum QUARTZ_API GFXLogicalOp
	{
		GFX_LOGICAL_OP_CLEAR,
		GFX_LOGICAL_OP_AND,
		GFX_LOGICAL_OP_AND_REVERSE,
		GFX_LOGICAL_OP_COPY,
		GFX_LOGICAL_OP_AND_INVERTED,
		GFX_LOGICAL_OP_NO_OP,
		GFX_LOGICAL_OP_XOR,
		GFX_LOGICAL_OP_OR,
		GFX_LOGICAL_OP_NOR,
		GFX_LOGICAL_OP_EQUIVALENT,
		GFX_LOGICAL_OP_INVERT,
		GFX_LOGICAL_OP_OR_REVERSE,
		GFX_LOGICAL_OP_COPY_INVERTED,
		GFX_LOGICAL_OP_OR_INVERTED,
		GFX_LOGICAL_OP_NAND,
		GFX_LOGICAL_OP_SET,
	};

	struct QUARTZ_API GFXColorBlendStateInfo
	{
		Bool8						logicalOpEnable;
		GFXLogicalOp				logicalOp;
		UInt32						attachmentCount;
		GFXColorBlendAttachment*	pAttachments;
		Float32						blendConstants[4];
	};

	class QUARTZ_API GFXColorBlendState
	{
	protected:
		Bool8						mLogicalOpEnabled;
		GFXLogicalOp				mLogicalOp;
		UInt32						mAttachmentCount;
		GFXColorBlendAttachment*	mpAttachments;
		Float32						mBlendConstants[4];

		GFXColorBlendState() = default;

	public:
		virtual ~GFXColorBlendState() = default;

		FORCE_INLINE Bool8						IsLogicalOpEnabled() const { return mLogicalOpEnabled; }
		FORCE_INLINE GFXLogicalOp				GetLogicalOp() const { return mLogicalOp; }
		FORCE_INLINE UInt32						GetAttachmentCount() const { return mAttachmentCount; }
		FORCE_INLINE GFXColorBlendAttachment*	GetColorBlendAttachments() const { return mpAttachments; }
		FORCE_INLINE const Float32*				GetBlendConstants() const { return mBlendConstants; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXDescriptorType
	{
		GFX_DESCRIPTOR_TYPE_UNIFORM
	};

	struct QUARTZ_API GFXDescriptorBinding
	{
		UInt32				binding;
		GFXDescriptorType	type;
		UInt32				count;
		GFXShaderStageFlags	stageFlags;
		// SAMPLER
	};

	struct QUARTZ_API GFXDescriptorSetLayoutInfo
	{
		UInt32					bindingCount;
		GFXDescriptorBinding*	pBindings;
	};

	class QUARTZ_API GFXDescriptorSetLayout
	{
	protected:
		GFXDevice*				mpParentDevice;
		UInt32					mBindingCount;
		GFXDescriptorBinding*	mpBindings;

		GFXDescriptorSetLayout() = default;

	public:
		virtual ~GFXDescriptorSetLayout() = default;

		FORCE_INLINE GFXDevice*				GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE UInt32					GetBindingCount() const { return mBindingCount; }
		FORCE_INLINE GFXDescriptorBinding*	GetDescriptorBindings() const { return mpBindings; }
	};


	/********************************************************************************************/


	struct QUARTZ_API GFXPipelineLayoutInfo
	{
		UInt32					layoutCount;
		GFXDescriptorSetLayout* pDescriptorSetLayouts;
	};

	class QUARTZ_API GFXPipelineLayout
	{
	protected:
		GFXDevice*				mpParentDevice;
		UInt32					mLayoutCount;
		GFXDescriptorSetLayout* mpDescriptorSetLayouts;

		GFXPipelineLayout() = default;

	public:
		virtual ~GFXPipelineLayout() = default;

		FORCE_INLINE GFXDevice*					GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE UInt32						GetLayoutCount() const { return mLayoutCount; }
		FORCE_INLINE GFXDescriptorSetLayout*	GetDescriptorSetLayouts() const { return mpDescriptorSetLayouts; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXLoadOp
	{
		GFX_LOAD_OP_LOAD,
		GFX_LOAD_OP_CLEAR,
		GFX_LOAD_OP_DONT_CARE
	};

	enum QUARTZ_API GFXStoreOp
	{
		GFX_STORE_OP_STORE,
		GFX_STORE_OP_DONT_CARE
	};

	enum QUARTZ_API GFXImageLayout
	{
		GFX_IMAGE_LAYOUT_UNDEFINED,
		GFX_IMAGE_LAYOUT_GENERAL,
		GFX_IMAGE_LAYOUT_COLOR_ATTACHMENT,
		GFX_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT,
		GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY,
		GFX_IMAGE_LAYOUT_SHADER_READ_ONLY,
		GFX_IMAGE_LAYOUT_TRANSFER_SRC,
		GFX_IMAGE_LAYOUT_TRANSFER_DEST,
		GFX_IMAGE_LAYOUT_PREINITIALIZED,
		GFX_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT,
		GFX_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY,
		GFX_IMAGE_LAYOUT_PRESENT_SRC,
		GFX_IMAGE_LAYOUT_SHARED_PRESENT
	};

	struct QUARTZ_API GFXRenderAttachmentInfo
	{
		GFXFormat	imageFormat;
		UInt32			multisamples;
		GFXLoadOp		loadOp;
		GFXStoreOp		storeOp;
		GFXLoadOp		stencilLoadOp;
		GFXStoreOp		stencilStoreOp;
		GFXImageLayout	initialLayout;
		GFXImageLayout	finalLayout;
	};

	class QUARTZ_API GFXRenderAttachment
	{
	protected:
		GFXFormat		mImageFormat;
		UInt32			mMultisamples;
		GFXLoadOp		mLoadOp;
		GFXStoreOp		mStoreOp;
		GFXLoadOp		mStencilLoadOp;
		GFXStoreOp		mStencilStoreOp;
		GFXImageLayout	mInitialLayout;
		GFXImageLayout	mFinalLayout;

		GFXRenderAttachment() = default;

	public:
		virtual ~GFXRenderAttachment() = default;

		FORCE_INLINE GFXFormat		GetImageFormat() const { return mImageFormat; }
		FORCE_INLINE UInt32			GetMultisamples() const { return mMultisamples; }
		FORCE_INLINE GFXLoadOp		GetLoadOp() const { return mLoadOp; }
		FORCE_INLINE GFXStoreOp		GetStoreOp() const { return mStoreOp; }
		FORCE_INLINE GFXLoadOp		GetStencilLoadOp() const { return mStencilLoadOp; }
		FORCE_INLINE GFXStoreOp		GetStencilStoreOp() const { return mStencilStoreOp; }
		FORCE_INLINE GFXImageLayout	GetInitialLayout() const { return mInitialLayout; }
		FORCE_INLINE GFXImageLayout	GetFinalLayout() const { return mFinalLayout; }
	};

	// Move into Attachment object?
	struct QUARTZ_API GFXRenderAttachmentReference
	{
		UInt32			index;
		GFXImageLayout	layout;
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXPipelineBindPoint
	{
		GFX_PIPELINE_BIND_POINT_GRAPHICS,
		GFX_PIPELINE_BIND_POINT_COMPUTE
	};

	struct QUARTZ_API GFXRenderSubpassInfo
	{
		GFXPipelineBindPoint			bindPoint;		//must be graphics?
		UInt32							inputAttachmentCount;
		GFXRenderAttachmentReference*	pInputAttachments;
		UInt32							colorAttachmentCount;
		GFXRenderAttachmentReference*	pColorAttachments;
		GFXRenderAttachmentReference*	pResolveAttachments;
		GFXRenderAttachmentReference*	pDepthStencilAttachment;
		UInt32							preservedAttachmentCount;
		UInt32*							pPreservedAttachmentIndices;
	};

	class QUARTZ_API GFXRenderSubpass
	{
	protected:
		GFXPipelineBindPoint			mBindPoint;		//must be graphics?
		UInt32							mInputAttachmentCount;
		GFXRenderAttachmentReference*	mpInputAttachments;
		UInt32							mColorAttachmentCount;
		GFXRenderAttachmentReference*	mpColorAttachments;
		GFXRenderAttachmentReference*	mpResolveAttachments;
		GFXRenderAttachmentReference*	mpDepthStencilAttachment;
		UInt32							mPreservedAttachmentCount;
		UInt32*							mpPreservedAttachmentIndices;

		GFXRenderSubpass() = default;

	public:
		virtual ~GFXRenderSubpass() = default;

		FORCE_INLINE GFXPipelineBindPoint			GetBindPoint() const { return mBindPoint; }
		FORCE_INLINE UInt32							GetInputAttachmentCount() const { return mInputAttachmentCount; }
		FORCE_INLINE GFXRenderAttachmentReference*	GetInputAttachments() const { return mpInputAttachments; }
		FORCE_INLINE UInt32							GetColorAttachmentCount() const { return mColorAttachmentCount; }
		FORCE_INLINE GFXRenderAttachmentReference*	GetColorAttachmnets() const { return mpColorAttachments; }
		FORCE_INLINE GFXRenderAttachmentReference*	GetResolveAttachments() const { return mpResolveAttachments; }
		FORCE_INLINE GFXRenderAttachmentReference*	GetDepthStencilAttachment() const { return mpDepthStencilAttachment; }
		FORCE_INLINE UInt32							GetPreservedAttachmentCount() const { return mPreservedAttachmentCount; }
		FORCE_INLINE UInt32*						GetPreservedAttachments() const { return mpPreservedAttachmentIndices; }
	};


	/********************************************************************************************/


	enum QUARTZ_API GFXPipelineStageMaskBits : Flags32
	{
		GFX_PIPELINE_STAGE_NONE									= 0x0000,
		GFX_PIPELINE_STAGE_TOP_OF_PIPE_BIT						= 0x0001,
		GFX_PIPELINE_STAGE_DRAW_INDIRECT_BIT					= 0x0002,
		GFX_PIPELINE_STAGE_VERTEX_INPUT_BIT						= 0x0004,
		GFX_PIPELINE_STAGE_VERTEX_SHADER_BIT					= 0x0008,
		GFX_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT		= 0x0010,
		GFX_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT	= 0x0020,
		GFX_PIPELINE_STAGE_GEOMETRY_SHADER_BIT					= 0x0040,
		GFX_PIPELINE_STAGE_FRAGMENT_SHADER_BIT					= 0x0080,
		GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT			= 0x0100,
		GFX_PIPELINE_STAGE_COMPUTE_SHADER_BIT					= 0x0200,
		GFX_PIPELINE_STAGE_TRANSFER_BIT							= 0x0400,
		GFX_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT					= 0x0800,
		GFX_PIPELINE_STAGE_HOST_BIT								= 0x1000
	};

	typedef Flags32 GFXPipelineStageMask;

	enum QUARTZ_API GFXAccessMaskBits : Flags32
	{
		GFX_ACCESS_NONE									= 0x00000,
		GFX_ACCESS_INDIRECT_COMMAND_READ_BIT			= 0x00001,
		GFX_ACCESS_INDEX_READ_BIT						= 0x00002,
		GFX_ACCESS_VERTEX_ATTRIBUTE_READ_BIT			= 0x00004,
		GFX_ACCESS_UNIFORM_READ_BIT						= 0x00008,
		GFX_ACCESS_INPUT_ATTACHMENT_READ_BIT			= 0x00010,
		GFX_ACCESS_SHADER_READ_BIT						= 0x00020,
		GFX_ACCESS_SHADER_WRITE_BIT						= 0x00040,
		GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT			= 0x00080,
		GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT			= 0x00100,
		GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT	= 0x00200,
		GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT	= 0x00400,
		GFX_ACCESS_TRANSFER_READ_BIT					= 0x00800,
		GFX_ACCESS_TRANSFER_WRITE_BIT					= 0x01000,
		GFX_ACCESS_HOST_READ_BIT						= 0x02000,
		GFX_ACCESS_HOST_WRITE_BIT						= 0x04000,
		GFX_ACCESS_MEMORY_READ_BIT						= 0x08000,
		GFX_ACCESS_MEMORY_WRITE_BIT						= 0x10000,
	};

	typedef Flags32 GFXAccessMask;

	struct QUARTZ_API GFXSubpassDependency
	{
		UInt32					srcSubpassIndex;
		UInt32					destSubpassIndex;
		GFXPipelineStageMask	srcStageMask;
		GFXPipelineStageMask	destStageMask;
		GFXAccessMask			srcAccessMask;
		GFXAccessMask			destAccessMask;
	};

	struct QUARTZ_API GFXRenderPassInfo
	{
		UInt32					attachmentCount;
		GFXRenderAttachment**	ppAttachments;
		UInt32					subpassCount;
		GFXRenderSubpass**		ppSubpasses;
		UInt32					dependencyCount;
		GFXSubpassDependency*	pDependencies;
	};

	class QUARTZ_API GFXRenderPass
	{
	protected:
		GFXDevice*				mpParentDevice;
		UInt32					mAttachmentCount;
		GFXRenderAttachment**	mppAttachments;
		UInt32					mSubpassCount;
		GFXRenderSubpass**		mppSubpasses;
		UInt32					mDependencyCount;
		GFXSubpassDependency*	mpDependencies;

		GFXRenderPass() = default;

	public:
		virtual ~GFXRenderPass() = default;

		FORCE_INLINE GFXDevice*				GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE UInt32					GetAttachmentCount() const { return mAttachmentCount; }
		FORCE_INLINE GFXRenderAttachment**	GetAttachments() const { return mppAttachments; }
		FORCE_INLINE UInt32					GetSubpassCount() const { return mSubpassCount; }
		FORCE_INLINE GFXRenderSubpass**		GetSubpasses() const { return mppSubpasses; }
		FORCE_INLINE UInt32					GetDependencyCount() const { return mDependencyCount; }
		FORCE_INLINE GFXSubpassDependency*	GetDependencies() const { return mpDependencies; }
	};


	/********************************************************************************************/


	class GFXPipeline;

	struct QUARTZ_API GFXPipelineInfo
	{
		UInt32					shaderCount;
		GFXShader**				ppShaders;
		GFXVertexInputState*	pVertexInputState;
		GFXInputAssemblyState*	pInputAssemblyState;
		GFXViewportState*		pViewportState;
		GFXRasterizationState*	pRasterizationState;
		GFXMultisampleState*	pMultisampleState;
		//GFXDepthStencilState* pDepthStencilState;
		GFXColorBlendState*		pColorBlendState;
		//GFXDynamicState*		pDynamicState;
		GFXPipelineLayout*		pPipelineLayout;
		GFXRenderPass*			pRenderPass;
		UInt32					subpassIndex;
		GFXPipeline*			pParentPipeline;
		UInt32					parentPipelineIndex;
	};

	class QUARTZ_API GFXPipeline
	{
	protected:
		GFXDevice*				mpParentDevice;
		UInt32					mShaderCount;
		GFXShader**				mppShaders;
		GFXVertexInputState*	mpVertexInputState;
		GFXInputAssemblyState*	mpInputAssemblyState;
		GFXViewportState*		mpViewportState;
		GFXRasterizationState*	mpRasterizationState;
		GFXMultisampleState*	mpMultisampleState;
		//GFXDepthStencilState* mpDepthStencilState;
		GFXColorBlendState*		mpColorBlendState;
		//GFXDynamicState*		mpDynamicState;
		GFXPipelineLayout*		mpPipelineLayout;
		GFXRenderPass*			mpRenderPass;
		UInt32					mSubpassIndex;
		GFXPipeline*			mpParentPipeline;
		UInt32					mParentPipelineIndex;

		GFXPipeline() = default;

	public:
		virtual ~GFXPipeline() = default;

		FORCE_INLINE GFXDevice*				GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE UInt32					GetShaderCount() const { return mShaderCount; }
		FORCE_INLINE GFXShader**			GetShaders() const { return mppShaders; }
		FORCE_INLINE GFXVertexInputState*	GetVertexInputState() const { return mpVertexInputState; }
		FORCE_INLINE GFXInputAssemblyState*	GetInputAssemblyState() const { return mpInputAssemblyState; }
		FORCE_INLINE GFXViewportState*		GetViewportState() const { return mpViewportState; }
		FORCE_INLINE GFXRasterizationState*	GetRasterizationState() const { return mpRasterizationState; }
		FORCE_INLINE GFXMultisampleState*	GetMultisampleState() const { return mpMultisampleState; }
		FORCE_INLINE GFXColorBlendState*	GetColorBlendState() const { return mpColorBlendState; }
		FORCE_INLINE GFXPipelineLayout*		GetPipelineLayout() const { return mpPipelineLayout; }
		FORCE_INLINE GFXRenderPass*			GetRenderPass() const { return mpRenderPass; }
		FORCE_INLINE UInt32					GetSubpassIndex() const { return mSubpassIndex; }
		FORCE_INLINE GFXPipeline*			GetParentPipeline() const { return mpParentPipeline; }
		FORCE_INLINE UInt32					GetParentPipelineIndex() const { return mParentPipelineIndex; }
	};


	/********************************************************************************************/


	struct GFXFramebufferInfo
	{
		GFXRenderPass*	pRenderPass;
		UInt32			attachmentCount;
		GFXImageView**	ppAttachments;
		UInt32			width;
		UInt32			height;
		UInt32			layers;
	};

	class QUARTZ_API GFXFramebuffer
	{
	protected:
		GFXDevice*		mpParentDevice;
		GFXRenderPass*	mpRenderPass;
		UInt32			mAttachmentCount;
		GFXImageView**	mppAttachments;
		UInt32			mWidth;
		UInt32			mHeight;
		UInt32			mLayers;

	public:
		FORCE_INLINE GFXDevice*		GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE GFXRenderPass*	GetRenderPass() const { return mpRenderPass; }
		FORCE_INLINE UInt32			GetAttachmentCount() const { return mAttachmentCount; }
		FORCE_INLINE GFXImageView**	GetRenderAttachments() const { return mppAttachments; }
		FORCE_INLINE UInt32			GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32			GetHeight() const { return mHeight; }
		FORCE_INLINE UInt32			GetLayers() const { return mLayers; }
	};


	/********************************************************************************************/


	struct GFXCommandPoolInfo
	{
		GFXQueueFamily*	pFamily;
		Flags32			flags;
	};

	class QUARTZ_API GFXCommandPool
	{
	protected:
		GFXDevice*		mpParentDevice;
		GFXQueueFamily*	mpFamily;
		Flags32			mFlags;

	public:
		FORCE_INLINE GFXDevice*			GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE GFXQueueFamily*	GetFamily() const { return mpFamily; }
		FORCE_INLINE Flags32			GetFlags() const { return mFlags; }
	};


	/********************************************************************************************/


	struct GFXCommandBufferInfo
	{
		GFXCommandPool* pCommandPool;
		UInt32			commandBufferCount;
	};

	class QUARTZ_API GFXCommandBuffer
	{
	protected:
		GFXDevice*		mpParentDevice;
		GFXCommandPool*	mpCommandPool;

	public:
		FORCE_INLINE GFXDevice*			GetParentDevice() const { return mpParentDevice; }
		FORCE_INLINE GFXCommandPool*	GetCommandPool() const { return mpCommandPool; }
	};

	/********************************************************************************************/


	class GFXModule;
	class GFXPlatformModule;

	class QUARTZ_API GFXContext
	{
	private:
		GFXModule*			mpGFX;
		GFXPlatformModule*	mpPlatformGFX;

	public:
		Bool8 SetGFXModule(GFXModule* pModule, Bool8 destroyOnReplace = true);
		Bool8 SetGFXPlatformModule(GFXPlatformModule* pPlatformModule, Bool8 destroyOnReplace = true);

		Bool8 InitGraphicsContext();
		Bool8 DestroyGraphicsContext();

		GFXDevice*				CreateDevice(GFXPhysicalDevice* pPhysicalDevice, GFXDeviceInfo info);
		void					DestroyDevice(GFXDevice* pDevice);
		GFXSurface*				CreateSurface(GFXDevice* pDevice, GFXSurfaceInfo info);
		void					DestroySurface(GFXSurface* pSurface);
		GFXSwapchain*			CreateSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, GFXSwapchainInfo info);
		void					DestroySwapchain(GFXSwapchain* pSwapchain);
		GFXBuffer*				CreateBuffer(GFXDevice* pDevice, GFXBufferInfo info);
		void					DestroyBuffer(GFXBuffer* pBuffer);
		GFXImageView*			CreateImageView(GFXImage* pImage, GFXImageViewInfo info);
		void					DestroyImageView(GFXImageView* pImageView);
		GFXShader*				CreateShader(GFXDevice* pDevice, GFXShaderInfo info);
		void					DestroyShader(GFXShader* pShader);

		GFXVertexInputState*	CreateVertexInputState(GFXVertexInputStateInfo info);
		void					DestroyVertexInputState(GFXVertexInputState* pVertexInputState);
		GFXInputAssemblyState*	CreateInputAssemblyState(GFXInputAssemblyStateInfo info);
		void					DestroyInputAssemblyState(GFXInputAssemblyState* pInputAssemblyState);
		GFXViewportState*		CreateViewportState(GFXViewportStateInfo info);
		void					DestroyViewportState(GFXViewportState* pViewportState);
		GFXRasterizationState*	CreateRasterizationState(GFXRasterizationStateInfo info);
		void					DestroyRasterizationState(GFXRasterizationState* pRasterizationState);
		GFXMultisampleState*	CreateMultisampleState(GFXMultisampleStateInfo info);
		void					DestroyMultisampleState(GFXMultisampleState* pMultisampleState);
		GFXColorBlendState*		CreateColorBlendState(GFXColorBlendStateInfo info);
		void					DestroyColorBlendState(GFXColorBlendState* pColorBlendState);
		GFXPipelineLayout*		CreatePipelineLayout(GFXDevice* pDevice, GFXPipelineLayoutInfo info);
		void					DestroyPipelineLatout(GFXPipelineLayout* pPipelineLayout);
		GFXRenderAttachment*	CreateRenderAttachment(GFXRenderAttachmentInfo info);
		void					DestroyRenderAttachment(GFXRenderAttachment* pRenderAttachment);
		GFXRenderSubpass*		CreateRenderSubpass(GFXRenderSubpassInfo info);
		void					DestroyRenderSubpass(GFXRenderSubpass* pRenderSubpass);
		GFXRenderPass*			CreateRenderPass(GFXDevice* pDevice, GFXRenderPassInfo info);
		void					DestroyRenderPass(GFXRenderPass* pRenderPass);
		GFXDescriptorSetLayout* CreateDescriptorSetLayout(GFXDevice* pDevice, GFXDescriptorSetLayoutInfo info);
		void					DestroyDescriptorSetLayout(GFXDescriptorSetLayout* pLayout);
		GFXPipeline*			CreatePipeline(GFXDevice* pDevice, GFXPipelineInfo info);
		void					DestroyPipleline(GFXPipeline* pPipeline);
		GFXFramebuffer*			CreateFramebuffer(GFXDevice* pDevice, GFXFramebufferInfo info);
		void					DestroyFramebuffer(GFXFramebuffer* pFramebuffer);
		GFXCommandPool*			CreateCommandPool(GFXDevice* pDevice, GFXCommandPoolInfo info);
		void					DestroyCommandPool(GFXCommandPool* pCommandPool);
		//GFXCommandBuffer*		CreateCommandBuffers();
		//void					DestroyCommandBuffers();

		GFXImageArray	GetSwapChainImages(GFXSwapchain* pSwapchain);

		Bool8 MapBufferMemory(GFXBuffer* pBuffer, USize offset, USize size, GFXBufferMapFlags flags, void** ppData);
		Bool8 UnmapBufferMemory(GFXBuffer* pBuffer);

		GFXPhysicalDevice*				GetPrimaryPhysicalDevice();
		const GFXPhysicalDeviceList&	GetPhysicalDeviceList() const;

		FORCE_INLINE GFXModule* GetGFXModule() { return mpGFX; }
	};


	/********************************************************************************************/


	class QUARTZ_API GFXModule : public Module
	{
	private:
		virtual Bool8		InitModuleImpl(const Char** pFailMessage) = 0;
		virtual Bool8		DestroyModuleImpl(const Char** pFailMessage) = 0;
		virtual const Char* GetModuleNameImpl() const = 0;

	public:
		virtual Bool8 InitGraphicsContext() = 0;
		virtual Bool8 DestroyGraphicsContext() = 0;

		virtual GFXDevice*				CreateDevice(GFXPhysicalDevice* pPhysicalDevice, GFXDeviceInfo info) = 0;
		virtual void					DestroyDevice(GFXDevice* pDevice) = 0;
		virtual GFXBuffer*				CreateBuffer(GFXDevice* pDevice, GFXBufferInfo info) = 0;
		virtual void					DestroyBuffer(GFXBuffer* pBuffer) = 0;
		virtual GFXImageView*			CreateImageView(GFXImage* pImage, GFXImageViewInfo info) = 0;
		virtual void					DestroyImageView(GFXImageView* pImageView) = 0;
		virtual GFXShader*				CreateShader(GFXDevice* pDevice, GFXShaderInfo info) = 0;
		virtual void					DestroyShader(GFXShader* pShader) = 0;

		virtual GFXVertexInputState*	CreateVertexInputState(GFXVertexInputStateInfo info) = 0;
		virtual void					DestroyVertexInputState(GFXVertexInputState* pVertexInputState) = 0;
		virtual GFXInputAssemblyState*	CreateInputAssemblyState(GFXInputAssemblyStateInfo info) = 0;
		virtual void					DestroyInputAssemblyState(GFXInputAssemblyState* pInputAssemblyState) = 0;
		virtual GFXViewportState*		CreateViewportState(GFXViewportStateInfo info) = 0;
		virtual void					DestroyViewportState(GFXViewportState* pViewportState) = 0;
		virtual GFXRasterizationState*	CreateRasterizationState(GFXRasterizationStateInfo info) = 0;
		virtual void					DestroyRasterizationState(GFXRasterizationState* pRasterizationState) = 0;
		virtual GFXMultisampleState*	CreateMultisampleState(GFXMultisampleStateInfo info) = 0;
		virtual void					DestroyMultisampleState(GFXMultisampleState* pMultisampleState) = 0;
		virtual GFXColorBlendState*		CreateColorBlendState(GFXColorBlendStateInfo info) = 0;
		virtual void					DestroyColorBlendState(GFXColorBlendState* pColorBlendState) = 0;
		virtual GFXPipelineLayout*		CreatePipelineLayout(GFXDevice* pDevice, GFXPipelineLayoutInfo info) = 0;
		virtual void					DestroyPipelineLatout(GFXPipelineLayout* pPipelineLayout) = 0;
		virtual GFXRenderAttachment*	CreateRenderAttachment(GFXRenderAttachmentInfo info) = 0;
		virtual void					DestroyRenderAttachment(GFXRenderAttachment* pRenderAttachment) = 0;
		virtual GFXRenderSubpass*		CreateRenderSubpass(GFXRenderSubpassInfo info) = 0;
		virtual void					DestroyRenderSubpass(GFXRenderSubpass* pRenderSubpass) = 0;
		virtual GFXRenderPass*			CreateRenderPass(GFXDevice* pDevice, GFXRenderPassInfo info) = 0;
		virtual void					DestroyRenderPass(GFXRenderPass* pRenderPass) = 0;
		virtual GFXDescriptorSetLayout* CreateDescriptorSetLayout(GFXDevice* pDevice, GFXDescriptorSetLayoutInfo info) = 0;
		virtual void					DestroyDescriptorSetLayout(GFXDescriptorSetLayout* layout) = 0;
		virtual GFXPipeline*			CreatePipeline(GFXDevice* pDevice, GFXPipelineInfo info) = 0;
		virtual void					DestroyPipleline(GFXPipeline* pPipeline) = 0;
		virtual GFXFramebuffer*			CreateFramebuffer(GFXDevice* pDevice, GFXFramebufferInfo info) = 0;
		virtual void					DestroyFramebuffer(GFXFramebuffer* pFramebuffer) = 0;
		virtual GFXCommandPool*			CreateCommandPool(GFXDevice* pDevice, GFXCommandPoolInfo info) = 0;
		virtual void					DestroyCommandPool(GFXCommandPool* pCommandPool) = 0;

		virtual Bool8 MapBufferMemory(GFXBuffer* pBuffer, USize offset, USize size, GFXBufferMapFlags flags, void** ppData) = 0;
		virtual Bool8 UnmapBufferMemory(GFXBuffer* pBuffer) = 0;

		virtual GFXPhysicalDevice*				GetPrimaryPhysicalDevice() = 0;
		virtual const GFXPhysicalDeviceList&	GetPhysicalDeviceList() const = 0;
	};


	/********************************************************************************************/
}