#pragma once

#include "../linked_list.h"
#include "../processor/update_processor_factory.h"
#include "canvas_node.h"

namespace playstate
{
	//
	// A CanvasGroup is a container which manages collections of CanvasNodes and their related action types.
	// 
	class CanvasGroup : public IUpdateProcessor
	{
	public:
		LinkedListLink<CanvasGroup> CanvasGroupLink;

	public:
		CanvasGroup();
		CanvasGroup(IUpdateProcessorFactory& updateProcessorFactory);
		virtual ~CanvasGroup();

		//
		// Adds the supplied node to this canvas group
		void AddCanvasNode(CanvasNode* node);

		//
		// Removes this node from the canvas group
		void RemoveCanvasNode(CanvasNode* node);

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	private:
		LinkedList<CanvasNode, &CanvasNode::CanvasNodeLink> mNodes;
		IUpdateProcessor* mUpdateProcessor;
	};		
}
