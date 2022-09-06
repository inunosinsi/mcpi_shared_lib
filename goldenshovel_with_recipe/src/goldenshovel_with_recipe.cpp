#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>
#include <mods/misc/misc.h>

// case item
typedef unsigned char *(*ToolItem_t)(unsigned char *ToolItem, int id);
static ToolItem_t ToolItem = (ToolItem_t) 0x99488;

// Custom golden shovel item
unsigned char *gs;
unsigned char *make_golden_shovel(){
    // golden shovel
    unsigned char *item = (unsigned char *) ::operator new(0x34); // Tool_SIZE
    ALLOC_CHECK(item);

    // case item
    (*ToolItem)(item, 148);

    // // Set VTable
    unsigned char *vtable = *(unsigned char **) item;

    // // Get Functions
    Item_setIcon_t Item_setIcon = *(Item_setIcon_t *) (vtable + Item_setIcon_vtable_offset);
    Item_setDescriptionId_t Item_setDescriptionId = *(Item_setDescriptionId_t *) (vtable + Item_setDescriptionId_vtable_offset);

    // Setup
    (*Item_setIcon)(item, 6, 6);
    (*Item_setDescriptionId)(item, "golden shovel");

    return item;
}

static void Item_initGSItems_injection(__attribute__((unused)) unsigned char *null) {
    gs = make_golden_shovel();
}

// Add golden shovel to creative inventory
static void Inventory_setupDefault_FillingContainer_addGSItem_call_injection(unsigned char *filling_container) {
    ItemInstance *gs_instance = new ItemInstance;
    ALLOC_CHECK(gs_instance);
    gs_instance->count = 255;
    gs_instance->auxiliary = 0; //auxiliary:補助
    gs_instance->id = 404;
    (*FillingContainer_addItem)(filling_container, gs_instance);
}

// Crafting Recipes
static void GSRecipes_injection(unsigned char *recipes) {
    // 2 Lava bucket
    Recipes_Type type1 = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 2,
            .id = 325,
            .auxiliary = 10
        },
        .letter = 'a'
    };
    // 1 Nether core
    Recipes_Type type2 = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 1,
            .id = 247,
            .auxiliary = 0
        },
        .letter = 'b'
    };
    // 1 Iron block
    Recipes_Type type3 = {
        .item = 0,
        .tile = 0,
        .instance = {
            .count = 1,
            .id = 42,
            .auxiliary = 0
        },
        .letter = 'c'
    };
    // goldshovel
    ItemInstance result = {
        .count = 1,
        .id = 404,
        .auxiliary = 0
    };
    (*Recipes_addShapedRecipe_2)(recipes, result, " b ", "aca", {type1, type2, type3});
}

__attribute__((constructor)) static void init() {
    misc_run_on_creative_inventory_setup(Inventory_setupDefault_FillingContainer_addGSItem_call_injection);
    misc_run_on_items_setup(Item_initGSItems_injection);
    misc_run_on_recipes_setup(GSRecipes_injection);
}
