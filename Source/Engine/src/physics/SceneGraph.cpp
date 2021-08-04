#include "SceneGraph.h"

namespace Quartz
{
    SceneGraphView::SceneGraphView(SceneGraph* pParentGraph, SceneNode* pRoot)
        : mpParentGraph(pParentGraph),
        mpViewRoot(pRoot)
    {
        // Nothing
    }

    SceneNode* SceneGraphView::GetNode(Entity entity)
    {
        // TODO: Add check here to ensure the entity is within the view?
        return mpParentGraph->GetNode(entity);
    }

    void SceneGraph::SetWorld(EntityWorld* pWorld)
    {
        this->pWorld = pWorld;
    }

    // Should never be called on root
    void SceneGraph::UpdateBranch(SceneNode* node)
    {
        node->globalTransform = node->pLocalTransform->GetMatrix() * node->pParent->globalTransform;

        for (SceneNode* pNode : node->children)
        {
            UpdateBranch(pNode);
        }
    }

    SceneGraph::SceneGraph()
        : pWorld(nullptr)
    {
        SceneNode rootNode;
        rootNode.pParent = nullptr;
        rootNode.entity = NullEntity;
     
        zeroTransform = TransformComponent(
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f });
        
        rootNode.pLocalTransform = &zeroTransform;
        rootNode.globalTransform = Matrix4().SetIdentity();

        pRoot = &nodes.Insert(NullEntity, rootNode);  
    }

    SceneNode* SceneGraph::GetNode(Entity entity)
    {
        if (nodes.Contains(entity))
        {
            return &nodes.Get(entity);
        }

        return nullptr;
    }

    Bool8 SceneGraph::ParentEntity(Entity entity, Entity parent)
    {
        // TODO: check for cycles

        if (entity == parent)
        {
            // Cant parent itself
            return false;
        }

        if (!nodes.Contains(parent))
        {
            // Parent must be present
            return false;
        }

        SceneNode& parentNode = nodes.Get(parent);
        SceneNode* pChildNode = nullptr;

        if (!nodes.Contains(entity))
        {
            SceneNode entityNode;
            entityNode.pParent = &parentNode;
            entityNode.entity = entity;

            if (pWorld->HasComponent<TransformComponent>(entity))
            {
                TransformComponent& transform = pWorld->GetComponent<TransformComponent>(entity);
                entityNode.pLocalTransform = &transform;
                entityNode.globalTransform = parentNode.globalTransform * transform.GetMatrix();
            }
            else
            {
                entityNode.pLocalTransform = &zeroTransform;
                entityNode.globalTransform = parentNode.globalTransform;
            }

            pChildNode = &nodes.Insert(entity, entityNode);
            parentNode.children.PushBack(pChildNode);
        }
        else
        {
            pChildNode = &nodes.Get(entity);
            SceneNode* pChildParentNode = pChildNode->pParent;

            // TODO: Find a way to speed this up?
            // If node exists, Find() should never fail
            UInt32 childIndex = pChildParentNode->children.Find(pChildNode);
            pChildParentNode->children.Remove(childIndex);

            parentNode.children.PushBack(pChildNode);

            UpdateBranch(pChildNode);
        }

        return true;
    }

    Bool8 SceneGraph::ParentEntityToRoot(Entity entity)
    {
        return ParentEntity(entity, NullEntity);
    }

    void SceneGraph::Update(Entity entity)
    {
        if (nodes.Contains(entity))
        {
            UpdateBranch(&nodes.Get(entity));
        }
    }

    SceneGraphView SceneGraph::CreateView(Entity entity)
    {
        if (nodes.Contains(entity))
        {
            SceneNode* pNode = &nodes.Get(entity);
            return SceneGraphView(this, pNode);
        }

        // TODO: find a better fail state?
        return CreateRootView();
    }

    SceneGraphView SceneGraph::CreateRootView()
    {
        return SceneGraphView(this, pRoot);
    }
}

