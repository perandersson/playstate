#pragma once

#include "memory/memory.h"
#include "config.h"
#include "types.h"
#include "exception.h"
#include "singleton.h"
#include "functions.h"

#include "processor/processors/linked_list_update_processor.h"
#include "processor/processors/octree_render_processor.h"
#include "processor/processors/octree_light_source_processor.h"

#include "filesystem/file_system.h"
#include "script/script_system.h"
#include "window/window.h"
#include "game/game_runner.h"
#include "input/input_system.h"
#include "scene/scene.h"
#include "resources/resource_manager.h"
#include "rendering/render_system.h"

#include "model/model.h"

#include "sound/music.h"
#include "sound/sound_effect.h"
#include "sound/sound_engine.h"

#include "component/scriptable_component.h"
#include "component/renderable/render_static_model.h"
#include "component/lightsources/point_light.h"

#include "logging/console/console_logger.h"

#include "kernel.h"