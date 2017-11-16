#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Pathfinding.h"

enum ObjectTypes {
	OBJECT_TYPE_UNKNOWN = 0,
	OBJECT_TYPE_GROUND,
	OBJECT_TYPE_DEATH,
	OBJECT_TYPE_DOOR,
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_ICE
};

struct Object {
	uint id;
	ObjectTypes type;
	int x;
	int y;
	int w;
	int h;
	float friction = 0;
};
struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

struct Layer {
	p2SString			name;
	uint				width;
	uint				height;
	uint*				data = nullptr;
	Properties			properties;
	
	Layer() : data(NULL)
	{}

	~Layer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

struct ObjectGroup {
	p2SString			name;
	p2List<Object*>		objects;
};

struct Tile
{
	int id;
	bool is_ground;
	float friction;
};




	// TODO 6: Short function to get the value of x,y
inline void ToRowsAndCols(int *x, int *y)  {
	int row = *x / *y;
	int column = *x - row*(*y);

	*x = column;
	*y = row;
}

// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
	bool IsWall(int id) const;
	Tile* FindTileWithid(int id) const;
	int GetIdFromPos(iPoint pos) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	p2List<Tile*>		tiles;

};


enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	p2List<Layer*>			layers;
	p2List<ObjectGroup*>	objectGroups;
	iPoint					player_start_position;
	bool					is_level_1;
	
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();
	void PlaceColliders();
	void PlaceTileColliders();


	// Called before quitting
	bool CleanUp();

	// Load new map
	MapData* Load(const char* path);

	
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;
	TileSet * GetTilesetFromTileId(int id) const;
	bool CreateWalkabilityMap(int & width, int & height, uchar ** buffer) const;
private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTile(pugi::xml_node& tile_node, Tile* tile);
	bool LoadLayer(pugi::xml_node& node, Layer* layer);
	
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadObjectGroup(pugi::xml_node& group_node, ObjectGroup* object_grup);
	bool LoadObject(pugi::xml_node& object_node, Object* object);


	void Get_pixels_from_tiles(int x, int y, int* dest_x, int* dest_y)
	{
		*dest_x = x * data.tilesets.At(0)->data->tile_height;

		*dest_y = y * data.tilesets.At(0)->data->tile_width;
	}


	// bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

public:

	MapData data;


	//////////////HARDCODE
	float layer2_parallax = -0.5f;



private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__