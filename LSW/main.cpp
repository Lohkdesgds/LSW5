#include "LSW/LSWv5.h"

#include <iostream>

using namespace LSW::v5;
using namespace LSW::v5::Interface;

const std::string configpath = "%appdata%\\Lohk's Studios\\TheBlast\\config\\config.lohk";
const std::string datapath = "%appdata%\\Lohk's Studios\\TheBlast\\data\\data.zip";

int main() {
	/*{
		Tools::TimedMemory<int> testmemory(700, 10);
		std::cout << "AutoMemory: " << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("not alloc"); }() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::cout << "AutoMemory: " << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("not alloc"); }() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "AutoMemory: " << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("not alloc"); }() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
		std::cout << "AutoMemory: " << [&] {auto c = testmemory.get(); if (c.use_count()) return std::to_string(*c.get()); return std::string("not alloc"); }() << std::endl;
	}*/

	Logger logg;

	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > SOCKET TEST < < < < <" << L::EL;
	{
		logg << L::SL << fsr(__FUNCSIG__) << "Initializing host..." << L::EL;

		Hosting host;

		logg << L::SL << fsr(__FUNCSIG__) << "Initialized. Trying to create a client to connect to this host..." << L::EL;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		Connection client;
		if (!client.connect()) {
			logg << L::SL << fsr(__FUNCSIG__) << "Deu ruim" << L::EL;
		}
		else {
			logg << L::SL << fsr(__FUNCSIG__) << "Yay success. Waiting a sec to send stuff to the host." << L::EL;

			std::this_thread::sleep_for(std::chrono::seconds(1));

			std::shared_ptr<Connection> host_c = host.get_connection(0);

			std::this_thread::sleep_for(std::chrono::seconds(1));

			const char* first_msg = "hello there my friend my name is Jonas and I'm here to test this thing out. If it has problems, it will show them now!";
			const char* second_msg = "bananas de pijamas are nice tho. Just testing a little bit of code with some huge messages, as you can see. This is bigger than min buf for sure.";


			logg << L::SL << fsr(__FUNCSIG__) << "Trying to send from client to server..." << L::EL;

			client.send_package(first_msg);

			logg << L::SL << fsr(__FUNCSIG__) << "Recovering data sent on server..." << L::EL;

			while (!host_c->has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			auto res = host_c->get_next();

			logg << L::SL << fsr(__FUNCSIG__) << "Did they match? " << ((res == first_msg) ? "YES" : "NO") << L::EL;

			logg << L::SL << fsr(__FUNCSIG__) << "Sending from server to client..." << L::EL;

			host_c->send_package(second_msg);

			while (!client.has_package()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			res = client.get_next();

			logg << L::SL << fsr(__FUNCSIG__) << "Did they match? " << ((res == second_msg) ? "YES" : "NO") << L::EL;
		}
	}
	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > END OF SOCKET TEST < < < < <" << L::EL;


	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > FILE TEST < < < < <" << L::EL;
	{
		Interface::SmartFile file;

		logg << L::SL << fsr(__FUNCSIG__) << "Opening &9test_example.txt&f..." << L::EL;

		if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_KEEP))
		{
			logg << L::SL << fsr(__FUNCSIG__) << "Opened in READ_WRITE_KEEP." << L::EL;

		}
		else {
			if (file.open("test_example.txt", smartfile::file_modes::READ_WRITE_OVERWRITE)) {
				logg << L::SL << fsr(__FUNCSIG__) << "Opened in READ_WRITE_OVERWRITE." << L::EL;
			}
			else {
				logg << L::SL << fsr(__FUNCSIG__, E::ERRR) << "Cannot open file." << L::EL;
			}
		}

		if (file.is_open()) {
			std::string buf;
			
			if (file.read(buf, 256)) {
				logg << L::SL << fsr(__FUNCSIG__) << "Got this from file: " << buf << L::EL;
			}
			else {
				logg << L::SL << fsr(__FUNCSIG__) << "Cannot read from file." << L::EL;
			}

			file.seek(0, smartfile::file_seek::BEGIN);
			
			file.write("this is a huge test.");
			logg << L::SL << fsr(__FUNCSIG__) << "Set file value." << L::EL;

			file.close();
		}

	}
	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > END OF FILE TEST < < < < <" << L::EL;


	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > PATHMANAGER TEST < < < < <" << L::EL;

	Interface::PathManager pather;
	pather.add_path(datapath);
	pather.apply();

	logg << L::SL << fsr(__FUNCSIG__) << "Paths found there:" << L::EL;

	for (auto& i : pather.paths_set()) {
		logg << L::SL << fsr(__FUNCSIG__) << "> &9" << i << L::EL;
	}
	logg << L::SL << fsr(__FUNCSIG__) << "Files found there:" << L::EL;

	for (auto& i : pather.files_in_paths()) {
		logg << L::SL << fsr(__FUNCSIG__) << "> &5" << i.path << " [" << i.size_str << "]" << L::EL;
	}

	pather.unapply();

	logg << L::SL << fsr(__FUNCSIG__) << "&e> > > > > ENDED SETUP OF PATHMANAGER < < < < <" << L::EL;


	logg << L::SL << fsr(__FUNCSIG__) << "Creating display..." << L::EL;

	Display disp;

	Tools::Future<bool> disp_fut = disp.init();

	logg << L::SL << fsr(__FUNCSIG__) << "Created display." << L::EL;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto working_on = disp.add_once_task([pather] {		
		Logger logg;
		Tools::SuperResource<Bitmap> source;

		auto atlas = source.create("ATLAS_GLOBAL");
		auto lmao = source.create("_test");

		pather.apply();
		atlas->load("atlas0.png");
		bool gud = *atlas;

		if (!gud) logg << L::SL << fsr(__FUNCSIG__, E::WARN) << "Failed to load atlas." << L::EL;

		if (gud) {
			gud = lmao->create_sub(*atlas, 0, 1536, 512, 512);
			if (!gud) {
				logg << L::SL << fsr(__FUNCSIG__, E::WARN) << "Failed to load sub bitmap from atlas." << L::EL;
			}
			else {
				logg << L::SL << fsr(__FUNCSIG__) << "Loaded a bitmap from atlas SUCESSFULLY" << L::EL;
			}
		}
		if (!gud) {
			lmao->create(300, 300);
			lmao->clear_to_color(Color(255, 255, 255));
			if (*lmao) {
				logg << L::SL << fsr(__FUNCSIG__) << "Successfully created a bitmap to show." << L::EL;
			}
			else {
				logg << L::SL << fsr(__FUNCSIG__, E::ERRR) << "Failed to create a bitmap." << L::EL;
			}
		}
		pather.unapply();

		return lmao;
	});
	auto got = working_on.get().get<std::shared_ptr<Bitmap>>();

	Tools::SuperResource<Config> configs;

	auto conf = configs.create("DEFAULT");
	conf->load(configpath);

	/*auto vector_lol1 = conf->get_array<float>("config", "playercolor");
	for (auto& i : vector_lol1) {
		logg << L::SL << fsr(__FUNCSIG__) << "ARR B4: " << i << L::EL;
	}

	conf->comment("config", "This is a new version of myself");
	conf->set("config", "was_osd_on", false);
	conf->set("config", "second_screen_debug", false);
	conf->set("config", "ultradebug", false);
	conf->set("config", "double_buffering_screen", true);
	conf->set("config", "last_volume", 0.200218);
	conf->set("config", "resolution_proportion", 2.000000);
	conf->set("config", "fx_amount", 0.280000);
	conf->set("config", "screen_width", 1920);
	conf->set("config", "screen_height", 1080);
	conf->set("config", "last_display_flags", 21);
	conf->set("config", "pref_refresh_rate", 144);
	conf->set("config", "times_open", 6);
	conf->set("config", "playername", "Lohk");
	conf->set("config", "last_version", "B202002113719");
	conf->set("config", "playercolor", { 0.4023f, 0.5307f, 0.2331f });
	conf->set("config", "hidemouse", true);
	conf->set("config", "limit_framerate_to", 0);
	conf->set("config", "prints_path", "%win_photos_path%/Lohk's Studios Screenshots/");
	conf->set("private", config::config_section_mode::MEMORY_ONLY);
	conf->set("private", "yo config", 10);
	conf->flush();

	auto vector_lol = conf->get_array<float>("config", "playercolor");
	for (auto& i : vector_lol) {
		logg << L::SL << fsr(__FUNCSIG__) << "ARR AFT: " << i << L::EL;
	}
	logg << L::SL << fsr(__FUNCSIG__) << "Only in memory: " << conf->get_as<int>("private", "yo config") << L::EL;
	*/

	Tools::SuperResource<Camera> cameras;

	auto cam = cameras.create("DEFAULT");
	cam->classic_transform(0.0, 0.0, 1.0, 1.0, 0.0);

	disp.set_camera(cam);

	disp.add_draw_task([got] {
		got->draw((Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, (Tools::random() % 1000) * 1.0 / 1000.0 - 0.5, 0.3, 0.3);
	});

	Tools::SuperResource<Voice> voices;
	Tools::SuperResource<Mixer> mixers;
	Tools::SuperResource<Track> tracks;
	Tools::SuperResource<Sample> samples;

	auto main_voice = voices.create("MAIN_VOICE");
	auto main_mixer = mixers.create("MAIN_MIXER");

	main_voice->load();
	main_mixer->load();
	main_mixer->attach_to(main_voice);
	main_mixer->set_gain(conf->get_as<float>("config", "last_volume"));

	auto a_music = samples.create("ONE_SAMPLE");
	auto track = tracks.create("MY_TRACK");

	if (a_music->load("whatever.ogg")) {
		track->load(a_music);
		track->attach_to(main_mixer);
		track->set_play_mode(track::play_mode::MODE_LOOP);

		track->play();
		logg << L::SL << fsr(__FUNCSIG__) << "Started background music with playmode " << static_cast<int>(track->get_play_mode()) << L::EL;
	}
	else {
		logg << L::SL << fsr(__FUNCSIG__, E::ERRR) << "Couldn't start track." << L::EL;
	}

	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	int nowt = -1;
	disp.add_draw_task([&] {
		if (auto lala = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()); lala -now > std::chrono::seconds(1)) {
			now = lala;
			if (track->exists()) {
				int ltr = 100.0 * track->get_position_ms() / track->get_length_ms();
				if (ltr != nowt) {
					nowt = ltr;
					logg << L::SL << fsr(__FUNCSIG__) << nowt << "% | " << (track->get_position_ms() / 1e3) << " of " << (track->get_length_ms() / 1e3) << " s" << L::EL;
				}
			}
		}
	});

	EventHandler fullscreen;
	fullscreen.add(get_keyboard_event());
	fullscreen.add(disp.get_event_source());
	fullscreen.set_run_autostart([&](RawEvent& ev) {
		switch (ev.type())
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			if (ev.keyboard_event().keycode == ALLEGRO_KEY_F11) {
				logg << L::SL << fsr(__FUNCSIG__) << "Toggle Fullscreen called" << L::EL;
				disp.toggle_fullscreen();
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_P) {
				logg << L::SL << fsr(__FUNCSIG__) << "Play/Pause button called" << L::EL;
				if (track->exists()) {
					if (track->is_playing()) track->pause();
					else track->play();
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_R) {
				logg << L::SL << fsr(__FUNCSIG__) << "Reverse/Continuous button called" << L::EL;
				if (track->exists()) {
					track->set_speed(-track->get_speed());
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_MINUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "SlowDown button called" << L::EL;
				if (track->exists()) {
					if (fabs(track->get_speed()) > 0.1) track->set_speed(track->get_speed() > 0.0 ? track->get_speed() - 0.05 : track->get_speed() + 0.05);
					logg << L::SL << fsr(__FUNCSIG__) << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_EQUALS) {
				logg << L::SL << fsr(__FUNCSIG__) << "Accel button called" << L::EL;
				if (track->exists()) {
					track->set_speed(track->get_speed() > 0.0 ? track->get_speed() + 0.05 : track->get_speed() - 0.05);
					logg << L::SL << fsr(__FUNCSIG__) << "Now speed = " << track->get_speed() << "x" << L::EL;
				}
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_PAD_MINUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "PAD- button called" << L::EL;
				float vol = main_mixer->get_gain();
				if (vol > 0.02) vol -= 0.02;
				main_mixer->set_gain(vol);
				conf->set("config", "last_volume", vol);
				logg << L::SL << fsr(__FUNCSIG__) << "Now volume = " << vol << "." << L::EL;
			}
			else if (ev.keyboard_event().keycode == ALLEGRO_KEY_PAD_PLUS) {
				logg << L::SL << fsr(__FUNCSIG__) << "PAD+ button called" << L::EL;
				float vol = main_mixer->get_gain();
				if (vol < 1.0) vol += 0.02;
				main_mixer->set_gain(vol);
				conf->set("config", "last_volume", vol);
				logg << L::SL << fsr(__FUNCSIG__) << "Now volume = " << vol << "." << L::EL;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			logg << L::SL << fsr(__FUNCSIG__) << "Close app called" << L::EL;
			if (track->exists()) track->stop();
			disp.set_stop();
			break;
		}
	});

	disp_fut.then<void>([&](bool good) {
		logg << L::SL << fsr(__FUNCSIG__) << "Display closed with SUCCESS=" << good << L::EL;
		logg.debug("disp_fut.then() called.");
	});

	disp_fut.wait();


	std::this_thread::sleep_for(std::chrono::seconds(1));

	logg.debug("DEBUG TEST LSW");

	return 0;
}