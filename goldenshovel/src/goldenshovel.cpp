#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>
#include <mods/misc/misc.h>

unsigned char **Material_iron = (unsigned char **) 0x17a7a8; // Material

typedef unsigned char *(*ToolItem_t)(unsigned char *ToolItem, int id, unsigned char **mat, int param_3, int param_4);
static ToolItem_t ToolItem = (ToolItem_t) 0x9362c;

// Custom golden shovel item
unsigned char *gs;
unsigned char *make_golden_shovel(){
    // golden shovel
    unsigned char *item = (unsigned char *) ::operator new(0x34); // Tool_SIZE
    ALLOC_CHECK(item);
    (*ToolItem)(item, 148, Material_iron, 2, 1);
    
    // Set VTable
    unsigned char *vtable = *(unsigned char **) item;

    // Get Functions
    Item_setIcon_t Item_setIcon = *(Item_setIcon_t *) (vtable + Item_setIcon_vtable_offset);
    Item_setDescriptionId_t Item_setDescriptionId = *(Item_setDescriptionId_t *) (vtable + Item_setDescriptionId_vtable_offset);

    // Setup
    (*Item_setIcon)(item, 6, 6);
    (*Item_setDescriptionId)(item, "golden shovel");
    *(int32_t *) (item + Item_is_stacked_by_data_property_offset) = 1;
    *(int32_t *) (item + Item_category_property_offset) = 2;
    *(int32_t *) (item + Item_max_damage_property_offset) = 250;
    *(int32_t *) (item + Item_max_stack_size_property_offset) = 1;

    return item;
}

static void Item_initGSItems_injection(__attribute__((unused)) unsigned char *null) {
    gs = make_golden_shovel();
}

// Add gold scoop to creative inventory
static void Inventory_setupDefault_FillingContainer_addGSItem_call_injection(unsigned char *filling_container) {
    ItemInstance *gs_instance = new ItemInstance;
    ALLOC_CHECK(gs_instance);
    gs_instance->count = 255;
    gs_instance->auxiliary = 0; //auxiliary:補助
    gs_instance->id = 404;
    (*FillingContainer_addItem)(filling_container, gs_instance);
}

__attribute__((constructor)) static void init() {
    misc_run_on_creative_inventory_setup(Inventory_setupDefault_FillingContainer_addGSItem_call_injection);
    misc_run_on_items_setup(Item_initGSItems_injection);
}
