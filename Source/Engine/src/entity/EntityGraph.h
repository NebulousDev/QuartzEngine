#pragma once

#include "Entity.h"
#include "component/Transform.h"
#include "util/Storage.h"
#include "util/Array.h"

namespace Quartz
{
	class EntityGraph;
	class EntityDatabase;

	struct SceneNode
	{
		SceneNode*			pParent;
		Entity				entity;
		TransformComponent* pLocalTransform;
		Matrix4				globalTransform;
		Array<SceneNode*>	children;
	};

	class QUARTZ_API EntityGraphView
	{
	private:
		EntityGraph* mpParentGraph;
		SceneNode*	mpViewRoot;

	public:
		EntityGraphView(EntityGraph* pParentGraph, SceneNode* pRoot);

		SceneNode* GetNode(Entity entity);

		FORCE_INLINE SceneNode* GetRoot() { return mpViewRoot; }
	};

	// TODO: transition away from linked lists to a more compact
	//		 data structure (indexed array?)

	class QUARTZ_API EntityGraph
	{
	private:
		friend class EntityWorld;

		using NodeStorage = Storage<SceneNode, Entity, Entity::HandleIntType, 1024>;

	private:
		EntityDatabase*		mpDatabase;
		NodeStorage			mNodes;
		SceneNode*			mpRoot;
		TransformComponent	mZeroTransform;

	private:
		void UpdateBranch(SceneNode* node);
	
	public:
		EntityGraph();
		EntityGraph(EntityDatabase* pDatabase);

		void SetDatabase(EntityDatabase* pDatabase);

		SceneNode* GetNode(Entity entity);

		Bool8 ParentEntity(Entity entity, Entity parent);
		Bool8 ParentEntityToRoot(Entity entity);

		void Update(Entity entity);

		EntityGraphView CreateView(Entity entity);
		EntityGraphView CreateRootView();
	};
}