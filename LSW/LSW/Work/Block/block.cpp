#include "block.h"


namespace LSW {
	namespace v5 {
		namespace Work {

			
			void Block::draw_self()
			{
				if (bitmaps.empty()) return;

				{
					const auto delta_t = get_direct<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION);

					if (const double _dd = get_direct<double>(block::e_double::TIE_SIZE_TO_DISPLAY_PROPORTION); _dd > 0.0 && (std::chrono::system_clock::now().time_since_epoch() > delta_t)) {
						set(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() + block::default_delta_t_frame_delay));
												
						if (!reference.empty()) {
							int tx = reference.get_width() * _dd;
							int ty = reference.get_height() * _dd;

							for (auto& i : bitmaps) {
								int _w = i.get_width();
								int _h = i.get_height();
								if (_w == tx && _h == ty) continue; // no need to "reload"

								Interface::Bitmap now;
								now.create(tx, ty);

								now.set_as_target();
								if (i) i.draw(0, 0, tx, ty);
								i = std::move(now); // unloads automatically
							}
							reference.set_as_target();
						}
					}
				}

				int frame = get_direct<int>(block::e_integer::FRAME);

				{
					const double delta = get_direct<double>(block::e_double::FRAMES_PER_SECOND); // delta t, 1/t = sec
					std::chrono::milliseconds last_time = get_direct<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_FRAME);

					if (delta > 0.0 && frame >= 0) { // if delta <= 0 or frame < 0, static
						std::chrono::milliseconds delta_tr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1.0 / delta));
						if (delta_tr.count() > 0) {
							while (std::chrono::system_clock::now().time_since_epoch() > last_time) {
								last_time += delta_tr;
								if (++frame >= bitmaps.size()) frame = 0;
							}
						}
					}
					else {
						frame = frame > 0 ? frame : -frame;
						if (frame >= bitmaps.size()) frame = static_cast<int>(bitmaps.size() - 1);
					}

					set<std::chrono::milliseconds>(block::e_chronomillis_readonly::LAST_FRAME, last_time);
				}

				if (!get_direct<bool>(block::e_boolean::SET_FRAME_VALUE_READONLY)) {
					set<int>(block::e_integer::FRAME, frame);
				}

				auto rnn = bitmaps[frame];
				if (!rnn) throw Handling::Abort(__FUNCSIG__, "Unexpected NULL on draw!");


				float cx, cy, px, py, dsx, dsy, rot_rad;
				int bmpx, bmpy;
				bmpx = rnn.get_width();
				bmpy = rnn.get_height();
				if (bmpx <= 0 || bmpy <= 0) {
					throw Handling::Abort(__FUNCSIG__, "Somehow the texture have < 0 width / height!");
				}

				cx = 1.0f * bmpx * ((get_direct<double>(sprite::e_double::CENTER_X) + 1.0) * 0.5);
				cy = 1.0f * bmpy * ((get_direct<double>(sprite::e_double::CENTER_Y) + 1.0) * 0.5);
				rot_rad = 1.0f * get_direct<double>(sprite::e_double_readonly::ROTATION) * ALLEGRO_PI / 180.0;
				px = get_direct<double>(sprite::e_double_readonly::POSX);
				py = get_direct<double>(sprite::e_double_readonly::POSY);
				dsx = 1.0f * (get_direct<double>(sprite::e_double::SCALE_X)) * (get_direct<double>(sprite::e_double::SCALE_G)) * (1.0 / bmpx);
				dsy = 1.0f * (get_direct<double>(sprite::e_double::SCALE_Y)) * (get_direct<double>(sprite::e_double::SCALE_G)) * (1.0 / bmpy);


				if (get_direct<bool>(sprite::e_boolean::USE_COLOR)) {
					rnn.draw(
						get_direct<Interface::Color>(sprite::e_color::COLOR),
						cx, cy,
						px, py,
						dsx, dsy,
						rot_rad);
				}
				else {
					rnn.draw(
						cx, cy,
						px, py,
						dsx, dsy,
						rot_rad);
				}

			}
			Block::Block() : Sprite_Base()
			{
				//if (!data_block) throw Handling::Abort(__FUNCSIG__, "Failed to create Block's data!");
				custom_draw_task = [&] {draw_self(); };

				reference.be_reference_to_target(true);

				set<double>(block::e_double_defaults);
				set<bool>(block::e_boolean_defaults);
				set<int>(block::e_integer_defaults);
				set<std::chrono::milliseconds>(block::e_chronomillis_readonly_defaults);

				set(block::e_chronomillis_readonly::LAST_FRAME, MILLI_NOW);
				set(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, MILLI_NOW);
			}
			Block::Block(const Block& other) : Sprite_Base(other)
			{
				*this = other;
			}
			Block::Block(Block&& other) : Sprite_Base(std::move(other))
			{
				*this = std::move(other);
			}

			void Block::operator=(const Block& other)
			{
				custom_draw_task = [&] {draw_self(); };

				reference.be_reference_to_target(true);

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(*other.get<std::chrono::milliseconds>());

				set(block::e_chronomillis_readonly::LAST_FRAME, MILLI_NOW);
				set(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, MILLI_NOW);
			}

			void Block::operator=(Block&& other)
			{
				custom_draw_task = [&] {draw_self(); };

				reference.be_reference_to_target(true);

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(std::move(other.get<std::chrono::milliseconds>()));

				set(block::e_chronomillis_readonly::LAST_FRAME, MILLI_NOW);
				set(block::e_chronomillis_readonly::LAST_TIE_FRAME_VERIFICATION, MILLI_NOW);
			}

			void Block::twin_up_attributes(const Block& oth)
			{
				this->Sprite_Base::twin_up_attributes(oth);

				// difference from Sprite_Base
				set<std::chrono::milliseconds>(oth.get<std::chrono::milliseconds>());
			}

			void Block::insert(const Interface::Bitmap& bmp)
			{
				bitmaps.push_back(bmp);
			}
			void Block::remove(const std::function<bool(const Interface::Bitmap&)> remf)
			{
				for (size_t p = 0; p < bitmaps.size(); p++) {
					auto& i = bitmaps[p];
					if (remf(i)) {
						bitmaps.erase(bitmaps.begin() + p);
					}
				}
			}

		}
	}
}