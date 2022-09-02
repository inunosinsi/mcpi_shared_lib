#include <libreborn/libreborn.h>
#include <symbols/minecraft.h>
#include <mods/misc/misc.h>
#include <stdio.h>

typedef ItemInstance *(*Player_getArmor_t)(unsigned char *player, int32_t slot);
static Player_getArmor_t Player_getArmor = (Player_getArmor_t) 0x8fda4;

unsigned char **Material_iron = (unsigned char **) 0x17a7a8; // Material
unsigned char **kuroobi_1_png = (unsigned char **) 0x137c58; // std::string

typedef unsigned char *(*ArmorItem_t)(unsigned char *ArmorItem, int id, unsigned char **mat, int param_3, int param_4);
static ArmorItem_t ArmorItem = (ArmorItem_t) 0x9362c;

unsigned char *player;
unsigned char *level;
static void mcpi_callback(unsigned char *minecraft){
    // Runs on every tick, sets the player and level vars.
    if (minecraft == NULL) return;
    player = *(unsigned char **) (minecraft + Minecraft_player_property_offset);
    level = *(unsigned char **) (minecraft + Minecraft_level_property_offset);
    if (player != NULL){
        // Uses texture changing to have the jetpack texture work.
        ItemInstance *item = Player_getArmor(player, 1);
        if (item == NULL) return;
        if (item->id == 404){
            *(std::string*) kuroobi_1_png = "armor/kuroobi_1.png";
        } else {
            *(std::string*) kuroobi_1_png = "armor/iron_1.png";
        }
    }
}

// Custom jetpack item
unsigned char *kuroobi;
unsigned char *make_kuroobi(){
    // kuroobi
    unsigned char *item = (unsigned char *) ::operator new(0x34); // ARMOR_SIZE
    ALLOC_CHECK(item);
    (*ArmorItem)(item, 148, Material_iron, 2, 1);
    // Set VTable
    unsigned char *vtable = *(unsigned char **) item;

    // Get Functions
    Item_setIcon_t Item_setIcon = *(Item_setIcon_t *) (vtable + Item_setIcon_vtable_offset);
    Item_setDescriptionId_t Item_setDescriptionId = *(Item_setDescriptionId_t *) (vtable + Item_setDescriptionId_vtable_offset);

    // Setup
    (*Item_setIcon)(item, 13, 4);
    (*Item_setDescriptionId)(item, "kuroobi");
    *(int32_t *) (item + Item_is_stacked_by_data_property_offset) = 1;
    *(int32_t *) (item + Item_category_property_offset) = 2;
    *(int32_t *) (item + Item_max_damage_property_offset) = 250;
    *(int32_t *) (item + Item_max_stack_size_property_offset) = 1;

    return item;
}

static void Item_initItems_injection(__attribute__((unused)) unsigned char *null) {
    kuroobi = make_kuroobi();
}

// Add jetpack to creative inventory
static void Inventory_setupDefault_FillingContainer_addItem_call_injection(unsigned char *filling_container) {
    ItemInstance *b_instance = new ItemInstance;
    ALLOC_CHECK(b_instance);
    b_instance->count = 255;
    b_instance->auxiliary = 0;
    b_instance->id = 404;
    (*FillingContainer_addItem)(filling_container, b_instance);
}

__attribute__((constructor)) static void init() {
    misc_run_on_update(mcpi_callback);
    misc_run_on_creative_inventory_setup(Inventory_setupDefault_FillingContainer_addItem_call_injection);
    misc_run_on_items_setup(Item_initItems_injection);
    //printf("include libb.so ok!");
}