#ifndef VOXEL_INSTANCE_EMITTER_H
#define VOXEL_INSTANCE_EMITTER_H

#include "../../util/containers/std_vector.h"
#include "../../util/thread/mutex.h"
#include "instance_emitter_listener.h"
#include "voxel_instance_generator.h"
#include "voxel_instance_library_item.h"

namespace zylann::voxel {

// Spawns one or multiple different items at a specific LOD using points produced by one generator.
class VoxelInstanceEmitter : public Resource {
	GDCLASS(VoxelInstanceEmitter, Resource)
public:
	// This is a small limit to allow a few minor optimizations, and also because I can't think of a reason to have so
	// many items in there
	static constexpr uint32_t MAX_ITEMS = 64;

	void set_lod_index(int index);
	int get_lod_index() const;

	void set_generator(Ref<VoxelInstanceGenerator> generator);
	Ref<VoxelInstanceGenerator> get_generator() const;

	// Editor

#ifdef TOOLS_ENABLED
	void get_configuration_warnings(PackedStringArray &warnings) const;
#endif

	// Internal

	void add_listener(IInstanceEmitterListener *listener);
	void remove_listener(const IInstanceEmitterListener *listener);

	Span<const Ref<VoxelInstanceLibraryItem>> get_items() const;

private:
	void on_generator_changed();

	TypedArray<VoxelInstanceLibraryItem> _b_get_items();
	void _b_set_items(TypedArray<VoxelInstanceLibraryItem> array);

	static void _bind_methods();

	uint8_t _lod_index = 0;
	Ref<VoxelInstanceGenerator> _generator;
	Mutex _mutex;

	// Can have nulls.
	// Cannot have duplicates.
	StdVector<Ref<VoxelInstanceLibraryItem>> _items;

	StdVector<IInstanceEmitterListener *> _listeners;
};

} // namespace zylann::voxel

#endif // VOXEL_INSTANCE_EMITTER_H