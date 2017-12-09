#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Physics.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"
#include <math.h>


j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Map Draw", Profiler::Color::DodgerBlue)
	if (map_loaded == false)
		return;

	TileSet* set = data.tilesets.At(0)->data;

	for (p2List_item<Layer*> *layer_iterator = data.layers.start; layer_iterator; layer_iterator = layer_iterator->next)
	{
		int tile_num = 0;
		int layer_width = layer_iterator->data->width;
		int layer_height = layer_iterator->data->height;
		int layer_dimensions = layer_iterator->data->height*data.layers.At(0)->data->width;

		for (int row = 0; row < layer_height; row++) {
			for (int col = 0; col < layer_width; col++) {
				int id = layer_iterator->data->data[tile_num];

				int real_row;
				int real_col;

				Get_pixels_from_tiles(row, col, &real_row, &real_col);

				//provisional parallax
				if (layer_iterator->data->name == "background") {
					float camera_increment = layer2_parallax * (App->render->camera.x);
					real_col += (int)camera_increment;
				}
				int margin = 100;
				SDL_Point p = { real_col,real_row };
				SDL_Rect drawRect = {
					-App->render->camera.x - margin,
					-App->render->camera.y - margin,
					App->render->camera.w + margin,
					App->render->camera.h + margin
				};

				if (SDL_PointInRect(&p, &drawRect))
					App->render->Blit(data.tilesets.At(0)->data->texture, real_col, real_row, &set->GetTileRect(id));


				tile_num++;
			}
		}
	}
}

bool j1Map::ReadPositions()
{
	if (!data.objectGroups.start)
		return false;

	SDL_Rect col_rect;

	ObjectGroup start = *data.objectGroups.start->data;

	p2List_item<ObjectGroup*>* object_group_iterator = nullptr;

	p2List_item<Object*>* object_iterator;

	for (object_group_iterator = data.objectGroups.start; object_group_iterator; object_group_iterator = object_group_iterator->next) {

		for (object_iterator = object_group_iterator->data->objects.start; object_iterator; object_iterator = object_iterator->next) {

			col_rect.x = object_iterator->data->x;
			col_rect.y = object_iterator->data->y;
			col_rect.w = object_iterator->data->w;
			col_rect.h = object_iterator->data->h;

			float friction = object_iterator->data->friction;

			switch (object_iterator->data->type)
			{
			case OBJECT_TYPE_DOOR:
				data.door_position.x = col_rect.x;
				data.door_position.y = col_rect.y;
				App->physics->AddCollider(&col_rect, DOOR);
				break;

			case OBJECT_TYPE_PLAYER:
				data.player_start_position.x = col_rect.x;
				data.player_start_position.y = col_rect.y;
				break;
			case OBJECT_TYPE_OVNI:
				data.ovni_position_list.add({ (float)col_rect.x, (float)col_rect.y });
				break;
			default:
				break;
			}
		}
	}

	return true;
}

void j1Map::PlaceTileColliders()
{
	if (map_loaded == false)
		return;

	TileSet* set = data.tilesets.At(0)->data;

	for (p2List_item<Layer*> *layer_iterator = data.layers.start; layer_iterator; layer_iterator = layer_iterator->next)
	{
		int tile_num = 0;
		int layer_width = layer_iterator->data->width;
		int layer_height = layer_iterator->data->height;
		int layer_dimensions = layer_iterator->data->height*data.layers.At(0)->data->width;

		for (int row = 0; row < layer_height; row++) {
			for (int col = 0; col < layer_width; col++) {
				int id = layer_iterator->data->data[tile_num];

				int real_row;
				int real_col;

				Get_pixels_from_tiles(row, col, &real_row, &real_col);

				SDL_Rect col_rect = {
					real_col,
					real_row,
					set->tile_width,
					set->tile_height
				};

				if (set->IsWall(id - 1) && id != 0) { //Why?
					App->physics->AddCollider(&col_rect, WALL);
				}

				if (set->IsDeath(id - 1)) { //Why?
					App->physics->AddCollider(&col_rect, DEATH);
				}


				tile_num++;
			}
		}
	}
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;
	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

bool TileSet::IsWall(int id) const
{
	Tile* tile = FindTileWithid(id);
	bool ret = false;
	ret = tile->is_ground;
	return ret;
}

bool TileSet::IsDeath(int id) const
{
	Tile* tile = FindTileWithid(id);
	bool ret = false;
	ret = tile->death;
	return ret;
}

Tile* TileSet::FindTileWithid(int id) const
{
	p2List_item<Tile*>* tile_iterator;

	for (tile_iterator = tiles.start; tile_iterator; tile_iterator = tile_iterator->next) {
		if (tile_iterator->data->id == id) {
			return tile_iterator->data;
		}
	}
	

}

int TileSet::GetIdFromPos(iPoint pos, Layer* layer) const
{
	int tile_num = 0;
	int layer_width = layer->width;
	int layer_height = layer->height;
	int id = -1;

	for (int row = 0; row < layer_height; row++) {
		for (int col = 0; col < layer_width; col++) {
			if (col == pos.x && row == pos.y) {
				id = layer->data[tile_num];
				id--;
				break;
			}
			tile_num++;
		}
	}

	return id;
}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	data.ovni_position_list.clear();

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Clean up all layer data
	p2List_item<Layer*>* item_layer;
	item_layer = data.layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.tilesets.clear();
	// Remove all layers
	data.layers.clear();

	// Clean up Objects
	if (data.objectGroups.start) {
		p2List_item<Object*>* item_object;
		p2List_item<ObjectGroup*>* item_objectGroup = data.objectGroups.start;
		item_object = data.objectGroups.start->data->objects.start;


		while (item_object != NULL)
		{
			if (item_object->data != NULL)
				RELEASE(item_object->data);
			if (item_object->next != NULL)
				item_object = item_object->next;
			else
				break;
		}

		data.objectGroups.clear();
	}

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
MapData* j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer_node;

	for (layer_node = map_file.child("map").child("layer"); layer_node && ret; layer_node = layer_node.next_sibling("layer"))
	{
		Layer* layer = new Layer();

		if (ret == true)
		{
			ret = LoadLayer(layer_node, layer);
		}

		data.layers.add(layer);
	}

	pugi::xml_node object_group_node;
	for (object_group_node = map_file.child("map").child("objectgroup"); object_group_node && ret; object_group_node = object_group_node.next_sibling()) {
		ObjectGroup* objectGroup = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroup(object_group_node, objectGroup); //HARDCODE
		}
		data.objectGroups.add(objectGroup);
	}






	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
	}

	map_loaded = ret;

	if (ret)
		return &data;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}
	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	pugi::xml_node tile_node;




	for (tile_node = tileset_node.child("tile"); tile_node; tile_node = tile_node.next_sibling()) {
		Tile *tile = new Tile();
		LoadTile(tile_node, tile);
		set->tiles.add(tile);
	}

	return ret;
}

bool j1Map::LoadTile(pugi::xml_node &tile_node, Tile * tile)
{
	tile->id = tile_node.attribute("id").as_int();
	pugi::xml_node property_node;
	pugi::xml_node properties_node;
	properties_node = tile_node.first_child();

	for (property_node = properties_node.first_child(); property_node != NULL; property_node = property_node.next_sibling()) { //This should be a function
		p2SString attribute_namme = property_node.attribute("name").as_string();
		if (attribute_namme == "wall")
			tile->is_ground = property_node.attribute("value").as_bool();
	}


	for (property_node = properties_node.first_child(); property_node != NULL; property_node = property_node.next_sibling()) {
		p2SString attribute_namme = property_node.attribute("name").as_string();
		if (attribute_namme == "friction")
			tile->friction = property_node.attribute("value").as_float();
	}

	for (property_node = properties_node.first_child(); property_node != NULL; property_node = property_node.next_sibling()) {
		p2SString attribute_namme = property_node.attribute("name").as_string();
		if (attribute_namme == "death")
			tile->death = property_node.attribute("value").as_bool();
	}

	
	return true;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node & group_node, ObjectGroup * object_grup)
{
	bool ret = true;

	object_grup->name = group_node.attribute("name").as_string();

	pugi::xml_node object_node;




	for (object_node = group_node.first_child(); object_node; object_node = object_node.next_sibling()) {
		Object *object = new Object();
		LoadObject(object_node, object);
		object_grup->objects.add(object);
	}

	return ret;
}

bool j1Map::LoadObject(pugi::xml_node &object_node, Object *object)
{
	bool ret = true;
	object->id = object_node.attribute("id").as_int();
	p2SString object_type(object_node.attribute("type").as_string());

	if (object_type == "ground")
	{
		object->type = OBJECT_TYPE_GROUND;
		object->friction = object_node.child("properties").child("property").attribute("value").as_float();
	}
	else if (object_type == "ice")
	{
		object->type = OBJECT_TYPE_ICE;
		object->friction = object_node.child("properties").child("property").attribute("value").as_float();
	}
	else if (object_type == "death")
	{
		object->type = OBJECT_TYPE_DEATH;
	}
	else if (object_type == "door")
	{
		object->type = OBJECT_TYPE_DOOR;
	}
	else if (object_type == "player")
	{
		object->type = OBJECT_TYPE_PLAYER;
	}
	else if (object_type == "ovni")
	{
		object->type = OBJECT_TYPE_OVNI;
	}
	else
	{
		object->type = OBJECT_TYPE_UNKNOWN;
		ret = false;
	}

	object->x = object_node.attribute("x").as_int();
	object->y = object_node.attribute("y").as_int();
	object->w = object_node.attribute("width").as_int();
	object->h = object_node.attribute("height").as_int();

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, Layer* layer)
{
	bool ret = true;
	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	uint data_size = layer->height * layer->width;
	layer->data = new uint[data_size];
	memset(layer->data, 0, data_size * sizeof(uint));

	pugi::xml_node tile_node_iterator = node.child("data").first_child();
	for (int i = 0; i < data_size; i++) {
		layer->data[i] = tile_node_iterator.attribute("gid").as_int();
		tile_node_iterator = tile_node_iterator.next_sibling();
	}
	return true;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

Layer * j1Map::FindLayerWithName(p2SString name) const
{
	for (p2List_item<Layer*> *layer_iterator = data.layers.start; layer_iterator; layer_iterator = layer_iterator->next)
	{
		if (layer_iterator->data->name == name) {
			return layer_iterator->data;
		}
	}
	return nullptr;
}



bool j1Map::isWalkableFromPos(iPoint pos)
{
	Layer* layer;

	layer = FindLayerWithName("plattforms");

	TileSet* tileset = App->map->data.tilesets.start->data;
	int id = tileset->GetIdFromPos(pos, layer);

	Tile* tile = tileset->FindTileWithid(id);

	if (!tile->is_ground)
		return true;
	return false;
}
