#include <iostream>

#include <gtk/gtk.h>
#include "miniaudio.h"

#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000

void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    ma_waveform* pSineWave = (ma_waveform*)device->pUserData;
    ma_waveform_read_pcm_frames(pSineWave, output, frame_count, NULL);
}

void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "AudioSynthesis");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    label = gtk_label_new("Hello GTK");
    gtk_window_set_child(GTK_WINDOW(window), label);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[]) {
    GtkApplication* app;
    int status;

    // Configure miniaudio
    ma_waveform sineWave;
    ma_waveform_config sineWaveConfig;

    ma_device device;
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format      = DEVICE_FORMAT;
    config.playback.channels    = DEVICE_CHANNELS;
    config.sampleRate           = DEVICE_SAMPLE_RATE;
    config.dataCallback         = data_callback;
    config.pUserData            = &sineWave;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        std::cout << "Failed to open output device" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Output device is - " << device.playback.name << std::endl;

    sineWaveConfig = ma_waveform_config_init(device.playback.format, device.playback.channels, device.sampleRate, ma_waveform_type_sine, 0.2, 220);
    ma_waveform_init(&sineWaveConfig, &sineWave);

    if (ma_device_start(&device) != MA_SUCCESS) {
        std::cout << "Failed to start device" << std::endl;
        ma_device_uninit(&device);
        return EXIT_FAILURE;
    }

    // Configure GTK
    app = gtk_application_new("org.michal.gagos", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // Uninitialize miniaudio
    ma_device_uninit(&device);

    return status;
}