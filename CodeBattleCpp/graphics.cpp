#include "client.hpp"
#include "graphics.hpp"


using namespace std::chrono_literals;

graphics::graphics(HWND hwnd, std::shared_ptr<client>& client) {
	if ((pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) {
		std::cout << "pD3D == nullptr\n";
		return;
	}
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;


	if (pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device_) < 0) {
		pd3d->Release();
		pd3d = nullptr;
		std::cout << "device is dead :c";
		return;
	}
	//Ignore this shit
	{
	D3DXCreateSprite(device_, &sprite_);
	D3DXCreateTextureFromFile(device_, L"resources/brick.png", &textures[0]);
	D3DXCreateTextureFromFile(device_, L"resources/drill_pit.png", &textures[1]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_ladder.png", &textures[2]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_left.png", &textures[3]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_pipe_left.png", &textures[4]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_pipe_right.png", &textures[5]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_pit.png", &textures[6]);
	D3DXCreateTextureFromFile(device_, L"resources/enemy_right.png", &textures[7]);
	D3DXCreateTextureFromFile(device_, L"resources/gold.png", &textures[8]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_die.png", &textures[9]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_drill_left.png", &textures[10]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_drill_right.png", &textures[11]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_fall_left.png", &textures[12]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_fall_right.png", &textures[13]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_ladder.png", &textures[14]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_left.png", &textures[15]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_pipe_left.png", &textures[16]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_pipe_right.png", &textures[17]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_right.png", &textures[18]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_drill_left.png", &textures[19]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_drill_right.png", &textures[20]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_fall_left.png", &textures[21]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_fall_right.png", &textures[22]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_ladder.png", &textures[23]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_left.png", &textures[24]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_right.png", &textures[25]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_pipe_left.png", &textures[26]);
	D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_pipe_right.png", &textures[27]);
	D3DXCreateTextureFromFile(device_, L"resources/ladder.png", &textures[28]);
	D3DXCreateTextureFromFile(device_, L"resources/none.png", &textures[29]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_die.png", &textures[30]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_ladder.png", &textures[31]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_left.png", &textures[32]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_pipe_left.png", &textures[33]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_pipe_right.png", &textures[34]);
	D3DXCreateTextureFromFile(device_, L"resources/other_hero_right.png", &textures[35]);
	D3DXCreateTextureFromFile(device_, L"resources/pipe.png", &textures[36]);
	D3DXCreateTextureFromFile(device_, L"resources/pit_fill_1.png", &textures[37]);
	D3DXCreateTextureFromFile(device_, L"resources/pit_fill_2.png", &textures[38]);
	D3DXCreateTextureFromFile(device_, L"resources/pit_fill_3.png", &textures[39]);
	D3DXCreateTextureFromFile(device_, L"resources/pit_fill_4.png", &textures[40]);
	D3DXCreateTextureFromFile(device_, L"resources/the_shadow_pill.png", &textures[41]);
	D3DXCreateTextureFromFile(device_, L"resources/undestroyable_wall.png", &textures[42]);
	}
	worker_ = std::thread([&]() {render_scene(client); });
}

void graphics::render_scene(std::shared_ptr<client>& gcb) {
	while (true) {
		device_->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
		auto hresult = device_->TestCooperativeLevel();
		if (hresult == D3DERR_DEVICELOST || hresult == D3DERR_DEVICENOTRESET) {
			device_->Reset(&d3dpp);
			sprite_->Release();
			sprite_ = nullptr;
			for (auto& tex : textures)
			{
				tex->Release();
				tex = nullptr;
			}

			if (!FAILED(device_->Reset(&d3dpp)))
			{
				D3DXCreateTextureFromFile(device_, L"resources/brick.png", &textures[0]);
				D3DXCreateTextureFromFile(device_, L"resources/drill_pit.png", &textures[1]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_ladder.png", &textures[2]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_left.png", &textures[3]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_pipe_left.png", &textures[4]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_pipe_right.png", &textures[5]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_pit.png", &textures[6]);
				D3DXCreateTextureFromFile(device_, L"resources/enemy_right.png", &textures[7]);
				D3DXCreateTextureFromFile(device_, L"resources/gold.png", &textures[8]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_die.png", &textures[9]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_drill_left.png", &textures[10]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_drill_right.png", &textures[11]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_fall_left.png", &textures[12]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_fall_right.png", &textures[13]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_ladder.png", &textures[14]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_left.png", &textures[15]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_pipe_left.png", &textures[16]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_pipe_right.png", &textures[17]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_right.png", &textures[18]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_drill_left.png", &textures[19]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_drill_right.png", &textures[20]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_fall_left.png", &textures[21]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_fall_right.png", &textures[22]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_ladder.png", &textures[23]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_left.png", &textures[24]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_right.png", &textures[25]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_pipe_left.png", &textures[26]);
				D3DXCreateTextureFromFile(device_, L"resources/hero_shadow_pipe_right.png", &textures[27]);
				D3DXCreateTextureFromFile(device_, L"resources/ladder.png", &textures[28]);
				D3DXCreateTextureFromFile(device_, L"resources/none.png", &textures[29]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_die.png", &textures[30]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_ladder.png", &textures[31]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_left.png", &textures[32]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_pipe_left.png", &textures[33]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_pipe_right.png", &textures[34]);
				D3DXCreateTextureFromFile(device_, L"resources/other_hero_right.png", &textures[35]);
				D3DXCreateTextureFromFile(device_, L"resources/pipe.png", &textures[36]);
				D3DXCreateTextureFromFile(device_, L"resources/pit_fill_1.png", &textures[37]);
				D3DXCreateTextureFromFile(device_, L"resources/pit_fill_2.png", &textures[38]);
				D3DXCreateTextureFromFile(device_, L"resources/pit_fill_3.png", &textures[39]);
				D3DXCreateTextureFromFile(device_, L"resources/pit_fill_4.png", &textures[40]);
				D3DXCreateTextureFromFile(device_, L"resources/the_shadow_pill.png", &textures[41]);
				D3DXCreateTextureFromFile(device_, L"resources/undestroyable_wall.png", &textures[42]);
				D3DXCreateSprite(device_, &sprite_);
			}
			else return;
		}
		auto result = device_->BeginScene();
		if (SUCCEEDED(result)) {
			std::vector t(gcb->get_map());
			struct {
				float x, y;
			} pos{ 0,0 };
			std::vector<vec> t_path;
			if (gcb->try_lock()) {
				if(gcb->get_game_board().get())
				t_path = gcb->get_game_board()->get_current_path();
				gcb->unlock();
			}
			for (auto& line : t) {
				for (auto& row : line) {
					auto texture = textures[29];
					switch (row) {
					case tile::BRICK:
						texture = textures[0];
						break;
					case tile::PIPE:
						texture = textures[36];
						break;
					case tile::YELLOW_GOLD:
					case tile::RED_GOLD:
					case tile::GREEN_GOLD:
						texture = textures[8];
						break;
					case tile::LADDER:
						texture = textures[28];
						break;
					case tile::UNDESTROYABLE_WALL:
						texture = textures[42];
						break;
					case tile::PIT_FILL_1:
						texture = textures[37];
						break;
					case tile::PIT_FILL_4:
						texture = textures[40];
						break;
					case tile::PIT_FILL_3:
						texture = textures[39];
						break;
					case tile::PIT_FILL_2:
						texture = textures[38];
						break;
					case tile::DRILL_PIT:
						texture = textures[1];
						break;
					case tile::HERO_DIE:
						texture = textures[9];
						break;
					case tile::HERO_DRILL_LEFT:
						texture = textures[10];
						break;
					case tile::HERO_DRILL_RIGHT:
						texture = textures[11];
						break;
					case tile::HERO_LADDER:
						texture = textures[14];
						break;
					case tile::HERO_LEFT:
						texture = textures[15];
						break;
					case tile::HERO_RIGHT:
						texture = textures[18];
						break;
					case tile::HERO_FALL_LEFT:
						texture = textures[12];
						break;
					case tile::HERO_FALL_RIGHT:
						texture = textures[13];
						break;
					case tile::HERO_PIPE_LEFT:
						texture = textures[16];
						break;
					case tile::HERO_PIPE_RIGHT:
						texture = textures[17];
						break;
					case tile::OTHER_HERO_DIE:
						texture = textures[30];
						break;
					case tile::OTHER_HERO_LEFT:
						texture = textures[32];
						break;
					case tile::OTHER_HERO_RIGHT:
						texture = textures[35];
						break;
					case tile::OTHER_HERO_LADDER:
						texture = textures[31];
						break;
					case tile::OTHER_HERO_PIPE_LEFT:
						texture = textures[33];
						break;
					case tile::OTHER_HERO_PIPE_RIGHT:
						texture = textures[34];
						break;
					case tile::ENEMY_LADDER:
						texture = textures[2];
						break;
					case tile::ENEMY_LEFT:
						texture = textures[3];
						break;
					case tile::ENEMY_RIGHT:
						texture = textures[7];
						break;
					case tile::ENEMY_PIPE_LEFT:
						texture = textures[4];
						break;
					case tile::ENEMY_PIPE_RIGHT:
						texture = textures[5];
						break;
					case tile::ENEMY_PIT:
						texture = textures[6];
						break;
					}
					D3DXMATRIX world;
					D3DXMATRIX scale;
					D3DXMATRIX translation;
					D3DXMatrixIdentity(&world);
					D3DSURFACE_DESC img_info;
					texture->GetLevelDesc(0, &img_info);
					float scl_x = float(img_info.Width) / 16.f;
					float scl_y = float(img_info.Height) / 16.f;
					D3DXMatrixScaling(&scale, scl_x, scl_y, 1.f);
					D3DXMatrixTranslation(&translation, 0.f, 0.f, 0.f);
					world = scale * translation;
					auto vec = D3DXVECTOR3(pos.x, pos.y, 0.f);
					sprite_->Begin(D3DXSPRITE_ALPHABLEND);
					sprite_->Draw(texture, nullptr, nullptr, &vec, D3DCOLOR_RGBA(255, 255, 255, 255));
					sprite_->End();
					pos.x += 16;
				}
				pos.x = 0;
				pos.y += 16;
			}
			if (t_path.size()) {
				auto prev = t_path.front();
				prev.y *= 16;
				prev.y += 8;
				prev.x *= 16;
				prev.x += 8;
				for (auto point : t_path) {
					point.y *= 16;
					point.y += 8;
					point.x *= 16;
					point.x += 8;
					draw_line(vec(prev.x -1, prev.y -1), vec(point.x-1,point.y-1), D3DCOLOR_ARGB(255, 0, 240, 0));
					draw_line(prev, point,D3DCOLOR_ARGB(255,0,240,0));
					draw_line(vec(prev.x + 1, prev.y + 1), vec(point.x + 1, point.y + 1), D3DCOLOR_ARGB(255, 0, 240, 0));
					prev = point;
				}
			}
			device_->EndScene();
		}
		result = device_->Present(NULL, NULL, NULL, NULL);
		if (!SUCCEEDED(result)) MessageBox(0, 0, 0, 0);
		std::this_thread::sleep_for(1ms);
	}
}

void graphics::draw_line(vec p1, vec p2, uint32_t col) {

	vertex vert[2] =
	{
		{ p1.x, p1.y, 0.0f, 1.0f, col },
		{ p2.x, p2.y, 0.0f, 1.0f, col }
	};

	device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device_->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vert, sizeof(vertex));

}

graphics::~graphics() {
	if (worker_.joinable())
		worker_.join();
}