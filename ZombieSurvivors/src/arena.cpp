#include "arena.h"
#include "rng.h"

namespace rr {
	// MAYBE tile_size in screen, and texture tile_size can be distinct
	Arena::Arena(const sf::IntRect& region, int tile_size): region_(region), tile_size_(tile_size) {
		vertices.setPrimitiveType(sf::Quads);
		int num_vertices_in_quads = 4;
		int horizontal_num_tiles = region.width / tile_size;
		int vertical_num_tiles = region.height / tile_size;
		vertices.resize(horizontal_num_tiles * vertical_num_tiles * num_vertices_in_quads);

		int quads_index = 0;
		for (int h = 0; h < horizontal_num_tiles; h++) {
			for (int v = 0; v < vertical_num_tiles; v++) {
				// clockwise vertices
				vertices[quads_index].position = sf::Vector2f(region.left + h * tile_size, region.top + v * tile_size);
				vertices[quads_index+1].position = sf::Vector2f(region.left + h * tile_size + tile_size -1, region.top + v * tile_size);
				vertices[quads_index+2].position = sf::Vector2f(region.left + h * tile_size + tile_size -1, region.top + v * tile_size + tile_size - 1);
				vertices[quads_index+3].position = sf::Vector2f(region.left + h * tile_size, region.top + v * tile_size + tile_size - 1);

				bool is_boundary_tile = h == 0 || h == horizontal_num_tiles - 1 || v == 0 || v == vertical_num_tiles - 1;
				int num_tiles_in_texture = 4;
				if (is_boundary_tile) {
					// the last one in texture
					vertices[quads_index].texCoords = sf::Vector2f(0, (num_tiles_in_texture - 1)*tile_size);
					vertices[quads_index+1].texCoords = sf::Vector2f(tile_size-1, (num_tiles_in_texture - 1)*tile_size);
					vertices[quads_index+2].texCoords = sf::Vector2f(tile_size-1, (num_tiles_in_texture - 1)*tile_size + tile_size-1);
					vertices[quads_index+3].texCoords = sf::Vector2f(0, (num_tiles_in_texture - 1)*tile_size + tile_size-1);
				}
				else {
					int num_content_tiles = num_tiles_in_texture - 1;
					int index = Rng::singleton().pick(num_content_tiles);

					vertices[quads_index].texCoords = sf::Vector2f(0, index*tile_size);
					vertices[quads_index+1].texCoords = sf::Vector2f(tile_size-1, index*tile_size);
					vertices[quads_index+2].texCoords = sf::Vector2f(tile_size-1, index*tile_size + tile_size-1);
					vertices[quads_index+3].texCoords = sf::Vector2f(0, index*tile_size + tile_size-1);
				}

				quads_index += num_vertices_in_quads;
			}
		}

		texture = &TextureMap::singleton()["graphics/background_sheet.png"];
	}

	void Arena::render(sf::RenderWindow* window) {
		window->draw(vertices, texture);
	}
}
