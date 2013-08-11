#include "../memory/memory.h"
#include "canvas_node.h"
#include "canvas_group.h"
using namespace playstate;

CanvasNode::CanvasNode(CanvasGroup* group)
	: mCanvasGroup(group)
{
	
}

CanvasNode::~CanvasNode()
{

}

CanvasGroup* CanvasNode::GetCanvasGroup()
{
	return mCanvasGroup;
}
