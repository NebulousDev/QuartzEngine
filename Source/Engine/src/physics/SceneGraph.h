#pragma once

#include "../Module.h"

#include "../entity/World.h"
#include "component/Transform.h"
#include "util/Storage.h"
#include "util/Array.h"

namespace Quartz
{
	struct SceneNode
	{
		SceneNode*			pParent;
		Entity				entity;
		TransformComponent* pLocalTransform;
		Matrix4				globalTransform;
		Array<SceneNode*>	children;
	};

	class SceneGraph;

	class QUARTZ_API SceneGraphView
	{
	private:
		SceneGraph* mpParentGraph;
		SceneNode*	mpViewRoot;

	public:
		SceneGraphView(SceneGraph* pParentGraph, SceneNode* pRoot);

		SceneNode* GetNode(Entity entity);

		FORCE_INLINE SceneNode* GetRoot() { return mpViewRoot; }
	};

	// TODO: transition away from linked lists to a more compact
	//		 data structure (indexed array?)

	class QUARTZ_API SceneGraph
	{
	private:
		friend class Engine;

		using NodeStorage = Storage<SceneNode, Entity, Entity::HandleIntType, 1024>;

	private:
		EntityWorld*		pWorld;
		NodeStorage			nodes;
		SceneNode*			pRoot;
		TransformComponent	zeroTransform;

	private:
		void SetWorld(EntityWorld* pWorld);
		void UpdateBranch(SceneNode* node);
	
	public:
		SceneGraph();

		SceneNode* GetNode(Entity entity);

		Bool8 ParentEntity(Entity entity, Entity parent);
		Bool8 ParentEntityToRoot(Entity entity);

		void Update(Entity entity);

		SceneGraphView CreateView(Entity entity);
		SceneGraphView CreateRootView();
	};
}